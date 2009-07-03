/****************************************************************************

 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)
 See COPYRIGHT.TXT
 
 Automatically manage gene regulatory network rates and parameters

****************************************************************************/

#ifndef TINKERCELL_AUTOMATICGENEREGULATORYMODELTOOL_H
#define TINKERCELL_AUTOMATICGENEREGULATORYMODELTOOL_H

#include <QtGui>
#include <QtDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTimeLine>
#include <QTextEdit>
#include <QTextCursor>

#include "Core/NodeGraphicsItem.h"
#include "Core/NodeGraphicsReader.h"
#include "Core/NodeGraphicsWriter.h"
#include "NodesTree/NodesTree.h"
#include "Core/ItemHandle.h"
#include "Core/Tool.h"

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

namespace Tinkercell
{

	class AutoGeneRegulatoryTool_FtoS : public QObject
	{
		Q_OBJECT
		signals:
			void partsIn(QSemaphore*, ItemHandle*, QList<ItemHandle*>* parts);
			void partsUpstream(QSemaphore*, ItemHandle*, QList<ItemHandle*>* parts);
			void partsDownstream(QSemaphore*, ItemHandle*, QList<ItemHandle*>* parts);
			
		public slots:
			Array partsIn(OBJ);
			Array partsUpstream(OBJ);
			Array partsDownstream(OBJ);
	};


	class MY_EXPORT AutoGeneRegulatoryTool : public Tool
	{
		Q_OBJECT

	public:
		AutoGeneRegulatoryTool();
		bool setMainWindow(MainWindow * main);
	
	public slots:
		void itemsInserted(NetworkWindow* , const QList<ItemHandle*>& handles);
		void itemsRemoved(GraphicsScene *, QList<QGraphicsItem*>&, QList<ItemHandle*>&);
		void itemsMoved(GraphicsScene * scene, const QList<QGraphicsItem*>& item, const QList<QPointF>& distance, Qt::KeyboardModifiers modifiers);
		void nodeCollided(const QList<QGraphicsItem*>& , NodeGraphicsItem * , const QList<QPointF>& , Qt::KeyboardModifiers );
		void toolLoaded(Tool*);
		void itemsSelected(GraphicsScene *,const QList<QGraphicsItem*>&, QPointF, Qt::KeyboardModifiers);
		void autoTFTriggered(const QString&);
		void autoDegradationTriggered();
		void autoPhosphateTriggered();
		void autoGeneProductTriggered();
		void autoTFTriggeredUp();
		void autoTFTriggeredDown();
		void insertmRNAstep();
		void autoAssignRates(QList<NodeHandle*>&);
		
	signals:
		void alignCompactHorizontal();
		void setMiddleBox(int,const QString&);
	
	private slots:
	
		void setupFunctionPointers( QLibrary * );
		void partsIn(QSemaphore*, ItemHandle*, QList<ItemHandle*>* parts);
		void partsUpstream(QSemaphore*, ItemHandle*, QList<ItemHandle*>* parts);
		void partsDownstream(QSemaphore*, ItemHandle*, QList<ItemHandle*>* parts);
	
	private:
		QTimeLine glowTimer;
		void connectPlugins();
		QAction autoTFUp, autoTFDown;
		QAction autoDegradation;
		QAction autoGeneProduct;
		QAction mRNAstep;
		QAction autoPhosphate;
		QAction * separator;
		bool doAssignment;
		
		static Array _partsIn(OBJ);
		static Array _partsUpstream(OBJ);
		static Array _partsDownstream(OBJ);
		static AutoGeneRegulatoryTool_FtoS fToS;
		
	public:
		static void findAllPart(GraphicsScene*,NodeGraphicsItem*,const QString&,QList<ItemHandle*>&,bool,const QStringList&, bool stopIfElongation = false);
		static QString hillEquation(NodeHandle *,ItemHandle* exclude = 0);
		static QString hillEquation(QList<ConnectionHandle*> connections, QList<NodeHandle*>& activators, QList<NodeHandle*> repressors);
	};


}

extern "C" MY_EXPORT void loadTCTool(Tinkercell::MainWindow * main);


#endif
