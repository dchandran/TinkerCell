/****************************************************************************
 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)

 see COPYRIGHT.TXT
 
****************************************************************************/

#ifndef TINKERCELL_OCTAVETOOL_H
#define TINKERCELL_OCTAVETOOL_H

#include <QMainWindow>
#include <QHash>
#include <QToolButton>
#include <QSemaphore>
#include <QThread>
#include <QMenu>
#include <QFile>
#include <QActionGroup>
#include <QButtonGroup>
#include "Tool.h"
#include "DynamicLibraryMenu.h"
#include "OctaveInterpreterThread.h"

namespace Tinkercell
{

	class OctaveTool_FToS : public QObject
	{
		Q_OBJECT
		signals:
			void runOctaveCode(QSemaphore*,const QString&);
			void runOctaveFile(QSemaphore*,const QString&);
			void addOctavePlugin(QSemaphore*,const QString&,const QString&,const QString&,const QString&, const QString&);
		public slots:
			void runOctaveCode(const char*);
			void runOctaveFile(const char*);
			void addOctavePlugin(const char*,const char*,const char*,const char*,const char*);
	};

	class TINKERCELLCOREEXPORT OctaveTool : public Tool
	{
		Q_OBJECT

	public:
		OctaveTool();
		bool setMainWindow(MainWindow*);
		OctaveInterpreterThread * octaveInterpreter;

	public slots:
		void setupFunctionPointers( QLibrary * );
		bool loadFromDir( QDir& );
		bool loadFromDir(DynamicLibraryMenu* , QDir& );
		void toolLoaded(Tool*);
		void runOctaveCode(const QString&);
		void runOctaveFile(const QString&);
		void runOctaveCode(QSemaphore*,const QString&);
		void runOctaveFile(QSemaphore*,const QString&);
		
	protected slots:
		void buttonPressed (int);	 
		void actionTriggered(QAction *);
		void addOctavePlugin(QSemaphore*,const QString&,const QString&,const QString&,const QString&, const QString&);

	protected:
		void connectTCFunctions();
		QActionGroup actionsGroup;
		QButtonGroup buttonsGroup;
		QStringList octFileNames;
		QHash<QAction*,QString> hashOctFile;
		
	private:
		static OctaveTool_FToS * fToS;
		static void _runOctaveCode(const char*);
        static void _runOctaveFile(const char*);
        static void _addOctavePlugin(const char*,const char*,const char*,const char*,const char*);
	};
}

#endif

