
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
    QObject::connect( &trussWindow, SIGNAL(onVisibleChange(bool)), SLOT(repaint()) );

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
    for ( ; rev_iter != trussWindows.rend(); ++rev_iter )
        if ( (*rev_iter)->inWindowRect(x, y) )
            return *rev_iter;
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
    if ( nodeBehaviour == onNodeSelect )
    {
        WindowListIter iter = trussWindows.begin();
        for ( ; iter != trussWindows.end(); ++iter ) 
            (*iter)->removeNodesHighlight ();
        nodeBehaviour = nodeIdle;
        update ();
    }
    if ( pivotBehaviour == onPivotSelect )
    {
        WindowListIter iter = trussWindows.begin();
        for ( ; iter != trussWindows.end(); ++iter ) 
            (*iter)->removePivotsHighlight ();
        pivotBehaviour = pivotIdle;
        update ();
    }
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
        if ( window->inTrussAreaRect ( x, y ) )
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

void TrussUnitDesignerWidget::saveNodeStateAfterCreate ( TrussUnit& truss, TrussNode& node )
{    
    ObjectState& state = node.createState();
    TrussNodeCreateAction* action = new TrussNodeCreateAction( truss, node );
    state.addAction( action );
    state.save();
}

void TrussUnitDesignerWidget::saveNodeStateAfterRemove ( TrussUnit& truss, TrussNode& node )
{
    ObjectState& state = node.createState();
    TrussNodeRemoveAction* action = new TrussNodeRemoveAction( truss, node );
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
    
    // State to save
    ObjectState& state = selectedPivot->createState();
    // Action to save
    ConcreteObjectAction<TrussNode, QPoint>* action = 0;    

    // First pos    
    action = new ConcreteObjectAction<TrussNode, QPoint>( firstNode, 
                                                          &TrussNode::setPoint,
                                                          &TrussNode::setPoint,
                                                          firstPos,
                                                          beforeDragFirstPos);
    state.addAction( action );

    // Last pos
    action = new ConcreteObjectAction<TrussNode, QPoint>( lastNode, 
                                                          &TrussNode::setPoint,
                                                          &TrussNode::setPoint,
                                                          lastPos,
                                                          beforeDragLastPos);
    state.addAction( action );

    state.save();
}

void TrussUnitDesignerWidget::savePivotStateAfterCreate ( TrussUnit& truss, 
                                                          TrussNode& firstNode,
                                                          TrussNode& lastNode, 
                                                          TrussPivot& pivot )
{    
    ObjectStateManager* mng = pivot.getStateManager();
    mng->startStateBlock();
    // Save created nodes
    saveNodeStateAfterCreate( truss, firstNode );
    saveNodeStateAfterCreate( truss, lastNode );
    // Save created pivot
    ObjectState& state = pivot.createState();
    TrussPivotCreateAction* action = new TrussPivotCreateAction( truss, pivot );
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
    for ( ; iter != trussWindows.end(); ++iter )
        (*iter)->paint( baseRend );
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
        if ( designerBehaviour  == onNodeDraw || 
             designerBehaviour  == onPivotFirstNodeDraw )
        {
            if ( nodeCanBeDrawn ( x, y ) )
                QWidget::setCursor ( Qt::CrossCursor );
            else
                QWidget::setCursor ( Qt::ArrowCursor );
        }
        if ( nodeBehaviour == onNodeDrag )
        {
                selectedWindow->moveTrussNode ( x, y, selectedNode );        
                update();
        }
        else if ( pivotBehaviour == onPivotDrag )
        {
                selectedWindow->moveTrussPivot( x, y, selectedPivot, 
                                               firstNodeClickDist, lastNodeClickDist );        
                update();
        }
        else if ( designerBehaviour  == onPivotLastNodeDraw )
        {
            selectedWindow->moveTrussNode ( x, y, selectedNode );
            update();
        }
        else
        {
            selectedWindow = findWindowByWidgetPos ( x, y );
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
                else if ( designerBehaviour == onSelect )
                {
                    selectedNode = selectedWindow->findNodeByWidgetPos( x, y );
                    selectedPivot = selectedWindow->findPivotByWidgetPos( x, y,
                                                                  pivotFindingPrecision);
                    if ( selectedNode )
                    {
                        removeAllHighlight ();
                        selectedWindow->setNodeHighlight ( selectedNode );
                        nodeBehaviour = onNodeSelect;
                        QWidget::setCursor ( Qt::ArrowCursor );
                        update();
                    }
                    else if ( selectedPivot )
                    {
                        removeAllHighlight ();
                        selectedWindow->setPivotHighlight ( selectedPivot );
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
            }
            else
            {
                QWidget::setCursor ( Qt::ArrowCursor );
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
        selectedWindow->checkAfterNodeManipulation ( selectedNode, false );
        selectedPivot = selectedWindow->findPivotByWidgetPos( x, y,
                                                      pivotFindingPrecision);
        update();
        designerBehaviour = onPivotFirstNodeDraw;
        selectedNode = 0;
        selectedPivot = 0;
    }
    else if ( nodeBehaviour == onNodeDrag ) 
    {
        nodeBehaviour = onNodeSelect;
        // Save state in Undo/Redo stack
        saveNodeStateAfterDrag( selectedNode->getPoint() );

        selectedWindow->checkAfterNodeManipulation ( selectedNode, true );
        update();

        selectedWindow = findWindowByWidgetPos ( x, y );
        if ( selectedWindow )
        {
            selectedNode = selectedWindow->findNodeByWidgetPos( x, y );
            if ( !selectedNode )
            {
                removeAllHighlight ();
                nodeBehaviour = nodeIdle;
            }
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

        // Save state in Undo/Redo stack
        savePivotStateAfterDrag( firstPos, lastPos );

        selectedWindow->checkAfterPivotManipulation ( selectedPivot, true );
        update();

        selectedWindow = findWindowByWidgetPos ( x, y );
        if ( selectedWindow )
        {
            selectedPivot = selectedWindow->findPivotByWidgetPos( x, y,
                                                          pivotFindingPrecision);
            if ( !selectedPivot )
            {
                removeAllHighlight ();
                pivotBehaviour = pivotIdle;
            }
            else
                selectedWindow->setPivotHighlight ( selectedPivot );
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
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    QWidget::setFocus ();
    clickX = me->x();
    clickY = flipY ? height() - me->pos().y() : me->pos().y();
    selectedWindow = findWindowByWidgetPos ( clickX, clickY );
    if ( selectedWindow )
    {
        focusOnWindow( *selectedWindow );

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
            selectedWindow->nodeToFront ( *selectedNode );
            update();
            // Save pos for Undo/Redo
            beforeDragNodePos = selectedNode->getPoint();
            nodeBehaviour = onNodeDrag;
        }
        else if ( pivotBehaviour == onPivotSelect )
        {
            // Convert widget coordinates into truss (absolute) coordinates
            QPoint trussCoord = selectedWindow->getTrussCoordFromWidgetPos(clickX, clickY);

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
        else if ( designerBehaviour == onNodeDraw )
        {
            if ( nodeCanBeDrawn ( clickX, clickY ) )
            {
                TrussNode* node = selectedWindow->findNodeByWidgetPos ( clickX, clickY );
                if ( node )
                    return;

                QPoint nodeCoord = selectedWindow->
                                            getTrussCoordFromWidgetPos ( clickX, clickY );
                
                TrussNode& newNode = selectedWindow->createNode ( nodeCoord.x(), 
                                                               nodeCoord.y() );

                ObjectStateManager* mng = selectedWindow->getStateManager();
                mng->startStateBlock();

                saveNodeStateAfterCreate( *selectedWindow, newNode );
                selectedWindow->checkAfterNodeManipulation ( &newNode, true );

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
                TrussNode* firstNode = selectedWindow->findNodeByWidgetPos( clickX, clickY );
                if ( firstNode == 0 )
                    firstNode = &selectedWindow->createNode( nodeCoord.x(), nodeCoord.y() );
                TrussNode& lastNode = selectedWindow->createNode( nodeCoord.x(), 
                                                                  nodeCoord.y() );
                lastNode.setVisible ( false );
                selectedPivot = &( selectedWindow->createPivot( *firstNode, lastNode ) );
                selectedPivot->setDrawingStatus ( false );
                selectedNode = &lastNode;
                designerBehaviour = onPivotLastNodeDraw;
                savePivotStateAfterCreate( *selectedWindow, *firstNode, lastNode, 
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
