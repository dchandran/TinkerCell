/****************************************************************************

Copyright (c) 2008 Deepak Chandran
Contact: Deepak Chandran (dchandran1@gmail.com)
See COPYRIGHT.TXT

The tool class is the parent class for all plugins.
A Tool is a Qt Widget with a name and pointer to the Tinkercell MainWindow.


****************************************************************************/

#ifndef TINKERCELL_TOOLDEF_H
#define TINKERCELL_TOOLDEF_H

#include <QWidget>
#include <QList>
#include <QHash>
#include <QAction>
#include <QActionGroup>
#include <QToolButton>
#include <QUndoCommand>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

#include "MainWindow.h"

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

namespace Tinkercell
{

	class MainWindow;
	class GraphicsScene;
	class NetworkHandle;
	class TextEditor;
	class ConsoleWindow;
	class Tool::GraphicsItem;

	/*! \brief everything other than the main window is a tool
	\ingroup core
	*/
	class MY_EXPORT Tool : public QWidget
	{

		Q_OBJECT

	public:
		/*! \brief name of this tool*/
		QString name;
		/*! \brief category that this tool belongs in*/
		QString category;
		/*! \brief brief description of this tool*/
		QString description;
		/*! \brief main window for this tool*/
		MainWindow * mainWindow;
		/*! \brief constructor*/
		Tool();
		/*! \brief destructor. removes graphicsItem and toolButton is not 0*/
		~Tool();
		/*! \brief constructor
			\param QString name
			\param QString category (default = empty)
			\param QWidget* parent (default = 0)
		*/
		Tool(const QString& Name, const QString& category = QString(), QWidget * parent = 0);
		/*! \brief set the main window for this tool*/
		virtual bool setMainWindow(MainWindow * main);
		/*! \brief console window (same as mainWindow->console())*/
		ConsoleWindow* console();
		/*! \brief the main window's current scene*/
		GraphicsScene* currentScene() const;
		/*! \brief the main window's current text editor*/
		TextEditor* currentTextEditor() const;
		/*! 
		* \brief the main window's current network
		* \return NetworkHandle* current network handle
		*/
		NetworkHandle* currentNetwork() const;
		/*!
		* \brief the main window's current network's current window
		* \return NetworkWindow* current network window
		*/
		NetworkWindow* currentWindow() const;
		/*!
		* \brief same as MainWindow::homeDir
		*/
		static QString homeDir();
		/*!
		* \brief same as MainWindow::tempDir
		*/
		static QString tempDir();
		/*!
		* \brief add an action that will be displayed in the context menu when specific items with this tool in their tools list are selected
		*/
		virtual void addAction(QAction*);
		/*!
		* \brief add a graphics item that will be displayed on the current scene when specific items with this tool in their tools list are selected
		*/
		virtual void addGraphicsItem(Tool::GraphicsItem*);
		/*! \brief tools that are drawn on the scene instead of displayed as a window
		\ingroup core
		*/
		class MY_EXPORT GraphicsItem : public QGraphicsItemGroup
		{

		public:
			/*! \brief constructor must have an associated Tool*/
			GraphicsItem(Tool*);
			/*! \brief this item has been selected*/
			virtual void select();
			/*! \brief this item has been deselected*/
			virtual void deselect();
			/*! \brief main window for this tool*/
			Tool * tool;
			/*! \brief for enabling dynamic_cast*/
			enum { Type = UserType + 10 };
			/*! \brief for enabling dynamic_cast*/
			int type() const
			{
				// Enable the use of dynamic_cast with this item.
				return Type;
			}
			/*! \brief show or hide this graphical tool. The graphical tool may choose whether or not to be visible based on other factors.*/
			virtual void visible(bool);
			/*! \brief cast a graphics item to a Tool::GraphicsItem
			\return Tool::GraphicsItem* can be 0 if invalid cast*/
			static GraphicsItem* cast(QGraphicsItem*);
		};

	public slots:
		/*! \brief what happens when this tool is selected */
		virtual void select(int i=0);
		/*! \brief what happens when this tool is deselected */
		virtual void deselect(int i=0);

	signals:
		/*! \brief this tool is selected */
		void selected();
		/*! \brief this tool is deselected */
		void deselected();
		
	private:
		/*! \brief optional graphics item used to display this tool */
		QList<GraphicsItem*> graphicsItems;
		/*! \brief actions displayed in the context menu when items related to this tool are selected */
		QActionGroup actionsGroup;
		
		friend class GraphicsScene;		
		friend class TextEditor;
		friend class MainWindow;
		friend class NetworkHandle;
	};

}

#endif
