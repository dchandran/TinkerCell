/****************************************************************************

Copyright (c) 2008 Deepak Chandran
Contact: Deepak Chandran (dchandran1@gmail.com)
See COPYRIGHT.TXT

A small class that generates the ode and rates file from the given items

****************************************************************************/

#include <QDebug>
#include "ConsoleWindow.h"
#include "ModelFileGenerator.h"
#include "StoichiometryTool.h"
#include "BasicInformationTool.h"

namespace Tinkercell
{
	QString ModelFileGenerator::toString(double d)
	{
		QString s = QString::number(d);
		if (!s.contains("."))
			s += QString(".0");
		return s;
	}

	ModelFileGenerator::ModelFileGenerator() : Tool(tr("Model File Generator"),tr("Modeling"))
	{
		connectCFuntions();
	}

	bool ModelFileGenerator::setMainWindow(MainWindow * main)
	{
		Tool::setMainWindow(main);

		if (mainWindow)
		{
			connect(mainWindow,SIGNAL(setupFunctionPointers( QLibrary * )),this,SLOT(setupFunctionPointers( QLibrary * )));
		}
		return (mainWindow != 0);
	}

	typedef void (*tc_ModelFileGenerator_api)(
		int (*writeModel)(const char*,ArrayOfItems)
		);

	void ModelFileGenerator::setupFunctionPointers( QLibrary * library)
	{
		tc_ModelFileGenerator_api f = (tc_ModelFileGenerator_api)library->resolve("tc_ModelFileGenerator_api");
		if (f)
		{
			f(
				&(_generateModelFile)
				);
		}
	}

	void ModelFileGenerator::connectCFuntions()
	{
		connect(&fToS,SIGNAL(generateModelFile(QSemaphore*,int*, const QString&,QList<ItemHandle*>&)),this,SLOT(generateModelFile(QSemaphore*,int*,const QString&,QList<ItemHandle*>&)));
	}

	ModelFileGenerator_FToS ModelFileGenerator::fToS;

	int ModelFileGenerator::_generateModelFile(const char* filename, ArrayOfItems a0)
	{
		return fToS.generateModelFile(filename,a0);
	}

	int ModelFileGenerator_FToS::generateModelFile(const char* filename, ArrayOfItems a0)
	{
		QList<ItemHandle*> * list = ConvertValue(a0);
		QSemaphore * s = new QSemaphore(1);
		s->acquire();
		int i=0;
		emit generateModelFile(s,&i,ConvertValue(filename),*list);
		s->acquire();
		s->release();
		delete s;
		delete list;
		return i;
	}

	void ModelFileGenerator::generateModelFile(QSemaphore * s, int* i, const QString& filename, QList<ItemHandle*>& items)
	{
		if (i)
			(*i) = ModelFileGenerator::generateModelFile(filename, items, tr("_"), currentNetwork());
		else
			ModelFileGenerator::generateModelFile(filename, items, tr("_"), currentNetwork());

		if (s)
			s->release();
	}

	int ModelFileGenerator::generateModelFile(const QString& prefix, QList<ItemHandle*>& handles,const QString& replaceDot, NetworkHandle * currentNetwork)
	{
		QString filename = prefix;

		if (currentNetwork && currentNetwork->globalHandle() && !handles.contains(currentNetwork->globalHandle()))
			handles << currentNetwork->globalHandle();

		filename.replace(QRegExp(tr("\\.\\s+$")),QString(""));
		QFile cfile (filename + QString(".c"));
		QFile pyfile (filename + QString(".py"));
		QString name;

		QList<ItemHandle*> namelessHandles;

		int i,j;

		if (!cfile.open(QFile::WriteOnly | QFile::Text) || !pyfile.open(QFile::WriteOnly | QFile::Text))
		{
			return 0;
		}
		QStringList code, pycode;

		//multiple cells

		QStringList assignedHandles;

		QList< QPair<ItemHandle*,double> > modifiedHandles;
		for (i=0; i < handles.size(); ++i)
		{
			if (handles[i] && handles[i]->data && handles[i]->hasNumericalData(QString("Numerical Attributes"))
				&& handles[i]->family() && handles[i]->family()->isA("Cell"))
			{
				QList<ItemHandle*> handlesInCell;
				double popSz = handles[i]->data->numericalData[QString("Numerical Attributes")].at("Count",0);
				if (popSz > 0)
				{
					handlesInCell << handles[i]->children;

					for (int j=0; j < handlesInCell.size(); ++j)
					{
						handlesInCell << handlesInCell[j]->children;
					}
					for (int j=0; j < handlesInCell.size(); ++j)
						modifiedHandles << QPair<ItemHandle*,double>(handlesInCell[j],popSz);

				}
			}
		}

		ItemHandle* handle;
		double popSz;
		for (i=0; i < modifiedHandles.size(); ++i)
		{
			handle = modifiedHandles[i].first;
			popSz =  modifiedHandles[i].second;
			if (handle && handle->family() && handle->family()->isA("Node") && handle->data && handle->hasNumericalData(QString("Initial Value")))
			{
				DataTable<qreal>& dat = handle->data->numericalData[QString("Initial Value")];
				dat.value(0,0) *= popSz;
			}
		}
		QStringList rates = StoichiometryTool::getRates(handles, replaceDot);

		for (i=0; i < rates.size(); ++i)
		{
			if (rates[i].isEmpty())
				rates[i] = QString("0.0");
			replaceHatWithPow(rates[i]);
		}

		DataTable<qreal> N = StoichiometryTool::getStoichiometry(handles,replaceDot,true);

		int r = N.rows(), c = N.cols();

		if (r < 1 || c < 1 || rates.isEmpty()) return 0;

		DataTable<qreal> params = BasicInformationTool::getParameters(handles,QStringList(), QStringList(), replaceDot);
		params.insertCol(1,tr("used"));

		bool used = false;
		for (i=0; i < params.rows(); ++i)
		{
			used = false;
			for (j=0; j < rates.size(); ++j)
			{
				if (rates[j].contains(params.rowName(i)))
				{
					used = true;
					break;
				}
			}
			if (used)
				params.value(i,1) = 1.0;
			else
				params.value(i,1) = 0.0;
		}

		//restore modified cells
		for (i=0; i < modifiedHandles.size(); ++i)
		{
			handle = modifiedHandles[i].first;
			popSz =  modifiedHandles[i].second;
			if (handle && handle->family() && handle->family()->isA("Node") && handle->data && handle->hasNumericalData("Initial Value"))
			{
				DataTable<qreal>& dat = handle->data->numericalData["Initial Value"];
				dat.value(0,0) /= popSz;
			}
		}

		QRegExp regex(tr("\\.(?!\\d)"));
		QString s1,s2;
		QStringList vars, fluxes, initValues, eventTriggers, eventActions, assignmentNames,
					assignmentDefs, fixedVars, functionNames, functionDefs, functionArgs;

		for (i=0; i < handles.size(); ++i)
		{
			if (handles[i])// && handles[i]->family())
			{
				if (handles[i]->data)
				{
					if (handles[i]->hasNumericalData(tr("Initial Value")))
					{
						vars << handles[i]->fullName(replaceDot);
						initValues << toString(handles[i]->data->numericalData[tr("Initial Value")].value(0,0));
					}
					/*else
						if (!handles[i]->name.isEmpty())
						{
							vars << handles[i]->fullName(replaceDot);
							initValues << "0.0";
						}*/
					if (handles[i]->hasNumericalData(tr("Fixed")) &&
						handles[i]->data->numericalData[tr("Fixed")].at(0,0) > 0)
					{
						int k = N.rowNames().indexOf(handles[i]->fullName(replaceDot));
						if (k >= 0)
						{
							fixedVars << handles[i]->fullName(replaceDot);
						}
					}
				}
				if (handles[i]->hasTextData(tr("Events")))
				{
					DataTable<QString>& dat = handles[i]->data->textData[tr("Events")];
					if (dat.cols() == 1)
						for (j=0; j < dat.rows(); ++j)
						{
							s1 =  dat.rowName(j);
							s2 =  dat.value(j,0);

                            replaceHatWithPow(s1);
							replaceHatWithPow(s2);
							s1.replace(regex,replaceDot);
							s2.replace(regex,replaceDot);

							if (s1.isEmpty() || s2.isEmpty()) continue;

							eventTriggers << s1;
							eventActions << s2;

							for (int k=0; k < params.rows(); ++k)
								if (s2.contains(params.rowName(k)) || s1.contains(params.rowName(k)))
									params.value(k,1) = 1.0;
						}
				}
				if (handles[i]->hasTextData(tr("Functions")))
				{
					DataTable<QString>& dat = handles[i]->data->textData[tr("Functions")];

					if (dat.cols() == 2)
					{
						for (j=0; j < dat.rows(); ++j)
						{
                            name = handles[i]->fullName(replaceDot);
							s1 = dat.value(j,1);
							replaceHatWithPow(s1);
							s1.replace(regex,replaceDot);

							s2 = dat.value(j,0);

							functionNames << name + replaceDot + dat.rowName(j);
							functionArgs << s2;
							functionDefs << s1;
						}
					}
				}

				if (handles[i]->hasTextData(tr("Assignments")))
				{
					DataTable<QString>& dat = handles[i]->data->textData[tr("Assignments")];
					if (dat.cols() == 1)
						for (j=0; j < dat.rows(); ++j)
						{
                            name = handles[i]->fullName(replaceDot);

							s1 =  dat.rowName(j);
							s2 =  dat.value(j,0);

                            s1.replace(regex,replaceDot);
							replaceHatWithPow(s2);
							s2.replace(regex,replaceDot);

							if (s1.isEmpty() || s2.isEmpty()) continue;

							if (s1.isEmpty() || s1 == name)
							{
								assignmentNames << name;
								assignmentDefs << s2;

								if (!fixedVars.contains(name))
									fixedVars << name;
							}
							else								
							{
								assignmentNames << name + replaceDot + s1;
								assignmentDefs << s2;
							}
							for (int k=0; k < params.rows(); ++k)
								if (s2.contains(params.rowName(k)) || s1.contains(params.rowName(k)))
									params.value(k,1) = 1.0;
						}
				}
			}
		}

		code += tr("#include <stdio.h>\n#include <stdlib.h>\n#include <math.h>\n\n");
		for (i=0; i < functionNames.size(); ++i)
		{
			name = functionNames[i];
			s2 = functionArgs[i];
			s1 = functionDefs[i];

			s2.replace(tr(","),tr(", double "));
			s2 = tr("(double ") + s2 + tr(")");
			code += tr("    double ") + name + s2 + tr("{ return ") + s1 + tr(";}\n\n");
		}

		code += tr("typedef struct\n{\n");
		pycode += "class TCmodel:\n";

		QStringList allValues;
		QStringList allSymbols;
		
		for (i=0; i < N.rows(); ++i)
			if (!vars.contains(N.rowName(i)))
			{
				vars << N.rowName(i);
				initValues << tr("0.0");
			}

		for (i=0; i < vars.size(); ++i)
            if (!allSymbols.contains(vars[i]))
            {
                code += tr("    double ");
                code += vars[i];
                code += tr(";\n");

                allSymbols << vars[i];
                allValues << initValues[i];

                pycode += tr("    ");
                pycode += vars[i];
                pycode += tr(" = ");
                pycode += initValues[i];
                pycode += tr(";\n");
            }

		fluxes = N.getColNames();

		for (i=0; i < fluxes.size(); ++i)
		{
			code += tr("    double ");
			code += fluxes[i];
			code += tr(";\n");
			allValues << tr("0.0");

			pycode += tr("    ");
			pycode += fluxes[i];
			pycode += tr(" = 0.0;\n");
		}

		for (i=0; i < assignmentNames.size(); ++i)
			if (!allSymbols.contains(assignmentNames[i]))
			{
				code += tr("    double ");
				code += assignmentNames[i];
				code += tr(";\n");

				allValues << tr("0.0");
				allSymbols << assignmentNames[i];

				pycode += tr("    ");
				pycode += assignmentNames[i];
				pycode += tr(" = 0.0;\n");
			}

		QStringList trueParams;
		for (i = 0; i < params.rows(); ++i)
			if (!allSymbols.contains(params.rowName(i)) && params.at(i,1) > 0)
			{
				trueParams << params.rowName(i);
				allSymbols << params.rowName(i);

				code += tr("    double ");
				code += params.rowName(i);
				code += tr(";\n");
				allValues << toString(params.at(i,0));

				pycode += tr("    ");
				pycode += params.rowName(i);
				pycode += tr(" = ");
				pycode += toString(params.at(i,0));
				pycode += tr(";\n");
			}

		code += tr("} TCmodel;\n\n");
		code += tr("TCmodel TC_initial_model = {");
		code += allValues.join(tr(","));
		code += tr("};\n\n");

		for (i=0; i < functionNames.size(); ++i)
            if (!allSymbols.contains(functionNames[i]))
            {
                name = functionNames[i];
                s2 = functionArgs[i];
                s1 = functionDefs[i];
                pycode += tr("    defn ") + name + s2 + tr(": return ") + s1 + tr(";\n");
            }

		/*if (fixedVars.size() < N.rows())
		{
			DataTable<qreal> N2(N);
			N.resize(N.rows()-fixedVars.size(), N.cols());
			N.setColNames(N2.getColNames());

			int i2;
			for (i=0, i2 = 0; i < N2.rows(); ++i)
			{
				if (!fixedVars.contains(N2.rowName(i)))
				{
					N.rowName(i2) = N2.rowName(i);
					for (j=0; j < N2.cols(); ++j)
					{
						N.value(i2,j) = N2.value(i,j);
					}
					++i2;
				}
			}
		}
		else*/
		for (i=0; i < r; ++i)
			if (fixedVars.contains(N.rowName(i)))
			{
				for (j=0; j < c; ++j)
				{
					N.value(i,j) = 0.0;
				}
			}

		//get array of current param values
		code += tr("\ndouble * TCgetParameters( TCmodel * model )\n{ \n");
		code += tr("   double * k = (double*)malloc(");
		code += QString::number(trueParams.size());
		code += tr("*sizeof(double));\n");

		pycode += tr("    defn TCgetParameters ():\n    k = range(0,");
		pycode += QString::number(trueParams.size());
		pycode += tr(");\n");

		for (i = 0; i < trueParams.size(); ++i)
		{
			code += tr("   k[");
			code += QString::number(i);
			code += tr("] = model->");
			code += trueParams[i];
			code += tr(";\n");

			pycode += tr("        k[");
			pycode += QString::number(i);
			pycode += tr("] = ");
			pycode += trueParams[i];
			pycode += tr(";\n");
		}
		code += tr("    return k;\n}\n\n");
		pycode += tr("        return k;\n\n");

		//get array of current variable values
		code += tr("\ndouble * TCgetVars( TCmodel * model )\n{ \n");
		code += tr("   double * k = (double*)malloc(");
		code += QString::number(r);
		code += tr("*sizeof(double));\n");

		pycode += tr("    defn TCgetVars ():\n    k = range(0,");
		pycode += QString::number(r);
		pycode += tr(");\n");

		for (i = 0; i < r; ++i)
		{
			code += tr("   k[");
			code += QString::number(i);
			code += tr("] = model->");
			code += N.rowName(i);
			code += tr(";\n");

			pycode += tr("        k[");
			pycode += QString::number(i);
			pycode += tr("] = ");
			pycode += N.rowName(i);
			pycode += tr(";\n");
		}
		code += tr("    return k;\n}\n\n");
		pycode += tr("        return k;\n\n");

		//get array of current rates
		code += tr("\ndouble * TCgetRates( TCmodel * model )\n{ \n");
		code += tr("   double * k = (double*)malloc(");
		code += QString::number(c);
		code += tr("*sizeof(double));\n");

		pycode += tr("    defn TCgetRates ():\n    k = range(0,");
		pycode += QString::number(c);
		pycode += tr(");\n");

		for (i = 0; i < c; ++i)
		{
			code += tr("   k[");
			code += QString::number(i);
			code += tr("] = model->");
			code += N.colName(i);
			code += tr(";\n");

			pycode += tr("        k[");
			pycode += QString::number(i);
			pycode += tr("] = ");
			pycode += N.colName(i);
			pycode += tr(";\n");
		}

		code += tr("    return k;\n}\n\n");
		pycode += tr("        return k;\n\n");

		//write assign-parameters function (not needed for simulation, but could be useful for some purpose)
		code += tr("\nvoid TCassignParameters( double * k, TCmodel * model )\n{ \n");
		pycode += tr("    defn TCassignParameters ( k ):\n");

		for (i = 0; i < trueParams.size(); ++i)
		{
			code += tr("   model->");
			code += trueParams[i];
			code += tr(" = k[");
			code += QString::number(i);
			code += "];\n";

			pycode += tr("        ");
			pycode += trueParams[i];
			pycode += tr(" = k[");
			pycode += QString::number(i);
			pycode += tr("];\n");
		}

		code += tr("}\n\n");
		pycode += tr("\n\n");
		
		code += tr("\nvoid TCassignVars( double * k, TCmodel * model )\n{ \n");
		pycode += tr("    defn TCassignVar ( k ):\n");

		for (i = 0; i < r; ++i)
		{
			code += tr("   model->");
			code += N.rowName(i);
			code += tr(" = k[");
			code += QString::number(i);
			code += "];\n";

			pycode += tr("        ");
			pycode += N.rowName(i);
			pycode += tr(" = k[");
			pycode += QString::number(i);
			pycode += tr("];\n");
		}

		code += tr("}\n\n");
		pycode += tr("\n\n");
		/*
		code += tr("\nvoid TCassignParametersAndVars( double * k, TCmodel * model )\n{ \n");
		pycode += tr("    defn TCassignParametersAndVars ( k ):\n");

		for (i = 0; i < trueParams.size(); ++i)
		{
			code += tr("   model->");
			code += trueParams[i];
			code += tr(" = k[");
			code += QString::number(i);
			code += "];\n";

			pycode += tr("        ");
			pycode += trueParams[i];
			pycode += tr(" = k[");
			pycode += QString::number(i);
			pycode += tr("];\n");
		}
		
		for (i = 0; i < r; ++i)
		{
			code += tr("   model->");
			code += N.rowName(i);
			code += tr(" = k[");
			code += QString::number(i+trueParams.size());
			code += "];\n";

			pycode += tr("        ");
			pycode += N.rowName(i);
			pycode += tr(" = k[");
			pycode += QString::number(i+trueParams.size());
			pycode += tr("];\n");
		}

		code += tr("}\n\n");
		pycode += tr("\n\n");*/

		//print header and beginning of propensity function
		code += tr("\nvoid TCpropensity(double time, double * u, double * rates, void * data)\n{\n    TCmodel * model = (TCmodel*)data;\n");
		pycode += tr("\n    defn TCpropensity(u, time):\n");

		//declare variables
		for (i = 0; i < N.rows(); ++i)
		{
			code += tr("    model->");
			code += N.rowName(i);
			code += tr(" = u[");
			code += QString::number(i);
			code += tr("];\n");

			pycode += tr("        ");
			pycode += N.rowName(i);
			pycode += tr(" = u[");
			pycode += QString::number(i);
			pycode += tr("];\n");
		}

		//assignments
		if (!assignmentNames.isEmpty())
		{
			for (i=0; i < assignmentNames.size(); ++i)
			{
				pycode += tr("        ");
				pycode += assignmentNames[i];
				pycode += tr(" = ");
				pycode += assignmentDefs[i];
				pycode += tr(";\n");

				code += tr("    model->");
				code += assignmentNames[i];
				code += tr(" = ");
				code += insertPrefix(handles,tr("model->"),assignmentDefs[i],replaceDot);
				code += tr(";\n");
			}
			
			for (i = 0; i < N.rows(); ++i)
			{
				code += tr("    u[");
				code += QString::number(i);
				code += tr("] = model->");
				code += N.rowName(i);
				code += tr(";\n");

				pycode += tr("        u[");
				pycode += QString::number(i);
				pycode += tr("] = ");
				pycode += N.rowName(i);
				pycode += tr(";\n");
			}
		}

		//print the rates
		pycode += tr("        rates = range(0,") + QString::number(rates.size()) + tr(");\n");
		for (i = 0; i < rates.size() && i < c; ++i)
		{
			code += tr("    rates[");
			code += QString::number(i);
			code += tr("] = model->");
			code += N.colName(i);
			code += tr(" = ");
			code += insertPrefix(handles,tr("model->"),rates[i],replaceDot);
			code += tr(";\n");

			pycode += tr("        rates[");
			pycode += QString::number(i);
			pycode += tr("] = ");
			pycode += N.colName(i);
			pycode += tr(" = ");
			pycode += rates[i];
			pycode += tr(";\n");
		}

		code += tr("}\n\ndouble TCstoic[] = { ");
		pycode << tr("        return rates;\n    TCstoic = ( ");


		for (i = 0; i < r; ++i)
			for (j=0; j < c; ++j)
			{
				code += toString( N.at(i,j) );
				pycode += QString::number( N.at(i,j) );

				if (i < (r-1) || j < (c-1))
				{
					code += tr(",");
					pycode += tr(",");
				}
			}

		code += tr("};\n\n");
		pycode += tr(");\n\n");
		
		//events
		
		if (eventTriggers.size() > 0)
		{		
			code += tr("int TCtriggers(int i, double time, double * y, void * data)\n{\n    TCmodel * model = (TCmodel*)data;\n");
			pycode += tr("TCtriggers(i,time, u)\n");
			for (i=0; i < eventTriggers.size(); ++i)
			{
				pycode += tr("    if (i == ");
				pycode += QString::number(i);
				pycode += tr("): return ");
				pycode += eventTriggers[i];
				pycode += tr(";\n");
			
				code += tr("    if (i == ");
				code += QString::number(i);
				code += tr(") return (");
				code += insertPrefix(handles,tr("model->"),eventTriggers[i],replaceDot);
				code += tr(");\n");
			}
			pycode += tr("    return 0\n");
			code += tr("    return 0;\n}\n");
		
			code += tr("void TCresponses(int i, double * u, void * data)\n{\n    TCmodel * model = (TCmodel*)data;\n");
			pycode += tr("TCresponses(u)\n");
			for (i=0; i < eventActions.size(); ++i)
			{
				pycode += tr("    if (i == ");
				pycode += QString::number(i);
				pycode += tr("): ");
				pycode += eventActions[i];
				pycode += tr(";\n");
			
				code += tr("    if (i == ");
				code += QString::number(i);
				code += tr(") { ");
				code += insertPrefix(handles,tr("model->"),eventActions[i],replaceDot);
				code += tr("; }\n");
			}
			
			for (i = 0; i < N.rows(); ++i)
			{
				code += tr("    u[");
				code += QString::number(i);
				code += tr("] = model->");
				code += N.rowName(i);
				code += tr(";\n");

				pycode += tr("        u[");
				pycode += QString::number(i);
				pycode += tr("] = ");
				pycode += N.rowName(i);
				pycode += tr(";\n");
			}
			
			pycode += tr("\n");
			code += tr("}\n");
		}
		else
		{
			code += tr("int (*TCtriggers)(int i, double time, double * y, void * data) = 0;\n");
			pycode += tr("TCtriggers(i,time, u): return;\n");
			
			code += tr("void (*TCresponses)(int i, double * u, void * data) = 0;\n");
			pycode += tr("TCresponses(u): return;\n");
		}
		

		//write simulation information
		code += tr("int TCvars = ");
		code += QString::number(r);
		code += tr(";\n");

		code += tr("int TCreactions = ");
		code += QString::number(c);
		code += tr(";\n");
		code += tr("int TCparams = ");
		code += QString::number(trueParams.size());
		code += tr(";\n");
		
		code += tr("int TCevents = ");
		code += QString::number(eventTriggers.size());
		code += tr(";\n");

		//variable names
		code += tr("char * TCvarnames[] = {\"");
		code += N.getRowNames().join("\",\"");
		code += tr("\",0};\n\n");

		pycode += tr("    TCvarnames = (\"");
		pycode += N.getRowNames().join("\",\"");
		pycode += tr("\");\n\n");

		//reaction names
		code += tr("char * TCreactionnames[] = {\"");
		code += N.getColNames().join("\",\"");
		code += tr("\",0};\n\n");

		pycode += tr("    TCreactionnames = (\"");
		pycode += N.getColNames().join("\",\"");
		pycode += tr("\");\n\n");

		code += tr("double TCinit[");
		code += QString::number(r);
		code += tr("];\n");
		pycode += tr("TCinit = range(0,");
		pycode += QString::number(r);
		pycode += tr(");\n");

		code += tr("\nvoid TCinitialize(TCmodel * model)\n{\n");
		pycode += tr("\n    defn TCinitialize():\n");

		for (i = 0; i < r; ++i)
		{
			code += tr("    TCinit[");
			code += QString::number(i);
			code += tr("] = model->");
			code += N.rowName(i);
			code += tr(";\n");

			pycode += tr("        ");
			pycode += tr(" = TCinit[");
			pycode += QString::number(i);
			pycode += tr("] = ");
			pycode += N.rowName(i);
			pycode += tr(";\n");
		}

		code += tr("}\n\n");
		pycode += tr("\n\n");
		
		/*for (i=0; i < namelessHandles.size(); ++i)
			if (namelessHandles[i])
				namelessHandles[i]->name = tr("");*/

		cfile.write(code.join("").toAscii());
		cfile.close();

		pyfile.write(pycode.join("").toAscii());
		pyfile.close();
		return 1;
	}

	QString ModelFileGenerator::insertPrefix(const QList<ItemHandle*>& handles, const QString& prefix, const QString& str, const QString& sep)
	{
		QString s = str;
		QString name;
		QList<ItemHandle*> lowerLevelItems;

		for (int i=0; i < handles.size(); ++i)
			if (handles[i])
			{
				name = handles[i]->fullName(sep);

				QRegExp regexp1(tr("^") + name + tr("$")),  //just old name
						regexp2(tr("^") + name + tr("([^A-Za-z0-9])")),  //oldname+(!letter/num)
						regexp3(tr("([^A-Za-z0-9_>])") + name + tr("$")), //(!letter/num)+oldname
						regexp4(tr("([^A-Za-z0-9_>])") + name + tr("([^A-Za-z0-9])")), //(!letter/num)+oldname+(!letter/num)
						regexp5(tr("([^A-Za-z0-9_>])max\\s*\\(")),
						regexp6(tr("([^A-Za-z0-9_>])min\\s*\\(")),
						regexp7(tr("([^A-Za-z0-9_>])abs\\s*\\("));

				s.replace(regexp1,prefix + name);
				s.replace(regexp2,prefix + name + tr("\\1"));
				s.replace(regexp3,tr("\\1") + prefix + name);
				s.replace(regexp4,tr("\\1") + prefix + name + tr("\\2"));
				s.replace(regexp5,tr("\\1fmax("));
				s.replace(regexp6,tr("\\1fmin("));
				s.replace(regexp7,tr("\\1fabs("));
			}
		return s;
	}
}
