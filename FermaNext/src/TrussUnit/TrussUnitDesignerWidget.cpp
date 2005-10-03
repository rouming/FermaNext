
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
    clickX(0), clickY(0), 
    firstNodeClickDist(0,0), 
    lastNodeClickDist(0,0),
    toolBar( new TrussUnitToolBar( QPoint(0,0), 15, 15, 8, 5, 5, 30 ) ),
    buttonPressed(false),
    // Temp
    X(50), Y(50)
{
    QWidget::setFocus();
    QWidget::setMouseTracking( true );
    initToolBar();
    designerBehaviour = onSelect;
    emit pressSelectButton();
}

TrussUnitDesignerWidget::~TrussUnitDesignerWidget ()
{
    clearTrussUnitWindows();
    delete toolBar;
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

void TrussUnitDesignerWidget::initToolBar ()
{
    QObject::connect( toolBar, SIGNAL( onRenderingStatusCleared() ),
                      this, SLOT( updateDesignerWidget() ) );

    int separation = toolBar->getButtonSeparation();

    QPoint leftTopPos ( bufferEmptyArea + toolBar->getBorderLeft() + separation, 
                        bufferEmptyArea + toolBar->getBorderTop() );
    toolBar->addButton ( imagesSvgPath() + "/arrowIcon.svg", "Arrow", leftTopPos, 
                         buttonWidth, buttonHeight, this, SIGNAL( pressSelectButton() ),
                         SLOT( changeBehaviourToSelect() ) );

    leftTopPos.setX ( leftTopPos.x() + buttonWidth + separation);
    toolBar->addButton ( imagesSvgPath() + "/nodeIcon.svg", "Node", leftTopPos, 
                         buttonWidth,buttonHeight, this, SIGNAL( pressNodeDrawButton() ),
                         SLOT( changeBehaviourToNodeDraw() ) );

    leftTopPos.setX ( leftTopPos.x() + buttonWidth + separation );
    toolBar->addButton ( imagesSvgPath() + "/pivotIcon.svg", "Pivot", leftTopPos, 
                         buttonWidth, buttonHeight, this, SIGNAL( pressPivotDrawButton() ),
                         SLOT(changeBehaviourToPivotDraw()) );

    leftTopPos.setX ( leftTopPos.x() + buttonWidth + separation );
    toolBar->addButton ( imagesSvgPath() + "/fixIcon.svg", "Fixation", leftTopPos, 
                         buttonWidth, buttonHeight, this, SIGNAL( pressFixDrawButton() ), 
                         SLOT( changeBehaviourToFixSet() ) );

    leftTopPos.setX ( leftTopPos.x() + buttonWidth + separation );
    toolBar->addButton ( imagesSvgPath() + "/forceIcon.svg", "Force", leftTopPos, 
                         buttonWidth, buttonHeight, this, SIGNAL( pressForceDrawButton() ),
                         SLOT( changeBehaviourToForceDraw() ) );

    leftTopPos.setX ( leftTopPos.x() + buttonWidth + separation );
    toolBar->addButton ( imagesSvgPath() + "/eraseIcon.svg", "Erase", leftTopPos, 
                         buttonWidth, buttonHeight, this, SIGNAL( pressEraseButton() ),
                         SLOT( changeBehaviourToErase() ) );
}

void TrussUnitDesignerWidget::changeBehaviourToSelect ()
{
    designerBehaviour = onSelect;
}

void TrussUnitDesignerWidget::changeBehaviourToNodeDraw ()
{
    designerBehaviour = onNodeDraw;
}

void TrussUnitDesignerWidget::changeBehaviourToPivotDraw ()
{
    designerBehaviour = onPivotFirstNodeDraw;
}

void TrussUnitDesignerWidget::changeBehaviourToFixDraw ()
{
//    designerBehaviour = onFixDraw;
}

void TrussUnitDesignerWidget::changeBehaviourToForceDraw ()
{
//    designerBehaviour = onForceDraw;
}

void TrussUnitDesignerWidget::changeBehaviourToErase ()
{
    designerBehaviour = onErase;
}

void TrussUnitDesignerWidget::updateDesignerWidget ()
{
    update();
}

TrussUnitWindow* TrussUnitDesignerWidget::findWindowByWidgetPos ( int x, int y )
{
    WindowList::reverse_iterator rev_iter = trussWindows.rbegin();
    for ( ; rev_iter != trussWindows.rend(); ++rev_iter ) {
        TrussUnitWindow* w = *rev_iter;
        if ( w->isAlive() && w->isVisible() && w->inWindowRect(x, y) )
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
    if ( ! (*newSelectedIter)->isAlive() || !(*newSelectedIter)->isVisible() )
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

void TrussUnitDesignerWidget::removeTrussElemHighlight ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        window->setResizeEllipseHighlighted ( TrussUnitWindow::None );
        //window->releaseButtons ();
        if ( nodeBehaviour == onNodeSelect )
            window->removeNodesHighlight ();
        if ( pivotBehaviour == onPivotSelect )
            window->removePivotsHighlight ();
    }
    if ( nodeBehaviour == onNodeSelect )
        nodeBehaviour = nodeIdle;
    if ( pivotBehaviour == onPivotSelect )
        pivotBehaviour = pivotIdle;

    update ();
}

void TrussUnitDesignerWidget::removeWindowButtonHighlight ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        window->removeButtonsHighlight();
    }
    update ();
}

void TrussUnitDesignerWidget::releaseWindowButtons ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        window->releaseButtons();
    }
    update ();
}

void TrussUnitDesignerWidget::clearAllCursorCoordFields ()
{
    bool fieldChanged = false;
    DoublePoint coord ( -1.0, -1.0 );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
    {
        TrussUnitWindow* win = *iter;
        if ( win->getCursorCoord() != coord )
        {
            win->setCursorCoord( coord );
            fieldChanged = true;
        }
    }
    if ( fieldChanged )
        update();
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

void TrussUnitDesignerWidget::saveNodeStateAfterDrag ( DoublePoint pos )
{
    // Actually nothing to save.
    if ( selectedNode == 0 )
        return;
    // Similar points. Nothing to save.
    if ( pos == beforeDragNodePos )
        return;

    ObjectState& state = selectedNode->createState();
    state.addAction( new ConcreteObjectAction<TrussNode, DoublePoint>( 
                                                     *selectedNode, 
                                                     &TrussNode::setPoint,
                                                     &TrussNode::setPoint,
                                                     pos,
                                                     beforeDragNodePos) );
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

void TrussUnitDesignerWidget::savePivotStateAfterDrag ( DoublePoint firstPos, 
                                                        DoublePoint lastPos )
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
    firstState.addAction( new ConcreteObjectAction<TrussNode, DoublePoint>( 
                                                         firstNode, 
                                                         &TrussNode::setPoint,
                                                         &TrussNode::setPoint,
                                                         firstPos,
                                                         beforeDragFirstPos) );
    firstState.save();

    // Last node pos state to save
    ObjectState& lastState = lastNode.createState();

    // Last pos
    lastState.addAction( new ConcreteObjectAction<TrussNode, DoublePoint>( 
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

/*****************************************************************************/
// Event handlers

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* )
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    solid_renderer solidRend ( baseRend );
    baseRend.clear ( agg::rgba( 10, 10, 10 ) );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) {
        TrussUnitWindow* w = *iter;
        if ( w->isAlive() )
        {
            w->paint( baseRend );
            w->setMaxSize( width(), height() );
        }
    }
    toolBar->setCenterPoint( QPoint( width() / 2, height() ) );
    toolBar->paint ( baseRend );
}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
    if ( ke->key() == Qt::Key_N && designerBehaviour != onNodeDraw )
    {
        designerBehaviour = onNodeDraw;
        emit pressNodeDrawButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_P && designerBehaviour != onPivotFirstNodeDraw )
    {
        designerBehaviour = onPivotFirstNodeDraw;
        emit pressPivotDrawButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_R && designerBehaviour != onErase )
    {
        designerBehaviour = onErase;
        emit pressEraseButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_Escape  && designerBehaviour != onSelect )
    {
        designerBehaviour = onSelect;
        emit pressSelectButton();
        QWidget::setCursor ( Qt::ArrowCursor );
    }
}

void TrussUnitDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
    int x = me->x();
    int y = flipY ? height() - me->y() : me->y();

    toolBar->checkMouseMoveEvent ( x, y );

    if ( toolBar->inToolBarRect ( x, y ) )
        QWidget::setCursor ( Qt::ArrowCursor );

    else if ( winBehaviour == onHorResize )
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

        TrussUnitWindow* window = findWindowByWidgetPos ( x, y );
        if ( window )
        {
            removeWindowButtonHighlight ();
            window->releaseButtons ();
            window->checkMouseMoveEvent( x, y, buttonPressed );
            update();
        }
        else
            removeWindowButtonHighlight ();

        if ( nodeBehaviour == onNodeDrag )
        {
            selectedWindow->moveTrussNode ( x, y, selectedNode );
            selectedWindow->setCursorCoord ( selectedNode->getPoint() );
            update();
        }
        else if ( pivotBehaviour == onPivotDrag )
        {
            clearAllCursorCoordFields ();
            selectedWindow->setCursorCoord ( QPoint(x, y) );
            selectedWindow->moveTrussPivot( x, y, selectedPivot, 
                                            firstNodeClickDist, 
                                            lastNodeClickDist );     
            update();
        }
        else if ( designerBehaviour  == onPivotLastNodeDraw )
        {
            selectedWindow->moveTrussNode ( x, y, selectedNode );
            selectedWindow->setCursorCoord( selectedNode->getPoint() );
            if ( nodeCanBeDrawn ( x, y ) )
                QWidget::setCursor ( Qt::CrossCursor );
            else
                QWidget::setCursor ( Qt::ArrowCursor );
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
                    selectedWindow->setCursorCoord ( QPoint(x, y) );
                    update();
                }
                else
                {
                    clearAllCursorCoordFields ();
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
                    selectedNode = selectedWindow->findNodeByWidgetPos( x, y );
                    selectedPivot = selectedWindow->findPivotByWidgetPos( x, y );
                    if ( selectedNode )
                    {
                        removeTrussElemHighlight ();
                        clearAllCursorCoordFields ();
                        selectedWindow->setFocusOnNode( selectedNode );
                        selectedWindow->setCursorCoord( selectedNode->getPoint() );
                        nodeBehaviour = onNodeSelect;
                        if ( designerBehaviour == onErase )
                            QWidget::setCursor ( Qt::CrossCursor );
                        else
                            QWidget::setCursor ( Qt::ArrowCursor );
                        update();
                    }
                    else if ( selectedPivot )
                    {
                        removeTrussElemHighlight ();
                        clearAllCursorCoordFields ();
                        selectedWindow->setFocusOnPivot ( selectedPivot );
                        selectedWindow->setCursorCoord ( QPoint(x, y) );
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
                        removeTrussElemHighlight ();
                        selectedNode = 0;
                        selectedPivot = 0;
                    }
                }

            }
            else
            {
                QWidget::setCursor ( Qt::ArrowCursor );
                clearAllCursorCoordFields ();
                removeTrussElemHighlight ();
                selectedNode = 0;
                selectedPivot = 0;
            }
        }
    }
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
    buttonPressed = false;

    int x = me->x();
    int y = flipY ? height() - me->y() : me->y();

    if ( designerBehaviour == onPivotLastNodeDraw )
    {
        selectedPivot->getLastNode().setVisible ( true );
        selectedPivot->setDrawingStatus ( true );

        ObjectStateManager* mng = selectedWindow->getStateManager();
        mng->startStateBlock();

        selectedWindow->updateAfterNodeManipulation ( selectedNode, false );

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

        TrussUnitWindow* window = findWindowByWidgetPos ( x, y );
        if ( window && window == selectedWindow )
        {
            selectedNode = window->findNodeByWidgetPos( x, y );
            window->removeNodesHighlight ();
            if ( !selectedNode )
                nodeBehaviour = nodeIdle;
            else
                window->setFocusOnNode ( selectedNode );
        }
        else
        {
            removeTrussElemHighlight ();
            selectedNode = 0;
            nodeBehaviour = nodeIdle;
        }
    }
    else if ( pivotBehaviour == onPivotDrag ) 
    {
        pivotBehaviour = onPivotSelect;
        const DoublePoint& firstPos = selectedPivot->getFirstNode().getPoint();
        const DoublePoint& lastPos  = selectedPivot->getLastNode().getPoint();

        ObjectStateManager* mng = selectedWindow->getStateManager();
        mng->startStateBlock();

        // Save state in Undo/Redo stack
        savePivotStateAfterDrag( firstPos, lastPos );
        selectedWindow->updateAfterPivotManipulation ( selectedPivot, true );

        mng->endStateBlock();

        update();

        TrussUnitWindow* window = findWindowByWidgetPos ( x, y );
        if ( window && window == selectedWindow )
        {
            window->removePivotsHighlight();
            selectedPivot = window->findPivotByWidgetPos( x, y );
            if ( !selectedPivot )
                pivotBehaviour = pivotIdle;
            else
                window->setFocusOnPivot ( selectedPivot );
        }
        else
        {
            removeTrussElemHighlight ();
            selectedPivot = 0;
            pivotBehaviour = pivotIdle;
        }
    }
    else
    {
        selectedWindow = findWindowByWidgetPos ( x, y );
        if ( selectedWindow )
        {
            if ( selectedWindow->inHideButtonRect( x, y ) ||
                 selectedWindow->inRollUpButtonRect( x, y ) )
            {
                selectedWindow->checkMouseReleaseEvent( x, y );
                update();
            }
        }
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

    if ( toolBar->inToolBarRect( clickX, clickY ) )
    {
        toolBar->checkMousePressEvent ( clickX, clickY );
        update();
    } 
    else 
    {
    selectedWindow = findWindowByWidgetPos ( clickX, clickY );
    if ( selectedWindow )
    {
        focusOnWindow( *selectedWindow );

        ObjectStateManager* mng = focusedWindow->getStateManager();
        mng->startStateBlock();

        if ( selectedWindow->inHideButtonRect( clickX, clickY ) ||
             selectedWindow->inRollUpButtonRect( clickX, clickY ) )
        {
            buttonPressed = true;
            selectedWindow->checkMousePressEvent( clickX, clickY );
            update();
        }
        else if ( selectedWindow->inHeadlineRect( clickX, clickY ) )
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
                DoublePoint trussCoord = selectedWindow->
                                  getTrussCoordFromWidgetPos( clickX, clickY );
                const DoublePoint& firstPos = selectedPivot->
                                                     getFirstNode().getPoint();
                const DoublePoint& lastPos  = selectedPivot->
                                                     getLastNode().getPoint();

                // Get first and last nodes displacements about a clicked point 
                firstNodeClickDist.setX( int(firstPos.x() - trussCoord.x()) );
                firstNodeClickDist.setY( int(firstPos.y() - trussCoord.y()) );
                lastNodeClickDist.setX( int(lastPos.x()  - trussCoord.x()) );
                lastNodeClickDist.setY( int(lastPos.y()  - trussCoord.y()) );

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
                TrussNode* node = selectedWindow->findNodeByWidgetPos( clickX, 
                                                                       clickY );
                if ( node )
                    return;

                DoublePoint nodeCoord = selectedWindow->
                                  getTrussCoordFromWidgetPos( clickX, clickY );
                
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
                DoublePoint nodeCoord = selectedWindow->
                                  getTrussCoordFromWidgetPos( clickX, clickY );

                TrussNode* firstNode = 
                         selectedWindow->findNodeByWidgetPos( clickX, clickY );
                bool wasCreated = false;
                if ( firstNode == 0 ) 
                {
                    firstNode = &selectedWindow->createNode( nodeCoord.x(), 
                                                             nodeCoord.y() );
                    wasCreated = true;
                }
                TrussPivot* divPivot = 
                                selectedWindow->findDividualPivot( *firstNode );
                if ( divPivot )
                    selectedWindow->dividePivot( *divPivot, *firstNode );

                TrussNode& lastNode = selectedWindow->createNode( nodeCoord.x(), 
                                                                  nodeCoord.y() );
                lastNode.setVisible ( false );
                selectedPivot = &( selectedWindow->createPivot( *firstNode, 
                                                                 lastNode ) );
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
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* )
{
}

/*****************************************************************************/
