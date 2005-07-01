
#include "TrussUnitDesignerWidget.h"
#include "SubsidiaryConstants.h"
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <qsize.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpoint.h>

/*****************************************************************************
 * Truss Unit Designer Widget
 *****************************************************************************/

TrussUnitDesignerWidget::TrussUnitDesignerWidget ( QWidget* p ) :
    AggQWidget(p, flipY),
    init(true),
    X1(50), Y1(50),
    X2(330), Y2(330)
{}

TrussUnitDesignerWidget::~TrussUnitDesignerWidget ()
{
    clearTrussUnitWindows();
}

void TrussUnitDesignerWidget::clearTrussUnitWindows ()
{
    trussWindows.clear();    
}

void TrussUnitDesignerWidget::addTrussUnitWindow ( TrussUnitWindow& trussWindow )
{    
    trussWindows.push_back(&trussWindow);

    QPoint pos1;
    pos1.setX ( X1 ); 
    pos1.setY ( Y1 ); 
    trussWindow.setWindowPosition ( pos1 );
    trussWindow.setBorderColor ( 40,65,60 );
    trussWindow.setCanvasColor ( 8, 10, 12 );
    trussWindow.setResEllColor ( 50, 50, 50 );
    trussWindow.setHeadlineFirstColor (180, 130, 100 );
    trussWindow.setHeadlineMiddleColor ( 230, 210, 200 );
    trussWindow.setHeadlineLastColor ( 150, 90, 110 );
}

bool TrussUnitDesignerWidget::removeTrussUnitWindow ( const TrussUnitWindow& window )
{
    WindowList::iterator iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( (*iter) == &window ) 
        {
            trussWindows.erase(iter);            
            return true;
        }
    return false; 
}

TrussUnitWindow* TrussUnitDesignerWidget::findTrussUnitWindowByCoord ( int x, int y )
{
    WindowList::reverse_iterator rev_iter = trussWindows.rbegin();
    for ( ; rev_iter != trussWindows.rend(); ++rev_iter )
        if ( (*rev_iter)->inWindowRect(x, y) )
            return *rev_iter;
    return 0;
}

void TrussUnitDesignerWidget::trussWindowToFront ( TrussUnitWindow& window )
{
    if ( trussWindows.back() == &window )
        return;
    trussWindows.push_back(&window);
    WindowList::iterator iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
    {
        if ( *iter == &window )
        {
    	    trussWindows.erase(iter);
            return;
        }
    }
}

void TrussUnitDesignerWidget::redrawTrussWindows ()
{
    WindowList::reverse_iterator rev_iter = trussWindows.rbegin();
    for ( ; rev_iter != trussWindows.rend(); ++rev_iter )
        (*rev_iter)->setRenderingStatus ( false );
    update();
}

void TrussUnitDesignerWidget::resizeTrussWindow ( TrussUnitWindow* window, 
                                                 QPoint newLeftTopPos, 
                                                 QPoint newRightBottomPos, 
                                                 rbuf_dynarow* rbuf )
{
    int dx = newRightBottomPos.x() - newLeftTopPos.x();
    int dy = newRightBottomPos.y() - newLeftTopPos.y();
    rbuf->init ( dx, dy );
    window->setWindowSize ( dx, dy );
    window->setWindowPosition ( newLeftTopPos );
}

void TrussUnitDesignerWidget::removeAllHighlight ()
{
    if ( nodeBehaviour == onNodeSelect )
    {
        WindowListIter iter = trussWindows.begin();
        for ( ; iter != trussWindows.end(); ++iter ) 
            (*iter)->removeNodeHighlight ();
        nodeBehaviour = nodeIdle;
        update ();
    }
    if ( pivotBehaviour == onPivotSelect )
    {
        WindowListIter iter = trussWindows.begin();
        for ( ; iter != trussWindows.end(); ++iter ) 
            (*iter)->removePivotHighlight ();
        pivotBehaviour = pivotIdle;
        update ();
    }
}

void TrussUnitDesignerWidget::moveTrussNode ( int x, int y, TrussUnitWindow* window, 
                                        TrussNode* node )
{
    // TODO: flipY comparison
    double scaleMultX = window->getScaleMultiplierX ();
    double scaleMultY = window->getScaleMultiplierY ();
    int newX = ( x - window->getWindowLeftTopPos ().x() - leftWindowIndent ) / scaleMultX;
    int newY = ( window->getWindowSize().height() - y + 
                window->getWindowLeftTopPos ().y() - bottomWindowIndent ) / scaleMultY;

    int areaWidth = window->getTrussAreaSize().width();
    int areaHeight = window->getTrussAreaSize().height();
    if ( newX < 0 )
        newX = 0;
    if ( newX > areaWidth )
        newX = areaWidth;
    if ( newY < 0 )
        newY = 0;
    if ( newY > areaHeight )
        newY = areaHeight;

    node->setX ( newX );
    node->setY ( newY );
}

void TrussUnitDesignerWidget::moveTrussPivot ( int dx, int dy, TrussUnitWindow* window, 
                                        TrussPivot* pivot )
{
    // TODO: flipY comparison
    double scaleMultX = window->getScaleMultiplierX ();
    double scaleMultY = window->getScaleMultiplierY ();

    int oldXFirst = pivot->getFirstNode().getX();
    int oldYFirst = pivot->getFirstNode().getY();
    int oldXLast = pivot->getLastNode().getX();
    int oldYLast = pivot->getLastNode().getY();
    QPoint oldFirstCoord ( oldXFirst, oldYFirst );
    QPoint oldLastCoord ( oldXLast, oldYLast );

    int newXFirst = ( oldXFirst + dx / scaleMultX );
    int newYFirst = ( oldYFirst - dy / scaleMultY );
    int newXLast = ( oldXLast + dx / scaleMultX );
    int newYLast = ( oldYLast - dy / scaleMultY );

    int areaWidth = window->getTrussAreaSize().width();
    int areaHeight = window->getTrussAreaSize().height();
    if ( newXFirst < 0 )
    {
        newXFirst = 0;
        newXLast = oldXLast - oldXFirst;
    }
    if ( newXLast < 0 )
    {
        newXLast = 0;
        newXFirst = oldXFirst - oldXLast;
    }
    if ( newYFirst < 0 )
    {
        newYFirst = 0;
        newYLast = oldYLast - oldYFirst;
    }
    if ( newYLast < 0 )
    {
        newYLast = 0;
        newYFirst = oldYFirst - oldYLast;
    }
    if ( newXFirst > areaWidth )
    {
        newXFirst = areaWidth;
        newXLast = oldXLast + ( areaWidth - oldXFirst );
    }
    if ( newXLast > areaWidth )
    {
        newXLast = areaWidth;
        newXFirst = oldXFirst + ( areaWidth - oldXLast );
    }
    if ( newYFirst > areaHeight )
    {
        newYFirst = areaHeight;
        newYLast = oldYLast + ( areaHeight - oldYFirst);
    }
    if ( newYLast > areaHeight )
    {
        newYLast = areaHeight;
        newYFirst = oldYFirst + ( areaHeight - oldYLast);
    }

    TrussNode* firstNode = window->findNodeByCoord ( oldFirstCoord );
    TrussNode* lastNode = window->findNodeByCoord ( oldLastCoord );
    firstNode->setX ( newXFirst );
    firstNode->setY ( newYFirst );
    lastNode->setX ( newXLast );
    lastNode->setY ( newYLast );
}

void TrussUnitDesignerWidget::onDraw ()
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    solid_renderer solidRend ( baseRend );
    baseRend.clear ( agg::rgba (10, 10, 10) );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end() - 1; ++iter ) 
    {
        TrussUnitWindow* trussWindow = (*iter);
        trussWindow->paint( baseRend );
    }
    TrussUnitWindow* trussWindow = (*iter);
    trussWindow->setBorderColor ( 25,55,65 );
    trussWindow->setResEllColor ( 50, 50, 50 );
    trussWindow->setHeadlineFirstColor (180, 130, 100 );
    trussWindow->setHeadlineMiddleColor ( 230, 190, 170 );
    trussWindow->setHeadlineLastColor ( 150, 90, 80 );
    trussWindows.back()->paint ( baseRend  ); // draw front window
    trussWindow->setBorderColor ( 40,65,60 );
    trussWindow->setResEllColor ( 120, 120, 120 );
    trussWindow->setHeadlineFirstColor (180, 130, 150 );
    trussWindow->setHeadlineMiddleColor ( 230, 210, 200 );
    trussWindow->setHeadlineLastColor ( 150, 90, 110 );
}

void TrussUnitDesignerWidget::initTrussUnitWindow ()  //temp method. Later to remove.
{
    QWidget::setMouseTracking(true);
    QPoint pos1, pivotPnt1, pivotPnt2;
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) { 
        TrussUnitWindow* trussWindow = (*iter);        
        pos1.setX ( X1 ); 
        pos1.setY ( Y1 ); 
        winBehaviour = windowIdle;
        trussWindow->createNode ( 280, 30 );
        pivotPnt1.setX ( 0 );
        pivotPnt1.setY ( 0 );
        pivotPnt2.setX ( 130 );
        pivotPnt2.setY ( 130 );
        trussWindow->createPivot ( pivotPnt1, pivotPnt2 );
        pivotPnt1.setX ( 252 );
        pivotPnt1.setY ( 300 );
        trussWindow->createPivot ( pivotPnt1, pivotPnt2 );
        pivotPnt1.setX ( 0 );
        pivotPnt1.setY ( 300 );
        trussWindow->createPivot ( pivotPnt1, pivotPnt2 );
        pivotPnt2.setX ( 250 );
        pivotPnt2.setY ( 300 );
        trussWindow->createPivot ( pivotPnt1, pivotPnt2 );
        pivotPnt2.setX ( 0 );
        pivotPnt2.setY ( 0 );
        trussWindow->createPivot ( pivotPnt1, pivotPnt2 );
    }
    init = false;
}

/*****************************************************************************/
// Event handlers

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* )
{
   if ( init ) 
       initTrussUnitWindow ();
   onDraw();
}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* )
{
}

void TrussUnitDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
    int x = me->x();
    int y = flipY ? height() - me->y() : me->y();
    if ( winBehaviour == onHorResize )
    {
		int dx = x - clickX;
        QPoint point1 = trussWindow->getWindowLeftTopPos ();
        QPoint point2 = trussWindow->getWindowRightBottomPos ();
        if ( abs ( clickX - point1.x() ) <= bordWidth )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
                point1.setX( point2.x() - resizeLimit );
        }
        if ( abs ( clickX - point2.x() ) <= bordWidth )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
               point2.setX( point1.x() + resizeLimit );
        }
        resizeTrussWindow ( trussWindow, point1, point2, trussWindow->getRBufDynarow() );
        update();
		clickX += dx;
        return;
    }
    if ( winBehaviour == onVerResize )
    {
	    int dy = y - clickY;
        QPoint point1 = trussWindow->getWindowLeftTopPos ();
        QPoint point2 = trussWindow->getWindowRightBottomPos ();
        if ( abs ( clickY - point1.y() ) <= bordWidth )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
                point1.setY( point2.y() - resizeLimit );
        }
        if ( abs ( clickY - point2.y() ) <= bordWidth )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
               point2.setY( point1.y() + resizeLimit );
        }
        resizeTrussWindow ( trussWindow, point1, point2, trussWindow->getRBufDynarow() );
        update();
		clickY += dy;
        return;
    }
    if ( winBehaviour == onBDiagResize )
    {
        int dx = x - clickX;
	    int dy = y - clickY;
        QPoint point1 = trussWindow->getWindowLeftTopPos ();
        QPoint point2 = trussWindow->getWindowRightBottomPos ();
        if ( abs ( clickX - (point1.x() + bordWidth) ) <= bordWidth )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
                point1.setX( point2.x() - resizeLimit );
        }
        if ( abs ( clickX - (point2.x() - bordWidth) ) <= bordWidth )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
               point2.setX( point1.x() + resizeLimit );
        }
        if ( abs ( clickY - (point1.y() + bordWidth) ) <= bordWidth )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
                point1.setY( point2.y() - resizeLimit );
        }
        if ( abs ( clickY - (point2.y() - bordWidth) ) <= bordWidth )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
               point2.setY( point1.y() + resizeLimit );
        }
        resizeTrussWindow ( trussWindow, point1, point2, trussWindow->getRBufDynarow() );
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
    if ( winBehaviour == onFDiagResize )
    {
        int dx = x - clickX;
        int dy = y - clickY;
        QPoint point1 = trussWindow->getWindowLeftTopPos ();
        QPoint point2 = trussWindow->getWindowRightBottomPos ();
        if ( abs ( clickX - (point1.x() + bordWidth) ) <= bordWidth )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
                point1.setX ( point2.x() - resizeLimit );
        }
        if ( abs ( clickX - (point2.x() - bordWidth) ) <= bordWidth )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
               point2.setX ( point1.x() + resizeLimit );
        }
        if ( abs ( clickY - (point1.y() + bordWidth) ) <= bordWidth )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
                point1.setY ( point2.y() - resizeLimit );
        }
        if ( abs ( clickY - (point2.y() - bordWidth) ) <= bordWidth )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
               point2.setY ( point1.y() + resizeLimit );
        }
        resizeTrussWindow ( trussWindow, point1, point2, trussWindow->getRBufDynarow() );
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
	if ( winBehaviour == onWindowDrag )
	{
		int dx = x - clickX;
		int dy = y - clickY;
        QPoint point1 = trussWindow->getWindowLeftTopPos ();
        QPoint point2 = trussWindow->getWindowRightBottomPos ();
        point1.setX ( point1.x() + dx ); 
        point1.setY ( point1.y() + dy );
        point2.setX ( point2.x() + dx );
        point2.setY ( point2.y() + dy );
		trussWindow->setWindowPosition ( point1 );
		update();
		clickX += dx;
		clickY += dy;
        return;
	}
	if ( nodeBehaviour == onNodeDrag )
	{
        moveTrussNode ( x, y, trussWindow, trussNode );
        trussWindow->setRenderingStatus ( false );
        update();
        return;
	}
	if ( pivotBehaviour == onPivotDrag )
	{
	    int dx = x - clickX;
	    int dy = y - clickY;
        moveTrussPivot ( dx, dy , trussWindow, trussPivot );
        trussWindow->setRenderingStatus ( false );
        update();
        clickX += dx;
        clickY += dy;
        return;
	}
    trussWindow = findTrussUnitWindowByCoord ( x, y );
    if ( trussWindow )
    {
        if ( trussWindow->inHorResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeHorCursor );
        else if ( trussWindow->inVerResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeVerCursor );
        else if ( trussWindow->inBDiagResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        else if ( trussWindow->inFDiagResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeFDiagCursor );
        else if ( trussWindow->inNodeRadius ( x, y ) )
        {
            removeAllHighlight ();
            trussWindow->setNodeHighlight ( x, y );
            nodeBehaviour = onNodeSelect;
            QWidget::setCursor ( Qt::ArrowCursor );
            update();
        }
        else if ( trussWindow->isPivotSelected ( x, y ) )
        {
            trussWindow->setPivotHighlight ( x, y );
            pivotBehaviour = onPivotSelect;
            QWidget::setCursor ( Qt::ArrowCursor );
            update();
        }
        else
        {
            QWidget::setCursor ( Qt::ArrowCursor );
            removeAllHighlight ();
            trussNode = 0;
            trussPivot = 0;
        }
    }
    else
    {
        QWidget::setCursor ( Qt::ArrowCursor );
        removeAllHighlight ();
        trussNode = 0;
        trussPivot = 0;
    }
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* )
{
    trussWindow = 0;
    winBehaviour = windowIdle;
    if ( trussNode )
        nodeBehaviour = onNodeSelect;
    else
        nodeBehaviour = nodeIdle;
    if ( trussPivot )
        pivotBehaviour = onPivotSelect;
    else
        pivotBehaviour = pivotIdle;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    clickX = me->x();
    clickY = flipY ? height() - me->pos().y() : me->pos().y();
    trussWindow = findTrussUnitWindowByCoord ( clickX, clickY );
    if ( trussWindow )
    {
        trussWindowToFront( *trussWindow );
        if ( trussWindow->inHeadlineRect ( clickX, clickY ) )
        {
            winBehaviour = onWindowDrag;
        }
        if ( trussWindow->inHorResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onHorResize;
            QWidget::setCursor ( Qt::SizeHorCursor );
        }
        if ( trussWindow->inVerResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onVerResize;
            QWidget::setCursor ( Qt::SizeVerCursor );
        }
        if ( trussWindow->inBDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onBDiagResize;
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        }
        if ( trussWindow->inFDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onFDiagResize;
            QWidget::setCursor ( Qt::SizeFDiagCursor );
        }
        if ( trussWindow->inNodeRadius ( clickX, clickY ) )
        {
            trussNode = trussWindow->findNodeByWidgetPos ( clickX, clickY );
            nodeBehaviour = onNodeDrag;
        }
        if ( trussWindow->isPivotSelected ( clickX, clickY ) )
        {
            trussPivot = trussWindow->findPivotByWidgetPos ( clickX, clickY );
            pivotBehaviour = onPivotDrag;
        }
        redrawTrussWindows ();
    }
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* )
{
}

/*****************************************************************************/
