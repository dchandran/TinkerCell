/****************************************************************************

 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)
 see COPYRIGHT.TXT
 
 Function that loads dll into main window

****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
//#include <QProcess>
#include "ConsoleWindow.h"
#include "DynamicCodeMain.h"

extern "C" MY_EXPORT void loadTCTool(Tinkercell::MainWindow * main)
{
	if (!main) return;

#ifdef Q_WS_WIN

	QProcess proc;
    QString appDir = QCoreApplication::applicationDirPath();
	QString homeDir = Tinkercell::MainWindow::userHome();
	
	proc.setWorkingDirectory(appDir);
	homeDir.replace(QObject::tr("/"),QObject::tr("\\"));
	appDir.replace(QObject::tr("/"),QObject::tr("\\"));
	
	QString s(QObject::tr("copy \"") + appDir + QObject::tr("\"\\c\\*.dll \"") + homeDir + QObject::tr("\" /Y"));
	//Tinkercell::ConsoleWindow::message(s);
	//proc.start(s);
	system(s.toAscii().data());
	
#endif
	Tinkercell::DynamicLibraryMenu * libMenu = new Tinkercell::DynamicLibraryMenu;
	main->addTool(libMenu);
	
	Tinkercell::LoadCLibrariesTool * cLibraries = new Tinkercell::LoadCLibrariesTool;
	main->addTool(cLibraries);
		
	Tinkercell::PythonTool * pythonTool = new Tinkercell::PythonTool;
	main->addTool(pythonTool);
		
	Tinkercell::CodingWindow * cScriptWindow = new Tinkercell::CodingWindow;
	main->addTool(cScriptWindow);

}

