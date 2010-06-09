/****************************************************************************

Copyright (c) 2008 Deepak Chandran
Contact: Deepak Chandran (dchandran1@gmail.com)
See COPYRIGHT.TXT

This file contains a collection of commands that perform simple operations that can be redone and undone.

****************************************************************************/

#include "NodeGraphicsItem.h"
#include "NodeGraphicsReader.h"
#include "ConnectionGraphicsItem.h"
#include "TextGraphicsItem.h"
#include "Tool.h"
#include "GraphicsScene.h"
#include "TextEditor.h"
#include "NetworkHandle.h"
#include "UndoCommands.h"
#include "ConsoleWindow.h"
#include <QRegExp>
#include <QStringList>
#include <QDebug>

namespace Tinkercell
{

	MoveCommand::MoveCommand(GraphicsScene * scene, const QList<QGraphicsItem *>& items, const QList<QPointF>& amounts)
		: QUndoCommand(QObject::tr("items moved by ..."))
	{
		graphicsScene = scene;
		graphicsItems.clear();

		change.clear();
		graphicsItems.clear();

		ConnectionGraphicsItem * connection = 0;
		NodeGraphicsItem * node = 0;
		for (int i=0; i < items.size(); ++i)
		{
			if ((connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(items[i])))
			{
				QList<QGraphicsItem*> cps = connection->controlPointsAsGraphicsItems();
				for (int j=0; j < cps.size(); ++j)
				{
					if (!graphicsItems.contains(cps[j]))
					{
						graphicsItems += cps[j];
						change += amounts[i];
					}
				}
			}
			else
			{
				if (!graphicsItems.contains(items[i]))
				{
					graphicsItems += items[i];
					change += amounts[i];
				}

				if ((node = qgraphicsitem_cast<NodeGraphicsItem*>(items[i])))
				{
					QVector<NodeGraphicsItem::ControlPoint*> cps = node->boundaryControlPoints;
					for (int j=0; j < cps.size(); ++j)
					{
						if (!graphicsItems.contains(cps[j]))
						{
							graphicsItems += cps[j];
							change += amounts[i];
						}
					}
				}
			}
		}
	}

	MoveCommand::MoveCommand(GraphicsScene * scene, const QList<QGraphicsItem*>& items, const QPointF& amount)
		: QUndoCommand(QObject::tr("items moved by (") + QString::number(amount.x()) + QObject::tr(",") + QString::number(amount.y()) + QObject::tr(")"))
	{
		graphicsScene = scene;
		change.clear();
		ConnectionGraphicsItem * connection = 0;
		NodeGraphicsItem * node = 0;
		for (int i=0; i < items.size(); ++i)
		{
			if ((connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(items[i])))
			{
				QList<QGraphicsItem*> cps = connection->controlPointsAsGraphicsItems();
				for (int j=0; j < cps.size(); ++j)
				{
					if (!graphicsItems.contains(cps[j]))
					{
						graphicsItems += cps[j];
						change += amount;
					}
				}
			}
			else
			{
				if (!graphicsItems.contains(items[i]))
				{
					graphicsItems += items[i];
					change += amount;
				}
				if ((node = qgraphicsitem_cast<NodeGraphicsItem*>(items[i])))
				{
					QVector<NodeGraphicsItem::ControlPoint*> cps = node->boundaryControlPoints;
					for (int j=0; j < cps.size(); ++j)
					{
						if (!graphicsItems.contains(cps[j]))
						{
							graphicsItems += cps[j];
							change += amount;
						}
					}
				}
			}
		}
	}

	MoveCommand::MoveCommand(GraphicsScene * scene, QGraphicsItem * item, const QPointF& amount)
		: QUndoCommand(QObject::tr("items moved by (") + QString::number(amount.x()) + QObject::tr(",") + QString::number(amount.y()) + QObject::tr(")"))
	{
		graphicsScene = scene;
		graphicsItems.clear();
		change.clear();
		ConnectionGraphicsItem * connection = 0;
		NodeGraphicsItem * node = 0;

		if ((connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(item)))
		{
			QList<QGraphicsItem*> cps = connection->controlPointsAsGraphicsItems();
			for (int j=0; j < cps.size(); ++j)
			{
				if (!graphicsItems.contains(cps[j]))
				{
					graphicsItems += cps[j];
					change += amount;
				}
			}
		}
		else
		{
			if (!graphicsItems.contains(item))
			{
				graphicsItems += item;
				change += amount;
			}
			if ((node = qgraphicsitem_cast<NodeGraphicsItem*>(item)))
			{
				QVector<NodeGraphicsItem::ControlPoint*> cps = node->boundaryControlPoints;
				for (int j=0; j < cps.size(); ++j)
				{
					if (!graphicsItems.contains(cps[j]))
					{
						graphicsItems += cps[j];
						change += amount;
					}
				}
			}
		}
	}

	void MoveCommand::refreshAllConnectionIn(const QList<QGraphicsItem*>& moving)
	{
		QList<ConnectionGraphicsItem*> connections;
		ConnectionGraphicsItem::ControlPoint* cgp = 0;
		for (int i=0; i < moving.size(); ++i)
			if ((cgp = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(moving[i]))
				&& cgp->connectionItem
				&& !connections.contains(cgp->connectionItem))
				connections += cgp->connectionItem;
			else
			{
				QList<QGraphicsItem*> children = moving[i]->childItems();
				for (int i=0; i < children.size(); ++i)
					if ((cgp = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(children[i]))
						&& cgp->connectionItem
						&& !connections.contains(cgp->connectionItem))
						connections += cgp->connectionItem;
			}

			for (int i=0; i < connections.size(); ++i)
				connections[i]->refresh();
	}

	void MoveCommand::redo()
	{
		QList<ControlPoint*> controlPoints;
		ConnectionGraphicsItem::ControlPoint * ccp = 0;
		NodeGraphicsItem::ControlPoint * pcp = 0;
		QGraphicsItem * parent = 0;
		for (int i=0; i<graphicsItems.size() && i<change.size(); ++i)
			if (graphicsItems[i])
			{
				parent = graphicsItems[i]->parentItem();
				graphicsItems[i]->setParentItem(0);

				graphicsItems[i]->moveBy(change[i].x(),change[i].y());
				if ((ccp = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(graphicsItems[i])))
					controlPoints += ccp;
				else
					if ((pcp = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(graphicsItems[i])) &&
						!graphicsItems.contains(pcp->nodeItem))
						controlPoints += pcp;

				graphicsItems[i]->setParentItem(parent);
			}
		for (int i=0; i < controlPoints.size(); ++i)
		{
			controlPoints[i]->sideEffect();
		}
		refreshAllConnectionIn(graphicsItems);
	}

	void MoveCommand::undo()
	{
		QList<ControlPoint*> controlPoints;
		ConnectionGraphicsItem::ControlPoint * ccp = 0;
		NodeGraphicsItem::ControlPoint * pcp = 0;
		QGraphicsItem * parent = 0;
		for (int i=0; i<graphicsItems.size() && i<change.size(); ++i)
			if (graphicsItems[i])
			{
				parent = graphicsItems[i]->parentItem();
				graphicsItems[i]->setParentItem(0);

				graphicsItems[i]->moveBy(-change[i].x(), -change[i].y());
				if ((ccp = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(graphicsItems[i])))
					controlPoints += ccp;
				else
					if ((pcp = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(graphicsItems[i])) &&
						!graphicsItems.contains(pcp->nodeItem))
						controlPoints += pcp;

				graphicsItems[i]->setParentItem(parent);
			}
			for (int i=0; i < controlPoints.size(); ++i)
			{
				controlPoints[i]->sideEffect();
			}
			refreshAllConnectionIn(graphicsItems);
	}
	
	InsertHandlesCommand::~InsertHandlesCommand()
	{
		for (int i=0; i < items.size(); ++i)
			if (items[i] && !MainWindow::invalidPointers.contains((void*)items[i]))
			{
				delete items[i];
				MainWindow::invalidPointers[ (void*)items[i] ] = true;
				items[i] = 0;
			}
		if (renameCommand)
			delete renameCommand;
	}

	InsertHandlesCommand::InsertHandlesCommand(TextEditor * textEditor, const QList<ItemHandle*> & list)
	{
		QStringList s;
		ItemHandle * h = 0;
		for (int i=0; i < list.size(); ++i)
			if (h = list[i])
				s << h->name;
		setText(s.join(QObject::tr(",")) + QObject::tr(" added"));
		this->textEditor = textEditor;
		items = list;
		renameCommand = 0;
	}

	InsertHandlesCommand::InsertHandlesCommand(TextEditor * textEditor, ItemHandle * h)
	{
		if (h)
			setText(h->name + QObject::tr(" added"));
		else
			setText(QObject::tr("items added"));
		this->textEditor = textEditor;
		items << h;
		renameCommand = 0;
	}

	void InsertHandlesCommand::redo()
	{
		if (textEditor && textEditor->network)
		{
			QHash<QString,ItemHandle*>& allNames = textEditor->network->symbolsTable.uniqueItems;
			QHash<QString, QPair<ItemHandle*,QString> >& allData = textEditor->network->symbolsTable.uniqueData;
			
			QStringList oldNames, newNames;
			QList<ItemHandle*> nameChangeHandles;
			QString s0,s1;
			bool isNum;
			
			QList<ItemHandle*>& list = textEditor->items();
			
			while (parentHandles.size() < items.size()) parentHandles += 0;
			
			for (int i=0; i < items.size(); ++i)
			{
				if (items[i] && !list.contains(items[i]))
				{
					if (parentHandles.size() > i && !MainWindow::invalidPointers.contains(parentHandles[i]))
						items[i]->setParent(parentHandles[i],false);					

					items[i]->network = textEditor->network;
					list << items[i];
					if (!renameCommand && !nameChangeHandles.contains(items[i]))
					{
						nameChangeHandles << items[i];
						s0 = s1 = items[i]->fullName();
						if (newNames.contains(s0) || 
							(allNames.contains(s0) && items[i] != allNames[s0] && !items[i]->parent) ||
							(allData.contains(s0) && items[i] != allData[s0].first && !items[i]->parent))
						{
							oldNames << items[i]->fullName();
							
							isNum = s0[ s0.length()-1 ].isNumber();
							if (isNum)
								s0 = s0.left( s0.length()-1 );
							int k=0;
							s1 = s0 + QString::number(k);
							while (allNames.contains(s1))
								s1 = s0 + QString::number(++k);								
							newNames << s1;
						}
					}
				}
			}
			
			if (!renameCommand && !newNames.isEmpty())
			{
				QList<ItemHandle*> allHandles;
				for (int i=0; i < items.size(); ++i)
					if (items[i])
						allHandles << items[i] << items[i]->allChildren();
					
				renameCommand = new RenameCommand(QString("rename"),textEditor->network,allHandles,oldNames,newNames);
			}
			
			if (renameCommand)
				renameCommand->redo();
		}
	}

	void InsertHandlesCommand::undo()
	{
		if (textEditor)
		{
			QList<ItemHandle*>& list = textEditor->items();
			for (int i=0; i < items.size(); ++i)
				if (items[i] && list.contains(items[i]))
				{
					items[i]->network = 0;
					list.removeAll(items[i]);
				}
			
			while (parentHandles.size() < items.size()) parentHandles += 0;
			
			for (int i=0; i < items.size(); ++i)
			{
				parentHandles[i] = items[i]->parent;
				items[i]->setParent(0,false);
			}
			
			if (renameCommand)
				renameCommand->undo();
		}
	}

	RemoveHandlesCommand::RemoveHandlesCommand(TextEditor * editor, const QList<ItemHandle*> & list) : changeDataCommand(0)
	{
		QStringList s;
		ItemHandle * h = 0;
		for (int i=0; i < list.size(); ++i)
			if (h = list[i])
				s << h->name;
		setText(s.join(QObject::tr(",")) + QObject::tr(" removed"));
		textEditor = editor;
		items = list;
	}

	RemoveHandlesCommand::RemoveHandlesCommand(TextEditor * editor, ItemHandle * h): changeDataCommand(0)
	{
		if (h)
			setText(h->name + QObject::tr(" removed"));
		else
			setText(QObject::tr("items removed"));
		textEditor = editor;
		items << h;
	}

	void RemoveHandlesCommand::undo()
	{
		if (textEditor)
		{
			QList<ItemHandle*>& list = textEditor->items();
			for (int i=0; i < items.size(); ++i)
				if (items[i] && !list.contains(items[i]))
				{
					if (parentHandles.size() > i && !MainWindow::invalidPointers.contains(parentHandles[i]))
					{
						items[i]->setParent(parentHandles[i],false);
					}
					items[i]->network = textEditor->network;
					list << items[i];
				}
		}
	}

	void RemoveHandlesCommand::redo()
	{
		if (textEditor)
		{
			QList<ItemHandle*>& list = textEditor->items();
			
			while (parentHandles.size() < items.size()) parentHandles += 0;
			
			for (int i=0; i < items.size(); ++i)
				if (items[i] && list.contains(items[i]))
				{
					parentHandles[i] = items[i]->parent;
					items[i]->setParent(0,false);

					items[i]->network = 0;
					list.removeAll(items[i]);
				}
		}
	}

	InsertGraphicsCommand::InsertGraphicsCommand(const QString& name, GraphicsScene * scene, QGraphicsItem * item)
		: QUndoCommand(name)
	{
		graphicsScene = scene;
		graphicsItems.clear();
		
		item = getGraphicsItem(item);
		graphicsItems.append(item);
		handles.clear();
		handles += getHandle(item);
		renameCommand = 0;
	}

	InsertGraphicsCommand::InsertGraphicsCommand(const QString& name, GraphicsScene * scene, const QList<QGraphicsItem*>& items)
		: QUndoCommand(name)
	{
		graphicsScene = scene;
		handles.clear();
		QGraphicsItem * item;
		for (int i=0; i < items.size(); ++i)
			if (item = getGraphicsItem(items[i]))
			{
				graphicsItems += item;
				parentGraphicsItems += item->parentItem();	
				handles += getHandle(item);
			}
		renameCommand = 0;
	}

	void InsertGraphicsCommand::redo()
	{
		QList<ConnectionGraphicsItem*> connections;
		ConnectionGraphicsItem * connection;
		bool isNum;
		
		if (graphicsScene && graphicsScene->network)
		{
			QHash<QString,ItemHandle*>& allNames = graphicsScene->network->symbolsTable.uniqueItems;
			QHash<QString, QPair<ItemHandle*,QString> >& allDataNames = graphicsScene->network->symbolsTable.uniqueData;
			
			QStringList newNames, oldNames;
			QList<ItemHandle*> nameChangeHandles;
			QString s0,s1;
			
			for (int i=0; i<graphicsItems.size(); ++i)
			{
				if (graphicsItems[i] && graphicsItems[i]->scene() != graphicsScene)
				{
					graphicsScene->addItem(graphicsItems[i]);
					if ((connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i])))
					{
						connections << connection;
					}

					if (parentGraphicsItems.size() > i &&
						parentGraphicsItems[i] &&
						parentGraphicsItems[i]->scene() == graphicsScene)
						graphicsItems[i]->setParentItem(parentGraphicsItems[i]);
						
					if (handles.size() > i)
					{
						setHandle(graphicsItems[i],handles[i]);
						if (handles[i] && !renameCommand && !nameChangeHandles.contains(handles[i]))
						{
							if (parentHandles.size() > i && !MainWindow::invalidPointers.contains(parentHandles[i]))
								handles[i]->setParent(parentHandles[i],false);
							
							handles[i]->network = graphicsScene->network;
							nameChangeHandles << handles[i];
							s0 = s1 = handles[i]->fullName();
							if (newNames.contains(s0) || 
								(allNames.contains(s0) && handles[i] != allNames[s0] && !handles[i]->parent) ||
								(allDataNames.contains(s0) && handles[i] != allDataNames[s0].first && !handles[i]->parent))
							{
								oldNames << s0;
								
								isNum = s0[ s0.length()-1 ].isNumber();
								if (isNum)
									s0 = s0.left( s0.length()-1 );
								int k=0;
								s1 = s0 + QString::number(k);
								while (allNames.contains(s1))
									s1 = s0 + QString::number(++k);
								
								newNames << s1;
							}
						}
					}
				}
			}
			
			if (!renameCommand && !newNames.isEmpty())
			{
				QList<ItemHandle*> allHandles;
				for (int i=0; i < handles.size(); ++i)
					if (handles[i])
						allHandles << handles[i] << handles[i]->allChildren();
					
				renameCommand = new RenameCommand(QString("rename"),graphicsScene->network,allHandles,oldNames,newNames);
			}
			
			if (renameCommand)
				renameCommand->redo();
		}

		for (int i=0; i < connections.size(); ++i)
		{
			connection = connections[i];
			QList<QGraphicsItem*> arrows = connection->arrowHeadsAsGraphicsItems();
			for (int j=0; j < arrows.size(); ++j)
				if (arrows[j] && arrows[j]->scene() != graphicsScene)
					graphicsScene->addItem(arrows[j]);
			connection->refresh();
			connection->setControlPointsVisible(false);
		}
	}

	void InsertGraphicsCommand::undo()
	{
		ConnectionGraphicsItem * connection = 0;
		if (graphicsScene)
		{
			for (int i=0; i<graphicsItems.size(); ++i)
			{
				if (graphicsItems[i] && graphicsItems[i]->scene() == graphicsScene)
				{
					while (parentGraphicsItems.size() <= i) parentGraphicsItems << 0;
					while (handles.size() <= i) handles << 0;
					
					if (handles[i])
						handles[i]->network = 0;

					parentGraphicsItems[i] = graphicsItems[i]->parentItem();

					if (handles[i] != getHandle(graphicsItems[i]))
					{
						if (handles[i])
							handles << handles[i];
						handles[i] = getHandle(graphicsItems[i]);
					}

					graphicsItems[i]->setParentItem(0);
					graphicsScene->removeItem(graphicsItems[i]);

					if ((connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i])))
					{
						QList<QGraphicsItem*> arrows = connection->arrowHeadsAsGraphicsItems();
						for (int j=0; j < arrows.size(); ++j)
							if (arrows[j] && arrows[j]->scene())
								arrows[j]->scene()->removeItem(arrows[j]);
					}
					
					if (handles[i] && !handles[i]->parent)
					{
						setHandle(graphicsItems[i],0);
					}
				}
			}
			
			while (parentHandles.size() < handles.size()) parentHandles += 0;
			
			for (int i=0; i < handles.size(); ++i)
			{
				parentHandles[i] = handles[i]->parent;
				handles[i]->setParent(0,false);
			}
			
			if (renameCommand)
				renameCommand->undo();
		}
	}
	
	InsertGraphicsCommand::~InsertGraphicsCommand()
	{
		ItemHandle * handle = 0;
		
		for (int i=0; i < handles.size(); ++i)
		{
			if (!MainWindow::invalidPointers.contains( (void*)handles[i]) && handles[i])
			{
			    delete handles[i];
				MainWindow::invalidPointers[ (void*) handles[i] ] = true;
			}
		}
		handles.clear();

        ConnectionGraphicsItem * connection;
        NodeGraphicsItem * node;

		for (int i=0; i < graphicsItems.size(); ++i)
		{
			if (graphicsItems[i] && !MainWindow::invalidPointers.contains((void*)graphicsItems[i]))
			{
				if ((handle = getHandle(graphicsItems[i])) && !MainWindow::invalidPointers.contains((void*)handle))
				{
					MainWindow::invalidPointers[ (void*) handle ] = true;
					delete handle;
				}

			    if (graphicsItems[i]->parentItem())
					graphicsItems[i]->setParentItem(0);

				if (graphicsItems[i]->scene())
					graphicsItems[i]->scene()->removeItem(graphicsItems[i]);
				
				MainWindow::invalidPointers[ (void*) graphicsItems[i] ] = true;
				delete graphicsItems[i];
			}
		}

		graphicsItems.clear();
		
		if (renameCommand)
			delete renameCommand;
	}

	RemoveGraphicsCommand::~RemoveGraphicsCommand()
	{
		/*
		if (graphicsScene)
		for (int i=0; i < graphicsItems.size(); ++i)
		if (!graphicsScene->items().contains(graphicsItems[i]))
		graphicsScene->addItem(graphicsItems[i]);
		*/
	}

	RemoveGraphicsCommand::RemoveGraphicsCommand(const QString& name, GraphicsScene * scene, QGraphicsItem * item)
		: QUndoCommand(name), changeDataCommand(0)
	{
		graphicsScene = scene;
		graphicsItems.clear();
		graphicsItems.append( getGraphicsItem(item) );

		itemHandles.append(getHandle(item));
		if (item)
			itemParents.append(item->parentItem());
		else
			itemParents.append(0);
	}

	RemoveGraphicsCommand::RemoveGraphicsCommand(const QString& name, GraphicsScene * scene, const QList<QGraphicsItem*>& items)
		: QUndoCommand(name), changeDataCommand(0)
	{
		graphicsScene = scene;
		graphicsItems.clear();
		itemParents.clear();
		
		QGraphicsItem * item;
		
		for (int i=0; i < items.size(); ++i)
			if ( (item = getGraphicsItem(items[i]) ) )
			{
				graphicsItems.append(item);
				itemHandles.append(getHandle(item));
				itemParents.append(item->parentItem());
			}
	}

	void RemoveGraphicsCommand::redo()
	{
		if (!graphicsScene) return;
		for (int i=0; i<graphicsItems.size(); ++i)
		{
			if (graphicsItems[i] && graphicsItems[i]->scene() == graphicsScene)
			{
				graphicsScene->removeItem(graphicsItems[i]);
				NodeGraphicsItem * node = qgraphicsitem_cast<NodeGraphicsItem*>(graphicsItems[i]);
				if (node)
				{
					node->setBoundingBoxVisible(false);
				}
				else
				{
					ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
					if (connection)
					{
						connection->setControlPointsVisible(false);
						for (int j=0; j < connection->curveSegments.size(); ++j)
						{
							if (connection->curveSegments[j].arrowStart && connection->curveSegments[j].arrowStart->scene() == graphicsScene)
								graphicsScene->removeItem(connection->curveSegments[j].arrowStart);
							if (connection->curveSegments[j].arrowEnd && connection->curveSegments[j].arrowEnd->scene() == graphicsScene)
								graphicsScene->removeItem(connection->curveSegments[j].arrowEnd);
						}
						if (connection->centerRegionItem)
							graphicsScene->removeItem(connection->centerRegionItem);
					}
				}
			}

			setHandle(graphicsItems[i],0);

			if (itemHandles.size() > i && itemHandles[i])
			{
				itemHandles[i]->network = 0;
				if (itemHandles[i]->graphicsItems.isEmpty())
				{
					if (itemHandles[i]->parent)
						itemHandles[i]->parent->children.removeAll(itemHandles[i]);

					for (int j=0; j < itemHandles[i]->children.size(); ++j)
						if (itemHandles[i]->children[j])
							itemHandles[i]->children[j]->parent = 0;
				}
			}

		}
		
		while (parentHandles.size() < itemHandles.size()) parentHandles += 0;
			
		for (int i=0; i < itemHandles.size(); ++i)
		{
			parentHandles[i] = itemHandles[i]->parent;
			itemHandles[i]->setParent(0,false);
		}

		bool firstTime = (changeDataCommand == 0);

		if (firstTime)
		{
			QList< DataTable<qreal>* > oldData1, newData1;
			QList< DataTable<QString>* > oldData2, newData2;

			QStringList namesToKill;
			for (int i=0; i < itemHandles.size(); ++i)
				if (itemHandles[i] && itemHandles[i]->graphicsItems.isEmpty())
					namesToKill << itemHandles[i]->fullName();

			ItemHandle * handle;
			QList<QGraphicsItem*> items = graphicsScene->items();
			for (int i=0; i < items.size(); ++i)
			{
				handle = getHandle(items[i]);
				if (handle && handle->data && !itemHandles.contains(handle) && !affectedHandles.contains(handle))
					affectedHandles += handle;
			}

			for (int i=0; i < affectedHandles.size(); ++i)
				if (affectedHandles[i]->data)
				{
					QList<QString> keys1 = affectedHandles[i]->data->numericalData.keys();
					QList<QString> keys2 = affectedHandles[i]->data->textData.keys();

					for (int j=0; j < keys1.size(); ++j)
						oldData1 += new DataTable<qreal>(affectedHandles[i]->data->numericalData[ keys1[j] ]);

					for (int j=0; j < keys2.size(); ++j)
						oldData2 += new DataTable<QString>(affectedHandles[i]->data->textData[ keys2[j] ]);
				}

			DataTable<qreal> * nDat = 0;
			DataTable<QString> * sDat = 0;

			for (int i=0; i < affectedHandles.size(); ++i) //change all the handle data
			{
				bool affected = false;
				for (int i2=0; i2 < namesToKill.size(); ++i2)
				{
					QString oldname(namesToKill[i2]);

					QRegExp regexp1(QString("^") + oldname + QString("$")),  //just old name
						regexp2(QString("^") + oldname + QString("([^A-Za-z0-9_])")),  //oldname+(!letter/num)
						regexp3(QString("([^A-Za-z0-9_.])") + oldname + QString("$")), //(!letter/num)+oldname
						regexp4(QString("([^A-Za-z0-9_.])") + oldname + QString("([^A-Za-z0-9_])")); //(!letter/num)+oldname+(!letter/num)

					QList< QString > keys = affectedHandles[i]->data->numericalData.keys();
					for (int j=0; j < keys.size(); ++j)  //go through each numeric data
					{
						affected = false;
						nDat = &(affectedHandles[i]->data->numericalData[ keys[j] ]);
						for (int k=0; k < nDat->rows(); ++k)
						{
							if (nDat->rowName(k).contains(regexp1) || nDat->rowName(k).contains(regexp2) ||
								nDat->rowName(k).contains(regexp3) || nDat->rowName(k).contains(regexp4))
							{
								nDat->removeRow(k);
								--k;
								affected = true;
							}
						}
						for (int k=0; k < nDat->cols(); ++k)
						{
							if (nDat->colName(k).contains(regexp1) || nDat->colName(k).contains(regexp2) ||
								nDat->colName(k).contains(regexp3) || nDat->colName(k).contains(regexp4))
							{
								nDat->removeCol(k);
								--k;
								affected = true;
							}
						}
						if (affected && graphicsScene->console())
                            graphicsScene->console()->message(QObject::tr("data changed : ") + keys[j] + QObject::tr(" in ") + affectedHandles[i]->fullName());
					}

					keys = affectedHandles[i]->data->textData.keys();

					for (int j=0; j < keys.size(); ++j)  //go through each text data
					{
						affected = false;
						sDat = &(affectedHandles[i]->data->textData[ keys[j] ]);
						for (int k=0; k < sDat->rows(); ++k)
						{
							if (sDat->rowName(k).contains(regexp1) || sDat->rowName(k).contains(regexp2) ||
								sDat->rowName(k).contains(regexp3) || sDat->rowName(k).contains(regexp4))
							{
								sDat->removeRow(k);
								--k;
								affected = true;
							}
						}
						for (int k=0; k < sDat->cols(); ++k)
						{
							if (sDat->colName(k).contains(regexp1) || sDat->colName(k).contains(regexp2) ||
								sDat->colName(k).contains(regexp3) || sDat->colName(k).contains(regexp4))
							{
								sDat->removeCol(k);
								--k;
								affected = true;
							}
						}

						QString newname("1.0");
						for (int k=0; k < sDat->rows(); ++k) //substitute each value in the table
							for (int l=0; l < sDat->cols(); ++l)
							{
								QString & target = sDat->value(k,l);// = QString("0.0");

								int n = regexp1.indexIn(target);
								if (n != -1)
								{
									target = newname;
									//target.replace(oldname,newname);
									//n = regexp1.indexIn(target);
									affected = true;
								}
								n = regexp2.indexIn(target);
								if (n != -1)
								{
									target = newname;
									//target.replace(regexp2,newname+QString("\\1"));
									//n = regexp2.indexIn(target);
									affected = true;
								}
								n = regexp3.indexIn(target);
								if (n != -1)
								{
									target = newname;
									//target.replace(regexp3,QString("\\1")+newname);
									//n = regexp3.indexIn(target);
									affected = true;
								}
								n = regexp4.indexIn(target);
								if (n != -1)
								{
									target = newname;
									//n = regexp4.indexIn(target);
									affected = true;
								}
							}
							if (affected && graphicsScene->console())
								graphicsScene->console()->message(QObject::tr("data changed : ") + keys[j] + QObject::tr(" in ") + affectedHandles[i]->fullName());
					}
				}
			}
			for (int i=0; i < affectedHandles.size(); ++i)
				if (affectedHandles[i]->data)
				{
					QList<QString> keys1 = affectedHandles[i]->data->numericalData.keys();
					QList<QString> keys2 = affectedHandles[i]->data->textData.keys();

					for (int j=0; j < keys1.size(); ++j)
						newData1 += &(affectedHandles[i]->data->numericalData[ keys1[j] ]);

					for (int j=0; j < keys2.size(); ++j)
						newData2 += &(affectedHandles[i]->data->textData[ keys2[j] ]);
				}
			changeDataCommand = new Change2DataCommand<qreal,QString>(QString(""), newData1, oldData1, newData2, oldData2);
			for (int i=0; i < oldData1.size(); ++i)
				if (oldData1[i])
					delete oldData1[i];
			for (int i=0; i < oldData2.size(); ++i)
				if (oldData2[i])
					delete oldData2[i];
		}
		else
		{
			if (changeDataCommand)
				changeDataCommand->undo();
		}
	}

	void RemoveGraphicsCommand::undo()
	{
		if (!graphicsScene) return;

		QList<ConnectionGraphicsItem*> connections;
		ConnectionGraphicsItem * connection;

		for (int i=0; i<graphicsItems.size(); ++i)
		{
			if (graphicsItems[i] && graphicsItems[i]->scene() != graphicsScene)
				graphicsScene->addItem(graphicsItems[i]);

			NodeGraphicsItem * node = qgraphicsitem_cast<NodeGraphicsItem*>(graphicsItems[i]);
			if (node)
			{
				node->setBoundingBoxVisible(false);
			}
			else
			{
				connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
				if (connection)
				{
					connections << connection;
				}
			}

			if (itemHandles.size() > i && itemHandles[i])
			{
				if (parentHandles.size() > i && !MainWindow::invalidPointers.contains(itemHandles[i]))
					itemHandles[i]->setParent(parentHandles[i],false);
				
				itemHandles[i]->network = graphicsScene->network;
				setHandle(graphicsItems[i],itemHandles[i]);

				if (itemHandles[i]->parent)
					itemHandles[i]->setParent(itemHandles[i]->parent,false);

				for (int j=0; j < itemHandles[i]->children.size(); ++j)
					if (itemHandles[i]->children[j])
						itemHandles[i]->children[j]->parent = itemHandles[i];

			}
			if (itemParents.size() > i && itemParents[i] != 0)
			{
				graphicsItems[i]->setParentItem(itemParents[i]);
			}
		}

		for (int i=0; i < connections.size(); ++i)
		{
			connections[i]->refresh();
			connections[i]->setControlPointsVisible(false);
		}

		if (changeDataCommand)
			changeDataCommand->redo();
	}


	ChangeBrushCommand::ChangeBrushCommand(const QString& name, QGraphicsItem * item, const QBrush& to)
		: QUndoCommand(name)
	{
		graphicsItems.clear();
		oldBrush.clear();
		newBrush.clear();

		QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(item);
		NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(item);
		ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(item);
		ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(item);
		if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(item);
		if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(item);
		if (shape != 0)
		{
			graphicsItems.append(shape);
			oldBrush.append(shape->defaultBrush);
		}
		else
			if (connection != 0)
			{
				graphicsItems.append(connection);
				oldBrush.append(connection->defaultBrush);
			}
			else
				if (controlPoint != 0)
				{
					graphicsItems.append(controlPoint);
					oldBrush.append(controlPoint->defaultBrush);
				}
				else
					if (aitem != 0)
					{
						graphicsItems.append(aitem);
						oldBrush.append(aitem->brush());
					}

					newBrush.append(to);
	}

	ChangeBrushCommand::ChangeBrushCommand(const QString& name, const QList<QGraphicsItem*>& items, const QList<QBrush>& to)
		: QUndoCommand(name)
	{
		newBrush.clear();
		for (int i=0; i < items.size(); ++i)
		{
			QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(items[i]);
			NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(items[i]);
			ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(items[i]);
			ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(items[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(items[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(items[i]);
			if (shape != 0)
			{
				graphicsItems.append(shape);
				oldBrush.append(shape->defaultBrush);
			}
			else
				if (connection != 0)
				{
					graphicsItems.append(connection);
					oldBrush.append(connection->defaultBrush);
				}
				else
					if (controlPoint != 0)
					{
						graphicsItems.append(controlPoint);
						oldBrush.append(controlPoint->defaultBrush);
					}
					else
						if (aitem != 0)
						{
							graphicsItems.append(aitem);
							oldBrush.append(aitem->brush());
						}
		}
		newBrush = to;
	}
	void ChangeBrushCommand::redo()
	{
		for (int i=0; i < graphicsItems.size() && i < newBrush.size() && i < oldBrush.size(); ++i)
		{
			QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(graphicsItems[i]);
			NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(graphicsItems[i]);
			ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
			ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (shape != 0)
				shape->setBrush( shape->defaultBrush = newBrush[i] );
			else
				if (connection != 0)
					connection->setBrush( connection->defaultBrush = newBrush[i] );
				else
					if (controlPoint != 0)
						controlPoint->setBrush( controlPoint->defaultBrush = newBrush[i] );
					else
						if (aitem != 0)
							aitem->setBrush(newBrush[i]);
		}
	}
	void ChangeBrushCommand::undo()
	{
		for (int i=0; i < graphicsItems.size() && i < oldBrush.size() && i < newBrush.size(); ++i)
		{
			QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(graphicsItems[i]);
			NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(graphicsItems[i]);
			ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
			ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (shape != 0)
				shape->setBrush( shape->defaultBrush = oldBrush[i] );
			else
				if (connection != 0)
					connection->setBrush( connection->defaultBrush = oldBrush[i] );
				else
					if (controlPoint != 0)
						controlPoint->setBrush( controlPoint->defaultBrush = oldBrush[i] );
					else
						if (aitem != 0)
							aitem->setBrush(oldBrush[i]);
		}
	}

	ChangePenCommand::ChangePenCommand(const QString& name, QGraphicsItem * item, const QPen& to)
		: QUndoCommand(name)
	{
		graphicsItems.clear();
		oldPen.clear();
		newPen.clear();
		QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(item);
		NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(item);
		ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(item);
		ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(item);
		if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(item);
		if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(item);
		TextGraphicsItem * textItem = qgraphicsitem_cast<TextGraphicsItem*>(item);
		if (shape != 0)
		{
			graphicsItems.append(shape);
			oldPen.append(shape->defaultPen);
		}
		else
			if (connection != 0)
			{
				graphicsItems.append(connection);
				oldPen.append(connection->defaultPen);
			}
			else
				if (controlPoint != 0)
				{
					graphicsItems.append(controlPoint);
					oldPen.append(controlPoint->defaultPen);
				}
				else
					if (textItem != 0)
					{
						graphicsItems.append(textItem);
						oldPen.append(QPen(textItem->defaultTextColor()));
					}
					else
						if (aitem != 0)
						{
							graphicsItems.append(aitem);
							oldPen.append(aitem->pen());
						}

						newPen.append(to);
	}

	ChangePenCommand::ChangePenCommand(const QString& name, const QList<QGraphicsItem*>& items, const QList<QPen>& to)
		: QUndoCommand(name)
	{
		newPen.clear();
		for (int i=0; i < items.size(); ++i)
		{
			QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(items[i]);
			NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(items[i]);
			ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(items[i]);
			ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(items[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(items[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(items[i]);
			TextGraphicsItem * textItem = qgraphicsitem_cast<TextGraphicsItem*>(items[i]);
			if (shape != 0)
			{
				graphicsItems.append(shape);
				oldPen.append(shape->defaultPen);
			}
			else
				if (connection != 0)
				{
					graphicsItems.append(connection);
					oldPen.append(connection->defaultPen);
				}
				else
					if (controlPoint != 0)
					{
						graphicsItems.append(controlPoint);
						oldPen.append(controlPoint->defaultPen);
					}
					else
						if (textItem != 0)
						{
							graphicsItems.append(textItem);
							oldPen.append(QPen(textItem->defaultTextColor()));
						}
						else
							if (aitem != 0)
							{
								graphicsItems.append(aitem);
								oldPen.append(aitem->pen());
							}
		}
		newPen = to;
	}
	void ChangePenCommand::redo()
	{
		for (int i=0; i < graphicsItems.size() && i < oldPen.size() && i < newPen.size(); ++i)
		{
			QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(graphicsItems[i]);
			NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(graphicsItems[i]);
			ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
			ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(graphicsItems[i]);
			TextGraphicsItem * textItem = qgraphicsitem_cast<TextGraphicsItem*>(graphicsItems[i]);
			if (shape != 0)
				shape->setPen( shape->defaultPen = newPen[i] );
			else
				if (connection != 0)
				{
					connection->setPen( connection->defaultPen = newPen[i] );
					connection->refresh();
				}
				else
					if (controlPoint != 0)
						controlPoint->setPen( controlPoint->defaultPen = newPen[i] );
					else
						if (textItem != 0)
							textItem->setDefaultTextColor( newPen[i].color() );
						else
							if (aitem != 0)
								aitem->setPen(newPen[i]);
		}
	}

	void ChangePenCommand::undo()
	{
		for (int i=0; i < graphicsItems.size() && i < oldPen.size() && i < newPen.size(); ++i)
		{
			QAbstractGraphicsShapeItem * aitem = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(graphicsItems[i]);
			NodeGraphicsItem::Shape * shape = qgraphicsitem_cast<NodeGraphicsItem::Shape*>(graphicsItems[i]);
			ConnectionGraphicsItem * connection = qgraphicsitem_cast<ConnectionGraphicsItem*>(graphicsItems[i]);
			ControlPoint * controlPoint = qgraphicsitem_cast<ControlPoint*>(graphicsItems[i]);
			TextGraphicsItem * textItem = qgraphicsitem_cast<TextGraphicsItem*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<ConnectionGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (controlPoint == 0) controlPoint = qgraphicsitem_cast<NodeGraphicsItem::ControlPoint*>(graphicsItems[i]);
			if (shape != 0)
				shape->setPen( shape->defaultPen = oldPen[i] );
			else
				if (connection != 0)
				{
					connection->setPen( connection->defaultPen = oldPen[i] );
					connection->refresh();
				}
				else
					if (controlPoint != 0)
						controlPoint->setPen( controlPoint->defaultPen = oldPen[i] );
					else
						if (textItem != 0)
							textItem->setDefaultTextColor( oldPen[i].color() );
						else
							if (aitem != 0)
								aitem->setPen(oldPen[i]);
		}
	}
	
	ChangeBrushAndPenCommand::~ChangeBrushAndPenCommand()
	{
		if (changeBrushCommand)
			delete changeBrushCommand;

		if (changePenCommand)
			delete changePenCommand;
	}

	ChangeBrushAndPenCommand::ChangeBrushAndPenCommand(const QString& name, QGraphicsItem * item, const QBrush& brush, const QPen& pen)
		: QUndoCommand(name)
	{
		changeBrushCommand = new ChangeBrushCommand(name,item,brush);
		changePenCommand = new ChangePenCommand(name,item,pen);
	}

	ChangeBrushAndPenCommand::ChangeBrushAndPenCommand(const QString& name, const QList<QGraphicsItem*>& items, const QList<QBrush>& brushes, const QList<QPen>& pens)
		: QUndoCommand(name)
	{
		changeBrushCommand = new ChangeBrushCommand(name,items,brushes);
		changePenCommand = new ChangePenCommand(name,items,pens);
	}

	void ChangeBrushAndPenCommand::redo()
	{
		if (changeBrushCommand)
			changeBrushCommand->redo();
			
		if (changePenCommand)
			changePenCommand->redo();
	}

	void ChangeBrushAndPenCommand::undo()
	{
		if (changeBrushCommand)
			changeBrushCommand->undo();
			
		if (changePenCommand)
			changePenCommand->undo();
	}

	TransformCommand::TransformCommand(const QString& name, QGraphicsScene * scene, QGraphicsItem * item,
		const QPointF& sizeChange, qreal changeInAngle,
		bool VFlip, bool HFlip)
		: QUndoCommand(name)
	{
		graphicsScene = scene;
		graphicsItems.append(item);
		sizeFactor.clear();
		angleChange.clear();

		sizeFactor.append(sizeChange);
		angleChange.append(changeInAngle);
		vFlip = VFlip;
		hFlip = HFlip;
	}

	TransformCommand::TransformCommand(const QString& name, QGraphicsScene * scene, const QList<QGraphicsItem *>& items,
		const QList<QPointF>& sizeChange, const QList<qreal>& changeInAngle,
		bool VFlip, bool HFlip)
		: QUndoCommand(name)
	{
		graphicsScene = scene;
		graphicsItems = items;

		sizeFactor = sizeChange;
		angleChange = changeInAngle;
		vFlip = VFlip;
		hFlip = HFlip;
	}

	void TransformCommand::redo()
	{
		for (int i=0; i < graphicsItems.size(); ++i)

			if (graphicsItems[i])
			{
				QTransform t = graphicsItems[i]->transform();
				
				if (sizeFactor.size() > i && !sizeFactor[i].isNull())
				{
					QTransform scale(sizeFactor[i].x(), 0, 0, sizeFactor[i].y(), 0, 0);
					t = (t * scale);
				}

				if (angleChange.size() > i)
				{
					double sinx = sin(angleChange[i] * 3.14/180.0),
						   cosx = cos(angleChange[i] * 3.14/180.0);
					QTransform rotate(cosx, sinx, -sinx, cosx, 0, 0);
					t = (t * rotate);
				}

				if (hFlip)
				{
					QTransform scale(-1.0, 0, 0, 1.0, 0, 0);
					t = (t * scale);
				}

				if (vFlip)
				{
					QTransform scale(1.0, 0, 0, -1.0, 0, 0);
					t = (t * scale);
				}
				
				graphicsItems[i]->setTransform(t);

				NodeGraphicsItem * node = qgraphicsitem_cast<NodeGraphicsItem*>(graphicsItems[i]);
				if (node)
				{
					node->adjustBoundaryControlPoints();
				}
			}
	}

	void TransformCommand::undo()
	{
		for (int i=0; i < graphicsItems.size(); ++i)

			if (graphicsItems[i])
			{
				QTransform t = graphicsItems[i]->transform();
				
				if (sizeFactor.size() > i && !sizeFactor[i].isNull())
				{
					QTransform scale(1.0/sizeFactor[i].x(), 0, 0, 1.0/sizeFactor[i].y(), 0, 0);
					t = (t * scale);
				}

				if (angleChange.size() > i)
				{
					double sinx = sin(-angleChange[i] * 3.14/180.0),
						   cosx = cos(-angleChange[i] * 3.14/180.0);
					QTransform rotate(cosx, sinx, -sinx, cosx, 0, 0);
					t = (t * rotate);
				}

				if (hFlip)
				{
					QTransform scale(-1.0, 0, 0, 1.0, 0, 0);
					t = (t * scale);
				}

				if (vFlip)
				{
					QTransform scale(1.0, 0, 0, -1.0, 0, 0);
					t = (t * scale);
				}
				
				graphicsItems[i]->setTransform(t);

				NodeGraphicsItem * node = qgraphicsitem_cast<NodeGraphicsItem*>(graphicsItems[i]);
				if (node)
				{
					node->adjustBoundaryControlPoints();
				}
			}
	}

	ChangeZCommand::ChangeZCommand(const QString& name, QGraphicsScene * scene, QGraphicsItem * item, double to)
		: QUndoCommand(name)
	{
		graphicsScene = scene;
		graphicsItems.clear();
		oldZ.clear();
		newZ.clear();
		if (item != 0)
		{
			graphicsItems.append(item->topLevelItem());
			oldZ.append(item->zValue());
			newZ.append(to);
		}
	}

	ChangeZCommand::ChangeZCommand(const QString& name, QGraphicsScene * scene, const QList<QGraphicsItem*>& items, const QList<double>& to)
		: QUndoCommand(name)
	{
		graphicsScene = scene;
		newZ.clear();
		for (int i=0; i < items.size(); ++i)
		{
			QGraphicsItem * aitem = (items[i]);
			if (aitem != 0)
			{
				graphicsItems.append(aitem->topLevelItem());
				oldZ.append(aitem->zValue());
			}
		}
		newZ = to;
	}

	void ChangeZCommand::redo()
	{
		for (int i=0; i < graphicsItems.size() && i < newZ.size(); ++i)
		{
			if (graphicsItems[i] != 0)
			{
				graphicsItems[i]->setZValue(newZ[i]);
			}
		}
	}

	void ChangeZCommand::undo()
	{
		for (int i=0; i < graphicsItems.size() && i < oldZ.size(); ++i)
		{
			if (graphicsItems[i] != 0)
			{
				graphicsItems[i]->setZValue(oldZ[i]);
			}
		}
	}

	ChangeParentCommand::ChangeParentCommand(const QString& name, QGraphicsScene * scene, QGraphicsItem * item, QGraphicsItem * newParent)
		: QUndoCommand(name)
	{
		this->scene = scene;
		graphicsItems.clear();
		oldParents.clear();
		newParents.clear();
		if (item != 0)
		{
			graphicsItems.append(item);
			oldParents.append(item->parentItem());
			newParents.append(newParent);
		}
	}

	ChangeParentCommand::ChangeParentCommand(const QString& name, QGraphicsScene * scene, const QList<QGraphicsItem*>& items, const QList<QGraphicsItem*>& to)
		: QUndoCommand(name)
	{
		this->scene = scene;
		oldParents.clear();
		newParents.clear();
		for (int i=0; i < items.size(); ++i)
		{
			QGraphicsItem * aitem = (items[i]);
			if (aitem != 0)
			{
				graphicsItems.append(aitem);
				oldParents.append(aitem->parentItem());
			}
		}
		newParents = to;
	}

	void ChangeParentCommand::redo()
	{
		if (scene == 0) return;

		for (int i=0; i < graphicsItems.size() && i < newParents.size(); ++i)
		{
			if (graphicsItems[i] != 0)
			{
				QPointF pos = graphicsItems[i]->scenePos();
				if (newParents[i] == 0)
				{
					graphicsItems[i]->setParentItem(0);
					if (graphicsItems[i]->scene() != scene)
						scene->addItem(graphicsItems[i]);
					graphicsItems[i]->setPos(pos);
				}
				else
				{
					graphicsItems[i]->setParentItem(newParents[i]);
					graphicsItems[i]->setPos( newParents[i]->mapFromScene(pos) );
				}
			}
		}
	}

	void ChangeParentCommand::undo()
	{
		if (scene == 0) return;

		for (int i=0; i < graphicsItems.size() && i < oldParents.size(); ++i)
		{
			if (graphicsItems[i] != 0)
			{
				QPointF pos = graphicsItems[i]->scenePos();
				if (oldParents[i] == 0)
				{
					graphicsItems[i]->setParentItem(0);
					if (graphicsItems[i]->scene() != scene)
						scene->addItem(graphicsItems[i]);
					graphicsItems[i]->setPos(pos);
				}
				else
				{
					graphicsItems[i]->setParentItem(oldParents[i]);
					graphicsItems[i]->setPos( oldParents[i]->mapFromScene(pos) );
				}
			}
		}
	}

	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, ItemHandle * handle, const QString& newname)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		if (net)
			allhandles = net->handles();

		handles.clear();
		oldNames.clear();
		newNames.clear();

		if (handle && net)
		{
			handles += handle;
			oldNames += handle->fullName();
			newNames += net->makeUnique(newname);
		}
	}

	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& allItems, const QString& oldname, const QString& newname)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		this->allhandles = allItems;
		handles.clear();
		oldNames.clear();
		newNames.clear();

		if (net)
		{
			oldNames += oldname;
			newNames += net->makeUnique(newname);
		}
	}
	
	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QString& oldname, const QString& newname)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		if (net)
			this->allhandles = net->handles();
		handles.clear();
		oldNames.clear();
		newNames.clear();

		if (net)
		{
			oldNames += oldname;
			newNames += net->makeUnique(newname);
		}
	}
	
	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& allItems, const QList<QString>& oldname, const QList<QString>& newname)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		this->allhandles = allItems;
		handles.clear();
		oldNames.clear();
		newNames.clear();

		if (net)
		{
			oldNames << oldname;
			newNames << net->makeUnique(newname);			
		}
	}
	
	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QList<QString>& oldname, const QList<QString>& newname)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		if (net)
			this->allhandles = net->handles();
		handles.clear();
		oldNames.clear();
		newNames.clear();
		
		if (net)
		{
			oldNames << oldname;
			newNames << net->makeUnique(newname);
		}
	}

	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& allItems, ItemHandle * handle, const QString& newname)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		this->allhandles = allItems;
		handles.clear();
		oldNames.clear();
		newNames.clear();

		ItemHandle * handle1;
		QStringList allNames;
		for (int i=0; i < allItems.size(); ++i)
			if ((handle1 = (allItems[i])) && (handle != handle1))
			{
				allNames << handle1->fullName();
				allNames << handle1->fullName(QObject::tr("_"));
			}

		if (handle && net)
		{
			handles += handle;
			oldNames += handle->fullName();
			newNames += net->makeUnique(newname);
		}
	}

	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& items, const QList<QString>& newnames)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		if (net)
			this->allhandles = net->handles();
		handles.clear();
		oldNames.clear();
		newNames.clear();

		ItemHandle * handle;
		QStringList allNames;
		for (int i=0; i < allhandles.size(); ++i)
			if ((handle = (allhandles[i])) && !items.contains(handle))
			{
				allNames << handle->fullName();
				allNames << handle->fullName(QObject::tr("_"));
			}

		if (net)
		{
			for (int i=0; i < items.size() && i < newnames.size() ; ++i)
			{
				handle = items[i];
				if (handle)
				{
					handles += handle;
					oldNames += handle->fullName();
				}
			}
			newNames = net->makeUnique(newnames);
		}
	}

	RenameCommand::RenameCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& allItems, const QList<ItemHandle*>& items, const QList<QString>& newnames)
		: QUndoCommand(name), changeDataCommand(0), network(net)
	{
		this->allhandles = allItems;
		handles.clear();
		oldNames.clear();
		newNames.clear();
		ItemHandle * handle;
		QStringList allNames;
		for (int i=0; i < allItems.size(); ++i)
			if ((handle = (allItems[i])) && !items.contains(handle))
			{
				allNames << handle->fullName();
				allNames << handle->fullName(QObject::tr("_"));
			}

		if (net)
		{
			for (int i=0; i < items.size() && i < newnames.size() ; ++i)
			{
				handle = (items[i]);
				if (handle)
				{
					handles += handle;
					oldNames += handle->fullName();
				}
			}
			newNames = net->makeUnique(newnames);
		}
	}

	void RenameCommand::substituteString(QString& target, const QString& oldname,const QString& newname0)
	{
		if (oldname == newname0 || target.size() > 1000) return;
		QString newname = newname0;
		newname.replace(QRegExp("[^A-Za-z0-9_]"),QString("_@@@_"));

		QRegExp regexp1(QString("^") + oldname + QString("$")),  //just old name
			regexp2(QString("^") + oldname + QString("([^A-Za-z0-9_])")),  //oldname+(!letter/num)
			regexp3(QString("([^A-Za-z0-9_.])") + oldname + QString("$")), //(!letter/num)+oldname
			regexp4(QString("([^A-Za-z0-9_.])") + oldname + QString("([^A-Za-z0-9_])")); //(!letter/num)+oldname+(!letter/num)
		int n = regexp1.indexIn(target);
		while (n != -1)
		{
			target.replace(oldname,newname);
			n = regexp1.indexIn(target);
		}
		n = regexp2.indexIn(target);
		while (n != -1)
		{
			target.replace(regexp2,newname+QString("\\1"));
			n = regexp2.indexIn(target);
		}
		n = regexp3.indexIn(target);
		while (n != -1)
		{
			target.replace(regexp3,QString("\\1")+newname);
			n = regexp3.indexIn(target);
		}
		n = regexp4.indexIn(target);
		while (n != -1)
		{
			target.replace(regexp4,QString("\\1")+newname+QString("\\2"));
			n = regexp4.indexIn(target);
		}
		target.replace(newname,newname0);
	}

	void RenameCommand::findReplaceAllHandleData(const QList<ItemHandle*>& handles,const QString& oldname,const QString& newname)
	{
		DataTable<qreal> * nDat = 0;
		DataTable<QString> * sDat = 0;

		for (int i=0; i < handles.size(); ++i)
		{
			if (handles[i] && handles[i]->data)  //go through each handles num data and text data
			{
				QString fullname = handles[i]->fullName();
				QString s = newname;
				s.remove(fullname + QObject::tr("."));
				
				QList< QString > keys = handles[i]->data->numericalData.keys();
				for (int j=0; j < keys.size(); ++j)  //go through each num data
				{
					nDat = &(handles[i]->data->numericalData[ keys[j] ]);
					for (int k=0; k < nDat->rows(); ++k)
					{
						if (nDat->rowName(k).contains(oldname))
							substituteString(nDat->rowName(k),oldname,newname);

						if (fullname + QObject::tr(".") + nDat->rowName(k) == oldname)						
							nDat->rowName(k) = s;
					}
					for (int k=0; k < nDat->cols(); ++k)
					{
						if (nDat->colName(k).contains(oldname))
							substituteString(nDat->colName(k),oldname,newname);
						
						if (fullname + QObject::tr(".") + nDat->colName(k) == oldname)						
							nDat->colName(k) = s;
					}
				}
				keys = handles[i]->data->textData.keys();
				for (int j=0; j < keys.size(); ++j)  //go through each text data
				{
					sDat = &(handles[i]->data->textData[ keys[j] ]);
					for (int k=0; k < sDat->rows(); ++k)
					{
						if (sDat->rowName(k).contains(oldname))
							substituteString(sDat->rowName(k),oldname,newname);

						if (fullname + QObject::tr(".") + sDat->rowName(k) == oldname)						
							sDat->rowName(k) = s;
					}
					for (int k=0; k < sDat->cols(); ++k)
					{
						if (sDat->colName(k).contains(oldname))
							substituteString(sDat->colName(k),oldname,newname);
						
						if (fullname + QObject::tr(".") + sDat->colName(k) == oldname)						
							sDat->colName(k) = s;
					}
					for (int k=0; k < sDat->rows(); ++k) //substitute each value in the table
						for (int l=0; l < sDat->cols(); ++l)
						{
							if (sDat->value(k,l).contains(oldname))
								substituteString(sDat->value(k,l),oldname,newname);
						}
				}
			}
		}
	}

	RenameCommand::~RenameCommand()
	{
		if (changeDataCommand)
            delete changeDataCommand;
		changeDataCommand = 0;
	}

	void RenameCommand::redo()
	{
		bool firstTime = (changeDataCommand == 0);
		QList< DataTable<qreal>* > oldData1, newData1;
		QList< DataTable<QString>* > oldData2, newData2;

		if (firstTime)
		{
			for (int i=0; i < allhandles.size(); ++i)
				if (allhandles[i]->data)
				{
					QList<QString> keys1 = allhandles[i]->data->numericalData.keys();
					QList<QString> keys2 = allhandles[i]->data->textData.keys();

					for (int j=0; j < keys1.size(); ++j)
						oldData1 += new DataTable<qreal>(allhandles[i]->data->numericalData[ keys1[j] ]);

					for (int j=0; j < keys2.size(); ++j)
						oldData2 += new DataTable<QString>(allhandles[i]->data->textData[ keys2[j] ]);
				}
		}

		if (firstTime)
		{
			QRegExp regexp("\\.([^\\.]+)$");

			for (int i=0; i < oldNames.size() && i < newNames.size(); ++i)
			{
				if (firstTime)
					findReplaceAllHandleData(allhandles,oldNames[i],newNames[i]);

				if (handles.size() == 0)
					for (int j=0; j < allhandles.size(); ++j)
						if (allhandles[j] && allhandles[j]->fullName() == oldNames[i])
						{
							oldItemNames << QPair<ItemHandle*,QString>(allhandles[j],allhandles[j]->name);

							regexp.indexIn(newNames[i]);
							for (int k=0; k < allhandles[j]->graphicsItems.size(); ++k)
							{
								TextGraphicsItem * textItem = qgraphicsitem_cast<TextGraphicsItem*>(allhandles[j]->graphicsItems[k]);
								if (textItem)
								{
									if (textItem->toPlainText() == allhandles[j]->name)
									{
										oldTextItemsNames << QPair<TextGraphicsItem*,QString>(textItem,allhandles[j]->name);

										if (regexp.numCaptures() > 0 && !regexp.cap(1).isEmpty())
										{
											textItem->setPlainText(regexp.cap(1));
											newTextItemsNames << QPair<TextGraphicsItem*,QString>(textItem,regexp.cap(1));
										}
										else
										{
											textItem->setPlainText(newNames[i]);
											newTextItemsNames << QPair<TextGraphicsItem*,QString>(textItem,newNames[i]);
										}
									}
								}
								else
								if (allhandles[j]->graphicsItems[k])
								{
									allhandles[j]->graphicsItems[k]->update();
								}
							}
							if (regexp.numCaptures() > 0 && !regexp.cap(1).isEmpty())
								allhandles[j]->name = regexp.cap(1);
							else
								allhandles[j]->name = newNames[i];

							newItemNames << QPair<ItemHandle*,QString>(allhandles[j],allhandles[j]->name);
						}
			}


			for (int i=0; i < allhandles.size(); ++i)
				if (allhandles[i]->data)
				{
					QList<QString> keys1 = allhandles[i]->data->numericalData.keys();
					QList<QString> keys2 = allhandles[i]->data->textData.keys();

					for (int j=0; j < keys1.size(); ++j)
						newData1 += &(allhandles[i]->data->numericalData[ keys1[j] ]);

					for (int j=0; j < keys2.size(); ++j)
						newData2 += &(allhandles[i]->data->textData[ keys2[j] ]);
				}
				changeDataCommand = new Change2DataCommand<qreal,QString>(QString(""), newData1, oldData1, newData2, oldData2);
				for (int i=0; i < oldData1.size(); ++i)
					if (oldData1[i])
						delete oldData1[i];
				for (int i=0; i < oldData2.size(); ++i)
					if (oldData2[i])
						delete oldData2[i];

			for (int i=0; i < handles.size() && i < newNames.size(); ++i)
			{
				ItemHandle * handle = handles[i];
				if (handle)
				{
					regexp.indexIn(newNames[i]);
					for (int j=0; j < handle->graphicsItems.size(); ++j)
					{
						TextGraphicsItem * textItem = qgraphicsitem_cast<TextGraphicsItem*>(handle->graphicsItems[j]);
						if (textItem)
						{
							if (textItem->toPlainText() == handle->name)
							{
								oldTextItemsNames << QPair<TextGraphicsItem*,QString>(textItem,handle->name);

								if (regexp.numCaptures() > 0 && !regexp.cap(1).isEmpty())
								{
									textItem->setPlainText(regexp.cap(1));
									newTextItemsNames << QPair<TextGraphicsItem*,QString>(textItem,regexp.cap(1));
								}
								else
								{
									textItem->setPlainText(newNames[i]);
									newTextItemsNames << QPair<TextGraphicsItem*,QString>(textItem,newNames[i]);
								}

							}
						}
					}

					oldItemNames << QPair<ItemHandle*,QString>(handle,handle->name);

					if (regexp.numCaptures() > 0 && !regexp.cap(1).isEmpty())
						handle->name = regexp.cap(1);
					else
						handle->name = newNames[i];

					newItemNames << QPair<ItemHandle*,QString>(handle,handle->name);
				}
			}
		}
		else
		{
			for (int i=0; i < newItemNames.size(); ++i)
				if (newItemNames[i].first)
					newItemNames[i].first->name = newItemNames[i].second;

			for (int i=0; i < newTextItemsNames.size(); ++i)
				if (newTextItemsNames[i].first)
					newTextItemsNames[i].first->setPlainText(newTextItemsNames[i].second);

			if (changeDataCommand)
				changeDataCommand->undo();
		}
		
		if (network)
		{
			QList<TextEditor*> editors = network->editors();
			for (int i=0; i < editors.size(); ++i)
				if (editors[i])
					for (int j=0; j < oldNames.size() && j < newNames.size(); ++j)
						editors[i]->replace(oldNames[j],newNames[j]);
		}
	}

	void RenameCommand::undo()
	{
		for (int i=0; i < oldItemNames.size(); ++i)
				if (oldItemNames[i].first)
					oldItemNames[i].first->name = oldItemNames[i].second;

		for (int i=0; i < oldTextItemsNames.size(); ++i)
			if (oldTextItemsNames[i].first)
				oldTextItemsNames[i].first->setPlainText(oldTextItemsNames[i].second);

		if (changeDataCommand)
		{
		    changeDataCommand->redo();
		}
		
		if (network)
		{
			QList<TextEditor*> editors = network->editors();
			for (int i=0; i < editors.size(); ++i)
				if (editors[i])
					for (int j=0; j < oldNames.size() && j < newNames.size(); ++j)
						editors[i]->replace(newNames[j],oldNames[j]);
		}
	}

	CompositeCommand::CompositeCommand(const QString& name, const QList<QUndoCommand*>& list, const QList<QUndoCommand*>& noDelete)
		: QUndoCommand(name)
	{
		commands = list;
		doNotDelete = noDelete;

		for (int i=0; i < noDelete.size(); ++i)
			if (!list.contains(noDelete[i]))
				commands += noDelete[i];
	}

	CompositeCommand::CompositeCommand(const QString& name, QUndoCommand* cmd1, QUndoCommand* cmd2, bool deleteCommand)
		: QUndoCommand(name)
	{
		commands += cmd1;
		commands += cmd2;
		if (!deleteCommand)
		{
			doNotDelete += cmd1;
			doNotDelete += cmd2;
		}
	}
	CompositeCommand::~CompositeCommand()
	{
		for (int i=0; i < commands.size(); ++i)
			if (commands[i] && !doNotDelete.contains(commands[i]) && !MainWindow::invalidPointers.contains(commands[i]))
			{
				MainWindow::invalidPointers[ (void*)commands[i] ] = true;
				delete commands[i];
			}
	}
	void CompositeCommand::redo()
	{
		for (int i=0; i < commands.size(); ++i)
		{
			if (commands[i])
				commands[i]->redo();
		}
	}
	void CompositeCommand::undo()
	{
		for (int i=commands.size()-1; i >= 0; --i)
		{
			if (commands[i])
				commands[i]->undo();
		}
	}

	ReverseUndoCommand::ReverseUndoCommand(const QString& name, QUndoCommand* cmd, bool deleteCmd)
		: QUndoCommand(name), command(cmd), deleteCommand(deleteCmd)
	{
	}
	ReverseUndoCommand::~ReverseUndoCommand()
	{
		if (command && deleteCommand && !MainWindow::invalidPointers.contains(command)) 
		{
			delete command;
			MainWindow::invalidPointers[ (void*)command ] = true;
		}
	}
	void ReverseUndoCommand::redo()
	{
		if (command) command->undo();
	}
	void ReverseUndoCommand::undo()
	{
		if (command) command->redo();
	}

	ReplaceNodeGraphicsCommand::ReplaceNodeGraphicsCommand(const QString& text,NodeGraphicsItem* node,const QString& filename,bool transform)
		: QUndoCommand(text), transform(transform)
	{
		if (node && !qgraphicsitem_cast<ToolGraphicsItem*>(node->topLevelItem()))
		{
			targetNodes += node;
			NodeGraphicsItem copy1(*node);
			oldNodes += copy1;

			NodeGraphicsItem copy2(*node);
			loadFromFile(&copy2,filename);
			newNodes += copy2;
		}
	}

	ReplaceNodeGraphicsCommand::ReplaceNodeGraphicsCommand(const QString& text,const QList<NodeGraphicsItem*>& nodes,const QList<QString>& filenames, bool transform)
		: QUndoCommand(text), transform(transform)
	{
		for (int i=0; i < nodes.size() && i < filenames.size(); ++i)
		{
			NodeGraphicsItem * node = nodes[i];
			QString filename = filenames[i];
			if (node && !qgraphicsitem_cast<ToolGraphicsItem*>(node->topLevelItem()))
			{
				targetNodes += node;
				NodeGraphicsItem copy1(*node);
				setHandle(&copy1,0);
				oldNodes += copy1;

				NodeGraphicsItem copy2(*node);
				setHandle(&copy2,0);
				loadFromFile(&copy2,filename);
				newNodes += copy2;
			}
		}
	}

	ReplaceNodeGraphicsCommand::~ReplaceNodeGraphicsCommand()
	{
		for (int i=0; i < itemsToDelete.size(); ++i)
			if (itemsToDelete[i] && !MainWindow::invalidPointers.contains( (void*)itemsToDelete[i]))
			{
				if (itemsToDelete[i]->scene())
					itemsToDelete[i]->scene()->removeItem(itemsToDelete[i]);
				
				MainWindow::invalidPointers[ (void*)itemsToDelete[i] ] = true;
				delete itemsToDelete[i];
			}
	}

	void ReplaceNodeGraphicsCommand::redo()
	{
		for (int i=0; i < targetNodes.size(); ++i)
		{
			if (targetNodes[i])
			{
				for (int j=0; j < targetNodes[i]->shapes.size(); ++j)
					if (!itemsToDelete.contains(targetNodes[i]->shapes[j]) && targetNodes[i]->shapes[j])
					{
						itemsToDelete << targetNodes[i]->shapes[j];
					}

					for (int j=0; j < targetNodes[i]->controlPoints.size(); ++j)
						if (!itemsToDelete.contains(targetNodes[i]->controlPoints[j]) && targetNodes[i]->controlPoints[j])
						{
							itemsToDelete << targetNodes[i]->controlPoints[j];
						}

						for (int j=0; j < targetNodes[i]->boundaryControlPoints.size(); ++j)
							if (!itemsToDelete.contains(targetNodes[i]->boundaryControlPoints[j]) && targetNodes[i]->boundaryControlPoints[j])
							{
								itemsToDelete << targetNodes[i]->boundaryControlPoints[j];
							}

				targetNodes[i]->shapes.clear();
				targetNodes[i]->controlPoints.clear();
				targetNodes[i]->boundaryControlPoints.clear();

				(*targetNodes[i]) = newNodes[i];
				if (!transform && targetNodes[i]->className == ArrowHeadItem::CLASSNAME)
				{
					ArrowHeadItem * arrow = static_cast<ArrowHeadItem*>(targetNodes[i]);
					arrow->angle = 0.0;
				}
			}
		}
		for (int i=0; i < itemsToDelete.size(); ++i)
			if (itemsToDelete[i] && itemsToDelete[i]->scene())
				itemsToDelete[i]->scene()->removeItem(itemsToDelete[i]);
	}
	void ReplaceNodeGraphicsCommand::undo()
	{
		for (int i=0; i < targetNodes.size(); ++i)
		{
			if (targetNodes[i])
			{
				for (int j=0; j < targetNodes[i]->shapes.size(); ++j)
					if (!itemsToDelete.contains(targetNodes[i]->shapes[j]) && targetNodes[i]->shapes[j])
					{
						itemsToDelete << targetNodes[i]->shapes[j];
					}

					for (int j=0; j < targetNodes[i]->controlPoints.size(); ++j)
						if (!itemsToDelete.contains(targetNodes[i]->controlPoints[j]) && targetNodes[i]->controlPoints[j])
						{
							itemsToDelete << targetNodes[i]->controlPoints[j];
						}

						for (int j=0; j < targetNodes[i]->boundaryControlPoints.size(); ++j)
							if (!itemsToDelete.contains(targetNodes[i]->boundaryControlPoints[j]) && targetNodes[i]->boundaryControlPoints[j])
							{
								itemsToDelete << targetNodes[i]->boundaryControlPoints[j];
							}

				targetNodes[i]->shapes.clear();
				targetNodes[i]->controlPoints.clear();
				targetNodes[i]->boundaryControlPoints.clear();

				(*targetNodes[i]) = oldNodes[i];
				
				if (!transform && targetNodes[i]->className == ArrowHeadItem::CLASSNAME)
				{
					ArrowHeadItem * arrow = static_cast<ArrowHeadItem*>(targetNodes[i]);
					arrow->angle = 0.0;
				}
			}
		}
		for (int i=0; i < itemsToDelete.size(); ++i)
			if (itemsToDelete[i] && itemsToDelete[i]->scene())
				itemsToDelete[i]->scene()->removeItem(itemsToDelete[i]);
	}
	void ReplaceNodeGraphicsCommand::loadFromFile(NodeGraphicsItem* node,const QString& fileName)
	{
		if (!node) return;

		QFile file (fileName);

		QString  home = MainWindow::homeDir(),
				temp = MainWindow::tempDir(),
				current = QDir::currentPath(),
				appDir = QCoreApplication::applicationDirPath();

		QString name[] = {  
			fileName,
			home + QObject::tr("/") + fileName,
			temp + QObject::tr("/") + fileName,
			current + QObject::tr("/") + fileName,
			appDir + QObject::tr("/") + fileName,
		};

		for (int i=0; i < 5; ++i)
		{
			file.setFileName(name[i]);
			if (file.exists())
				break;
		}

		if (!file.open(QFile::ReadOnly | QFile::Text))
		{
			return;
		}

		node->setParentItem(0);

		QPointF p = node->scenePos();
		QTransform t0 = node->sceneTransform();
		QTransform t1(t0.m11(),t0.m12(),0,t0.m21(),t0.m22(),0,0,0,1);

		node->setBoundingBoxVisible(false);
		node->clear();
		node->setPos(QPointF());
		node->resetTransform();
		
		NodeGraphicsReader reader;
		reader.readNodeGraphics(node,&file);
		node->normalize();

		node->setPos(p);
		
		if (transform)
		{
			node->setTransform(t1);
		}
		else
		{
			QRectF rect = node->boundingRect();
			node->scale( node->defaultSize.width()/rect.width() , node->defaultSize.height()/rect.height() );
		}

		//node->setParentItem(parent);
	}

	AssignHandleCommand::AssignHandleCommand(const QString& text, const QList<QGraphicsItem*>& items, QList<ItemHandle*>& handles)
		: QUndoCommand(text)
	{
		ItemHandle * handle = 0;

		for (int i=0; i < items.size() && i < handles.size(); ++i)
			if (items[i])
			{
				graphicsItems += items[i];

				handle = getHandle(items[i]);

				oldHandles += handle;
				newHandles += handles[i];

				oldItemHandles += QPair< QGraphicsItem*, ItemHandle* >(items[i], handle);
				newItemHandles += QPair< QGraphicsItem*, ItemHandle* >(items[i], handles[i]);
			}
	}

	AssignHandleCommand::AssignHandleCommand(const QString& text, const QList<QGraphicsItem*>& items, ItemHandle* newHandle)
		: QUndoCommand(text)
	{

		ItemHandle * handle = 0;
		for (int i=0; i < items.size(); ++i)
			if (items[i])
			{
				graphicsItems += items[i];
				handle = getHandle(items[i]);

				oldHandles += handle;
				newHandles += newHandle;

				oldItemHandles += QPair< QGraphicsItem*, ItemHandle* >( items[i], handle );
				newItemHandles += QPair< QGraphicsItem*, ItemHandle* >( items[i], newHandle );
			}

	}

	AssignHandleCommand::AssignHandleCommand(const QString& text, QGraphicsItem* item, ItemHandle* newHandle)
		: QUndoCommand(text)
	{
		graphicsItems += item;
		ItemHandle * handle = getHandle(item);

		oldHandles += handle;
		newHandles += newHandle;

		oldItemHandles += QPair< QGraphicsItem*, ItemHandle* >( item, handle );
		newItemHandles += QPair< QGraphicsItem*, ItemHandle* >( item, newHandle );
	}

	void AssignHandleCommand::redo()
	{
		TextGraphicsItem* textItem = 0;
		ItemHandle * handle = 0;
		for (int i=0; i < newItemHandles.size(); ++i)
		{
			if (newItemHandles[i].first)
			{
				handle = getHandle(newItemHandles[i].first);
				if ((textItem = qgraphicsitem_cast<TextGraphicsItem*>(newItemHandles[i].first)) &&
					handle && textItem->toPlainText() == handle->name &&
					newItemHandles[i].second)
					textItem->setPlainText(newItemHandles[i].second->name);
				setHandle(newItemHandles[i].first, newItemHandles[i].second);
			}
		}
	}

	void AssignHandleCommand::undo()
	{
		TextGraphicsItem* textItem = 0;
		ItemHandle * handle = 0;
		for (int i=0; i < oldItemHandles.size(); ++i)
		{
			if (oldItemHandles[i].first)
			{
				handle = getHandle(oldItemHandles[i].first);
				if ((textItem = qgraphicsitem_cast<TextGraphicsItem*>(oldItemHandles[i].first)) &&
					handle && textItem->toPlainText() == handle->name &&
					oldItemHandles[i].second)
					textItem->setPlainText(oldItemHandles[i].second->name);
				setHandle(oldItemHandles[i].first, oldItemHandles[i].second);
			}
		}
	}

	AssignHandleCommand::~AssignHandleCommand()
	{
		oldHandles << newHandles;
		for (int i=0; i < oldHandles.size(); ++i)
			if (oldHandles[i] && !MainWindow::invalidPointers.contains( (void*)oldHandles[i]))
			{
				bool pointedTo = false;
				for (int j=0; j < oldHandles[i]->graphicsItems.size(); ++j)
					if (getHandle(oldHandles[i]->graphicsItems[j]) == oldHandles[i])
					{
						pointedTo = true;
						break;
					}
					if (!pointedTo)
					{
						for (int j=0; j < oldHandles.size(); ++j)
							if (i != j && oldHandles[j] == oldHandles[i])
								oldHandles[j] = 0;

						oldHandles[i]->graphicsItems.clear();
						
						MainWindow::invalidPointers[ (void*)oldHandles[i] ] = true;
						delete oldHandles[i];
						oldHandles[i] = 0;
					}
			}
	}

	MergeHandlesCommand::MergeHandlesCommand(const QString& text, NetworkHandle * net, const QList<ItemHandle*>& handles) :
		QUndoCommand(text)
	{
		newHandle = 0;
		oldHandles = handles;

		QList<QString> oldNames, newNames;

		if (handles.size() > 0)
		{
			for (int i=0; i < handles.size(); ++i)
				if (handles[i])
				{
					if (newHandle == 0)
						newHandle = handles[i];//->clone();

					for (int j=0; j < handles[i]->graphicsItems.size(); ++j)
						if (handles[i]->graphicsItems[j])
						{
							if (!allGraphicsItems.contains(handles[i]->graphicsItems[j]))
								allGraphicsItems << handles[i]->graphicsItems[j];

							if (!oldGraphicsItems.contains(handles[i]))
								oldGraphicsItems[ handles[i] ] = QList<QGraphicsItem*>();

							if (!oldGraphicsItems[ handles[i] ].contains(handles[i]->graphicsItems[j]))
								oldGraphicsItems[ handles[i] ] << handles[i]->graphicsItems[j];
						}

					for (int j=0; j < handles[i]->children.size(); ++j)
						if (handles[i]->children[j])
						{
							if (!allChildren.contains(handles[i]->children[j]))
								allChildren << handles[i]->children[j];

							if (!oldChildren.contains(handles[i]))
								oldChildren[ handles[i] ] = QList<ItemHandle*>();

							if (!oldChildren[ handles[i] ].contains(handles[i]->children[j]))
								oldChildren[ handles[i] ] << handles[i]->children[j];
						}

					oldParents[ handles[i] ] = handles[i]->parent;

					/*for (int j=0; j < handles[i]->tools.size(); ++j)
						if (!newHandle->tools.contains(handles[i]->tools[j]))
							newHandle->tools += handles[i]->tools[j];*/
				}
		}

		for (int i =0; i < handles.size(); ++i)
			if (handles[i] && handles[i] != newHandle)
				oldNames << handles[i]->fullName();

		if (newHandle)
			for (int i=0; i < oldNames.size(); ++i)
				newNames << newHandle->fullName();

		QList<ItemHandle*> allHandles = net->handles();

		for (int i=0; i < handles.size(); ++i)
			allHandles.removeAll(handles[i]);
		renameCommand = new RenameCommand(QString("rename"),net,allHandles,oldNames,newNames);
	}

	MergeHandlesCommand::~MergeHandlesCommand()
	{
		ItemHandle * handle = 0;
		oldHandles << newHandle;
		for (int i=0; i < oldHandles.size(); ++i)
		{
			if (oldHandles[i] && !oldHandles[i]->parent)
			{
				handle = oldHandles[i];
				oldHandles.removeAll(handle);
				if (!MainWindow::invalidPointers.contains( (void*)handle ))
				{
					MainWindow::invalidPointers[ (void*)handle ] = true;
					delete handle;				
				}
			}
		}

		if (renameCommand)
			delete renameCommand;
	}

	void MergeHandlesCommand::redo()
	{
		if (newHandle == 0) return;

		for (int i=0; i < allChildren.size(); ++i)
			if (allChildren[i])
				allChildren[i]->setParent(newHandle,false);

		TextGraphicsItem * textItem = 0;
		ItemHandle * handle = 0;

		for (int i=0; i < allGraphicsItems.size(); ++i)
			if (allGraphicsItems[i])
			{
				handle = getHandle(allGraphicsItems[i]);

				if ( 	handle &&
						(textItem = qgraphicsitem_cast<TextGraphicsItem*>(allGraphicsItems[i])) &&
						(textItem->toPlainText() == handle->name || textItem->toPlainText() == handle->fullName()) )
						textItem->setPlainText(newHandle->name);

				setHandle(allGraphicsItems[i],newHandle);
			}

		if (renameCommand)
			renameCommand->redo();
	}

	void MergeHandlesCommand::undo()
	{
		if (newHandle == 0) return;

		if (renameCommand)
			renameCommand->undo();

		QList<ItemHandle*> keyHandles = oldChildren.keys();
		for (int i=0; i < keyHandles.size(); ++i)
			if (keyHandles[i])
			{
				QList<ItemHandle*> children = oldChildren[ keyHandles[i] ];
				for (int j=0; j < children.size(); ++j)
					if (children[j])
						children[j]->setParent(keyHandles[i],false);
			}

		TextGraphicsItem * textItem = 0;
		ItemHandle * handle = 0;

		keyHandles = oldGraphicsItems.keys();

		for (int i=0; i < keyHandles.size(); ++i)
			if (keyHandles[i])
			{
				QList<QGraphicsItem*> items = oldGraphicsItems[ keyHandles[i] ];
				for (int j=0; j < items.size(); ++j)
					if (items[j])
					{
						handle = getHandle(items[j]);

						if ( 	handle &&
								(textItem = qgraphicsitem_cast<TextGraphicsItem*>(items[j])) &&
								(textItem->toPlainText() == handle->name || textItem->toPlainText() == handle->fullName()) )
								textItem->setPlainText(keyHandles[i]->name);

						setHandle(items[j],keyHandles[i]);
					}
			}
	}

	SetParentHandleCommand::SetParentHandleCommand(const QString& name, NetworkHandle * net, ItemHandle * child, ItemHandle * parent)
		: QUndoCommand(name)
	{
		this->net = net;
		children += child;
		newParents += parent;
		oldParents += child->parent;
		renameCommand = 0;
	}

	SetParentHandleCommand::SetParentHandleCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& childlist, const QList<ItemHandle*>& parents)
		: QUndoCommand(name)
	{
		this->net = net;
		
		for (int i=0; i < childlist.size() && i < parents.size(); ++i)
			if (childlist[i] && !children.contains(childlist[i]))
			{
				children += childlist[i];
				newParents += parents[i];
				if (childlist[i])
					oldParents += childlist[i]->parent;
				else
					oldParents += 0;
			}
		renameCommand = 0;
	}

	SetParentHandleCommand::SetParentHandleCommand(const QString& name, NetworkHandle * net, const QList<ItemHandle*>& childlist, ItemHandle * parent)
		: QUndoCommand(name)
	{
		this->net = net;
		
		for (int i=0; i < childlist.size(); ++i)
			if (childlist[i] && !children.contains(childlist[i]))
			{
				children += childlist[i];
				newParents += parent;
				if (childlist[i])
					oldParents += childlist[i]->parent;
				else
					oldParents += 0;
			}
		renameCommand = 0;
	}

	SetParentHandleCommand::~SetParentHandleCommand()
	{
		if (renameCommand)
			delete renameCommand;
		/*for (int i=0; i < children.size(); ++i)
		{
			if (children[i] && 
				children[i]->graphicsItems.isEmpty() && 
				net && 
				
				!net->symbolsTable.isValidPointer((void*)(children[i])))
			{
				for (int j=i+1; j < children.size(); ++j)
					if (children[i] == children[j])
						children[j] = 0;
				delete children[i];
			}
		}*/
	}

	void SetParentHandleCommand::redo()
	{
		if (!renameCommand && net)
		{
			QList<QString> newNames, oldNames;
			QStringList allNames;
			QString s0, s1,s2;
			
			for (int i=0; i < children.size() && i < newParents.size() && i < oldParents.size(); ++i)
				if (children[i] && newParents[i] != oldParents[i])
				{
					if (children[i] != newParents[i] && !children[i]->isChildOf(newParents[i]))
					{
						//MainWindow::instance()->console()->message(children[i]->name);
						
						children[i]->setParent(newParents[i],false);
						s1 = children[i]->fullName();
						children[i]->setParent(oldParents[i],false);
						
						oldNames += children[i]->fullName();
						
						if (allNames.contains(s1) || 
							(net->symbolsTable.uniqueItems.contains(s1) && net->symbolsTable.uniqueItems[s1] != children[i]) ||
							(net->symbolsTable.uniqueData.contains(s1) && net->symbolsTable.uniqueData[s1].first != children[i])
							)
							s2 = net->makeUnique(s1,allNames);
						else
							s2 = s1;
						
						newNames += s2;
						allNames += s2;
					}
				}
			renameCommand = new RenameCommand(QString("rename"),net,oldNames,newNames);
		}

		if (renameCommand)
			renameCommand->redo();
		
		for (int i=0; i < children.size() && i < newParents.size() && i < oldParents.size(); ++i)
			if (children[i] && newParents[i] != oldParents[i])
			{
				if (children[i] != newParents[i] && !children[i]->isChildOf(newParents[i]))
				{
					children[i]->setParent(newParents[i],false);
				}
			}
	}

	void SetParentHandleCommand::undo()
	{
		if (renameCommand)
			renameCommand->undo();
			
		for (int i=0; i < children.size() && i < newParents.size() && i < oldParents.size(); ++i)
			if (children[i] && newParents[i] != oldParents[i])
			{
				if (children[i] != oldParents[i] && !children[i]->isChildOf(oldParents[i]))
				{
					children[i]->setParent(oldParents[i],false);
				}
			}
	}

	SetGraphicsSceneVisibilityCommand::SetGraphicsSceneVisibilityCommand(const QString& name, const QList<QGraphicsItem*>& list, const QList<bool>& values)
		: QUndoCommand(name)
	{
		ConnectionGraphicsItem * connection;
		NodeGraphicsItem * node;
		for (int i=0; i < list.size() && i < values.size(); ++i)
		{
			if (list[i] && list[i]->isVisible() != values[i])
			{
				items << list[i];
				before << list[i]->isVisible();
				if (connection = ConnectionGraphicsItem::cast(list[i]))
				{
					QList<QGraphicsItem*> list2 = connection->controlPointsAsGraphicsItems();
					list2 << connection->arrowHeadsAsGraphicsItems();
					if (connection->centerRegionItem)
						list2 << connection->centerRegionItem;
					for (int j=0; j < list2.size(); ++j)
						if (list2[j] && list[i]->isVisible() == list2[j]->isVisible())
						{
							items << list2[j];
							before << list[i]->isVisible();
						}
				}
				else
				if (node = NodeGraphicsItem::cast(list[i]))
				{
					QList<ControlPoint*> list2 = node->allControlPoints();
					for (int j=0; j < list2.size(); ++j)
						if (list2[j] && list[i]->isVisible() == list2[j]->isVisible())
						{
							items << list2[j];
							before << list[i]->isVisible();
						}
				}
			}
		}
	}

	SetGraphicsSceneVisibilityCommand::SetGraphicsSceneVisibilityCommand(const QString& name, QGraphicsItem* item, bool value)
		: QUndoCommand(name)
	{
		if (item && item->isVisible() != value)
		{
			ConnectionGraphicsItem * connection;
			NodeGraphicsItem * node;
			
			items << item;
			before << item->isVisible();
			
			if (connection = ConnectionGraphicsItem::cast(item))
			{
				QList<QGraphicsItem*> list2 = connection->controlPointsAsGraphicsItems();
				list2 << connection->arrowHeadsAsGraphicsItems();
				if (connection->centerRegionItem)
					list2 << connection->centerRegionItem;
				for (int j=0; j < list2.size(); ++j)
					if (list2[j] && item->isVisible() == list2[j]->isVisible())
					{
						items << list2[j];
						before << item->isVisible();
					}
			}
			else
			if (node = NodeGraphicsItem::cast(item))
			{
				QList<ControlPoint*> list2 = node->allControlPoints();
				for (int j=0; j < list2.size(); ++j)
					if (list2[j] && item->isVisible() == list2[j]->isVisible())
					{
						items << list2[j];
						before << item->isVisible();
					}
			}
		}
	}

	SetGraphicsSceneVisibilityCommand::SetGraphicsSceneVisibilityCommand(const QString& name, const QList<QGraphicsItem*>& list, bool value)
		: QUndoCommand(name)
	{
		ConnectionGraphicsItem * connection;
		NodeGraphicsItem * node;
		for (int i=0; i < list.size(); ++i)
		{
			if (list[i] && list[i]->isVisible() != value)
			{
				items << list[i];
				before << list[i]->isVisible();
				
				if (connection = ConnectionGraphicsItem::cast(list[i]))
				{
					QList<QGraphicsItem*> list2 = connection->controlPointsAsGraphicsItems();
					list2 << connection->arrowHeadsAsGraphicsItems();
					if (connection->centerRegionItem)
						list2 << connection->centerRegionItem;
					for (int j=0; j < list2.size(); ++j)
						if (list2[j] && list[i]->isVisible() == list2[j]->isVisible())
						{
							items << list2[j];
							before << list[i]->isVisible();
						}
				}
				else
				if (node = NodeGraphicsItem::cast(list[i]))
				{
					QList<ControlPoint*> list2 = node->allControlPoints();
					for (int j=0; j < list2.size(); ++j)
						if (list2[j] && list[i]->isVisible() == list2[j]->isVisible())
						{
							items << list2[j];
							before << list[i]->isVisible();
						}
				}
			}
		}
	}

	void SetGraphicsSceneVisibilityCommand::redo()
	{
		for (int i=0; i < items.size() && i < before.size(); ++i)
		{
			items[i]->setVisible(!before[i]);
		}
	}

	void SetGraphicsSceneVisibilityCommand::undo()
	{
		for (int i=0; i < items.size() && i < before.size(); ++i)
		{
			items[i]->setVisible(before[i]);
		}
	}
	
	SetHandleFamilyCommand::SetHandleFamilyCommand(const QString& name, const QList<ItemHandle*>& items, const QList<ItemFamily*>& families) 
		: QUndoCommand(name)
	{
		for (int i=0; i < items.size() && i < families.size(); ++i)
			if (items[i])
			{
				handles << items[i];
				oldFamily << items[i]->family();
				newFamily << families[i];
			}
	}
	
	SetHandleFamilyCommand::SetHandleFamilyCommand(const QString& name, ItemHandle* item, ItemFamily* family)
	{
		if (item)
		{
			handles << item;
			oldFamily << item->family();
			newFamily << family;
		}
	}
	
	void SetHandleFamilyCommand::redo()
	{
		for (int i=0; i < handles.size() && i < newFamily.size(); ++i)
			if (handles[i])			
				handles[i]->setFamily(newFamily[i],false);
	}
	
	void SetHandleFamilyCommand::undo()
	{
		for (int i=0; i < handles.size() && i < oldFamily.size(); ++i)
			if (handles[i])			
				handles[i]->setFamily(oldFamily[i],false);
	}
}

