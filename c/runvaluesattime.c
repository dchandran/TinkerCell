/****************************************************************************

Asks user for a parameter or variable name (string), and then generates a code that generates
the steady state table by changing this value.

****************************************************************************/

#include "cvodesim.h"
#include "TC_api.h"

static char * selected_var;
static char ** allNames = 0;
void run(Matrix input);
void setup();

void unload()
{
	if (allNames)
		TCFreeChars(allNames);
}

void loadAllNames()
{
	int i,len;
	Matrix params;
	char ** names;
	Array A = tc_allItems();

	if (allNames)
		TCFreeChars(allNames);

	allNames = 0;

	if (A && A[0])
	{
		params = tc_getModelParameters(A);
		names = tc_getNames(tc_itemsOfFamilyFrom("Species\0",A));
		len = 0;
		while (names[len]) ++len;
		allNames = malloc((len+params.rows+1)*sizeof(char*));
		for (i=0; i < params.rows; ++i) allNames[i] = params.rownames[i];
		for (i=0; i < len; ++i) allNames[i+params.rows] = names[i];
		allNames[(len+params.rows)] = 0;
		params.rownames = 0;
		TCFreeMatrix(params);
		TCFreeArray(A);
	}
}

void callback()
{
	loadAllNames();
	tc_addInputWindowOptions("At Time T",2, 0, allNames);
}

void tc_main()
{
	allNames = 0;

	selected_var = "";
	//add function to menu. args : function, name, description, category, icon file, target part/connection family, in functions list?, in context menu?
	tc_addFunction(&setup, "Values at time=T0", "uses repeated simulation to compute state of system at the given time", "Parameter scan", "Plugins/c/steadystate.PNG", "", 1, 0, 0);
	tc_callback(&callback);
	tc_callWhenExiting(&unload);
}

void setup()
{
	Matrix m;
	char * cols[] = { "value", 0 };
	char * rows[] = { "model", "simulation", "variable", "start", "end", "increments", "time", 0};
	double values[] = { 0.0, 0.0, 0.0, 0.0, 10, 0.5 , 100.0  };
	char * options1[] = { "Full model", "Selected only", 0};
	char * options2[] = { "ODE", "Stochastic", 0  }; //null terminated -- very very important

	loadAllNames();

	m.rows = 7;
	m.cols = 1;
	m.colnames = cols;
	m.rownames = rows;
	m.values = values;

	//tc_createInputWindow(m,"dlls/runvaluesattime","run2","At Time T");
	tc_createInputWindow(m,"At Time T",&run);
	tc_addInputWindowOptions("At Time T",0, 0, options1);
	tc_addInputWindowOptions("At Time T",1, 0, options2);
	tc_addInputWindowOptions("At Time T",2, 0, allNames);

	return;
}

void run(Matrix input)
{
	double start = 0.0, end = 50.0;
	double dt = 0.1, time = 100.0;
	int doStochastic = 0;
	int selection = 0, index = 0, sz = 0;
	Array A;
	char * param;
	FILE * out;

	if (input.cols > 0)
	{
		if (input.rows > 0)
			selection = (int)valueAt(input,0,0);
		if (input.rows > 1)
			doStochastic = (int)(valueAt(input,1,0) > 0);
		if (input.rows > 2)
			index = valueAt(input,2,0);
		if (input.rows > 3)
			start = valueAt(input,3,0);
		if (input.rows > 4)
			end = valueAt(input,4,0);
		if (input.rows > 5)
			dt = valueAt(input,5,0);
		if (input.rows > 6)
			time = valueAt(input,6,0);
	}

	if (selection > 0)
	{
		A = tc_selectedItems();
		if (A[0] == 0)
		{
			TCFreeArray(A);
			A = tc_allItems();
		}
	}
	else
	{
		A = tc_allItems();
	}

	sz = (int)((end - start) / dt);

	if (A[0] != 0)
	{
		tc_writeModel( "timet", A );
	}
	else
	{
		TCFreeArray(A);
		return;
	}

	TCFreeArray(A);

	if (index < 0)
	{
		tc_print("steady state: no variable selected\0");
		return;
	}

	param = allNames[index]; //the parameter to vary
	selected_var = param;

	out = fopen("timet.c","a");

	fprintf( out , "#include \"TC_api.h\"\n#include \"cvodesim.h\"\n#include \"ssa.h\"\n\n\
				   void run(Matrix input) \n\
				   {\n   initMTrand();\n   Matrix dat;\n" );

	fprintf( out, "   dat.rows = (int)((%lf-%lf)/%lf);\n\
				  dat.cols = 1+TCvars;\n\
				  double * values = malloc(dat.cols * dat.rows * sizeof(double));\n\
				  TCmodel * model = (TCmodel*)malloc(sizeof(TCmodel));\n\
				  (*model) = TC_initial_model;\n\
				  dat.values = values;\n\
				  dat.rownames = 0;\n\
				  dat.colnames = malloc( (1+TCvars) * sizeof(char*) );\n\
				  int i,j;\n\
				  for(i=0; i<TCvars; ++i) dat.colnames[1+i] = TCvarnames[i];\n\
				  dat.colnames[0] = \"%s\";\n",end,start,dt,param);

	fprintf( out, "\n\
				  for (i=0; i < dat.rows; ++i)\n\
				  {\n\
				      (*model) = TC_initial_model;\n\
					  model->%s = %lf + i * %lf;\n\
					  valueAt(dat,i,0) = model->%s;\n\
					  TCinitialize(model);\n\
					  double * y = 0;\n\
					  int sz = (int)(%lf*10.0);\n\
					  if (%i)\n\
						y = SSA(TCvars, TCreactions, TCstoic, &(TCpropensity), TCinit, 0, %lf, 200000, &sz, (void*)model );\n\
					  else \n\
						y = ODEsim2(TCvars, TCreactions, TCstoic, &(TCpropensity),TCinit, 0, %lf, 0.1, (void*)model );\n\
					  if (y)\n\
					  {\n\
						for (j=0; j<TCvars; ++j)\n\
						valueAt(dat,i,j+1) = getValue(y,1+TCvars,sz-1,j+1);\n\
						free(y);\n\
					  }\n\
					  else\n\
					  {\n\
						for (j=0; j<TCvars; ++j)\n\
						valueAt(dat,i,j+1) = 0;\n\
					  }\n\
					  tc_showProgress(\"At Time T\",(100*i)/dat.rows);\n\
				  }\n\
				  free(model);\n\
				  tc_plot(dat,0,\"At time=%lf\",0);\n\
				  free(dat.colnames);\n     free(dat.values);\n}\n",param,start,dt,param,time,doStochastic,time,time,time);

	fclose(out);

	tc_compileBuildLoad("timet.c -lodesim -lssa\0","run\0","At Time T\0");
	return;
}

