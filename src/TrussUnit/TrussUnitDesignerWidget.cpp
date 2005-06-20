
#include "TrussUnitDesignerWidget.h"
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
    AggQWidget(p),
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

    QPoint pos1, pos2;
    pos1.setX ( X1 ); 
    pos1.setY ( Y1 ); 
    pos2.setX ( X2 );
    pos2.setY ( Y2 );

    trussWindow.setCoordinateLineWidth ( 1 );
    trussWindow.setNodesRadius ( 4 );
    trussWindow.setPivotsWidth ( 2 );
    trussWindow.setResEllRad ( 3 );
    trussWindow.setMinResizeVal ( 200 );
    trussWindow.setWinCornerRadius ( 25 );
    trussWindow.setPosition ( pos1, pos2 );
    trussWindow.setHeadlineWidth ( 26 );
    trussWindow.setBorderWidth ( 4 );
    trussWindow.setBorderColor ( 20,35,40 );
    trussWindow.setCanvasColor ( 8, 10, 12 );
    trussWindow.setHeadlineColor ( 75, 105, 95 );
    trussWindow.setResEllColor ( 50, 50, 50 );
    trussWindow.setTrussNodesPosition ();
}

bool TrussUnitDesignerWidget::removeTrussUnitWindow ( const TrussUnitWindow& window )
{
    WindowList::iterator iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( (*iter) == &window ) {
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

void TrussUnitDesignerWidget::removeAllHighlight ()
{
    if ( trussElemBehaviour == onNodeSelect )
    {
        WindowListIter iter = trussWindows.begin();
        for ( ; iter != trussWindows.end(); ++iter ) 
            (*iter)->removeNodeHighlight ();
        trussElemBehaviour = trussElementIdle;
        update();
    }
    if ( trussElemBehaviour == onPivotSelect )
    {
        WindowListIter iter = trussWindows.begin();
        for ( ; iter != trussWindows.end(); ++iter ) 
            (*iter)->removePivotHighlight ();
        trussElemBehaviour = trussElementIdle;
        update();
    }
}

void TrussUnitDesignerWidget::onDraw ()
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    solid_renderer solidRend ( baseRend );
    baseRend.clear ( agg::rgba (10, 10, 10) );
    agg::ellipse ell;
    glyph_gen glyph(0);
    text_renderer textRend ( baseRend, glyph );
    glyph.font ( agg::verdana17_bold );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end() - 1; ++iter ) 
    {
        TrussUnitWindow* trussWindow = (*iter);
        trussWindow->paint( baseRend );
    }
    
    TrussUnitWindow* trussWindow = (*iter); 
    trussWindow->setTrussNodesPosition ();
    trussWindow->setNodesRadius ( 4 );
    trussWindow->setPivotsWidth ( 2 );

    trussWindow->setBorderColor ( 25,55,65 );
    trussWindow->setHeadlineColor ( 55, 155, 165 );
    trussWindow->setResEllColor ( 120, 120, 120 );
    trussWindows.back()->paint ( baseRend  );
    trussWindow->setBorderColor ( 20,35,40 );
    trussWindow->setHeadlineColor ( 75, 105, 95 );
    trussWindow->setResEllColor ( 50, 50, 50 );
}

void TrussUnitDesignerWidget::initTrussUnitWindow ()  //temp method. Later to remove.
{
    QWidget::setMouseTracking(true);
    flipY( true );
    QPoint pos1, pos2, pivotPnt1, pivotPnt2;

    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) { 
        TrussUnitWindow* trussWindow = (*iter);        
        pos1.setX ( X1 ); 
        pos1.setY ( Y1 ); 
        pos2.setX ( X2 );
        pos2.setY ( Y2 );
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
        trussWindow->setCoordinateLineWidth ( 1 );
        trussWindow->setNodesRadius ( 4 );
        trussWindow->setPivotsWidth ( 2 );
        trussWindow->setResEllRad ( 3 );
        trussWindow->setMinResizeVal ( 200 );
        trussWindow->setWinCornerRadius ( 25 );
        trussWindow->setPosition ( pos1, pos2 );
        trussWindow->setHeadlineWidth ( 26 );
        trussWindow->setBorderWidth ( 4 );
        trussWindow->setBorderColor ( 20,35,40 );
        trussWindow->setCanvasColor ( 8, 10, 12 );
        trussWindow->setHeadlineColor ( 75, 105, 95 );
        trussWindow->setResEllColor ( 50, 50, 50 );
        trussWindow->setTrussNodesPosition ();
        X1 = X1 + 180;
        X2 = X2 + 180;
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
    int y = flipY() ? height() - me->y() : me->y();
    if ( winBehaviour == onHorResize )
    {
        int resizeLimit = trussWindow->getMinResizeVal ();
		int dx = x - clickX;
        QPoint point1 = trussWindow->getPoint1 ();
        QPoint point2 = trussWindow->getPoint2 ();
        int bordW = trussWindow->getBorderWidth ();
        if ( abs ( clickX - point1.x() ) <= bordW )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
                point1.setX( point2.x() - resizeLimit );
        }
        if ( abs ( clickX - point2.x() ) <= bordW )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
               point2.setX( point1.x() + resizeLimit );
        }
        trussWindow->setPosition ( point1, point2 );
        trussWindow->setTrussNodesPosition ();
        update();
		clickX += dx;
        return;
    }
    if ( winBehaviour == onVerResize )
    {
        int resizeLimit = trussWindow->getMinResizeVal ();
	    int dy = y - clickY;
        QPoint point1 = trussWindow->getPoint1 ();
        QPoint point2 = trussWindow->getPoint2 ();
        int bordW = trussWindow->getBorderWidth ();
        if ( abs ( clickY - point1.y() ) <= bordW )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
                point1.setY( point2.y() - resizeLimit );
        }
        if ( abs ( clickY - point2.y() ) <= bordW )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
               point2.setY( point1.y() + resizeLimit );
        }
        trussWindow->setPosition ( point1, point2 );
        trussWindow->setTrussNodesPosition ();
        update();
		clickY += dy;
        return;
    }
    if ( winBehaviour == onBDiagResize )
    {
        int resizeLimit = trussWindow->getMinResizeVal ();
        int dx = x - clickX;
	    int dy = y - clickY;
        QPoint point1 = trussWindow->getPoint1 ();
        QPoint point2 = trussWindow->getPoint2 ();
        int bordW = trussWindow->getBorderWidth ();
        if ( abs ( clickX - (point1.x() + bordW) ) <= bordW )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
                point1.setX( point2.x() - resizeLimit );
        }
        if ( abs ( clickX - (point2.x() - bordW) ) <= bordW )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
               point2.setX( point1.x() + resizeLimit );
        }
        if ( abs ( clickY - (point1.y() + bordW) ) <= bordW )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
                point1.setY( point2.y() - resizeLimit );
        }
        if ( abs ( clickY - (point2.y() - bordW) ) <= bordW )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
               point2.setY( point1.y() + resizeLimit );
        }
        trussWindow->setPosition ( point1, point2 );
        trussWindow->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
    if ( winBehaviour == onFDiagResize )
    {
        int resizeLimit = trussWindow->getMinResizeVal ();
        int dx = x - clickX;
        int dy = y - clickY;
        QPoint point1 = trussWindow->getPoint1 ();
        QPoint point2 = trussWindow->getPoint2 ();
        int bordW = trussWindow->getBorderWidth ();
        if ( abs ( clickX - (point1.x() + bordW) ) <= bordW )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
                point1.setX( point2.x() - resizeLimit );
        }
        if ( abs ( clickX - (point2.x() - bordW) ) <= bordW )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < resizeLimit )
               point2.setX( point1.x() + resizeLimit );
        }
        if ( abs ( clickY - (point1.y() + bordW) ) <= bordW )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < resizeLimit )
                point1.setY( point2.y() - resizeLimit );
        }
        if ( abs ( clickY - (point2.y() - bordW) ) <= bordW )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < trussWindow->getMinResizeVal () )
               point2.setY( point1.y() + trussWindow->getMinResizeVal () );
        }
        trussWindow->setPosition ( point1, point2 );
        trussWindow->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
	if ( winBehaviour == onDrag )
	{
		int dx = x - clickX;
		int dy = y - clickY;
        QPoint point1 = trussWindow->getPoint1 ();
        QPoint point2 = trussWindow->getPoint2 ();
        point1.setX ( point1.x() + dx ); 
        point1.setY ( point1.y() + dy );
        point2.setX ( point2.x() + dx );
        point2.setY ( point2.y() + dy );
		trussWindow->setPosition ( point1, point2 );
        trussWindow->setTrussNodesPosition ();
		update();
		clickX += dx;
		clickY += dy;
        return;
	}
	if ( trussElemBehaviour == onNodeDrag )
	{
/*        QPoint point;
	    int dx = x - clickX;
	    int dy = y - clickY;
        point = trussNode->getNodeWidgetPosition ();
        point.setX ( point.x() + dx );
        point.setY ( point.y() + dy );
        trussNode->setNodeWidgetPosition ( point );
        update();
        clickX += dx;
        clickY += dy;
        return;
*/	}
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
            trussElemBehaviour = onNodeSelect;
            QWidget::setCursor ( Qt::ArrowCursor );
            update();
        }
        else if ( trussWindow->isPivotSelected ( x, y ) )
        {
            removeAllHighlight ();
            trussWindow->setPivotHighlight ( x, y );
            trussElemBehaviour = onPivotSelect;
            QWidget::setCursor ( Qt::ArrowCursor );
            update();
        }
        else
        {
            QWidget::setCursor ( Qt::ArrowCursor );
            removeAllHighlight ();
        }
    }
    else
    {
        QWidget::setCursor ( Qt::ArrowCursor );
        removeAllHighlight ();
    }
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* )
{
    trussWindow = 0;
    trussNode = 0;
    winBehaviour = windowIdle;
    trussElemBehaviour = trussElementIdle;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    clickX = me->x();
    clickY = flipY() ? height() - me->pos().y() : me->pos().y();
    trussWindow = findTrussUnitWindowByCoord ( clickX, clickY );
    if ( trussWindow )
    {
        trussWindowToFront( *trussWindow );
        if ( trussWindow->inHeadlineRect ( clickX, clickY ) )
        {
            winBehaviour = onDrag;
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
            trussNode = trussWindow->findNodeByCoord ( clickX, clickY );
            trussElemBehaviour = onNodeDrag;
        }
        update();
    }
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* )
{
}

/*****************************************************************************/
