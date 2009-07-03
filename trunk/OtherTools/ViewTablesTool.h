/****************************************************************************

 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)
 See COPYRIGHT.TXT

 This class adds the "events" data to each item in Tinkercell.

****************************************************************************/

#ifndef TINKERCELL_VIEWALLTABLESWIDGET_H
#define TINKERCELL_VIEWALLTABLESWIDGET_H

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
#include <QListView>
#include <QTextEdit>

#include "NodeGraphicsItem.h"
#include "DataTable.h"
#include "ItemHandle.h"
#include "Tool.h"
#include "MainWindow.h"

namespace Tinkercell
{

	class ViewTablesTool : public Tool
	{
		Q_OBJECT

	public:
		ViewTablesTool();
		bool setMainWindow(MainWindow * main);
		QSize sizeHint() const;

	public slots:
                void select();
                void deselect();
		void itemsSelected(GraphicsScene * scene, const QList<QGraphicsItem*>& list, QPointF , Qt::KeyboardModifiers );
		void itemsInserted(GraphicsScene * scene, const QList<QGraphicsItem*>& items, const QList<ItemHandle*>& handles);

	protected:

		QTextEdit * textEdit;
		QListWidget numericalTables;
		QListWidget textTables;
		void updateList();

	protected slots:
		void currentNumericalItemChanged(QListWidgetItem *, QListWidgetItem *);
		void currentTextItemChanged(QListWidgetItem *, QListWidgetItem *);

	private:

                class GraphicsItem2 : public Tool::GraphicsItem
                {
                    public:
                        GraphicsItem2(Tool * tool);
                        void setVisible(bool);
                };

                bool openedByUser;
                NodeGraphicsItem item;
                ItemHandle* itemHandle;
		QDockWidget * dockWidget;
                QTextCharFormat headerFormat, regularFormat;

	};


}

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

extern "C" MY_EXPORT void loadTCTool(Tinkercell::MainWindow * main);

#endif
