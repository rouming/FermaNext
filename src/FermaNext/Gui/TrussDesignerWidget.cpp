
#include <algorithm>
#include <vector>
#include <string>
#include <math.h>

#include <QSize>
#include <QWidget>
#include <QCursor>
#include <QPoint>
#include <QApplication>

#include "TrussDesignerWidget.h"
#include "Global.h"
#include "TrussUnitActions.h"
#include "AggPopupHint.h"
#include "AggTrussToolBar.h"

/*****************************************************************************
 * Fixation Popup Menu
 *****************************************************************************/

FixationPopupMenu::FixationPopupMenu ( QWidget* parent ) :
    QMenu( parent ),
    node(0)
{
    addAction( "Fixation by X", this, SLOT(fixNodeByX()) );
    addAction( "Fixation by Y", this, SLOT(fixNodeByY()) );
    addAction("Fixation by XY", this, SLOT(fixNodeByXY()) );
    addSeparator();
    addAction( "Unfixed", this, SLOT(unfixNode()) );
}

void FixationPopupMenu::showFixationPopup ( QMouseEvent* e, TrussNode* n)
{
    node = n;
    popup( e->globalPos() );

// According to Qt documentation popup menu _must_ steal 
// release mouse event, but this doesn't happen on Windows. 
// So only on other OS we should post it manually.
#ifndef QT_OS_WIN32
    // Restore stolen release event
    if ( parent() ) {
        QMouseEvent* releaseEvent = new QMouseEvent( 
                         QEvent::MouseButtonRelease,  e->pos(), e->globalPos(),
                         e->button(), e->buttons(), e->modifiers() );
        QApplication::postEvent( parent(), releaseEvent );        
    }
#endif
}

void FixationPopupMenu::fixNodeByX ()
{
    if ( node ) {
        node->setFixation( TrussNode::FixationByX );
        emit nodeWasFixed( *node );
    }
}

void FixationPopupMenu::fixNodeByY ()
{
    if ( node ) {
        node->setFixation( TrussNode::FixationByY );
        emit nodeWasFixed( *node );
    }
}

void FixationPopupMenu::fixNodeByXY ()
{
    if ( node ) {
        node->setFixation( TrussNode::FixationByXY );
        emit nodeWasFixed( *node );
    }
}

void FixationPopupMenu::unfixNode ()
{
    if ( node )
        node->setFixation( TrussNode::Unfixed );
}

/*****************************************************************************
 * Load Popup Menu
******************************************************************************/

LoadPopupMenu::LoadPopupMenu ( QWidget* parent ) :
    QMenu( parent ),
    node(0),
    selectedWindow(0),
    // Init X layout and box
    xBox( new QWidget(this) ),
    xLayout( new QHBoxLayout ),
    // Init Y layout and box
    yBox( new QWidget(this) ),
    yLayout( new QHBoxLayout ),
    // Init Button layout and box
    buttonBox( new QWidget(this) ),
    buttonLayout( new QHBoxLayout ),
    // Init Main layout
    vLayout( new QVBoxLayout ),
    // buttons
    ok( new QPushButton(tr("OK")) ),
    cancel( new QPushButton(tr("Cancel")) ),
    // labels
    loadXLabel( new QLabel(tr("Load by X:")) ),
    loadYLabel( new QLabel(tr("Load by Y:")) ),
    // edits
    loadXLine( new QLineEdit(xBox) ),
    loadYLine( new QLineEdit(yBox) )
{
    // Init X layout and box    
    xLayout->addWidget(loadXLabel);
    xLayout->addWidget(loadXLine);
    xLayout->setMargin(0);
    xLayout->setSpacing(0);
    xBox->setLayout(xLayout);
    // Init Y layout and box    
    yLayout->addWidget(loadYLabel);
    yLayout->addWidget(loadYLine);
    yLayout->setMargin(0);
    yLayout->setSpacing(0);
    yBox->setLayout(yLayout);
    // Init Button layout and box    
    buttonLayout->addWidget(ok);
    buttonLayout->addWidget(cancel);
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);
    buttonBox->setLayout(buttonLayout);
    // Init Main layout
    vLayout->addWidget(xBox);
    vLayout->addWidget(yBox);
    vLayout->setMargin(3);
    vLayout->setSpacing(3);
    vLayout->addWidget(buttonBox);
    setLayout(vLayout);

    loadXLine->setValidator( new QDoubleValidator(loadXLine) );
    loadYLine->setValidator( new QDoubleValidator(loadYLine) );

    QObject::connect( cancel, SIGNAL(clicked()), SLOT(close()) );
    QObject::connect( ok, SIGNAL(clicked()), SLOT(okClicked()) );
}

void LoadPopupMenu::showLoadPopup ( QMouseEvent* e, 
                                    TrussNode* n, 
                                    TrussUnitWindow* t)
{
    double x = 0, y = 0;
    selectedWindow = t;
    node = n;
    if ( selectedWindow && node ) {
        TrussUnit::LoadCase* loadCase = 
            selectedWindow->getLoadCases().getCurrentLoadCase();        
        if ( loadCase )  {
            TrussLoad* load = loadCase->findLoad( *node );
            if ( load ) {
                x = load->getXForce();
                y = load->getYForce();
            }
        }
    }
    loadXLine->setText( QString::number(x) );
    loadYLine->setText( QString::number(y) );
    
    popup( e->globalPos() );

// According to Qt documentation popup menu _must_ steal 
// release mouse event, but this doesn't happen on Windows. 
// So only on other OS we should post it manually.
#ifndef QT_OS_WIN32
    // Restore stolen release event
    if ( parent() ) {
        QMouseEvent* releaseEvent = new QMouseEvent( 
                         QEvent::MouseButtonRelease,  e->pos(), e->globalPos(),
                         e->button(), e->buttons(), e->modifiers() );
        QApplication::postEvent( parent(), releaseEvent );        
    }
#endif
}

void LoadPopupMenu::okClicked()
{
    if ( selectedWindow && node ) {
        TrussUnit::LoadCase* loadCase = selectedWindow->
            getLoadCases().getCurrentLoadCase();        
        if ( loadCase ) {
            TrussLoad* load = loadCase->findLoad( *node );
            if ( load ) {
                if ( loadXLine->text().toDouble() != 0 ||
                     loadYLine->text().toDouble() != 0 ) {
                    load->setXForce( loadXLine->text().toDouble() );
                    load->setYForce( loadYLine->text().toDouble() );
                    node->setFixation( Node::Unfixed );
                }
                else {
                    loadCase->removeLoad( *node );
                }
            }
            else if ( loadXLine->text().toDouble() != 0 ||
                      loadYLine->text().toDouble() != 0 ) {
                loadCase->addLoad( *node, loadXLine->text().toDouble(),
                                          loadYLine->text().toDouble() );
                node->setFixation( Node::Unfixed );
            }
        }
        else {
            TrussUnit::LoadCase& currentCase = 
                selectedWindow->getLoadCases().createLoadCase();
            selectedWindow->getLoadCases().setCurrentLoadCase( currentCase );
            currentCase.addLoad( *node, loadXLine->text().toDouble(),
                                        loadYLine->text().toDouble() );
        }
    }
    close();
}

/*****************************************************************************
 * Truss Designer Widget
 *****************************************************************************/

TrussDesignerWidget::TrussDesignerWidget ( QWidget* p ) :
    AggWidget(p, Global::flipY),
    fixationPopup( new FixationPopupMenu(this) ),
    loadPopup( new LoadPopupMenu(this) ),
    focusedWindow(0),
    selectedWindow(0),
    selectedNode(0),
    selectedPivot(0),    
    winBehaviour(windowIdle),
    nodeBehaviour(nodeIdle),
    pivotBehaviour(pivotIdle),
    buttonPressed(false),
    animationPlays(false),
    xPos(0), yPos(0), 
    firstNodeClickDist(0,0), 
    lastNodeClickDist(0,0),
    mouseOffset(0,0),
    toolBar( 0 ),
    aggHint( new AggPopupHint( *this ) ),
    // Temp
    X(50), Y(50)
{
    // Note that tool bar cannot be initialized in the initialize list
    // if we want to add buttons within the tool bar constructor
    toolBar = new AggTrussToolBar( this );

    QObject::connect( toolBar, 
                      SIGNAL(onHintShowsUp(const QString&,const QPoint,bool)),
                      aggHint, 
                      SLOT(show(const QString&,const QPoint,bool)) );

    QObject::connect( toolBar, SIGNAL( onHintHides(bool) ),
                      aggHint, SLOT( hide(bool) ) );
    
    QObject::connect( toolBar, SIGNAL( onAnimationPlays(bool) ),
                                 SLOT( setToolBarAnimStatus(bool) ) );

    QObject::connect( fixationPopup, SIGNAL(nodeWasFixed(const TrussNode&)),
                                SLOT(removeLoadsFromNode(const TrussNode&)) );

    QWidget::setFocus();
    QWidget::setMouseTracking( true );

    changeBehaviourToSelect();
}   

TrussDesignerWidget::~TrussDesignerWidget ()
{
    clearTrussUnitWindows();
    delete toolBar;
    delete aggHint;
    delete fixationPopup;
}

void TrussDesignerWidget::clearTrussUnitWindows ()
{
    trussWindows.clear();
}

void TrussDesignerWidget::addTrussUnitWindow ( TrussUnitWindow& trussWindow )
{   
    // Window owner
    trussWindow.setWindowOwner( this );

    // We should repaint all area after truss changes its visibility or state
    QObject::connect( &trussWindow, 
                      SIGNAL(onVisibleChange(bool)), 
                      SLOT(trussWindowChangedVisibility(bool)) );
    QObject::connect( &trussWindow, 
                      SIGNAL(onStateChange()),
                      SLOT(update()) );
    
    QObject::connect( &trussWindow, 
                      SIGNAL(onSwitchLoadCase()),
                      SLOT(update()) );

    QObject::connect( &trussWindow, 
                      SIGNAL(onBeforeDesist(StatefulObject&)), 
                      SLOT(trussWindowDesisted(StatefulObject&)));

    QObject::connect( &trussWindow, SIGNAL(onHintShowsUp(const QString&, 
                                                         const QPoint,bool)),
                      aggHint, SLOT(show(const QString&,const QPoint,bool)) );
    QObject::connect( &trussWindow, SIGNAL(onHintShowsUp(const QString&, 
                                                         const QPoint,bool)),
                                    SLOT( update() ) );

    QObject::connect( &trussWindow, SIGNAL( onHintHides(bool) ),
                      aggHint, SLOT( hide(bool) ) );
    QObject::connect( &trussWindow, SIGNAL( onHintHides(bool) ),
                                    SLOT( update() ) );

    trussWindows.push_back(&trussWindow);
    // If window position was not set, we set default value
    if ( ! trussWindow.hasPosition() ) {
        X += 15; 
        Y += 10;
        trussWindow.setWindowPosition ( QPoint( X, Y ) );
    }
    focusOnWindow(trussWindow);
}

bool TrussDesignerWidget::removeTrussUnitWindow ( TrussUnitWindow& window )
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( (*iter) == &window ) {
            TrussUnitWindow* trussWindow = *iter;
            trussWindow->setWindowOwner(0);
            // Do disconnection of all signals
            trussWindow->disconnect( this );
            trussWindows.erase(iter);
            repaint();
            return true;
        }
    return false; 
}

void TrussDesignerWidget::changeBehaviourToSelect ()
{
    designerBehaviour = onSelect;
}

void TrussDesignerWidget::changeBehaviourToNodeDraw ()
{
    designerBehaviour = onNodeDraw;
}

void TrussDesignerWidget::changeBehaviourToPivotDraw ()
{
    designerBehaviour = onPivotFirstNodeDraw;
}

void TrussDesignerWidget::changeBehaviourToFixDraw ()
{
    designerBehaviour = onFixDraw;
}

void TrussDesignerWidget::changeBehaviourToLoadDraw ()
{
    designerBehaviour = onLoadDraw;
}

void TrussDesignerWidget::changeBehaviourToErase ()
{
    designerBehaviour = onErase;
}

void TrussDesignerWidget::setToolBarAnimStatus ( bool status )
{
    animationPlays = status;
    if ( status )
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
    else
        QApplication::restoreOverrideCursor();
}

void TrussDesignerWidget::trussWindowChangedVisibility ( bool visible )
{
    QObject* senderObj = const_cast<QObject*>(sender());
    if ( senderObj == 0 )
        return;
    // Safe conversion
    try { 
        TrussUnitWindow& window = dynamic_cast<TrussUnitWindow&>(*senderObj);
        if ( visible )
            focusOnWindow( window );
        else
            focusOnPrevWindow( window );
        update();
    }
    catch ( ... ) { return; }
}

void TrussDesignerWidget::trussWindowDesisted ( StatefulObject& obj )
{
    // Safe conversion
    try { 
        TrussUnitWindow& window = dynamic_cast<TrussUnitWindow&>(obj);
        focusOnPrevWindow( window );
    }
    catch ( ... ) { return; }
}

TrussUnitWindow* TrussDesignerWidget::findWindowByWidgetPos ( int x, int y )
{
    WindowListRevIter revIter = trussWindows.rbegin();
    for ( ; revIter != trussWindows.rend(); ++revIter ) {
        TrussUnitWindow* w = *revIter;
        if ( w->isAlive() && w->isVisible() && w->inWindowRect(x, y) )
            return *revIter;
    }
    return 0;
}

void TrussDesignerWidget::focusOnWindow ( TrussUnitWindow& window )
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
    if ( focusedWindow ) {
        clearWindowFocus();
    }
    // Focus on window
    focusedWindow = &window;
    focusedWindow->setHighlighted(true);
    // Focused window to front
    trussWindows.erase(newSelectedIter);
    trussWindows.push_back(focusedWindow);

    emit onFocusReceive( *focusedWindow );

    update();
}

/*  
 *  Sends window to the bottom of the stack.
 */ 
void TrussDesignerWidget::sendWindowToBack ( TrussUnitWindow& window )
{
    focusOnPrevWindow( window );
    // Send to back.
    trussWindows.erase( std::find( trussWindows.begin(), trussWindows.end(), 
                                   &window ) );
    trussWindows.insert( trussWindows.begin(), &window );
}

/* 
 *  Focus on prev window, defocus old one.
 */
void TrussDesignerWidget::focusOnPrevWindow ( TrussUnitWindow& window )
{
    // We start searching from the end of the vector to get the top 
    // of the window stack.
    WindowListRevIter windowIt = std::find( trussWindows.rbegin(),
                                            trussWindows.rend(), 
                                            &window );
    if ( windowIt == trussWindows.rend() )
        // Hm. Wrong argument?
        return;

    if ( &window == focusedWindow ) {
        // We should defocus if it has focus and find 
        // prev alive window to bring it to front.

        WindowListRevIter focusedWindowIt = windowIt;

        // Next window in the stack.
        ++focusedWindowIt;

        bool foundAlivePrevWindow = false;
        for ( ; focusedWindowIt != trussWindows.rend(); ++focusedWindowIt ) {
            TrussUnitWindow* prevWindow = *focusedWindowIt;
            if ( prevWindow->isAlive() && prevWindow->isVisible() ) {
                foundAlivePrevWindow = true;
                focusOnWindow( *prevWindow );                    
                break;
            }
        }
        if ( ! foundAlivePrevWindow )
            clearWindowFocus();
    }
}

TrussUnitWindow* TrussDesignerWidget::getFocusedWindow () const
{
    return focusedWindow;
}

/** 
 * Returns window stack.
 * Note: this method is differ from 
 *       TrussUnitWindowManager::getTrussUnitWindowList.
 *       This method returns windows in order they are displayed
 *       by window owner.
 */
WindowList TrussDesignerWidget::getTrussUnitWindowList () const
{
    WindowList aliveWindows;
    WindowListConstIter wIter = trussWindows.begin();
    for ( ; wIter != trussWindows.end(); ++wIter )
        if ( (*wIter)->isAlive() )
            aliveWindows.push_back( *wIter );
    return aliveWindows;
}

void TrussDesignerWidget::clearWindowFocus ()
{
    if ( focusedWindow ) {
        focusedWindow->setHighlighted(false);
        // Correct block close if we catch focus change before release event.
        ObjectStateManager* mng = focusedWindow->getStateManager();
        if ( mng && mng->stateBlockIsNotEnded() )
            mng->endStateBlock();
        TrussUnitWindow* fWindow = focusedWindow;
        focusedWindow = 0;
        emit onFocusLose( *fWindow );
        update();
    }
}

void TrussDesignerWidget::removeTrussElemHighlight ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        window->removeNodesHighlight ();
        window->removePivotsHighlight ();
    }
    if ( nodeBehaviour == onNodeSelect )
        nodeBehaviour = nodeIdle;
    if ( pivotBehaviour == onPivotSelect )
        pivotBehaviour = pivotIdle;

    update ();
}

void TrussDesignerWidget::removeWindowButtonHighlight ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        window->removeButtonsHighlight();
    }
    update ();
}

void TrussDesignerWidget::releaseWindowButtons ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* window = (*iter);
        window->releaseButtons();
    }
    update ();
}

void TrussDesignerWidget::clearAllCursorCoordFields ()
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

void TrussDesignerWidget::clearHintsFromNonSelectedWindows ( 
                                       TrussUnitWindow* selWindow )
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
    {
        TrussUnitWindow* window = *iter;
        if ( window != selWindow )
            window->clearButtonHint();
    }
}

bool TrussDesignerWidget::nodeCanBeDrawn ( int x, int y )
{
    bool wrongArea = false;
    uint i = trussWindows.size();
    WindowListRevIter revIter = trussWindows.rbegin();
    for ( ; revIter != trussWindows.rend(); ++revIter )
    {
        i--;
        TrussUnitWindow* window = *revIter;
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

void TrussDesignerWidget::removeLoadsFromNode ( const TrussNode& node )
{
    if ( ! focusedWindow )
        return;

    const TrussUnit::LoadCases& loadCases = focusedWindow->getLoadCases();
    for ( int i = 1; i <= loadCases.countLoadCases(); ++i ) {
        TrussUnit::LoadCase* loadCase = loadCases.findLoadCase( i );
        if ( loadCase )
            loadCase->removeLoad( node );
    }
}

void TrussDesignerWidget::saveNodeStateAfterDrag ( TrussNode& node,
                                                   const DoublePoint& pos )
{
    // Similar points. Nothing to save.
    if ( pos == beforeDragNodePos )
        return;

    // Save move node action
    ObjectState& state = node.createState( "move node" );
    state.addAction( new ConcreteObjectAction<TrussNode, DoublePoint>( 
                                                     node, 
                                                     &TrussNode::setPoint,
                                                     &TrussNode::setPoint,
                                                     pos,
                                                     beforeDragNodePos) );
    state.save();    
}

void TrussDesignerWidget::saveNodeStateAfterCreate ( TrussNode& node )
{    
    // Save create node action
    ObjectState& state = node.createState( "create node" );
    TrussNodeCreateAction* action = new TrussNodeCreateAction( node );
    state.addAction( action );
    state.save();
}

void TrussDesignerWidget::saveNodeStateAfterRemove ( TrussNode& node )
{
    // Save remove node action
    ObjectState& state = node.createState( "remove node" );
    TrussNodeRemoveAction* action = new TrussNodeRemoveAction( node );
    state.addAction( action );
    state.save();
}

void TrussDesignerWidget::savePivotStateAfterDrag ( 
    TrussPivot& pivot,
    const DoublePoint& firstPos, 
    const DoublePoint& lastPos )
{
    // Similar points. Nothing to save.
    if ( firstPos == beforeDragFirstPos && 
         lastPos == beforeDragLastPos )
        return;

    TrussNode& firstNode = pivot.getFirstNode();
    TrussNode& lastNode  = pivot.getLastNode();    

    ObjectStateManager* mng = pivot.getStateManager();
    mng->startStateBlock();
    
    // First node pos state to save
    ObjectState& firstState = firstNode.createState( "move node" );

    // First pos    
    firstState.addAction( new ConcreteObjectAction<TrussNode, DoublePoint>( 
                                                         firstNode, 
                                                         &TrussNode::setPoint,
                                                         &TrussNode::setPoint,
                                                         firstPos,
                                                         beforeDragFirstPos) );
    firstState.save();

    // Last node pos state to save
    ObjectState& lastState = lastNode.createState( "move node" );

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

void TrussDesignerWidget::savePivotStateAfterCreate ( TrussNode& firstNode,
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
    ObjectState& state = pivot.createState( "create pivot" );
    TrussPivotCreateAction* action = new TrussPivotCreateAction( pivot );
    state.addAction( action );
    state.save();
    
    mng->endStateBlock();
}

/*****************************************************************************/
// Event handlers

void TrussDesignerWidget::aggPaintEvent ( QPaintEvent* )
{
    pixfmt pixf ( getAggRenderingBuffer() );
    base_renderer baseRend ( pixf );
    solid_renderer solidRend ( baseRend );
    baseRend.clear ( agg::rgba( 10, 10, 10 ) );
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) {
        TrussUnitWindow* w = *iter;
        if ( w->isAlive() )
            w->paint( baseRend );
    }
    toolBar->paint( baseRend );
    aggHint->paint( baseRend );
}

void TrussDesignerWidget::aggResizeEvent ( QResizeEvent* )
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter ) 
    {
        TrussUnitWindow* w = *iter;
        if ( w->isAlive() && w->isMaximized() )
            w->maximize( false );
    }
    int width = getAggRenderingBuffer().width(),
        height = getAggRenderingBuffer().height();
    toolBar->setCenterPosition( QPoint( width/2, height + 2 ) );
    aggHint->renewWidgetSize( size() );
}

void TrussDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
    if ( ke->key() == Qt::Key_N && designerBehaviour != onNodeDraw && 
         designerBehaviour != onPivotLastNodeDraw )
    {
        designerBehaviour = onNodeDraw;
        emit pressNodeDrawButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_P && designerBehaviour != onPivotLastNodeDraw )
    {
        designerBehaviour = onPivotFirstNodeDraw;
        emit pressPivotDrawButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_R && designerBehaviour != onErase &&
         designerBehaviour != onPivotLastNodeDraw )
    {
        designerBehaviour = onErase;
        emit pressEraseButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_F && designerBehaviour != onFixDraw &&
         designerBehaviour != onPivotLastNodeDraw )
    {
        designerBehaviour = onFixDraw;
        emit pressFixDrawButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_L && designerBehaviour != onLoadDraw &&
         designerBehaviour != onPivotLastNodeDraw )
    {
        designerBehaviour = onLoadDraw;
        emit pressLoadDrawButton();
        removeTrussElemHighlight ();
    }
    if ( ke->key() == Qt::Key_Escape  && 
         designerBehaviour != onSelect &&
         designerBehaviour != onPivotLastNodeDraw )
    {
        designerBehaviour = onSelect;
        emit pressSelectButton();
        QWidget::setCursor ( Qt::ArrowCursor );
    }
    if ( ke->key() == Qt::Key_Escape  && 
         designerBehaviour == onPivotLastNodeDraw )
    {
        if ( selectedWindow ) {
            ObjectStateManager* mng = selectedWindow->getStateManager();
            // Rollback state block which was started in press mouse event.
            mng->rollbackNotEndedBlock();
        }
        selectedNode = 0;
        selectedPivot = 0;
        designerBehaviour = onPivotFirstNodeDraw;
        QWidget::setCursor ( Qt::ArrowCursor );
    }
}

void TrussDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
    if ( animationPlays )
        qApp->processEvents( QEventLoop::ExcludeUserInputEvents );

    /*  
        x, y - current mouse position;
        xPos, yPos - previous mouse position;
        dx, dy - difference between them.
    */    
    int x = me->x();
    int y = Global::flipY ? height() - me->y() : me->y();

    if ( winBehaviour == windowIdle )
        toolBar->mouseMove( x, y );

    if ( toolBar->inToolBarRect( x, y ) && winBehaviour == windowIdle )
        QWidget::setCursor ( Qt::ArrowCursor );

    else if ( winBehaviour == onHorResize )
    {
        int dx = x - xPos;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
        if ( abs ( xPos - point1.x() ) <= Global::bordWidth )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < Global::resizeLimit )
                point1.setX( point2.x() - Global::resizeLimit );
        }
        if ( abs ( xPos - point2.x() ) <= Global::bordWidth )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < Global::resizeLimit )
               point2.setX( point1.x() + Global::resizeLimit );
        }
        selectedWindow->resize( point1, point2 );
        update();
        xPos += dx;
    }
    else if ( winBehaviour == onVerResize )
    {
        int dy = y - yPos;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
        if ( abs ( yPos - point1.y() ) <= Global::bordWidth )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < Global::resizeLimit )
                point1.setY( point2.y() - Global::resizeLimit );
        }
        if ( abs ( yPos - point2.y() ) <= Global::bordWidth )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < Global::resizeLimit )
               point2.setY( point1.y() + Global::resizeLimit );
        }
        selectedWindow->resize( point1, point2 );
        update();
        yPos += dy;
    }
    else if ( winBehaviour == onBDiagResize )
    {
        int dx = x - xPos;
        int dy = y - yPos;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
        if ( abs ( xPos - (point1.x() + Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < Global::resizeLimit )
                point1.setX( point2.x() - Global::resizeLimit );
        }
        if ( abs ( xPos - (point2.x() - Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < Global::resizeLimit )
               point2.setX( point1.x() + Global::resizeLimit );
        }
        if ( abs ( yPos - (point1.y() + Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < Global::resizeLimit )
                point1.setY( point2.y() - Global::resizeLimit );
        }
        if ( abs ( yPos - (point2.y() - Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < Global::resizeLimit )
               point2.setY( point1.y() + Global::resizeLimit );
        }
        selectedWindow->resize( point1, point2 );
        update();
        xPos += dx;
        yPos += dy;
    }
    else if ( winBehaviour == onFDiagResize )
    {
        int dx = x - xPos;
        int dy = y - yPos;
        QPoint point1 = selectedWindow->getWindowLeftTopPos ();
        QPoint point2 = selectedWindow->getWindowRightBottomPos ();
        if ( abs ( xPos - (point1.x() + Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point1.setX ( point1.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < Global::resizeLimit )
                point1.setX ( point2.x() - Global::resizeLimit );
        }
        if ( abs ( xPos - (point2.x() - Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point2.setX ( point2.x() + dx );
            if ( abs ( point2.x() - point1.x() ) < Global::resizeLimit )
               point2.setX ( point1.x() + Global::resizeLimit );
        }
        if ( abs ( yPos - (point1.y() + Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point1.setY ( point1.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < Global::resizeLimit )
                point1.setY ( point2.y() - Global::resizeLimit );
        }
        if ( abs ( yPos - (point2.y() - Global::bordWidth) ) <= 
             Global::bordWidth )
        {
            point2.setY ( point2.y() + dy );
            if ( abs ( point2.y() - point1.y() ) < Global::resizeLimit )
               point2.setY ( point1.y() + Global::resizeLimit );
        }
        selectedWindow->resize( point1, point2 );
        update();
        xPos += dx;
        yPos += dy;
    }
    else if ( winBehaviour == onWindowDrag )
    {
        int dx = x - xPos;
        int dy = y - yPos;
        QPoint leftTop, rightBottom;

        leftTop.setX( x - mouseOffset.x() );
        leftTop.setY( y - mouseOffset.y() ); 

        int left = x - mouseOffset.x();
        int top = y - mouseOffset.y();
        int right = left + selectedWindow->getWindowSize().width();
        int bottom = top + selectedWindow->getWindowSize().height();

	    // snap window to screen edges
        int vertOffset = 0,
            horOffset = 1;

	    if ( top < Global::snapPixels && top > -Global::snapPixels )
		    leftTop.setY( 0 );

	    if ( left < Global::snapPixels && left > -Global::snapPixels)
		    leftTop.setX( 0 - horOffset );

        int w = getAggRenderingBuffer().width(),
            h = getAggRenderingBuffer().height();
	    if ( right < w - horOffset + Global::snapPixels && 
             right > w - horOffset - Global::snapPixels )
		    leftTop.setX( w - horOffset - 
                          selectedWindow->getWindowSize().width() );

	    if ( bottom < h - vertOffset + Global::snapPixels && 
             bottom > h - vertOffset - Global::snapPixels )
		    leftTop.setY( h - vertOffset - 
                          selectedWindow->getWindowSize().height() );

        selectedWindow->setWindowPosition( leftTop );
        update();
        xPos += dx;
        yPos += dy;
    }
    else
    {
        TrussUnitWindow* window = findWindowByWidgetPos ( x, y );
        if ( window )
        {
            if ( window->inTrussAreaRect( x, y ) )
            {
                clearAllCursorCoordFields ();
                window->setCursorCoord ( QPoint(x, y) );
            }
            else
                clearAllCursorCoordFields ();

            removeWindowButtonHighlight ();
            clearHintsFromNonSelectedWindows( window );
            window->releaseButtons();
            window->checkMouseMoveEvent( x, y, buttonPressed );
            update();
        }
        else
        {
            clearHintsFromNonSelectedWindows( 0 );
            removeWindowButtonHighlight();
        }

        if ( designerBehaviour == onNodeDraw || 
             designerBehaviour == onPivotFirstNodeDraw ||
             designerBehaviour == onErase )
        {
            TrussUnitWindow* w = findWindowByWidgetPos( x, y );
            if ( w )
            {
                removeTrussElemHighlight();
                TrussNode* node = w->findNodeByWidgetPos( x, y );
                TrussPivot* pivot = 
                    w->findPivotByWidgetPos(x, y, 2 * w->getPivotPrecision());
                if ( node )
                {
                    w->setCursorCoord( node->getPoint() );
                    w->setFocusOnNode( node );
                    update();
                }
                else if ( pivot )
                {
                    
                    w->setFocusOnPivot( pivot );
                    update();
                }
                if ( nodeCanBeDrawn( x, y ) || node )
                    QWidget::setCursor( Qt::CrossCursor );
                else
                    QWidget::setCursor( Qt::ArrowCursor );
            }
        }

        if ( nodeBehaviour == onNodeDrag )
        {
            selectedWindow->moveTrussNode( x, y, selectedNode );
            selectedWindow->setCursorCoord( selectedNode->getPoint() );
            update();
        }
        else if ( pivotBehaviour == onPivotDrag )
        {
            clearAllCursorCoordFields();
            selectedWindow->setCursorCoord( QPoint(x, y) );
            selectedWindow->moveTrussPivot( x, y, selectedPivot, 
                                            firstNodeClickDist, 
                                            lastNodeClickDist );     
            update();
        }
        else if ( designerBehaviour  == onPivotLastNodeDraw )
        {
            selectedWindow->moveTrussNode( x, y, selectedNode );
            selectedWindow->setCursorCoord( selectedNode->getPoint() );
            TrussNode* node = selectedWindow->findNodeByWidgetPos( x, y );
            if ( nodeCanBeDrawn( x, y ) || node )
                QWidget::setCursor( Qt::CrossCursor );
            else
                QWidget::setCursor( Qt::ArrowCursor );
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
                else if ( designerBehaviour == onSelect || 
                          designerBehaviour == onErase ||
                          designerBehaviour == onFixDraw ||
                          designerBehaviour == onLoadDraw )
                {
                    selectedNode = selectedWindow->findNodeByWidgetPos(x, y);
                    selectedPivot = selectedWindow->findPivotByWidgetPos(x, y);
                    if ( selectedNode )
                    {
                        removeTrussElemHighlight ();
                        clearAllCursorCoordFields ();
                        selectedWindow->setFocusOnNode( selectedNode );
                        selectedWindow->setCursorCoord( 
                                                   selectedNode->getPoint() );
                        nodeBehaviour = onNodeSelect;
                        if ( designerBehaviour == onErase ||
                             designerBehaviour == onFixDraw ||
                             designerBehaviour == onLoadDraw )
                            QWidget::setCursor ( Qt::CrossCursor );
                        else
                            QWidget::setCursor ( Qt::ArrowCursor );
                        update();
                    }
                    else if ( selectedPivot && designerBehaviour == onSelect ||
                              selectedPivot && designerBehaviour == onErase )
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
    emit cursorMoved();
}

void TrussDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
    buttonPressed = false;

    int x = me->x();
    int y = Global::flipY ? height() - me->y() : me->y();

    removeTrussElemHighlight();
    if ( designerBehaviour == onPivotLastNodeDraw )
    {
        selectedPivot->getLastNode().setVisible ( true );
        selectedPivot->setDrawingStatus ( true );

        ObjectStateManager* mng = selectedWindow->getStateManager();
        mng->startStateBlock();

        selectedWindow->updateAfterNodeManipulation ( selectedNode, false );

        QString msg( tr("Create pivot (%1,%2)(%3,%4)").
                       arg(int(selectedPivot->getFirstNode().getX())).
                       arg(int(selectedPivot->getFirstNode().getY())).
                       arg(int(selectedPivot->getLastNode().getX())).
                       arg(int(selectedPivot->getLastNode().getY())) );
        mng->endStateBlock( msg );

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
        if( selectedNode )
            saveNodeStateAfterDrag( *selectedNode, selectedNode->getPoint() );

        selectedWindow->updateAfterNodeManipulation ( selectedNode, true );

        QString msg( tr("Move node (%1,%2) -> (%3,%4)").
                       arg(int(beforeDragNodePos.getX())).
                       arg(int(beforeDragNodePos.getY())).
                       arg(int(selectedNode->getX())).
                       arg(int(selectedNode->getY())) );
        mng->endStateBlock( msg );

        update();

        TrussUnitWindow* window = findWindowByWidgetPos ( x, y );
        if ( window && window == selectedWindow )
        {
            selectedNode = window->findNodeByWidgetPos( x, y );
            if ( !selectedNode )
                nodeBehaviour = nodeIdle;
            else
                window->setFocusOnNode ( selectedNode );
        }
        else
        {
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
        if( selectedPivot )
            savePivotStateAfterDrag( *selectedPivot, firstPos, lastPos );

        selectedWindow->updateAfterPivotManipulation ( selectedPivot, true );

        QString msg( tr("Move pivot (%1,%2)(%3,%4)").
                       arg(int(selectedPivot->getFirstNode().getX())).
                       arg(int(selectedPivot->getFirstNode().getY())).
                       arg(int(selectedPivot->getLastNode().getX())).
                       arg(int(selectedPivot->getLastNode().getY())) );
        mng->endStateBlock( msg );

        update();

        TrussUnitWindow* window = findWindowByWidgetPos ( x, y );
        if ( window && window == selectedWindow )
        {
            selectedPivot = window->findPivotByWidgetPos( x, y );
            if ( !selectedPivot )
                pivotBehaviour = pivotIdle;
            else
                window->setFocusOnPivot ( selectedPivot );
        }
        else
        {
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

    // End state block which was started in press mouse event.
    if ( focusedWindow ) {
        ObjectStateManager* mng = focusedWindow->getStateManager();
        if ( mng && mng->stateBlockIsNotEnded() ) 
            mng->endStateBlock();
    }
}

void TrussDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    QWidget::setFocus();
    xPos = me->x();
    yPos = Global::flipY ? height() - me->pos().y() : me->pos().y();

    if ( toolBar->inToolBarRect( xPos, yPos ) )
    {
        toolBar->mousePress( xPos, yPos );
        update();
    } 
    else 
    {
        selectedWindow = findWindowByWidgetPos ( xPos, yPos );
        if ( selectedWindow )
        {
            focusOnWindow( *selectedWindow );

            ObjectStateManager* mng = focusedWindow->getStateManager();
            mng->startStateBlock();

            if ( selectedWindow->inHideButtonRect( xPos, yPos ) ||
                 selectedWindow->inRollUpButtonRect( xPos, yPos ) )
            {
                buttonPressed = true;
                selectedWindow->checkMousePressEvent( xPos, yPos );
                update();
            }
            else if ( selectedWindow->inHeadlineRect( xPos, yPos ) )
            {
                // save offset to click position for snapping test
                mouseOffset = 
                    QPoint( xPos - selectedWindow->getWindowLeftTopPos().x(), 
                            yPos - selectedWindow->getWindowLeftTopPos().y() );
                winBehaviour = onWindowDrag;
            }
            else if ( selectedWindow->inHorResizeRect ( xPos, yPos ) )
            {
                winBehaviour = onHorResize;
                QWidget::setCursor ( Qt::SizeHorCursor );
            }
            else if ( selectedWindow->inVerResizeRect ( xPos, yPos ) )
            {
                winBehaviour = onVerResize;
                QWidget::setCursor ( Qt::SizeVerCursor );
            }
            else if ( selectedWindow->inBDiagResizeRect ( xPos, yPos ) )
            {
                winBehaviour = onBDiagResize;
                QWidget::setCursor ( Qt::SizeBDiagCursor );
            }
            else if ( selectedWindow->inFDiagResizeRect ( xPos, yPos ) )
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
                    ObjectState& state = 
                        selectedNode->createState( "remove node" );
                    state.addAction( new TrussNodeRemoveAction(*selectedNode));
                    state.save();
                    selectedNode->desist();

                    QString msg( tr("Remove node (%1,%2)").
                                   arg(int(selectedNode->getX())).
                                   arg(int(selectedNode->getY())) );
                    mng->endStateBlock( msg );
    
                    update();
                }
                else
                {
                    selectedWindow->nodeToFront ( *selectedNode );

                    if ( designerBehaviour == onFixDraw ) {
                         {
                            fixationPopup->showFixationPopup( me, selectedNode );
                            emit onFixationSet( *selectedNode );
                        }
                    }
                    else if ( designerBehaviour == onLoadDraw ) {
                        loadPopup->showLoadPopup( me, selectedNode, 
                                                  selectedWindow );
                        emit onLoadSet( *selectedNode ); 
                    }
                    else
                    {
                        // Save pos for Undo/Redo
                        beforeDragNodePos = selectedNode->getPoint();
                        nodeBehaviour = onNodeDrag;
                    }
                    update();
                }
            }
            else if ( pivotBehaviour == onPivotSelect )
            {
                if ( designerBehaviour == onErase )
                {
                    // Save remove pivot action
                    ObjectStateManager* mng = selectedPivot->getStateManager();
                    mng->startStateBlock();
                    ObjectState& state = 
                        selectedPivot->createState( "remove pivot" );
                    state.addAction( 
                        new TrussPivotRemoveAction( *selectedPivot ) );
                    state.save();
                    selectedPivot->desist();

                    QString msg( tr("Remove pivot (%1,%2)(%3,%4)").
                                arg(int(selectedPivot->getFirstNode().getX())).
                                arg(int(selectedPivot->getFirstNode().getY())).
                                arg(int(selectedPivot->getLastNode().getX())).
                                arg(int(selectedPivot->getLastNode().getY())));
                    mng->endStateBlock( msg );

                    update();
                }
                else
                {
                    // Convert widget coordinates into truss (absolute) 
                    // coordinates
                    DoublePoint trussCoord = selectedWindow->
                                      getTrussCoordFromWidgetPos( xPos, yPos );
                    const DoublePoint& firstPos = 
                        selectedPivot->getFirstNode().getPoint();
                    const DoublePoint& lastPos  = 
                        selectedPivot->getLastNode().getPoint();

                    // Get first and last nodes displacements 
                    // about a clicked point 
                    firstNodeClickDist.setX( 
                        int(firstPos.x() - trussCoord.x()) );
                    firstNodeClickDist.setY( 
                        int(firstPos.y() - trussCoord.y()) );
                    lastNodeClickDist.setX( 
                        int(lastPos.x()  - trussCoord.x()) );
                    lastNodeClickDist.setY( 
                        int(lastPos.y()  - trussCoord.y()) );

                    // Save pos for Undo/Redo
                    beforeDragFirstPos = firstPos;
                    beforeDragLastPos  = lastPos;

                    pivotBehaviour = onPivotDrag;
                }
            }    
            else if ( designerBehaviour == onNodeDraw )
            {
                if ( nodeCanBeDrawn ( xPos, yPos ) )
                {
                    TrussNode* node = 
                        selectedWindow->findNodeByWidgetPos( xPos, yPos );
                    if ( node )
                        return;

                    DoublePoint nodeCoord = selectedWindow->
                                      getTrussCoordFromWidgetPos( xPos, yPos );
                
                    TrussNode& newNode = 
                        selectedWindow->createNode ( nodeCoord.x(), 
                                                     nodeCoord.y() );

                    ObjectStateManager* mng = 
                        selectedWindow->getStateManager();
                    mng->startStateBlock();

                    saveNodeStateAfterCreate( newNode );
                    selectedWindow->updateAfterNodeManipulation( 
                        &newNode, true );

                    QString msg( tr("Create node (%1,%2)").
                                   arg(int(newNode.getX())).
                                   arg(int(newNode.getY())) );  
                    mng->endStateBlock( msg );

                    update();
                }
            }
            else if ( designerBehaviour == onPivotFirstNodeDraw )
            {
                TrussNode* firstNode = 
                         selectedWindow->findNodeByWidgetPos( xPos, yPos );

                if ( nodeCanBeDrawn ( xPos, yPos ) || firstNode )
                {
                    DoublePoint nodeCoord = selectedWindow->
                                      getTrussCoordFromWidgetPos( xPos, yPos );

                    bool wasCreated = false;
                    if ( firstNode == 0 ) 
                    {
                        firstNode = 
                            &selectedWindow->createNode( nodeCoord.x(), 
                                                         nodeCoord.y() );
                        saveNodeStateAfterCreate( *firstNode );
                        wasCreated = true;
                    }
                    TrussPivot* divPivot = 
                               selectedWindow->findDividualPivot( *firstNode );
                    if ( divPivot )
                        selectedWindow->dividePivot( *divPivot, *firstNode );

                    TrussNode& lastNode = 
                        selectedWindow->createNode( nodeCoord.x(), 
                                                    nodeCoord.y() );
                    lastNode.setVisible ( false );
                    selectedPivot = &selectedWindow->createPivot( *firstNode, 
                                                                  lastNode );
                    selectedPivot->setDrawingStatus ( false );
                    selectedNode = &lastNode;
                    designerBehaviour = onPivotLastNodeDraw;
                    savePivotStateAfterCreate( *firstNode, wasCreated, 
                                                lastNode, *selectedPivot );
                    update();
                }
            }
        }
        else
            clearWindowFocus();
    }
}

void TrussDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* )
{
}

/*****************************************************************************/
