/****************************************************************************

 Copyright (c) 2008 Deepak Chandran
 Contact: Deepak Chandran (dchandran1@gmail.com)
 see COPYRIGHT.TXT
 
 Tool for displaying the NodesTree and selecting the nodes on that tree.
 This tool also stores the tree of node families as a hashtable of <name,family> pairs.

****************************************************************************/

#include "NodesTree.h"
#include <QtDebug>
#include <QRegExp>

namespace Tinkercell
{
     NodesTree::~NodesTree()
     {
		  windowClosing(0,0);
          QList<NodeFamily*> list = nodeFamilies.values();

          for (int i=0; i < list.size(); ++i)
          {
               NodeFamily * node = list[i];
               if (node)
               {
                    delete node;
                    node = 0;
               }
          }
     }

     QSize NodesTree::sizeHint() const
     {
          return QSize(140, 200);
     }

     NodesTree::NodesTree(QWidget * parent, const QString& filename) :
               Tool(tr("Nodes Tree"),parent)
     {
          QVBoxLayout * layout = new QVBoxLayout;
          treeWidget.setHeaderHidden(true);

          treeWidget.header()->setResizeMode(QHeaderView::Stretch);

          layout->addWidget(&treeWidget);
          layout->setContentsMargins(0,0,0,0);
          layout->setSpacing(0);
          setLayout(layout);

          NodesTreeReader reader;
          QString appDir = QCoreApplication::applicationDirPath();

          QCoreApplication::setOrganizationName("TinkerCell");
          QCoreApplication::setOrganizationDomain("www.tinkercell.com");
          QCoreApplication::setApplicationName("TinkerCell");
          QSettings settings("TinkerCell", "TinkerCell");
          settings.beginGroup("NodesTree");

          QString xmlFile ;
          if (xmlFile.isNull() || xmlFile.isEmpty())
               xmlFile = (appDir + tr("/NodesTree/NodesTree.xml"));

          reader.readXml(this,xmlFile);

          QList<QString> keys = nodeFamilies.keys();

          for (int i=0; i < keys.size(); ++i)
          {
               NodeFamily * node = nodeFamilies.value(keys[i]);
               if (!node)
                   continue;

               QList<QTreeWidgetItem*> treeItem = treeItems.values(keys[i]);
               if (node && !treeItem.isEmpty())
               {
                    QString setting = settings.value(node->name,QString()).toString();
                    if (!setting.isEmpty())
                    {
                         QStringList lst = setting.split(tr(","));
                         if (lst.size() > 0)
                              for (int j=0; j < treeItem.size(); ++j)
                                   treeItem[j]->setExpanded(lst[1] == tr("expanded"));
                    }

                    for (int j=0; j < treeItem.size(); ++j)
                    {
                         FamilyTreeButton * button = new FamilyTreeButton(node,this);
                         button->nodesTree = this;

                         connect(button,SIGNAL(nodeSelected(NodeFamily*)),this,SLOT(buttonPressed(NodeFamily*)));
                         //button->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
                         //button->setDefault(false);
                         //button->setFlat(true);

                         QHBoxLayout * layout = new QHBoxLayout;
                         layout->addWidget(button,0,Qt::AlignLeft);
                         layout->setContentsMargins(0,0,0,0);
                         QWidget * widget = new QWidget;
                         widget->setPalette(QPalette(QColor(255,255,255)));
                         widget->setAutoFillBackground (true);
                         widget->setLayout(layout);

                         treeWidget.setItemWidget(treeItem[j],0,widget);
						 treeButtons.insertMulti(keys[i],button);
                    }
               }
          }

          settings.endGroup();
          makeNodeSelectionDialog();
     }

     void NodesTree::buttonPressed(NodeFamily * node)
     {
          if (node)
          {
               emit sendEscapeSignal(this);
               emit nodeSelected(node);
          }
     }

     bool NodesTree::setMainWindow(MainWindow * TinkercellWindow)
     {
          Tool::setMainWindow(TinkercellWindow);

          if (mainWindow)
          {
               connect(this,SIGNAL(sendEscapeSignal(const QWidget*)),mainWindow,SIGNAL(escapeSignal(const QWidget*)));
			   return true;
          }
          return false;
     }

     /*void NodesTree::keyPressEvent ( QKeyEvent * event )
     {
          emit keyPressed(event->key(),event->modifiers());
          if (event->key() == Qt::Key_Escape || event->key() == Qt::Key_Space)
               emit sendEscapeSignal(this);
     }*/

     void NodesTree::changeTree()
     {
          QString fileName =
                    QFileDialog::getOpenFileName(this, tr("Nodes Tree File"),
                                                 tr(""),
                                                 tr("XML Files (*.xml)"));
          if (fileName.isEmpty())
               return;

          QCoreApplication::setOrganizationName("TinkerCell");
          QCoreApplication::setOrganizationDomain("www.tinkercell.com");
          QCoreApplication::setApplicationName("TinkerCell");

          QSettings settings("TinkerCell", "TinkerCell");

          settings.beginGroup("NodesTree");
          settings.setValue("file", fileName);
          settings.endGroup();

          QMessageBox::information (this, tr("Change tree"), tr("The new nodes tree will be active the next time you start TinkerCell"));
     }

     void NodesTree::contextMenuEvent(QContextMenuEvent * event)
     {
          emit sendEscapeSignal(this);
          static QMenu * menu = 0;
          if (!menu)
          {
               menu = new QMenu(this);
               menu->addAction(QIcon(":/images/image.png"),tr("Change nodes catalog"),this,SLOT(changeTree()));
          }

          QWidget * widget = this;
          QPoint pos = event->pos();
          while (widget)
          {
               pos += widget->pos();
               widget = widget->parentWidget();
          }
          if (event)
               menu->exec(pos);
     }
	 
	 void NodesTree::itemActivated( QListWidgetItem * ) 
	 {
		nodeFileAccepted();
	 }


     void NodesTree::makeNodeSelectionDialog()
     {
          QString appDir = QCoreApplication::applicationDirPath();

          QDir dir(appDir + tr("/NodeItems/"));
          dir.setFilter(QDir::Files);
          dir.setSorting(QDir::Name);

          QFileInfoList list = dir.entryInfoList();

          nodesListWidget = new QListWidget(this);
          nodesFilesList.clear();

          for (int i = 0; i < list.size(); ++i)
          {
               QFileInfo fileInfo = list.at(i);
               if (fileInfo.completeSuffix().toLower() == tr("png") &&
                   dir.exists(fileInfo.baseName() + tr(".xml")))
               {
                    QListWidgetItem * item = new QListWidgetItem(QIcon(fileInfo.absoluteFilePath()),
                                                                 dir.absolutePath() + tr("/") + fileInfo.baseName() + tr(".xml"),nodesListWidget);
                    nodesListWidget->addItem(item);
                    nodesFilesList << item->text();
               }
          }

          QDir userdir(MainWindow::userHome());
          if (userdir.exists(tr("/NodeItems/")))
          {
               userdir.cd(tr("/NodeItems/"));
               userdir.setFilter(QDir::Files);
               userdir.setSorting(QDir::Name);
               list = userdir.entryInfoList();

               for (int i = 0; i < list.size(); ++i)
               {
                    QFileInfo fileInfo = list.at(i);
                    if (fileInfo.completeSuffix().toLower() == tr("png") &&
                        userdir.exists(fileInfo.baseName() + tr(".xml")) &&
                        !nodesFilesList.contains(userdir.absolutePath() + tr("/") + fileInfo.baseName() + tr(".xml")))
                    {
                         QListWidgetItem * item = new QListWidgetItem(QIcon(fileInfo.absoluteFilePath()),
                                                                      userdir.absolutePath() + tr("/") + fileInfo.baseName() + tr(".xml"),nodesListWidget);
                         nodesListWidget->addItem(item);
                         nodesFilesList << item->text();
                    }
               }
          }

          nodeSelectionDialog = new QDialog(this);
          nodeSelectionDialog->setSizeGripEnabled(true);
          QVBoxLayout * layout = new QVBoxLayout;

          layout->addWidget(nodesListWidget);
		  connect(nodesListWidget,SIGNAL(itemActivated( QListWidgetItem * )),this,SLOT(itemActivated( QListWidgetItem * )));

          QHBoxLayout * buttonsLayout = new QHBoxLayout;
          QPushButton * ok = new QPushButton(tr("Replace"));
          QPushButton * cancel = new QPushButton(tr("Cancel"));
          QPushButton * otherFile = new QPushButton(tr("File not listed..."));

          connect(ok,SIGNAL(released()),nodeSelectionDialog,SLOT(accept()));
          connect(cancel,SIGNAL(released()),nodeSelectionDialog,SLOT(reject()));
          connect(otherFile,SIGNAL(released()),this,SLOT(selectNewNodeFile()));

          connect(nodeSelectionDialog,SIGNAL(accepted()),this,SLOT(nodeFileAccepted()));


          buttonsLayout->addWidget(otherFile);
          buttonsLayout->addWidget(cancel);
          buttonsLayout->addWidget(ok);

          layout->addLayout(buttonsLayout);

          nodeSelectionDialog->setLayout(layout);
     }

     void NodesTree::selectNewNodeFile()
     {
          QString fileName =
                    QFileDialog::getOpenFileName(mainWindow, tr("New Graphics File"),
                                                 QDir::currentPath(),
                                                 tr("XML Files (*.xml)"));
          if (fileName.isEmpty())
               return;

          QFileInfo fileInfo(fileName);

          if (fileInfo.completeSuffix().toLower() == tr("xml") &&
              !nodesFilesList.contains(fileInfo.dir().absolutePath() + tr("/") + fileInfo.baseName() + tr(".xml")))
          {
               QListWidgetItem * item = new QListWidgetItem(
                         QIcon(fileInfo.dir().absolutePath() + tr("/") + fileInfo.baseName() + tr(".png")),
                         fileInfo.dir().absolutePath() + tr("/") + fileInfo.baseName() + tr(".xml"),nodesListWidget);
               nodesListWidget->addItem(item);
               nodesFilesList << item->text();
          }
     }

     QString NodesTree::replaceNodeFile()
     {
          temporaryFileName = QString();

          nodeSelectionDialog->exec();

          return temporaryFileName;
     }

     void NodesTree::nodeFileAccepted()
     {
          if (!nodesListWidget)
               temporaryFileName = QString();
          else
               if (!nodesListWidget->currentItem())
                    temporaryFileName = QString();
          else
               temporaryFileName = nodesListWidget->currentItem()->text();
     }

     void NodesTree::windowClosing(NetworkWindow * , bool *)
     {
          //save state of the tree
          QCoreApplication::setOrganizationName("TinkerCell");
          QCoreApplication::setOrganizationDomain("www.tinkercell.com");
          QCoreApplication::setApplicationName("TinkerCell");

          QSettings settings("TinkerCell", "TinkerCell");

          settings.beginGroup("NodesTree");

          QList<QString> keys = nodeFamilies.keys();

          for (int i=0; i < keys.size(); ++i)
          {
               NodeFamily * family = nodeFamilies[keys[i] ];
               QTreeWidgetItem* item = treeItems.value( keys[i] );
               if (family && item && family->graphicsItems.size() > 0 && qgraphicsitem_cast<NodeGraphicsItem*>(family->graphicsItems[0]))
               {
                    if (item->isExpanded())
                         settings.setValue(family->name, (qgraphicsitem_cast<NodeGraphicsItem*>(family->graphicsItems[0]))->fileName + tr(",expanded"));
                    else
                         settings.setValue(family->name, (qgraphicsitem_cast<NodeGraphicsItem*>(family->graphicsItems[0]))->fileName + tr(",collapsed"));
               }

          }
          settings.endGroup();
     }
	 
	 QString NodesTree::iconFile(QString name)
	 {
		QString file = tr("NodeItems/");
		file += name;
		file.replace(tr(" "),tr(""));
		file += tr(".PNG");
		return  file;
	 }

	 QString NodesTree::nodeImageFile(QString name)
	 {
		QString file = tr("NodeItems/");
		file += name;
		file.replace(tr(" "),tr(""));
		file += tr(".xml");
		return  file;
	 }
	 
	QTreeWidget & NodesTree::widget() 
	{ 
		return treeWidget; 
	}
	
	void NodesTree::addNewButton(const QList<QToolButton*>& buttons,const QString& group)
	{
		emit addNewButtonSignal(buttons,group);
	}
}


