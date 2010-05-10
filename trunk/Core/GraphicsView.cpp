/****************************************************************************

Copyright (c) 2008 Deepak Chandran
Contact: Deepak Chandran (dchandran1@gmail.com)
See COPYRIGHT.TXT

The GraphicsView class provides a view for a GraphicsScene. It 

****************************************************************************/

#include "NetworkWindow.h"
#include "MainWindow.h"
#include "NodeGraphicsItem.h"
#include "NodeGraphicsReader.h"
#include "ConnectionGraphicsItem.h"
#include "TextGraphicsItem.h"
#include "ItemHandle.h"
#include "UndoCommands.h"
#include "ConsoleWindow.h"
#include "CloneItems.h"
#include "GraphicsScene.h"
#include "GraphicsView.h"

namespace Tinkercell
{
	void GraphicsView::drawBackground( QPainter * painter, const QRectF & rect )
	{
		if (!background.isNull() && painter)
			painter->drawPixmap(rect,background,QRectF(background.rect()));
	}

	void GraphicsView::drawForeground( QPainter * painter, const QRectF & rect )
	{
		if (!foreground.isNull() && painter)
			painter->drawPixmap(rect,foreground,QRectF(foreground.rect()));
	}

	void GraphicsView::wheelEvent(QWheelEvent * wheelEvent)
	{
		if (wheelEvent->modifiers() & Qt::ControlModifier)
		{
			double factor = 1.0 + 0.2 * qAbs(wheelEvent->delta()/120.0);
			if (wheelEvent->delta() > 0)
				scale(factor,factor);
			else
				scale(1.0/factor,1.0/factor);
		}
		else
		{
			QGraphicsView::wheelEvent(wheelEvent);
		}
	}

	void GraphicsView::scrollContentsBy ( int dx, int dy )
	{
		QGraphicsView::scrollContentsBy(dx,dy);
		if (scene) 
			scene->update();
	}
	
	QSize GraphicsView::sizeHint() const
	{
		return QSize(500,500);
	}

	void GraphicsView::dragEnterEvent(QDragEnterEvent * /*event*/)
	{
		//event->acceptProposedAction();
	}

	void GraphicsView::dropEvent(QDropEvent * /*event*/)
	{
		/*if (parentWidget())
		{
		QList<QUrl> urlList;
		QList<QFileInfo> files;
		QString fName;
		QFileInfo info;

		if (event->mimeData()->hasUrls())
		{
		urlList = event->mimeData()->urls(); // returns list of QUrls

		// if just text was dropped, urlList is empty (size == 0)
		if ( urlList.size() > 0) // if at least one QUrl is present in list
		{
		fName = urlList[0].toLocalFile(); // convert first QUrl to local path
		info.setFile( fName ); // information about file
		if ( info.isFile() )
		files += info;
		}
		}
		MainWindow * main = static_cast<MainWindow*>(parentWidget());
		main->dragAndDropFiles(files);
		}*/
	}
	/*! \brief Constructor: connects all the signals of the new window to that of the main window */
	GraphicsView::GraphicsView(NetworkWindow * network)
		: QGraphicsView (network->scene,network), scene(network->scene)
	{		
		setCacheMode(QGraphicsView::CacheBackground);
		setViewportUpdateMode (QGraphicsView::BoundingRectViewportUpdate);

		//setViewportUpdateMode (QGraphicsView::FullViewportUpdate);
		//setViewportUpdateMode (QGraphicsView::SmartViewportUpdate);
		//setViewport(new QGLWidget);
		//setDragMode(QGraphicsView::RubberBandDrag);
		//setDragMode(QGraphicsView::ScrollHandDrag);
		//setOptimizationFlags(QGraphicsView::DontClipPainter | QGraphicsView::DontSavePainterState);

		setPalette(QPalette(QColor(255,255,255)));
		setAutoFillBackground(true);
		setAcceptDrops(true);

		setRenderHint(QPainter::Antialiasing);
		setCacheMode(QGraphicsView::CacheBackground);
		setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
		fitInView(QRectF(0,0,1000,800),Qt::KeepAspectRatio);

		setFocusPolicy(Qt::StrongFocus);
	}

	void GraphicsView::mousePressEvent ( QMouseEvent * event )
	{
		if (scene && scene->networkWindow)
		{
			scene->networkWindow->setAsCurrentWindow();
		}
		
		QGraphicsView::mousePressEvent(event);
	}

	void GraphicsView::keyPressEvent ( QKeyEvent * event )
	{
		if (scene && scene->networkWindow)
		{
			scene->networkWindow->setAsCurrentWindow();
		}
		QGraphicsView::keyPressEvent(event);
	}
}
