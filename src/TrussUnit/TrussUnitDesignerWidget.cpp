
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
    trussUnits.push_back(&truss); 
}

bool TrussUnitDesignerWidget::removeTrussUnit ( const TrussUnit& truss )
{
    TrussUnitList::iterator iter = trussUnits.begin();
    for ( ; iter != trussUnits.end(); ++iter )
        if ( (*iter) == &truss ) {            
            trussUnits.erase(iter);
            return true;
        }            
    return false; 
}

TrussUnit* TrussUnitDesignerWidget::whoIsInRect ( int x, int y )
{
    TrussUnitList::reverse_iterator rev_iter = trussUnits.rbegin();
    for ( ; rev_iter != trussUnits.rend(); ++rev_iter )
        if ( (*rev_iter)->inWindowRect(x, y) )
		{
            TrussUnit* tmpTruss = *rev_iter;
            trussUnits.push_back(tmpTruss);
            TrussUnitList::iterator iter = trussUnits.begin();
            for ( ; iter != trussUnits.end(); ++iter )
                if ( (*iter)->inWindowRect(x, y) && *iter == tmpTruss )
                {
					trussUnits.erase(iter);
                    if ( tmpTruss->inHeadlineRect(x, y) )
                       behaviour = onDrag; 
                    return tmpTruss;
				}
        }
    return 0;
}

bool TrussUnitDesignerWidget::isHorResize ( int x, int y )
{
    TrussUnitList::reverse_iterator rev_iter = trussUnits.rbegin();
    for ( ; rev_iter != trussUnits.rend(); ++rev_iter )
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
    TrussUnitList::reverse_iterator rev_iter = trussUnits.rbegin();
    for ( ; rev_iter != trussUnits.rend(); ++rev_iter )
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
    TrussUnitList::reverse_iterator rev_iter = trussUnits.rbegin();
    for ( ; rev_iter != trussUnits.rend(); ++rev_iter )
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
    TrussUnitList::reverse_iterator rev_iter = trussUnits.rbegin();
    for ( ; rev_iter != trussUnits.rend(); ++rev_iter )
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
    TrussUnitList::iterator iter = trussUnits.begin();
    for ( ; iter != trussUnits.end() - 1; ++iter )
    {
         TrussUnit* trussUnit = (*iter);
         trussUnit->paint( baseRend, solidRend, textRend, ras, sl, ell );
    }
    TrussUnit* trussUnit = (*iter);
    trussUnit->setBorderColor ( 25,55,65 );
    trussUnit->setHeadlineColor ( 55, 155, 165 );
    trussUnit->setResEllColor ( 120, 120, 120 );
    trussUnits.back()->paint ( baseRend, solidRend, textRend, ras, sl, ell );
    trussUnit->setBorderColor ( 20,35,40 );
    trussUnit->setHeadlineColor ( 75, 105, 95 );
    trussUnit->setResEllColor ( 50, 50, 50 );
   
}

void TrussUnitDesignerWidget::initTruss ()  //temp method. Later to remove.
{
	QWidget::setMouseTracking(true);
    flipY( true );
    QPoint pos1, pos2, pivotPnt1, pivotPnt2;
	TrussUnitList::iterator iter = trussUnits.begin();
	for ( ; iter != trussUnits.end(); ++iter )
	{
        TrussUnit* trussUnit = (*iter);
        pos1.setX ( X1 ); 
        pos1.setY ( Y1 ); 
        pos2.setX ( X2 );
        pos2.setY ( Y2 );
        behaviour = nothing;
        trussUnit->setTrussArea ( 300, 300 );
        trussUnit->createNode ( 280, 30 );
        pivotPnt1.setX ( 0 );
        pivotPnt1.setY ( 0 );
        pivotPnt2.setX ( 130 );
        pivotPnt2.setY ( 130 );
        trussUnit->createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt1.setX ( 252 );
        pivotPnt1.setY ( 300 );
        trussUnit->createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt1.setX ( 0 );
        pivotPnt1.setY ( 300 );
        trussUnit->createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt2.setX ( 250 );
        pivotPnt2.setY ( 300 );
        trussUnit->createPivot ( pivotPnt1, pivotPnt2, 5 );
        pivotPnt2.setX ( 0 );
        pivotPnt2.setY ( 0 );
        trussUnit->createPivot ( pivotPnt1, pivotPnt2, 5 );
        trussUnit->setLineWidth ( 1 );
        trussUnit->setNodesRadius ( 5 );
        trussUnit->setPivotsWidth ( 3 );
        trussUnit->setResEllRad ( 3 );
        trussUnit->setMinResizeVal ( 300 );
        trussUnit->setWinRoundRad ( 25 );
		trussUnit->setPosition ( pos1, pos2 );
		trussUnit->setHeadlineWidth ( 30 );
		trussUnit->setBorderWidth ( 4 );
	    trussUnit->setBorderColor ( 20,35,40 );
	    trussUnit->setCanvasColor ( 8, 10, 12 );
        trussUnit->setHeadlineColor ( 75, 105, 95 );
        trussUnit->setResEllColor ( 50, 50, 50 );
        trussUnit->setTrussNodesPosition ();
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
		initTruss();
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
        int resizeLimit = selectedTruss->getMinResizeVal ();
		dx = me->x() - clickX;
        QPoint point1 = selectedTruss->getPoint1 ();
        QPoint point2 = selectedTruss->getPoint2 ();
        int bordW = selectedTruss->getBorderWidth ();
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
        selectedTruss->setPosition ( point1, point2 );
        selectedTruss->setTrussNodesPosition ();
        update();
		clickX += dx;
        return;
    }
    if ( behaviour == onVerResize )
    {
        int dy;
        int resizeLimit = selectedTruss->getMinResizeVal ();
	    dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedTruss->getPoint1 ();
        QPoint point2 = selectedTruss->getPoint2 ();
        int bordW = selectedTruss->getBorderWidth ();
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
        selectedTruss->setPosition ( point1, point2 );
        selectedTruss->setTrussNodesPosition ();
        update();
		clickY += dy;
        return;
    }
    if ( behaviour == onBDiagResize )
    {
        int dx;
        int dy;
        int resizeLimit = selectedTruss->getMinResizeVal ();
        dx = me->x() - clickX;
	    dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedTruss->getPoint1 ();
        QPoint point2 = selectedTruss->getPoint2 ();
        int bordW = selectedTruss->getBorderWidth ();
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
        selectedTruss->setPosition ( point1, point2 );
        selectedTruss->setTrussNodesPosition ();
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
    if ( behaviour == onFDiagResize )
    {
        int dx;
        int dy;
        int resizeLimit = selectedTruss->getMinResizeVal ();
        dx = me->x() - clickX;
        dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
        QPoint point1 = selectedTruss->getPoint1 ();
        QPoint point2 = selectedTruss->getPoint2 ();
        int bordW = selectedTruss->getBorderWidth ();
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
            if ( abs ( point2.y() - point1.y() ) < selectedTruss->getMinResizeVal () )
               point2.setY( point1.y() + selectedTruss->getMinResizeVal () );
        }
        selectedTruss->setPosition ( point1, point2 );
        selectedTruss->setTrussNodesPosition ();
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
        QPoint point1 = selectedTruss->getPoint1 ();
        QPoint point2 = selectedTruss->getPoint2 ();
        point1.setX ( point1.x() + dx ); 
        point1.setY ( point1.y() + dy );
        point2.setX ( point2.x() + dx );
        point2.setY ( point2.y() + dy );
		selectedTruss->setPosition ( point1, point2 );
        selectedTruss->setTrussNodesPosition ();
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
	selectedTruss = NULL;
    behaviour = nothing;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
	clickX = me->x();
	clickY = flipY() ? height() - me->pos().y() : me->pos().y();
    selectedTruss = whoIsInRect ( clickX, clickY );
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
