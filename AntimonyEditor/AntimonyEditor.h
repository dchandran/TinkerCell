/****************************************************************************
 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)
 see COPYRIGHT.TXT

 A tool that allows users to construct models using Antimony scripts in the TextEditor

****************************************************************************/

#ifndef TINKERCELL_ANTIOMNYEDITOR_H
#define TINKERCELL_ANTIOMNYEDITOR_H

#include <QList>
#include <QMainWindow>
#include <QTextEdit>
#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QDialog>
#include <QCompleter>
#include <QListWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QThread>
#include <QToolBar>
#include <QTimeLine>
#include <QActionGroup>
#include <QLineEdit>
#include <QHash>
#include <QSemaphore>
#include "NetworkHandle.h"
#include "TextEditor.h"
#include "ItemHandle.h"
#include "TextParser.h"
#include "AntimonySyntaxHighlighter.h"

namespace Tinkercell
{
	class AntimonyEditor_FtoS : public QObject
	{
		Q_OBJECT

	signals:
		void loadSBMLStringSignal(QSemaphore*,const QString&);
		void loadAntimonyStringSignal(QSemaphore*,const QString&);
		void loadSBMLFileSignal(QSemaphore*,const QString&);
		void loadAntimonyFileSignal(QSemaphore*,const QString&);
		void getSBMLStringSignal(QSemaphore*,const QList<ItemHandle*>&, QString*);
		void getAntimonyStringSignal(QSemaphore*,const QList<ItemHandle*>&, QString*);
		void writeSBMLFileSignal(QSemaphore*,const QList<ItemHandle*>&, const QString&);
		void writeAntimonyFileSignal(QSemaphore*,const QList<ItemHandle*>&, const QString&);
	public slots:
		void loadSBMLString(const char *);
		void loadAntimonyString(const char *);
		void loadSBMLFile(const char *);
		void loadAntimonyFile(const char *);
		const char* getSBMLString(ArrayOfItems);
		const char* getAntimonyString(ArrayOfItems);
		void writeSBMLFile(ArrayOfItems,const char*);
		void writeAntimonyFile(ArrayOfItems,const char*);

	};

	class AntimonyEditor : public TextParser
	{
	    Q_OBJECT

	public:
		AntimonyEditor();
		/*! \brief make necessary signal/slot connections*/
		bool setMainWindow(MainWindow*);
		/*! \brief parse text and convert them to items*/
		QList<ItemHandle*> parse(const QString& modelString);

		static QString getAntimonyScript(const QList<ItemHandle*>&);

	public slots:
		/*! \brief parse text and insert items*/
		void parse(TextEditor * editor);

		/*! \brief parse text and insert items*/
		void parse();

		/*! \brief some text inside this editor has been changed
            \param QString old text
            \param QString new text
        */
        void textChanged(TextEditor *, const QString&, const QString&, const QString&);
        /*! \brief the cursor has moved to a different line
            \param int index of the current line
            \param QString current line text
        */
        void lineChanged(TextEditor *, int, const QString&);
		/*!
        * \brief if text editor is opened, sets its syntax highlighter
        * \param NetworkHandle* the current new window
        * \return void
        */
        void networkOpened(NetworkHandle*);
		/*!
        * \brief insert module(s) in the scene
        */
		//void insertModule();
		/*!
        * \brief copy the antimony script of items as they are copied
        */
		void copyItems(GraphicsScene * scene, QList<QGraphicsItem*>& , QList<ItemHandle*>& );

		void loadSBMLFile();
		void pasteSBMLText();
		void saveSBMLFile();
		void copySBMLText();
		void saveAntimonyFile();
		void copyAntimonyText();

	signals:
		/*! \brief invalid syntax*/
		void validSyntax(bool);

	protected:
		/*! \brief clone given items
			\param QList<TextItem*> items to clone
		*/
		static QList<ItemHandle*> clone(const QList<ItemHandle*>&);

		CodeEditor * scriptDisplayWindow;

	private slots:
		/*! \brief display antimony script when a module info is being displayed (see modelSummaryTool)        */
		//void displayModel(QTabWidget&, const QList<ItemHandle*>&, QHash<QString,qreal>&, QHash<QString,QString>&);
		/*! \brief makes a new text window with the script representing the given items*/
		void createTextWindow(TextEditor *, const QList<ItemHandle*>&);
		/*! \brief used to connect to modelSummaryTool*/
		void toolLoaded(Tool*);
		void setupFunctionPointers( QLibrary * library);
		void loadSBMLStringSlot(QSemaphore*,const QString&);
		void loadAntimonyStringSlot(QSemaphore*,const QString&);
		void loadSBMLFileSlot(QSemaphore*,const QString&);
		void loadAntimonyFileSlot(QSemaphore*,const QString&);
		void getSBMLStringSlot(QSemaphore*,const QList<ItemHandle*>&, QString*);
		void getAntimonyStringSlot(QSemaphore*,const QList<ItemHandle*>&, QString*);
		void writeSBMLFileSlot(QSemaphore*,const QList<ItemHandle*>&, const QString&);
		void writeAntimonyFileSlot(QSemaphore*,const QList<ItemHandle*>&, const QString&);

	private:
		static AntimonyEditor_FtoS fToS;
		static void _loadSBMLString(const char *);
		static void _loadAntimonyString(const char *);
		static void _loadSBMLFile(const char *);
		static void _loadAntimonyFile(const char *);
		static const char* _getSBMLString(ArrayOfItems);
		static const char* _getAntimonyString(ArrayOfItems);
		static void _writeSBMLFile(ArrayOfItems,const char*);
		static void _writeAntimonyFile(ArrayOfItems,const char*);
		void connectTCFunctions();
		static void appendScript(QString&, const QList<ItemHandle*>&);

	};

}

extern "C" TINKERCELLEXPORT void loadTCTool(Tinkercell::MainWindow * main);

#endif
