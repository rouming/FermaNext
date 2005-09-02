
#include "TrussUnitDesignerWidget.h"
#include "SubsidiaryConstants.h"
#include "TrussUnitActions.h"

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
    focusedWindow(0),
    selectedWindow(0),
    selectedNode(0),
    selectedPivot(0),    
    winBehaviour(windowIdle),
    nodeBehaviour(nodeIdle),
    pivotBehaviour(pivotIdle),
    designerBehaviour(onSelect),
    clickX(0), clickY(0), 
    firstNodeClickDist(0,0), 
    lastNodeClickDist(0,0),
    // Temp
    X(50), Y(50)
{
    QWidget::setFocus();
    QWidget::setMouseTracking( true );
}

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
    X += 15; 
    Y += 10;

    // We should repaint all area after truss changes its visibility
    // or becomes desisted/revived
    QObject::connect( &trussWindow, SIGNAL(onVisibleChange(bool)), 
                                    SLOT(repaint()) );
    QObject::connect( &trussWindow, SIGNAL(onAfterDesist(StatefulObject&)), 
                                    SLOT(repaint()) );
    QObject::connect( &trussWindow, SIGNAL(onAfterRevive(StatefulObject&)), 
                                    SLOT(repaint()) );

    trussWindows.push_back(&trussWindow);
    trussWindow.setWindowPosition ( QPoint( X, Y ) );
    focusOnWindow(trussWindow);
}

bool TrussUnitDesignerWidget::removeTrussUnitWindow ( TrussUnitWindow& window )
{
    WindowList::iterator iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( (*iter) == &window ) {
            TrussUnitWindow* trussWindow = *iter;
            // Do disconnection of all signals
            trussWindow->disconnect( this );
            trussWindows.erase(iter);
            repaint();
            return true;
        }
    return false; 
}

TrussUnitWindow* TrussUnitDesignerWidget::findWindowByWidgetPos ( int x, int y )
{
    WindowList::reverse_iterator rev_iter = trussWindows.rbegin();
    for ( ; rev_iter != trussWindows.rend(); ++rev_iter ) {
        TrussUnitWindow* w = *rev_iter;
        if ( w->isAlive() && w->isEnabled() && w->inWindowRect(x, y) )
            return *rev_iter;
    }
    return 0;
}

void TrussUnitDesignerWidget::focusOnWindow ( TrussUnitWindow& window )
{
    // Nothing to focus
    if ( focusedWindow == &window )
        return;

    WindowListIter newSelectedIter = std::find( trussWindows.begin(),
                                                trussWindows.end(), 
                                                &window );
    // Should be sure window is in vector
    if ( newSelectedIter == trussWindows.end() )
        return;
    if ( ! (*newSelectedIter)->isAlive() || !(*newSelectedIter)->isEnabled() )
        return;
    // Defocus previous focused window
    if ( focusedWindow )
        focusedWindow->setHighlighted(false);
    // Focus on window
    focusedWindow = &window;
    focusedWindow->setHighlighted(true);
    // Focused window to front
    trussWindows.erase(newSelectedIter);
    trussWindows.push_back(focusedWindow);

    update();
}

void TrussUnitDesignerWidget::clearWindowFocus ()
{
    if ( focusedWindow ) {
        focusedWindow->setHighlighted(false);
        focusedWindow = 0;
        update();
    }
}

void TrussUnitDesignerWidget::removeAllHighlight ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        if ( nodeBehaviour == onNodeSelect )
            window->removeNodesHighlight ();
        if ( pivotBehaviour == onPivotSelect )
            window->removePivotsHighlight ();
        window->setResizeEllipseHighlighted ( TrussUnitWindow::None );
    }

    if ( nodeBehaviour == onNodeSelect )
        nodeBehaviour = nodeIdle;
    if ( pivotBehaviour == onPivotSelect )
        pivotBehaviour = pivotIdle;

    update ();
}

bool TrussUnitDesignerWidget::nodeCanBeDrawn ( int x, int y )
{
    bool wrongArea = false;
    uint i = trussWindows.size();
    WindowList::reverse_iterator rev_iter = trussWindows.rbegin();
    for ( ; rev_iter != trussWindows.rend(); ++rev_iter )
    {
        i--;
        TrussUnitWindow* window = *rev_iter;
        if ( window->isAlive() && window->inTrussAreaRect ( x, y ) )
        {
            WindowListIter iter = trussWindows.begin() + i;
            for ( ; iter != trussWindows.end(); ++iter ) 
            {
                if ( (*iter)->inWindowRect ( x, y ) && (*iter) != window )
                {
                    wrongArea = true;
                    break;
                }
            }
            if ( !wrongArea )
                return true;
        }
    }
    return false;
}


void TrussUnitDesignerWidget::clearAllCursorCoordFields ()
{
    QPoint coord ( -1, -1 );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        (*iter)->setCursorCoord( coord );
    update();
}

void TrussUnitDesignerWidget::saveNodeStateAfterDrag ( QPoint pos )
{
    // Actually nothing to save.
    if ( selectedNode == 0 )
        return;
    // Similar points. Nothing to save.
    if ( pos == beforeDragNodePos )
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

void TrussUnitDesignerWidget::saveNodeStateAfterCreate ( TrussNode& node )
{    
    ObjectState& state = node.createState();
    TrussNodeCreateAction* action = new TrussNodeCreateAction( node );
    state.addAction( action );
    state.save();
}

void TrussUnitDesignerWidget::saveNodeStateAfterRemove ( TrussNode& node )
{
    ObjectState& state = node.createState();
    TrussNodeRemoveAction* action = new TrussNodeRemoveAction( node );
    state.addAction( action );
    state.save();
}

void TrussUnitDesignerWidget::savePivotStateAfterDrag ( QPoint firstPos, QPoint lastPos )
{
    // Actually nothing to save.
    if ( selectedPivot == 0 )
        return;
    // Similar points. Nothing to save.
    if ( firstPos == beforeDragFirstPos && 
         lastPos == beforeDragLastPos )
        return;

    TrussNode& firstNode = selectedPivot->getFirstNode();
    TrussNode& lastNode  = selectedPivot->getLastNode();    

    ObjectStateManager* mng = selectedPivot->getStateManager();
    mng->startStateBlock();
    
    // First node pos state to save
    ObjectState& firstState = firstNode.createState();

    // First pos    
    firstState.addAction( new ConcreteObjectAction<TrussNode, QPoint>( 
                                                         firstNode, 
                                                         &TrussNode::setPoint,
                                                         &TrussNode::setPoint,
                                                         firstPos,
                                                         beforeDragFirstPos) );
    firstState.save();

    // Last node pos state to save
    ObjectState& lastState = lastNode.createState();

    // Last pos
    lastState.addAction( new ConcreteObjectAction<TrussNode, QPoint>( 
                                                          lastNode, 
                                                          &TrussNode::setPoint,
                                                          &TrussNode::setPoint,
                                                          lastPos,
                                                          beforeDragLastPos) );
    lastState.save();

    mng->endStateBlock();
}

void TrussUnitDesignerWidget::savePivotStateAfterCreate ( TrussNode& firstNode,
                                                          bool firstWasCreated,
                                                          TrussNode& lastNode, 
                                                          TrussPivot& pivot )
{    
    ObjectStateManager* mng = pivot.getStateManager();
    mng->startStateBlock();
    // Save created nodes
    if ( firstWasCreated )
        saveNodeStateAfterCreate( firstNode );
    saveNodeStateAfterCreate( lastNode );
    // Save created pivot
    ObjectState& state = pivot.createState();
    TrussPivotCreateAction* action = new TrussPivotCreateAction( pivot );
    state.addAction( action );
    state.save();
    
    mng->endStateBlock();
}

void TrussUnitDesignerWidget::parseSvg ( const char* fname )
{
    agg::svg::parser svgParcer ( pathRend );
    svgParcer.parse ( fname );
    pathRend.arrange_orientations ();
}

void TrussUnitDesignerWidget::drawSvg ( base_renderer& baseRend, 
                                        solid_renderer& solidRend,
                                        int x, int y )
{
    scanline_rasterizer& ras = getAggRasterizerScanline ();    
    agg::scanline_p8& sl = getAggScanline ();
    agg::trans_affine mtx = getAggResizeMatrix ();
    mtx *= agg::trans_affine_scaling(0.1);
    mtx *= agg::trans_affine_translation( x, y );
    pathRend.render ( ras, sl, solidRend, mtx, baseRend.clip_box(), 1.0 );
}

/*****************************************************************************/
// Event handlers

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* )
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    solid_renderer solidRend ( baseRend );
    baseRend.clear ( agg::rgba (10, 10, 10) );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) {
        TrussUnitWindow* w = *iter;
        if ( w->isAlive() )
            w->paint( baseRend );
    }
//    drawSvg ( baseRend, solidRend, 10, 500 );
}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
    if ( ke->key() == Qt::Key_N && designerBehaviour != onNodeDraw )
    {
        designerBehaviour = onNodeDraw;
        removeAllHighlight ();
    }
    if ( ke->key() == Qt::Key_P && designerBehaviour != onPivotFirstNodeDraw )
    {
        designerBehaviour = onPivotFirstNodeDraw;
        removeAllHighlight ();
    }
    if ( ke->key() == Qt::Key_R && designerBehaviour != onErase )
    {
        designerBehaviour = onErase;
        removeAllHighlight ();
    }
    if ( ke->key() == Qt::Key_Escape  && designerBehaviour != onSelect )
    {
        designerBehaviour = onSelect;
        QWidget::setCursor ( Qt::ArrowCursor );
    }
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
    }
    else if ( winBehaviour == onVerResize )
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
    }
    else if ( winBehaviour == onBDiagResize )
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
    }
    else if ( winBehaviour == onFDiagResize )
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
    }
    else if ( winBehaviour == onWindowDrag )
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
    }
    else
    {
        if ( designerBehaviour == onNodeDraw || 
             designerBehaviour == onPivotFirstNodeDraw ||
             designerBehaviour == onErase )
        {
            if ( nodeCanBeDrawn ( x, y ) )
                QWidget::setCursor ( Qt::CrossCursor );
            else
                QWidget::setCursor ( Qt::ArrowCursor );
        }
        if ( nodeBehaviour == onNodeDrag )
        {
            selectedWindow->moveTrussNode ( x, y, selectedNode );
            selectedWindow->setCursorCoord ( selectedNode->getPoint() );
            update();
        }
        else if ( pivotBehaviour == onPivotDrag )
        {
            clearAllCursorCoordFields ();
            QPoint cursorCoord = selectedWindow->getTrussCoordFromWidgetPos( x, y );
            QSize size = selectedWindow->getTrussAreaSize();
            if ( cursorCoord.x() > size.width() || cursorCoord.x() < 0 ||
                 cursorCoord.y() > size.height() || cursorCoord.y() < 0 )
            {
                 cursorCoord.setX ( -1 );
                 cursorCoord.setY ( -1 );
            }
            selectedWindow->setCursorCoord ( cursorCoord );
            selectedWindow->moveTrussPivot( x, y, selectedPivot, 
                                           firstNodeClickDist, lastNodeClickDist );     
            update();
        }
        else if ( designerBehaviour  == onPivotLastNodeDraw )
        {
            selectedWindow->moveTrussNode ( x, y, selectedNode );
            selectedWindow->setCursorCoord ( selectedNode->getPoint() );
            update();
        }
        else
        {
            selectedWindow = findWindowByWidgetPos ( x, y );
            if ( selectedWindow )
            {
                if ( selectedWindow->inTrussAreaRect( x, y ) )
                {
                    clearAllCursorCoordFields ();
                    QPoint cursorCoord = selectedWindow->getTrussCoordFromWidgetPos( x, y );
                    selectedWindow->setCursorCoord ( cursorCoord );
                    update();
                }
                else
                {
                    clearAllCursorCoordFields ();
                    QPoint cursorCoord ( -1, -1 );
                    selectedWindow->setCursorCoord ( cursorCoord );
                    update();
                }
                if ( selectedWindow->inHorResizeRect ( x, y ) )
                    QWidget::setCursor ( Qt::SizeHorCursor );
                else if ( selectedWindow->inVerResizeRect ( x, y ) )
                    QWidget::setCursor ( Qt::SizeVerCursor );
                else if ( selectedWindow->inBDiagResizeRect ( x, y ) )
                    QWidget::setCursor ( Qt::SizeBDiagCursor );
                else if ( selectedWindow->inFDiagResizeRect ( x, y ) )
                    QWidget::setCursor ( Qt::SizeFDiagCursor );
                else if ( designerBehaviour == onSelect || 
                          designerBehaviour == onErase )
                {
                    int nodePrecision = selectedWindow->getNodeFindingPrecision ();
                    int pivotPrecision = selectedWindow->getPivotFindingPrecision ();
                    selectedNode = selectedWindow->findNodeByWidgetPos( x, y, 
                                                                  nodePrecision );
                    selectedPivot = selectedWindow->findPivotByWidgetPos( x, y,
                                                                  pivotPrecision);
                    if ( selectedNode )
                    {
                        removeAllHighlight ();
                        clearAllCursorCoordFields ();
                        selectedWindow->setFocusOnNode ( selectedNode );
                        selectedWindow->setCursorCoord ( selectedNode->getPoint() );
                        nodeBehaviour = onNodeSelect;
                        if ( designerBehaviour == onErase )
                            QWidget::setCursor ( Qt::CrossCursor );
                        else
                            QWidget::setCursor ( Qt::ArrowCursor );
                        update();
                    }
                    else if ( selectedPivot )
                    {
                        removeAllHighlight ();
                        clearAllCursorCoordFields ();
                        selectedWindow->setFocusOnPivot ( selectedPivot );
                        QPoint cursorCoord = 
                            selectedWindow->getTrussCoordFromWidgetPos( x, y );
                        selectedWindow->setCursorCoord ( cursorCoord );
                        pivotBehaviour = onPivotSelect;
                        if ( designerBehaviour == onErase )
                            QWidget::setCursor ( Qt::CrossCursor );
                        else
                            QWidget::setCursor ( Qt::ArrowCursor );
                        update();
                    }
                    else
                    {
                        if ( designerBehaviour == onErase && 
                             selectedWindow->inTrussAreaRect( x, y ) )
                            QWidget::setCursor ( Qt::CrossCursor );
                        else
                            QWidget::setCursor ( Qt::ArrowCursor );
                        removeAllHighlight ();
                        selectedNode = 0;
                        selectedPivot = 0;
                    }
                }
            }
            else
            {
                QWidget::setCursor ( Qt::ArrowCursor );
                clearAllCursorCoordFields ();
                removeAllHighlight ();
                selectedNode = 0;
                selectedPivot = 0;
            }
        }
    }
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
    int x = me->x();
    int y = flipY ? height() - me->y() : me->y();
    QPoint pos ( x, y );
    if ( designerBehaviour == onPivotLastNodeDraw )
    {
        selectedPivot->getLastNode().setVisible ( true );
        selectedPivot->setDrawingStatus ( true );

        ObjectStateManager* mng = selectedWindow->getStateManager();
        mng->startStateBlock();

        selectedWindow->updateAfterPivotManipulation ( selectedPivot, false );

        mng->endStateBlock();

        update();
        designerBehaviour = onPivotFirstNodeDraw;
        selectedNode = 0;
        selectedPivot = 0;
    }
    else if ( nodeBehaviour == onNodeDrag ) 
    {
        nodeBehaviour = onNodeSelect;

        ObjectStateManager* mng = selectedWindow->getStateManager();
        mng->startStateBlock();

        // Save state in Undo/Redo stack
        saveNodeStateAfterDrag( selectedNode->getPoint() );
        selectedWindow->updateAfterNodeManipulation ( selectedNode, true );

        mng->endStateBlock();

        update();

        selectedWindow = findWindowByWidgetPos ( x, y );
        if ( selectedWindow )
        {
            int nodePrecision = selectedWindow->getNodeFindingPrecision ();
            selectedNode = selectedWindow->findNodeByWidgetPos( x, y, nodePrecision );
            selectedWindow->removeNodesHighlight ();
            if ( !selectedNode )
                nodeBehaviour = nodeIdle;
            else
                selectedWindow->setFocusOnNode ( selectedNode );
        }
        else
        {
            removeAllHighlight ();
            selectedNode = 0;
            nodeBehaviour = nodeIdle;
        }
    }
    else if ( pivotBehaviour == onPivotDrag ) 
    {
        pivotBehaviour = onPivotSelect;
        QPoint firstPos = selectedPivot->getFirstNode().getPoint();
        QPoint lastPos  = selectedPivot->getLastNode().getPoint();

        ObjectStateManager* mng = selectedWindow->getStateManager();
        mng->startStateBlock();

        // Save state in Undo/Redo stack
        savePivotStateAfterDrag( firstPos, lastPos );
        selectedWindow->updateAfterPivotManipulation ( selectedPivot, true );

        mng->endStateBlock();

        update();

        selectedWindow = findWindowByWidgetPos ( x, y );
        if ( selectedWindow )
        {
            selectedWindow->removePivotsHighlight();
            int pivotPrecision = selectedWindow->getPivotFindingPrecision();
            selectedPivot = selectedWindow->findPivotByWidgetPos( x, y, pivotPrecision);
            if ( !selectedPivot )
                pivotBehaviour = pivotIdle;
            else
                selectedWindow->setFocusOnPivot ( selectedPivot );
        }
        else
        {
            removeAllHighlight ();
            selectedPivot = 0;
            pivotBehaviour = pivotIdle;
        }
    }
    else
    {
        nodeBehaviour = nodeIdle;
        pivotBehaviour = pivotIdle;
        selectedWindow = 0;
        winBehaviour = windowIdle;
    }

    if ( focusedWindow ) {
        ObjectStateManager* mng = focusedWindow->getStateManager();
        mng->endStateBlock();
    }
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    QWidget::setFocus();
    clickX = me->x();
    clickY = flipY ? height() - me->pos().y() : me->pos().y();
    selectedWindow = findWindowByWidgetPos ( clickX, clickY );
    if ( selectedWindow )
    {
        focusOnWindow( *selectedWindow );

        ObjectStateManager* mng = focusedWindow->getStateManager();
        mng->startStateBlock();

        if ( selectedWindow->inHeadlineRect ( clickX, clickY ) )
        {
            winBehaviour = onWindowDrag;
        }
        else if ( selectedWindow->inHorResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onHorResize;
            QWidget::setCursor ( Qt::SizeHorCursor );
        }
        else if ( selectedWindow->inVerResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onVerResize;
            QWidget::setCursor ( Qt::SizeVerCursor );
        }
        else if ( selectedWindow->inBDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onBDiagResize;
            QWidget::setCursor ( Qt::SizeBDiagCursor );
        }
        else if ( selectedWindow->inFDiagResizeRect ( clickX, clickY ) )
        {
            winBehaviour = onFDiagResize;
            QWidget::setCursor ( Qt::SizeFDiagCursor );
        }

        if ( nodeBehaviour == onNodeSelect )
        {
            if ( designerBehaviour == onErase )
            {
                // Save remove node action
                ObjectStateManager* mng = selectedNode->getStateManager();
                mng->startStateBlock();
                selectedWindow->desistAdjoiningPivots( *selectedNode );
                ObjectState& state = selectedNode->createState();
                state.addAction( new TrussNodeRemoveAction( *selectedNode ) );
                state.save();
                selectedNode->desist();
                mng->endStateBlock();
    
                update();
            }
            else
            {
                selectedWindow->nodeToFront ( *selectedNode );
                update();
                // Save pos for Undo/Redo
                beforeDragNodePos = selectedNode->getPoint();
                nodeBehaviour = onNodeDrag;
            }
        }
        else if ( pivotBehaviour == onPivotSelect )
        {
            if ( designerBehaviour == onErase )
            {
                // Save remove pivot action
                ObjectState& state = selectedPivot->createState();
                state.addAction( new TrussPivotRemoveAction( *selectedPivot ) );
                state.save();
                selectedPivot->desist();
                update();
            }
            else
            {
                // Convert widget coordinates into truss (absolute) coordinates
                QPoint trussCoord = selectedWindow->getTrussCoordFromWidgetPos(clickX, 
                                                                               clickY);
                QPoint firstPos = selectedPivot->getFirstNode().getPoint();
                QPoint lastPos  = selectedPivot->getLastNode().getPoint();

                // Get first and last nodes displacements about a clicked point 
                firstNodeClickDist.setX ( firstPos.x() - trussCoord.x() );
                firstNodeClickDist.setY ( firstPos.y() - trussCoord.y() );
                lastNodeClickDist.setX (  lastPos.x()  - trussCoord.x() );
                lastNodeClickDist.setY ( lastPos.y()  - trussCoord.y() );

                // Save pos for Undo/Redo
                beforeDragFirstPos = firstPos;
                beforeDragLastPos  = lastPos;

                pivotBehaviour = onPivotDrag;
            }
        }    
        else if ( designerBehaviour == onNodeDraw )
        {
            if ( nodeCanBeDrawn ( clickX, clickY ) )
            {
                int nodePrecision = selectedWindow->getNodeFindingPrecision();
                TrussNode* node = selectedWindow->findNodeByWidgetPos ( clickX, clickY, 
                                                                    nodePrecision );
                if ( node )
                    return;

                QPoint nodeCoord = selectedWindow->
                                            getTrussCoordFromWidgetPos ( clickX, clickY );
                
                TrussNode& newNode = selectedWindow->createNode ( nodeCoord.x(), 
                                                               nodeCoord.y() );

                ObjectStateManager* mng = selectedWindow->getStateManager();
                mng->startStateBlock();

                saveNodeStateAfterCreate( newNode );
                selectedWindow->updateAfterNodeManipulation ( &newNode, true );

                mng->endStateBlock();
                update();
            }
        }
        else if ( designerBehaviour == onPivotFirstNodeDraw )
        {
            if ( nodeCanBeDrawn ( clickX, clickY ) )
            {
                QPoint nodeCoord = selectedWindow->getTrussCoordFromWidgetPos( clickX, 
                                                                               clickY );
                int nodePrecision = selectedWindow->getNodeFindingPrecision();
                TrussNode* firstNode = selectedWindow->findNodeByWidgetPos( clickX, clickY, 
                                                                    nodePrecision );
                bool wasCreated = false;
                if ( firstNode == 0 ) {
                    firstNode = &selectedWindow->createNode( nodeCoord.x(), nodeCoord.y() );
                    wasCreated = true;
                }
                TrussNode& lastNode = selectedWindow->createNode( nodeCoord.x(), 
                                                                  nodeCoord.y() );
                lastNode.setVisible ( false );
                selectedPivot = &( selectedWindow->createPivot( *firstNode, lastNode ) );
                selectedPivot->setDrawingStatus ( false );
                selectedNode = &lastNode;
                designerBehaviour = onPivotLastNodeDraw;
                savePivotStateAfterCreate( *firstNode, wasCreated, lastNode, 
                                           *selectedPivot );
                update();
            }
        }
    }
    else
        clearWindowFocus();
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* )
{
}

/*****************************************************************************/
