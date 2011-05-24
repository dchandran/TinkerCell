/****************************************************************************

 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)
 See COPYRIGHT.TXT

 This tool handles connections that represent wetlab experiments

****************************************************************************/

#ifndef TINKERCELL_WETLABMODULETOOL_H
#define TINKERCELL_WETLABMODULETOOL_H

#include <QtGui>
#include <QIcon>
#include <QPixmap>
#include <QString>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QAction>
#include <QFile>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QUndoCommand>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QSlider>
#include <QToolButton>
#include <QButtonGroup>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPair>
#include <QAbstractButton>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QLineEdit>
#include <QDir>
#include <QToolBar>
#include <QMessageBox>
#include <QButtonGroup>
#include <QDockWidget>
#include <QScrollArea>
#include <QSplashScreen>
#include <QDialog>

#include "GraphicsScene.h"
#include "NodeGraphicsItem.h"
#include "NodeGraphicsReader.h"
#include "NodeGraphicsWriter.h"
#include "NodesTree.h"
#include "ConnectionsTree.h"
#include "CatalogWidget.h"
#include "ItemHandle.h"
#include "Tool.h"

namespace Tinkercell
{
	/*! This class allows users to insert wet-lab experiment in the scene
	*/
	class TINKERCELLEXPORT WetLabTool : public Tool
	{
		Q_OBJECT

	public:
		WetLabTool();
		bool setMainWindow(MainWindow * main);

	private slots:
		void escapeSignal(const QWidget *);
		void itemsAboutToBeInserted (GraphicsScene* scene, QList<QGraphicsItem *>& items, QList<ItemHandle*>& handles, QList<QUndoCommand*>&);
		void itemsAboutToBeRemoved(GraphicsScene * scene, QList<QGraphicsItem*>& item, QList<ItemHandle*>& handles, QList<QUndoCommand*>&);
		void toolLoaded (Tool * tool);
		void itemsInserted(NetworkHandle * network, const QList<ItemHandle*>& handles);
		void mouseDoubleClicked (GraphicsScene * scene, QPointF point, QGraphicsItem *, Qt::MouseButton, Qt::KeyboardModifiers modifiers);
		void keyPressed(GraphicsScene*,QKeyEvent *);
		void sceneClicked(GraphicsScene *scene, QPointF point, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
		void itemsDropped(GraphicsScene *, const QString&, const QPointF&);
		void mouseMoved(GraphicsScene* scene, QGraphicsItem*, QPointF point, Qt::MouseButton, Qt::KeyboardModifiers, QList<QGraphicsItem*>& items);
		void itemsRenamed(NetworkHandle * window, const QList<ItemHandle*>& items, const QList<QString>& oldnames, const QList<QString>& newnames);

		void labButtonPressed(const QString&);
		void labButtonClicked (QAbstractButton *);
	};


}


#endif
