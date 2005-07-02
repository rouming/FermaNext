
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
    selectedWindow(0),
    selectedNode(0),
    selectedPivot(0),    
    winBehaviour(windowIdle),
    nodeBehaviour(nodeIdle),
    pivotBehaviour(pivotIdle),
    clickX(0), clickY(0), dxFirst(0), 
    dxLast(0), dyFirst(0), dyLast(0),
    // Temp
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
    int newX = int(( x - window->getWindowLeftTopPos ().x() - leftWindowIndent ) / scaleMultX);
    int newY = int(( window->getWindowSize().height() - y + 
                window->getWindowLeftTopPos ().y() - bottomWindowIndent ) / scaleMultY);

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

    node->setPoint( newX, newY );
}

void TrussUnitDesignerWidget::moveTrussPivot ( int x, int y, TrussUnitWindow* window, 
                                        TrussPivot* pivot )
{
    // TODO: flipY comparison
    double scaleMultX = window->getScaleMultiplierX ();
    double scaleMultY = window->getScaleMultiplierY ();
    int newXFirst = int( ( x - window->getWindowLeftTopPos ().x() - 
                         leftWindowIndent ) / scaleMultX )  + dxFirst;
    int newXLast = int( ( x - window->getWindowLeftTopPos ().x() - 
                         leftWindowIndent ) / scaleMultX ) + dxLast;
    int newYFirst = int( ( window->getWindowSize().height() - y + 
                          window->getWindowLeftTopPos ().y() - bottomWindowIndent ) / 
                          scaleMultY ) + dyFirst;
    int newYLast = int( ( window->getWindowSize().height() - y + 
                         window->getWindowLeftTopPos ().y() - bottomWindowIndent ) / 
                         scaleMultY ) + dyLast;
    
    QPoint oldFirstCoord = pivot->getFirstNode().getPoint();
    QPoint oldLastCoord  = pivot->getLastNode().getPoint();

    int oldXFirst = oldFirstCoord.x();
    int oldYFirst = oldFirstCoord.y();
    int oldXLast  = oldLastCoord.x();
    int oldYLast  = oldLastCoord.y();

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
    firstNode->setPoint( newXFirst, newYFirst );
    lastNode->setPoint( newXLast, newYLast );    
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

void TrussUnitDesignerWidget::saveNodeStateAfterDrag ( QPoint pos )
{
    if ( selectedNode == 0 )
        return;

    ObjectState& state = selectedNode->createState();
    ConcreteObjectAction<TrussNode, QPoint>* action = 
        new ConcreteObjectAction<TrussNode, QPoint>( *selectedNode, 
                                                     &TrussNode::setPoint,
                                                     &TrussNode::setPoint,
                                                     pos,
                                                     beforeDragNodePos);
    state.addAction( action );
    state.save();    
}

void TrussUnitDesignerWidget::savePivotStateAfterDrag ( QPoint firstPos, QPoint lastPos )
{
    if ( selectedPivot == 0 )
        return;

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
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
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
        selectedWindow->resize( point1, point2 );
        update();
		clickX += dx;
        return;
    }
    if ( winBehaviour == onVerResize )
    {
	    int dy = y - clickY;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
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
        selectedWindow->resize( point1, point2 );
        update();
		clickY += dy;
        return;
    }
    if ( winBehaviour == onBDiagResize )
    {
        int dx = x - clickX;
	    int dy = y - clickY;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
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
        selectedWindow->resize( point1, point2 );
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
    if ( winBehaviour == onFDiagResize )
    {
        int dx = x - clickX;
        int dy = y - clickY;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
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
        selectedWindow->resize( point1, point2 );
        update();
        clickX += dx;
		clickY += dy;
        return;
    }
	if ( winBehaviour == onWindowDrag )
	{
		int dx = x - clickX;
		int dy = y - clickY;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
        point1.setX ( point1.x() + dx ); 
        point1.setY ( point1.y() + dy );
        point2.setX ( point2.x() + dx );
        point2.setY ( point2.y() + dy );
		selectedWindow->setWindowPosition ( point1 );
		update();
		clickX += dx;
		clickY += dy;
        return;
	}
	if ( nodeBehaviour == onNodeDrag )
	{
        moveTrussNode( x, y, selectedWindow, selectedNode );        
        update();
        return;
	}
	if ( pivotBehaviour == onPivotDrag )
	{
//	    int dx = x - clickX;
//	    int dy = y - clickY;
        moveTrussPivot( x, y, selectedWindow, selectedPivot );        
        update();
//        clickX += dx;
//        clickY += dy;
        return;
	}
    selectedWindow = findTrussUnitWindowByCoord ( x, y );
    if ( selectedWindow )
    {
        if ( selectedWindow->inHorResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeHorCursor );
        else if ( selectedWindow->inVerResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeVerCursor );
        else if ( selectedWindow->inBDiagResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        else if ( selectedWindow->inFDiagResizeRect ( x, y ) )
            QWidget::setCursor ( Qt::SizeFDiagCursor );
        else if ( selectedWindow->inNodeRadius ( x, y ) )
        {
            removeAllHighlight ();
            selectedWindow->setNodeHighlight ( x, y );
            nodeBehaviour = onNodeSelect;
            QWidget::setCursor ( Qt::ArrowCursor );
            update();
        }
        else if ( selectedWindow->isPivotSelected ( x, y ) )
        {
            selectedWindow->setPivotHighlight ( x, y );
            pivotBehaviour = onPivotSelect;
            QWidget::setCursor ( Qt::ArrowCursor );
            update();
        }
        else
        {
            QWidget::setCursor ( Qt::ArrowCursor );
            removeAllHighlight ();
            selectedNode = 0;
            selectedPivot = 0;
        }
    }
    else
    {
        QWidget::setCursor ( Qt::ArrowCursor );
        removeAllHighlight ();
        selectedNode = 0;
        selectedPivot = 0;
    }
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* )
{
    selectedWindow = 0;
    winBehaviour = windowIdle;

    if ( selectedNode ) {
        nodeBehaviour = onNodeSelect;
        // Save state in Undo/Redo stack
        saveNodeStateAfterDrag( selectedNode->getPoint() );
    }
    else
        nodeBehaviour = nodeIdle;
    if ( selectedPivot ) {
        pivotBehaviour = onPivotSelect;
        
        QPoint firstPos = selectedPivot->getFirstNode().getPoint();
        QPoint lastPos  = selectedPivot->getLastNode().getPoint();
        // Save state in Undo/Redo stack
        savePivotStateAfterDrag( firstPos, lastPos );
    }
    else
        pivotBehaviour = pivotIdle;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    clickX = me->x();
    clickY = flipY ? height() - me->pos().y() : me->pos().y();
    selectedWindow = findTrussUnitWindowByCoord ( clickX, clickY );
    if ( selectedWindow )
    {
        trussWindowToFront( *selectedWindow );
        if ( selectedWindow->inHeadlineRect ( clickX, clickY ) )
        {
            winBehaviour = onWindowDrag;
        }
        if ( selectedWindow->inHorResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onHorResize;
            QWidget::setCursor ( Qt::SizeHorCursor );
        }
        if ( selectedWindow->inVerResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onVerResize;
            QWidget::setCursor ( Qt::SizeVerCursor );
        }
        if ( selectedWindow->inBDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onBDiagResize;
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        }
        if ( selectedWindow->inFDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onFDiagResize;
            QWidget::setCursor ( Qt::SizeFDiagCursor );
        }
        if ( selectedWindow->inNodeRadius ( clickX, clickY ) )
        {
            selectedNode = selectedWindow->findNodeByWidgetPos ( clickX, clickY );
            // Save pos for Undo/Redo
            beforeDragNodePos = selectedNode->getPoint();
            nodeBehaviour = onNodeDrag;
        }
        // TODO: flipY comparison
        if ( selectedWindow->isPivotSelected ( clickX, clickY ) )
        {
            selectedPivot = selectedWindow->findPivotByWidgetPos ( clickX, clickY );

            double scaleMultX = selectedWindow->getScaleMultiplierX ();
            double scaleMultY = selectedWindow->getScaleMultiplierY ();
            int absX = int( ( clickX - selectedWindow->getWindowLeftTopPos ().x() - 
                             leftWindowIndent ) / scaleMultX );
            int absY = int( ( selectedWindow->getWindowSize().height() - clickY + 
                             selectedWindow->getWindowLeftTopPos ().y() - 
                             bottomWindowIndent ) / scaleMultY );

            QPoint firstPos = selectedPivot->getFirstNode().getPoint();
            QPoint lastPos  = selectedPivot->getLastNode().getPoint();

            dxFirst = firstPos.x() - absX;
            dxLast  = lastPos.x()  - absX;
            dyFirst = firstPos.y() - absY;
            dyLast  = lastPos.y()  - absY;

            // Save pos for Undo/Redo
            beforeDragFirstPos = firstPos;
            beforeDragLastPos  = lastPos;

            pivotBehaviour = onPivotDrag;
        }        
        update();
    }
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* )
{
}

/*****************************************************************************/
