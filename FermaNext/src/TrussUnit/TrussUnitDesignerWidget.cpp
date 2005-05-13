
#include "TrussUnitDesignerWidget.h"
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>
#include <qsize.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpoint.h>

int X1 = 50, Y1 = 50;       //temp
int X2 = 400, Y2 = 400;         

/*****************************************************************************
 * Truss Designer Widget
 *****************************************************************************/

TrussUnitDesignerWidget::TrussUnitDesignerWidget ( QWidget* p ) :
    AggQWidget(p)
{}

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
            pseudoWindows.erase(iter);
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
            pseudoWindows.erase(iter);
            return true;
        }            
    return false; 
}

TrussUnitPseudoWindow* TrussUnitDesignerWidget::whoIsInRect ( int x, int y )
{
    PseudoWindowList::reverse_iterator rev_iter = pseudoWindows.rbegin();
    for ( ; rev_iter != pseudoWindows.rend(); ++rev_iter )
        if ( (*rev_iter)->inWindowRect(x, y) )
		{
            TrussUnitPseudoWindow* tmpTruss = *rev_iter;
            pseudoWindows.push_back(tmpTruss);
            PseudoWindowList::iterator iter = pseudoWindows.begin();
            for ( ; iter != pseudoWindows.end(); ++iter )
                if ( (*iter)->inWindowRect(x, y) && *iter == tmpTruss )
                {
					pseudoWindows.erase(iter);
                    if ( tmpTruss->inHeadlineRect(x, y) )
                       behaviour = onDrag; 
                    return tmpTruss;
				}
        }
    return 0;
}

bool TrussUnitDesignerWidget::isHorResize ( int x, int y )
{
    PseudoWindowList::reverse_iterator rev_iter = pseudoWindows.rbegin();
    for ( ; rev_iter != pseudoWindows.rend(); ++rev_iter )
    {
        if ( (*rev_iter)->inWindowRect(x, y) )
        {
            if ( (*rev_iter)->inHorResizeRect(x, y) )
                return true;
            return false;
        }
    }
    return false;
}

bool TrussUnitDesignerWidget::isVerResize ( int x, int y )
{
    PseudoWindowList::reverse_iterator rev_iter = pseudoWindows.rbegin();
    for ( ; rev_iter != pseudoWindows.rend(); ++rev_iter )
    {
        if ( (*rev_iter)->inWindowRect(x, y) )
        {
            if ( (*rev_iter)->inVerResizeRect(x, y) )
                return true;
            return false;
        }
    }
    return false;
}

bool TrussUnitDesignerWidget::isBDiagResize ( int x, int y )
{
    PseudoWindowList::reverse_iterator rev_iter = pseudoWindows.rbegin();
    for ( ; rev_iter != pseudoWindows.rend(); ++rev_iter )
    {
        if ( (*rev_iter)->inWindowRect(x, y) )
        {
            if ( (*rev_iter)->inBDiagResizeRect(x, y) )
                return true;
            return false;
        }
    }
    return false;
}

bool TrussUnitDesignerWidget::isFDiagResize ( int x, int y )
{
    PseudoWindowList::reverse_iterator rev_iter = pseudoWindows.rbegin();
    for ( ; rev_iter != pseudoWindows.rend(); ++rev_iter )
    {
        if ( (*rev_iter)->inWindowRect(x, y) )
        {
            if ( (*rev_iter)->inFDiagResizeRect(x, y) )
                return true;
            return false;
        }
    }
    return false;
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
	for ( ; iter != pseudoWindows.end(); ++iter )
	{
        TrussUnitPseudoWindow* pseudoWindow = (*iter);
        pos1.setX ( X1 ); 
        pos1.setY ( Y1 ); 
        pos2.setX ( X2 );
        pos2.setY ( Y2 );
        behaviour = nothing;        
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
	if(init)
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
    if ( behaviour == onHorResize )
    {
        int dx;
        int resizeLimit = selectedPseudoWindow->getMinResizeVal ();
		dx = me->x() - clickX;
        QPoint point1 = selectedPseudoWindow->getPoint1 ();
        QPoint point2 = selectedPseudoWindow->getPoint2 ();
        int bordW = selectedPseudoWindow->getBorderWidth ();
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
        selectedPseudoWindow->setPosition ( point1, point2 );
        selectedPseudoWindow->setTrussNodesPosition ();
        update();
		clickX += dx;
        return;
    }
    if ( behaviour == onVerResize )
    {
        int dy;
        int resizeLimit = selectedPseudoWindow->getMinResizeVal ();
	    dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedPseudoWindow->getPoint1 ();
        QPoint point2 = selectedPseudoWindow->getPoint2 ();
        int bordW = selectedPseudoWindow->getBorderWidth ();
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
        selectedPseudoWindow->setPosition ( point1, point2 );
        selectedPseudoWindow->setTrussNodesPosition ();
        update();
		clickY += dy;
        return;
    }
    if ( behaviour == onBDiagResize )
    {
        int dx;
        int dy;
        int resizeLimit = selectedPseudoWindow->getMinResizeVal ();
        dx = me->x() - clickX;
	    dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedPseudoWindow->getPoint1 ();
        QPoint point2 = selectedPseudoWindow->getPoint2 ();
        int bordW = selectedPseudoWindow->getBorderWidth ();
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
        selectedPseudoWindow->setPosition ( point1, point2 );
        selectedPseudoWindow->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
    if ( behaviour == onFDiagResize )
    {
        int dx;
        int dy;
        int resizeLimit = selectedPseudoWindow->getMinResizeVal ();
        dx = me->x() - clickX;
        dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedPseudoWindow->getPoint1 ();
        QPoint point2 = selectedPseudoWindow->getPoint2 ();
        int bordW = selectedPseudoWindow->getBorderWidth ();
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
            if ( abs ( point2.y() - point1.y() ) < selectedPseudoWindow->getMinResizeVal () )
               point2.setY( point1.y() + selectedPseudoWindow->getMinResizeVal () );
        }
        selectedPseudoWindow->setPosition ( point1, point2 );
        selectedPseudoWindow->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
	if ( behaviour == onDrag )
	{
		int dx, dy;
		dx = me->x() - clickX;
		dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedPseudoWindow->getPoint1 ();
        QPoint point2 = selectedPseudoWindow->getPoint2 ();
        point1.setX ( point1.x() + dx ); 
        point1.setY ( point1.y() + dy );
        point2.setX ( point2.x() + dx );
        point2.setY ( point2.y() + dy );
		selectedPseudoWindow->setPosition ( point1, point2 );
        selectedPseudoWindow->setTrussNodesPosition ();
		update();
		clickX += dx;
		clickY += dy;
        return;
	}
    if ( isHorResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
        QWidget::setCursor ( Qt::SizeHorCursor );
    else if ( isVerResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
        QWidget::setCursor ( Qt::SizeVerCursor );
    else if ( isBDiagResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
    {
        QWidget::setCursor ( Qt::SizeBDiagCursor );
    }
    else if ( isFDiagResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
    {
        QWidget::setCursor ( Qt::SizeFDiagCursor );
    }
    else
         QWidget::setCursor ( Qt::ArrowCursor );     
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
	selectedPseudoWindow = NULL;
    behaviour = nothing;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
	clickX = me->x();
	clickY = flipY() ? height() - me->pos().y() : me->pos().y();
    selectedPseudoWindow = whoIsInRect ( clickX, clickY );
    if ( isHorResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
    {
        behaviour = onHorResize;
        QWidget::setCursor ( Qt::SizeHorCursor );
    }
    if ( isVerResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
    {
        behaviour = onVerResize;
        QWidget::setCursor ( Qt::SizeVerCursor );
    }
    if ( isBDiagResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
    {
        behaviour = onBDiagResize;
        QWidget::setCursor ( Qt::SizeBDiagCursor );
    }
    if ( isFDiagResize ( me->x(), flipY() ? height() - me->y() : me->y() ) )
    {
        behaviour = onFDiagResize;
        QWidget::setCursor ( Qt::SizeFDiagCursor );
    }
	update();
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* ctrl )
{
}

/*****************************************************************************/
