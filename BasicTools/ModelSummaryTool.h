/****************************************************************************

Copyright (c) 2008 Deepak Chandran
Contact: Deepak Chandran (dchandran1@gmail.com)
See COPYRIGHT.TXT

This class adds the "attributes" data to each item in Tinkercell.
Two types of attributes are added -- "Numerical Attributes" and "Text Attributes".
Attributes are essentially a <name,value> pair that are used to characterize an item.

The BasicInformationTool also comes with two GraphicalTools, one for text attributes and one
for numerical attributes. The buttons are drawn as NodeGraphicsItems using the datasheet.xml and
textsheet.xml files that define the NodeGraphicsItems.

****************************************************************************/

#ifndef TINKERCELL_MODELSUMMARYTOOL_H
#define TINKERCELL_MODELSUMMARYTOOL_H

#include <stdlib.h>
#include <QtGui>
#include <QIcon>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHash>
#include <QGroupBox>
#include <QUndoCommand>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QSlider>
#include <QToolButton>
#include <QButtonGroup>
#include <QTableView>
#include <QTabWidget>

#include "NodeGraphicsItem.h"
#include "DataTable.h"
#include "ItemHandle.h"
#include "Tool.h"
#include "MainWindow.h"
#include "NetworkWindow.h"
#include "SpinBoxDelegate.h"

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

namespace Tinkercell
{
	class MY_EXPORT ModelSummaryTool : public Tool
	{
		Q_OBJECT;

	public:
		ModelSummaryTool();
		bool setMainWindow(MainWindow * main);
		QSize sizeHint() const;

	signals:
		void aboutToDisplayModel(const QList<ItemHandle*>& items, QHash<QString,qreal>& constants, QHash<QString,QString>& equations);
		void displayModel(QTabWidget& widgets, const QList<ItemHandle*>& items, QHash<QString,qreal>& constants, QHash<QString,QString>& equations);
		void dataChanged(const QList<ItemHandle*>&);

	public slots:
		void select(int i=0);
		void deselect(int i=0);
		void sceneClosing(NetworkWindow * , bool *);
		void updateToolTips(const QList<ItemHandle*>&);
		void itemsInserted(NetworkWindow *, const QList<ItemHandle*>& handles);
		void itemsInserted(GraphicsScene * scene, const QList<QGraphicsItem*>& , const QList<ItemHandle*>& );
		void keyPressed(GraphicsScene* scene,QKeyEvent * keyEvent);
		void mouseDoubleClicked(GraphicsScene* scene, QPointF, QGraphicsItem* item, Qt::MouseButton, Qt::KeyboardModifiers modifiers);
		void itemsSelected(GraphicsScene * scene, const QList<QGraphicsItem*>& items, QPointF point, Qt::KeyboardModifiers modifiers);
		void setValue(int,int);

	protected:
		QGroupBox groupBox;
		QTableWidget tableWidget;
		TextComboDoubleDelegate delegate;
		QWidget * currentWidget;
		QTabWidget * tabWidget;

	private slots:
		void currentChanged ( int index );
		void updateTables();
		void fixedAction();

	private:
		bool openedByUser;
		NodeGraphicsItem item;
		QDockWidget * dockWidget;
		QList<ItemHandle*> itemHandles;
		QAction * toggleFixedAction, *separator;

		friend class VisualTool;
	};


}

extern "C" MY_EXPORT void loadTCTool(Tinkercell::MainWindow * main);

#endif
