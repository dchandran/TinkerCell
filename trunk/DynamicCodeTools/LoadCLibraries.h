/****************************************************************************
 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)

 see COPYRIGHT.TXT

 Provides a toolbar with buttons that call C functions (run of separate threads)
 
****************************************************************************/


#ifndef TINKERCELL_LOADCLIBRARIES_H
#define TINKERCELL_LOADCLIBRARIES_H

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

#ifdef Q_WS_WIN
#define MY_EXPORT __declspec(dllexport)
#else
#define MY_EXPORT
#endif

namespace Tinkercell
{

	class LoadCLibrariesTool_FToS : public QObject
	{
		Q_OBJECT

                 typedef void (*VoidFunction)();

                signals:
			void compileAndRun(QSemaphore*,int*,const QString&,const QString&);
                        void compileBuildLoad(QSemaphore*,int*,const QString&,const QString&,const QString&);
			void loadLibrary(QSemaphore*,const QString&);
                        void addFunction(QSemaphore*,VoidFunction, const QString& , const QString& , const QString& , const QString& ,const QString& , int, int);
                        void callback(QSemaphore*,VoidFunction);
		public slots:
			int compileAndRun(const char * cfile,const char* args);
                        int compileBuildLoad(const char * cfile,const char* f,const char* title);
			void loadLibrary(const char*);
                        void addFunction(VoidFunction, const char*, const char*, const char*, const char*, const char*, int, int);
                        void callback(VoidFunction);
	};

	class MY_EXPORT LoadCLibrariesTool : public Tool
	{
		Q_OBJECT

                typedef void (*VoidFunction)();

	public:
		LoadCLibrariesTool();
		bool setMainWindow(MainWindow*);

	public slots:
		void setupFunctionPointers( QLibrary * );
                //void loadFromFile(DynamicLibraryMenu* , QFile& file);
		void toolLoaded(Tool*);

	protected slots:
		void compileAndRunC(QSemaphore*,int*,const QString&,const QString&);
                void compileBuildLoadC(QSemaphore*,int*,const QString&,const QString&,const QString&);
                void loadLibrary(QSemaphore*,const QString&);
                void addFunction(QSemaphore*,VoidFunction, const QString& , const QString& , const QString& , const QString& ,const QString& , int, int);
                void callback(QSemaphore*,VoidFunction);
                //void buttonPressed (int);
                //void actionTriggered(QAction *);
		
	protected:
		void connectTCFunctions();
		QActionGroup actionsGroup;
		QButtonGroup buttonsGroup;
		QStringList dllFileNames;
		QHash<QAction*,QString> hashDll;
                DynamicLibraryMenu * libMenu;
		
	private:
		static LoadCLibrariesTool_FToS fToS;		
		static int _compileAndRun(const char * cfile,const char* args);
                static int _compileBuildLoad(const char * cfile,const char* f,const char* title);
		static void _loadLibrary(const char*);
                static void _addFunction(VoidFunction, const char*, const char*, const char*, const char*, const char *, int, int);
                static void _callback(VoidFunction);
            };
}

#endif
