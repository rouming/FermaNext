
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
 * Truss Designer Widget
 *****************************************************************************/

TrussUnitDesignerWidget::TrussUnitDesignerWidget ( QWidget* p ) :
    AggQWidget(p),
    init(true),
    X1(50), Y1(50),
    X2(400), Y2(400)
{}

TrussUnitDesignerWidget::~TrussUnitDesignerWidget ()
{
    clearPseudoWindows();
}

void TrussUnitDesignerWidget::clearPseudoWindows ()
{
    PseudoWindowListIter iter = pseudoWindows.begin();
    for ( ; iter != pseudoWindows.end(); ++iter )
        delete *iter;
    pseudoWindows.clear();    
}

void TrussUnitDesignerWidget::addTrussUnit ( TrussUnit& truss )
{    
    TrussUnitPseudoWindow* window = new TrussUnitPseudoWindow(truss);
    pseudoWindows.push_back(window);
}

bool TrussUnitDesignerWidget::removeTrussUnit ( const TrussUnit& truss )
{
    PseudoWindowList::iterator iter = pseudoWindows.begin();
    for ( ; iter != pseudoWindows.end(); ++iter )
        if ( &((*iter)->getTrussUnit()) == &truss ) {
            TrussUnitPseudoWindow* w = *iter;
            pseudoWindows.erase(iter);
            delete w;
            return true;
        }
    return false;     
}

bool TrussUnitDesignerWidget::removeTrussUnitPseudoWindow 
                                    ( const TrussUnitPseudoWindow& window )
{
    PseudoWindowList::iterator iter = pseudoWindows.begin();
    for ( ; iter != pseudoWindows.end(); ++iter )
        if ( (*iter) == &window ) {
            TrussUnitPseudoWindow* w = *iter;
            pseudoWindows.erase(iter);
            delete w;
            return true;
        }
    return false; 
}

TrussUnitPseudoWindow* TrussUnitDesignerWidget::findPseudoWindowByCoord ( int x, int y )
{
    PseudoWindowList::reverse_iterator rev_iter = pseudoWindows.rbegin();
    for ( ; rev_iter != pseudoWindows.rend(); ++rev_iter )
        if ( (*rev_iter)->inWindowRect(x, y) )
            return *rev_iter;
    return 0;
}

void TrussUnitDesignerWidget::pseudoWindowToFront ( TrussUnitPseudoWindow* window )
{
    int s = pseudoWindows.size();
    if ( pseudoWindows.back() == window )
        return;
    pseudoWindows.push_back(window);
    PseudoWindowList::iterator iter = pseudoWindows.begin();
    for ( ; iter != pseudoWindows.end(); ++iter )
    {
        if ( *iter == window )
        {
    	    pseudoWindows.erase(iter);
            return;
        }
    }
}

void TrussUnitDesignerWidget::onDraw ()
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    solid_renderer solidRend ( baseRend );
    baseRend.clear ( agg::rgba (10, 10, 10) );
    agg::rasterizer_scanline_aa<>& ras = getAggRasterizerScanline ();
    agg::scanline_p8& sl = getAggScanline ();
    agg::ellipse ell;
    glyph_gen glyph(0);
    text_renderer textRend ( baseRend, glyph );
    glyph.font ( agg::verdana18_bold );
    PseudoWindowListIter iter = pseudoWindows.begin();
    for ( ; iter != pseudoWindows.end() - 1; ++iter ) 
    {
        TrussUnitPseudoWindow* pseudoWindow = (*iter);
        pseudoWindow->paint( baseRend, solidRend, textRend, ras, sl, ell );
    }  
    TrussUnitPseudoWindow* pseudoWindow = (*iter);
    pseudoWindow->setBorderColor ( 25,55,65 );
    pseudoWindow->setHeadlineColor ( 55, 155, 165 );
    pseudoWindow->setResEllColor ( 120, 120, 120 );
    pseudoWindows.back()->paint ( baseRend, solidRend, textRend, ras, sl, ell );
    pseudoWindow->setBorderColor ( 20,35,40 );
    pseudoWindow->setHeadlineColor ( 75, 105, 95 );
    pseudoWindow->setResEllColor ( 50, 50, 50 );   
}

void TrussUnitDesignerWidget::initPseudoWindow ()  //temp method. Later to remove.
{
    QWidget::setMouseTracking(true);
    flipY( true );
    QPoint pos1, pos2, pivotPnt1, pivotPnt2;

    PseudoWindowListIter iter = pseudoWindows.begin();
    for ( ; iter != pseudoWindows.end(); ++iter ) { 
        TrussUnitPseudoWindow* pseudoWindow = (*iter);        
        pos1.setX ( X1 ); 
        pos1.setY ( Y1 ); 
        pos2.setX ( X2 );
        pos2.setY ( Y2 );
        winBehaviour = windowIdle;

        pseudoWindow->getTrussUnit().createNode ( 280, 30 );
        pivotPnt1.setX ( 0 );
        pivotPnt1.setY ( 0 );
        pivotPnt2.setX ( 130 );
        pivotPnt2.setY ( 130 );
        pseudoWindow->getTrussUnit().createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt1.setX ( 252 );
        pivotPnt1.setY ( 300 );
        pseudoWindow->getTrussUnit().createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt1.setX ( 0 );
        pivotPnt1.setY ( 300 );
        pseudoWindow->getTrussUnit().createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt2.setX ( 250 );
        pivotPnt2.setY ( 300 );
        pseudoWindow->getTrussUnit().createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt2.setX ( 0 );
        pivotPnt2.setY ( 0 );
        pseudoWindow->getTrussUnit().createPivot ( pivotPnt1, pivotPnt2, 5 );
        pseudoWindow->setCoordinateLineWidth ( 1 );
        pseudoWindow->getTrussUnit().setNodesRadius ( 5 );
        pseudoWindow->getTrussUnit().setPivotsWidth ( 3 );
        pseudoWindow->setResEllRad ( 3 );
        pseudoWindow->setMinResizeVal ( 300 );
        pseudoWindow->setWinRoundRad ( 25 );
        pseudoWindow->setPosition ( pos1, pos2 );
        pseudoWindow->setHeadlineWidth ( 30 );
        pseudoWindow->setBorderWidth ( 4 );
        pseudoWindow->setBorderColor ( 20,35,40 );
        pseudoWindow->setCanvasColor ( 8, 10, 12 );
        pseudoWindow->setHeadlineColor ( 75, 105, 95 );
        pseudoWindow->setResEllColor ( 50, 50, 50 );
        pseudoWindow->setTrussNodesPosition ();
        X1 = X1 + 180;
        X2 = X2 + 180;
    }
    init = false;
}

/*****************************************************************************/
// Event handlers

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* pe )
{
   if ( init ) 
       initPseudoWindow ();
   onDraw();
}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* re )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
}

void TrussUnitDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
    int x = me->x();
    int y = flipY() ? height() - me->y() : me->y();
    if ( winBehaviour == onHorResize )
    {
        int dx;
        int resizeLimit = pseudoWindow->getMinResizeVal ();
		dx = x - clickX;
        QPoint point1 = pseudoWindow->getPoint1 ();
        QPoint point2 = pseudoWindow->getPoint2 ();
        int bordW = pseudoWindow->getBorderWidth ();
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
        pseudoWindow->setPosition ( point1, point2 );
        pseudoWindow->setTrussNodesPosition ();
        update();
		clickX += dx;
        return;
    }
    if ( winBehaviour == onVerResize )
    {
        int dy;
        int resizeLimit = pseudoWindow->getMinResizeVal ();
	    dy = y - clickY;
        QPoint point1 = pseudoWindow->getPoint1 ();
        QPoint point2 = pseudoWindow->getPoint2 ();
        int bordW = pseudoWindow->getBorderWidth ();
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
        pseudoWindow->setPosition ( point1, point2 );
        pseudoWindow->setTrussNodesPosition ();
        update();
		clickY += dy;
        return;
    }
    if ( winBehaviour == onBDiagResize )
    {
        int dx;
        int dy;
        int resizeLimit = pseudoWindow->getMinResizeVal ();
        dx = x - clickX;
	    dy = y - clickY;
        QPoint point1 = pseudoWindow->getPoint1 ();
        QPoint point2 = pseudoWindow->getPoint2 ();
        int bordW = pseudoWindow->getBorderWidth ();
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
        pseudoWindow->setPosition ( point1, point2 );
        pseudoWindow->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
    if ( winBehaviour == onFDiagResize )
    {
        int dx;
        int dy;
        int resizeLimit = pseudoWindow->getMinResizeVal ();
        dx = x - clickX;
        dy = y - clickY;
        QPoint point1 = pseudoWindow->getPoint1 ();
        QPoint point2 = pseudoWindow->getPoint2 ();
        int bordW = pseudoWindow->getBorderWidth ();
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
            if ( abs ( point2.y() - point1.y() ) < pseudoWindow->getMinResizeVal () )
               point2.setY( point1.y() + pseudoWindow->getMinResizeVal () );
        }
        pseudoWindow->setPosition ( point1, point2 );
        pseudoWindow->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
	if ( winBehaviour == onDrag )
	{
		int dx, dy;
		dx = x - clickX;
		dy = y - clickY;
        QPoint point1 = pseudoWindow->getPoint1 ();
        QPoint point2 = pseudoWindow->getPoint2 ();
        point1.setX ( point1.x() + dx ); 
        point1.setY ( point1.y() + dy );
        point2.setX ( point2.x() + dx );
        point2.setY ( point2.y() + dy );
		pseudoWindow->setPosition ( point1, point2 );
        pseudoWindow->setTrussNodesPosition ();
		update();
		clickX += dx;
		clickY += dy;
        return;
	}
    pseudoWindow = findPseudoWindowByCoord ( x, y );
    if ( pseudoWindow )
    {
        if ( pseudoWindow->inHorResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeHorCursor );
        else if ( pseudoWindow->inVerResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeVerCursor );
        else if ( pseudoWindow->inBDiagResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        else if ( pseudoWindow->inFDiagResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeFDiagCursor );
/* TODO   
        else if ( pseudoWindow->inNodeRadius ( x, y ) )
        {
            trussElemBehaviour = onNodeSelect;
            pseudoWindow->setNodeHighlight ( x, y );
            QWidget::setCursor ( Qt::ArrowCursor );
        }
*/ 
        else
            QWidget::setCursor ( Qt::ArrowCursor );
    }
    else
        QWidget::setCursor ( Qt::ArrowCursor );
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
	pseudoWindow = 0;
    winBehaviour = windowIdle;
    trussElemBehaviour = trussElementIdle;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    clickX = me->x();
    clickY = flipY() ? height() - me->pos().y() : me->pos().y();
    pseudoWindow = findPseudoWindowByCoord ( clickX, clickY );
    if ( pseudoWindow )
    {
        pseudoWindowToFront ( pseudoWindow );
        if ( pseudoWindow->inHeadlineRect ( clickX, clickY ) )
        {
            winBehaviour = onDrag;
        }
        if ( pseudoWindow->inHorResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onHorResize;
            QWidget::setCursor ( Qt::SizeHorCursor );
        }
        if ( pseudoWindow->inVerResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onVerResize;
            QWidget::setCursor ( Qt::SizeVerCursor );
        }
        if ( pseudoWindow->inBDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onBDiagResize;
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        }
        if ( pseudoWindow->inFDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onFDiagResize;
            QWidget::setCursor ( Qt::SizeFDiagCursor );
        }
        update();
    }
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* ctrl )
{
}

/*****************************************************************************/