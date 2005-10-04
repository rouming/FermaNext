
#include "TrussUnitWindow.h"
#include "TrussUnitActions.h"

/*****************************************************************************
 * Truss Unit Window
 *****************************************************************************/

TrussUnitWindow::TrussUnitWindow ( const QString& name, 
                                   ObjectStateManager* mng ) :
    TrussUnit(name, mng),
    headFont(agg::verdana14_bold),
    numbersFont(agg::verdana13),
    windowBuf( new rbuf_dynarow( 250, 250 ) ),
    coordBuf( new rbuf_dynarow( 100, 10 ) ),
    numbersBuf( new rbuf_dynarow( 60, 10 ) ),
    buttonBuf( new rbuf_dynarow( 36, 16 ) ),
    hideButton( new TrussUnitWindowButton( QPoint( 22, 1 ),
                imagesSvgPath() + "/closeIcon.svg" ) ),
    rollUpButton( new TrussUnitWindowButton( QPoint( 7, 1 ),
                  imagesSvgPath() + "/rollUpIcon.svg" ) ),
    cursorCoord ( -1, -1 ),
    windowSize ( 250, 250),
    coordFieldSize ( 49, 12 ),
    buttonBufRendered( false ),
    coordFieldRendered( false ),
    maximized( false )
{
    QObject::connect( hideButton, SIGNAL( onButtonHighlightChange() ),
                      SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( rollUpButton, SIGNAL( onButtonHighlightChange() ),
                      SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( hideButton, SIGNAL( onButtonPress() ),
                      SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( rollUpButton, SIGNAL( onButtonPress() ),
                      SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( hideButton, SIGNAL( onButtonRelease() ),
                      SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( rollUpButton, SIGNAL( onButtonRelease() ),
                      SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( this, SIGNAL( onHighlightChange( bool ) ),
                      hideButton, SLOT( setWindowHighlightStatus( bool ) ) );

    QObject::connect( this, SIGNAL( onHighlightChange( bool ) ),
                      rollUpButton, SLOT( setWindowHighlightStatus( bool ) ) );

    setCanvasColor( 8, 10, 12 );
    setHighlighted( false );    
}

TrussUnitWindow::~TrussUnitWindow ()
{
    delete numbersBuf;
    delete coordBuf;
    delete windowBuf;
    delete buttonBuf;
    delete hideButton;
    delete rollUpButton;
}

void TrussUnitWindow::clearButtonBufRenderedFlag ()
{
    buttonBufRendered = false;
}

void TrussUnitWindow::hide ()
{
    // Save visibility change to undo-redo
    ObjectState& state = createState();
    state.addAction( new TrussUnitWindowVisibilityChange( *this, false ) );
    state.save();

    setVisible( false );
}

void TrussUnitWindow::maximize ( bool saveOldSize )
{
    removeButtonsHighlight();

    if ( saveOldSize )
    {
        // save old window position for future minimization
        minLeftTopPos = windowLeftTopPos;
        minRightBottomPos = windowRightBottomPos;
    }
    resize ( QPoint( 0, 0 ), 
             QPoint( maxSize.width(), maxSize.height() ) );
    maximized = true;
}

void TrussUnitWindow::minimize ()
{
    removeButtonsHighlight();

    resize ( minLeftTopPos, minRightBottomPos );
    maximized = false;
}

QPoint TrussUnitWindow::getWindowLeftTopPos () const
{
    return windowLeftTopPos;
}

QPoint TrussUnitWindow::getWindowRightBottomPos () const
{
    return windowRightBottomPos;
}

QPoint TrussUnitWindow::getTrussAreaLeftTopPos () const
{
    QPoint point;
    point.setX ( windowLeftTopPos.x() + leftWindowIndent );
    point.setY ( windowLeftTopPos.y() + topWindowIndent );
    return point;
}

QPoint TrussUnitWindow::getTrussAreaRightBottomPos () const
{
    QPoint point;
    point.setX ( windowLeftTopPos.x() + windowSize.width() - rigthWindowIndent );
    point.setY ( windowLeftTopPos.y() + windowSize.height() - bottomWindowIndent );
    return point;
}

DoublePoint TrussUnitWindow::getTrussCoordFromWidgetPos ( int x, int y ) const
{
    // TODO: flipY comparison
    double scaleMultX = getScaleMultiplierX ();
    double scaleMultY = getScaleMultiplierY ();
    double absX = ( x - windowLeftTopPos.x() - leftWindowIndent ) / scaleMultX;
    double absY = ( windowSize.height() - y + windowLeftTopPos.y() - bottomWindowIndent )
                   / scaleMultY;
    return DoublePoint( absX, absY );
}

DoublePoint TrussUnitWindow::getTrussCoordFromWidgetPos ( QPoint pos ) const
{
    return getTrussCoordFromWidgetPos ( pos.x(), pos.y() );
}

QPoint TrussUnitWindow::getWidgetPosFromTrussCoord ( double x, double y ) const
{
    double scaleMultX = getScaleMultiplierX ();
    double scaleMultY = getScaleMultiplierY ();
    int widgetX = int ( x * scaleMultX ) + leftWindowIndent + windowLeftTopPos.x();
    int widgetY = ( flipY ? int( ( getTrussAreaSize().height() - y ) * 
                   scaleMultY ) + topWindowIndent + windowLeftTopPos.y() : 
                   int(y * scaleMultY) + topWindowIndent + windowLeftTopPos.y() );
    return QPoint( widgetX, widgetY );
}

QPoint TrussUnitWindow::getWidgetPosFromTrussCoord ( const DoublePoint& coord ) const
{
    return getWidgetPosFromTrussCoord ( coord.x(), coord.y() );
}

double TrussUnitWindow::getScaleMultiplierX () const
{
    const DoubleSize& trussAreaSize = getTrussAreaSize();
    QPoint p1, p2;
    p1 = getTrussAreaLeftTopPos ();
    p2 = getTrussAreaRightBottomPos ();
    double realTrussAreaLen = abs ( p2.x() - p1.x() );
    double scaleMultiplierX = realTrussAreaLen / trussAreaSize.width();
    return scaleMultiplierX;
}

double TrussUnitWindow::getScaleMultiplierY () const
{
    const DoubleSize& trussAreaSize = getTrussAreaSize();
    QPoint p1, p2;
    p1 = getTrussAreaLeftTopPos ();
    p2 = getTrussAreaRightBottomPos ();
    double realTrussAreaWid = abs ( p2.y() - p1.y() );
    double scaleMultiplierY = realTrussAreaWid / trussAreaSize.height();
    return scaleMultiplierY;
}

void TrussUnitWindow::setWindowPosition ( QPoint pos )
{
    QPoint oldPos = windowLeftTopPos;
    windowLeftTopPos = pos;
    windowRightBottomPos.setX ( windowLeftTopPos.x() + windowSize.width() );
    windowRightBottomPos.setY ( windowLeftTopPos.y() + windowSize.height() );
    emit onMove( oldPos, pos );
}

void TrussUnitWindow::resize ( QPoint leftTop, QPoint rightBottom )
{
    int dx = rightBottom.x() - leftTop.x();
    int dy = rightBottom.y() - leftTop.y();
    windowBuf->init( dx, dy );
    setWindowSize( dx, dy );
    setWindowPosition( leftTop );    
}

const QSize& TrussUnitWindow::getWindowSize () const
{
    return windowSize;
}

void TrussUnitWindow::setWindowSize ( int width, int height )
{
    QSize oldSize = windowSize;
    windowSize.setWidth ( width );
    windowSize.setHeight ( height );
    rendered(false);
    emit onResize( oldSize, windowSize );
}

void TrussUnitWindow::setCursorCoord ( const QPoint& p )
{
    if ( cursorCoord == DoublePoint( p.x(), p.y() ) )
        return;

    if ( p.x() != -1 && p.y() != -1 ) 
    {
        DoublePoint coord = getTrussCoordFromWidgetPos( p.x(), p.y() );
        const DoubleSize& size = getTrussAreaSize();
        if ( coord.x() > size.width() || coord.x() < 0 ||
             coord.y() > size.height() || coord.y() < 0 ) {
            cursorCoord.setX( -1 );
            cursorCoord.setY( -1 );
        }
        else 
            cursorCoord = DoublePoint( coord.x(), coord.y() );
    }
    else
        cursorCoord = DoublePoint( p.x(), p.y() );

    coordFieldRendered = false;
}

void TrussUnitWindow::setCursorCoord ( const DoublePoint& p )
{
    if ( cursorCoord == p )
        return;
    cursorCoord = p;
    coordFieldRendered = false;
}

DoublePoint TrussUnitWindow::getCursorCoord () const
{
    return cursorCoord;
}

void TrussUnitWindow::setMaxSize ( int x, int y )
{
    maxSize = QSize( x - bordWidth, y - bordWidth );
}

bool TrussUnitWindow::isMaximized () const
{
    return maximized;
}

bool TrussUnitWindow::inWindowRect ( int x, int y ) const
{
    if ( x >= windowLeftTopPos.x() && x <= windowRightBottomPos.x() && 
         y >= windowLeftTopPos.y() && y <= windowRightBottomPos.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inCanvasRect ( int x, int y ) const
{
    if ( x >= windowLeftTopPos.x() + bordWidth && 
        x <= windowRightBottomPos.x() - bordWidth && 
        y >= windowLeftTopPos.y() + bordWidth + headWidth && 
        y <= windowRightBottomPos.y() - bordWidth )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inHeadlineRect ( int x, int y ) const
{
    if ( maximized )
        return false;

    if ( x >= windowLeftTopPos.x() + 3 * bordWidth && 
         x <= windowRightBottomPos.x() - 3 * bordWidth && y >= 
         windowLeftTopPos.y() + bordWidth && 
         y <= windowLeftTopPos.y() + bordWidth + headWidth &&
         ! inHideButtonRect( x, y ) && 
         ! inRollUpButtonRect( x, y ) )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inTrussAreaRect ( int x, int y ) const
{
    if ( x >= getTrussAreaLeftTopPos().x() && 
         x <= getTrussAreaRightBottomPos().x() &&
         y >= getTrussAreaLeftTopPos().y() &&
         y <= getTrussAreaRightBottomPos().y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inHorResizeRect ( int x, int y ) const
{
    if ( maximized )
        return false;

    if ( x >= windowLeftTopPos.x() && 
         x <= windowLeftTopPos.x() + bordWidth && 
         y >= windowLeftTopPos.y() + winCornerRadius && 
         y <= windowRightBottomPos.y() - winCornerRadius ||
         x >= windowRightBottomPos.x() - bordWidth && 
         x <= windowRightBottomPos.x() && 
         y >= windowLeftTopPos.y() + winCornerRadius && 
         y <= windowRightBottomPos.y() - winCornerRadius )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inVerResizeRect ( int x, int y ) const
{
    if ( maximized )
        return false;

    if ( x >= windowLeftTopPos.x() + winCornerRadius && 
         x <= windowRightBottomPos.x() - winCornerRadius && 
         y >= windowLeftTopPos.y() && 
         y <= windowLeftTopPos.y() + bordWidth / 2 ||
         x >= windowLeftTopPos.x() + winCornerRadius && 
         x <= windowRightBottomPos.x() - winCornerRadius && 
         y >= windowRightBottomPos.y() - bordWidth && 
         y <= windowRightBottomPos.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inBDiagResizeRect ( int x, int y )
{
    if ( maximized )
        return false;

    if ( sqrt( (x - (windowRightBottomPos.x() - bordWidth)) * 
        (x - (windowRightBottomPos.x() - bordWidth)) +
        (y - (windowLeftTopPos.y() + bordWidth)) * 
        (y - (windowLeftTopPos.y() + bordWidth)) ) <= resEllRad + 2 )
    {
        if ( resEll != RightTop )
        {
            resEll = RightTop;
            rendered( false );
        }
        return true;
    }
    else if ( sqrt( (x - (windowLeftTopPos.x() + bordWidth)) * 
            (x - (windowLeftTopPos.x() + bordWidth)) + 
            (y - (windowRightBottomPos.y() - bordWidth)) * 
            (y - (windowRightBottomPos.y() - bordWidth)) ) <= resEllRad + 2 )
    {
        if ( resEll != LeftBottom )
        {
            resEll = LeftBottom;
            rendered( false );
        }
        return true;
    }
    else if ( resEll != None )
    {
        resEll = None;
        rendered( false );
    }
    return false;
}

bool TrussUnitWindow::inFDiagResizeRect ( int x, int y )
{
    if ( maximized )
        return false;

    if ( sqrt( (x - (windowLeftTopPos.x() + bordWidth)) * 
        (x - (windowLeftTopPos.x() + bordWidth)) +
        (y - (windowLeftTopPos.y() + bordWidth)) * 
        (y - (windowLeftTopPos.y() + bordWidth)) ) <= resEllRad + 2 )
    {
        if ( resEll != LeftTop )
        {
            resEll = LeftTop;
            rendered( false );
        }
        return true;
    }
    else if ( sqrt( (x - (windowRightBottomPos.x() - bordWidth)) * 
            (x - (windowRightBottomPos.x() - bordWidth)) +
            (y - (windowRightBottomPos.y() - bordWidth)) * 
            (y - (windowRightBottomPos.y() - bordWidth)) ) <= resEllRad + 2 )
    {
        if ( resEll != RightBottom )
        {
            resEll = RightBottom;
            rendered( false );
        }
        return true;
    }
    else if ( resEll != None )
    {
        resEll = None;
        rendered( false );
    }
    return false;
}

bool TrussUnitWindow::inHideButtonRect ( int x, int y ) const
{
    QPoint widgetPos( x, y );
    // inner buffer cordinates
    QPoint pos = widgetPos - getButtonBufPos();    
   if ( hideButton->inButtonRect( pos.x(), pos.y() ) )
       return true;
   return false;
}

bool TrussUnitWindow::inRollUpButtonRect ( int x, int y ) const
{
    QPoint widgetPos( x, y );
    // inner buffer cordinates
    QPoint pos = widgetPos - getButtonBufPos();    
   if ( rollUpButton->inButtonRect( pos.x(), pos.y() ) )
       return true;
   return false;
}

QPoint TrussUnitWindow::getButtonBufPos () const
{
    return QPoint( windowRightBottomPos.x() - 54, 
                   windowLeftTopPos.y() + 4 );
}

void TrussUnitWindow::checkMouseMoveEvent ( int x, int y, bool mousePressed )
{
    removeButtonsHighlight();

    if ( inHideButtonRect( x, y ) )
    {
        hideButton->setHighlighted( true );
        if( mousePressed )
            hideButton->setPressed( true );
    }
    else if ( inRollUpButtonRect( x, y ) )
    {
        rollUpButton->setHighlighted( true );
        if( mousePressed )
            rollUpButton->setPressed( true );
    }
}

void TrussUnitWindow::checkMousePressEvent ( int x, int y )
{
    releaseButtons();

    if ( inHideButtonRect( x, y ) )
        hideButton->setPressed( true );
    else if ( inRollUpButtonRect( x, y ) )
        rollUpButton->setPressed( true );
}

void TrussUnitWindow::checkMouseReleaseEvent ( int x, int y )
{
    if ( inHideButtonRect( x, y ) )
    {
        if ( hideButton->isPressed() )
        {
            hideButton->setPressed( false );
            hide();
        }
    }
    else if ( inRollUpButtonRect( x, y ) )
    {
        if ( rollUpButton->isPressed() )
        {
            rollUpButton->setPressed( false );
            if ( maximized )
                minimize ();
            else
                maximize ();
        }
    }
}

void TrussUnitWindow::setHighlighted ( bool h )
{
    if ( h ) {
        setBorderColor( 25,55,65 );
        setResEllColor( 120, 120, 120 );
        setHeadlineFirstColor(180, 130, 100 );
        setHeadlineMiddleColor( 230, 190, 170 );
        setHeadlineLastColor( 150, 90, 80 );
    } else {
        setBorderColor( 40,65,60 );
        setResEllColor( 50, 50, 50 );
        setHeadlineFirstColor( 180, 130, 150 );
        setHeadlineMiddleColor( 230, 210, 200 );
        setHeadlineLastColor( 150, 90, 110 );        
    }
    TrussUnit::setHighlighted(h);
    coordFieldRendered = false;
    buttonBufRendered = false;
}

void TrussUnitWindow::setResizeEllipseHighlighted ( selectedEllipse ellipseType )
{
    resEll = ellipseType;
    rendered( false );
}

void TrussUnitWindow::setFocusOnNode ( TrussNode* selectedNode )
{
    selectedNode->setHighlighted ( true );
    nodeToFront ( *selectedNode );
    rendered( false );
}

void TrussUnitWindow::setFocusOnPivot ( TrussPivot* selectedPivot )
{
    selectedPivot->setHighlighted ( true );
    nodeToFront ( selectedPivot->getFirstNode() );
    nodeToFront ( selectedPivot->getLastNode() );
    rendered( false );
}

void TrussUnitWindow::removeNodesHighlight ()
{
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {
        TrussNode* node = *iter;
        if ( node->isHighlighted () )
        {
            node->setHighlighted ( false );
            rendered( false );
        }
    }
}

void TrussUnitWindow::removePivotsHighlight ()
{
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        if ( pivot->isHighlighted () )
        {
            pivot->setHighlighted ( false );
            pivot->getFirstNode().setHighlighted ( false );
            pivot->getLastNode().setHighlighted ( false );
            rendered( false );
        }
    }
}

void TrussUnitWindow::removeButtonsHighlight ()
{
    hideButton->setHighlighted( false );
    rollUpButton->setHighlighted( false );
}

void TrussUnitWindow::releaseButtons ()
{
    hideButton->setPressed( false );
    rollUpButton->setPressed( false );
}

double TrussUnitWindow::getNodePrecision ( bool inPix ) const
{
    if ( inPix )
    // Return node finding precision in pixels
        return 4.0;

    double precision = ( 1.7 / getScaleMultiplierX() ) * 
                       ( 1.7 / getScaleMultiplierX() ) +
                       ( 1.7 / getScaleMultiplierY() ) *
                       ( 1.7 / getScaleMultiplierY() );
    // Returns node finding precision in truss coordinates
    // relative to current truss area size
    return sqrt( precision );
}

double TrussUnitWindow::getPivotPrecision () const
// Return pivot finding precision in truss coordinates
// relative to current truss area size
{
    double precision = ( 1.0 / getScaleMultiplierX() ) * 
                       ( 1.0 / getScaleMultiplierX() ) +
                       ( 1.0 / getScaleMultiplierY() ) *
                       ( 1.0 / getScaleMultiplierY() );
    return sqrt( precision );
}

TrussNode* TrussUnitWindow::findNodeByWidgetPos ( const QPoint& pos, 
                                                  double precision ) const
{
    QPoint nodePos;
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {   
        TrussNode* node = *iter;
        nodePos = getWidgetPosFromTrussCoord ( node->getPoint() );
        if ( abs ( nodePos.x() - pos.x() ) < precision && 
             abs ( nodePos.y() - pos.y() ) < precision )
        {
            return node;
        }
    }
    return 0;
}

TrussNode* TrussUnitWindow::findNodeByWidgetPos ( const QPoint& pos ) const
{
    return findNodeByWidgetPos ( pos, getNodePrecision() );
}

TrussNode* TrussUnitWindow::findNodeByWidgetPos ( int x, int y, 
                                                  double precision ) const
{
    return findNodeByWidgetPos ( QPoint( x, y ), precision );
}

TrussNode* TrussUnitWindow::findNodeByWidgetPos ( int x, int y ) const
{
    return findNodeByWidgetPos ( x, y, getNodePrecision() );
}

TrussPivot* TrussUnitWindow::findPivotByCoord ( const DoublePoint& coord, 
                                                double precision ) const
{
    DoublePoint firstNodeCoord, lastNodeCoord;
    PivotList pivotList = getPivotList ();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        firstNodeCoord = pivot->getFirstNode().getPoint();
        lastNodeCoord = pivot->getLastNode().getPoint();
        double dist = pointToSegmentDist ( coord, firstNodeCoord, 
                                                  lastNodeCoord );
        if ( dist < precision  && 
            ( firstNodeCoord.x() - coord.x() ) * 
            ( firstNodeCoord.x() - coord.x() ) + 
            ( firstNodeCoord.y() - coord.y() ) *
            ( firstNodeCoord.y() - coord.y() ) > dist * dist &&
            ( lastNodeCoord.x() - coord.x() ) * 
            ( lastNodeCoord.x() - coord.x() ) + 
            ( lastNodeCoord.y() - coord.y() ) *
            ( lastNodeCoord.y() - coord.y() ) > dist * dist )
            return pivot;
    }
    return 0;
}

TrussPivot* TrussUnitWindow::findPivotByCoord ( const DoublePoint& coord ) const
{
    return findPivotByCoord ( coord, getPivotPrecision() );
}

TrussPivot* TrussUnitWindow::findPivotByWidgetPos ( const QPoint& pos, 
                                                    double precision ) const
{
    DoublePoint coord = getTrussCoordFromWidgetPos ( pos );
    return findPivotByCoord ( coord, precision );
}

TrussPivot* TrussUnitWindow::findPivotByWidgetPos ( const QPoint& pos ) const
{
    return findPivotByWidgetPos ( pos, getPivotPrecision() );
}

TrussPivot* TrussUnitWindow::findPivotByWidgetPos ( int x, int y, 
                                                    double precision ) const
{
    return findPivotByWidgetPos ( QPoint( x, y ), precision );
}

TrussPivot* TrussUnitWindow::findPivotByWidgetPos ( int x, int y ) const
{
    return findPivotByWidgetPos ( x, y, getPivotPrecision() );
}

void TrussUnitWindow::moveTrussNode ( int x, int y, TrussNode* node )
{
    DoublePoint newCoord = getTrussCoordFromWidgetPos ( x, y );
    const DoubleSize& size = getTrussAreaSize();

    double areaWidth = size.width();
    double areaHeight = size.height();
    if ( newCoord.x() < 0 )
        newCoord.setX( 0 );
    if ( newCoord.x() > areaWidth )
        newCoord.setX( areaWidth );
    if ( newCoord.y() < 0 )
        newCoord.setY( 0 );
    if ( newCoord.y() > areaHeight )
        newCoord.setY( areaHeight );

    node->setPoint( newCoord.x(), newCoord.y() );
    rendered(false);
}

void TrussUnitWindow::moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                       QPoint firstNodeClickDist, 
                                       QPoint lastNodeClickDist )
{
    DoublePoint newCoord = getTrussCoordFromWidgetPos ( x, y );
    double newXFirst = newCoord.x()  + firstNodeClickDist.x();
    double newYFirst = newCoord.y() + firstNodeClickDist.y();
    double newXLast = newCoord.x() + lastNodeClickDist.x();
    double newYLast = newCoord.y() + lastNodeClickDist.y();
    
    const DoublePoint& oldFirstCoord = pivot->getFirstNode().getPoint();
    const DoublePoint& oldLastCoord  = pivot->getLastNode().getPoint();
    const DoubleSize& size = getTrussAreaSize();

    double oldXFirst = oldFirstCoord.x();
    double oldYFirst = oldFirstCoord.y();
    double oldXLast  = oldLastCoord.x();
    double oldYLast  = oldLastCoord.y();
    double areaWidth = size.width();
    double areaHeight = size.height();

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

    pivot->getFirstNode().setPoint( newXFirst, newYFirst );
    pivot->getLastNode().setPoint( newXLast, newYLast );
    rendered(false);
}

TrussNode* TrussUnitWindow::nodesMergingComparison ( TrussNode& comparableNode, 
                                                     double precision, 
                                                     bool fixationCheck )
{
    QPoint nodePos = getWidgetPosFromTrussCoord ( comparableNode.getPoint() );
    TrussNode* node = 0;
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {   
        TrussNode* n = *iter;
        QPoint pos = getWidgetPosFromTrussCoord ( n->getPoint() );
        if ( abs ( nodePos.x() - pos.x() ) < precision && 
             abs ( nodePos.y() - pos.y() ) < precision &&
             n != &comparableNode )
        {
            node = n;
        }
    }
    if ( node != 0 )
    {
        if ( fixationCheck )
        {
            if ( node->getFixation() == comparableNode.getFixation() )
            {
                return node;
            }
        }
        else
            return node;
    }
    rendered(false);
    return 0;
}

void TrussUnitWindow::mergeNodes ( TrussNode* mergingNode, TrussNode* node )
{
    ObjectStateManager* mng = mergingNode->getStateManager();
    mng->startStateBlock();

    TrussPivot* fakePivot = findPivotByNodes ( *mergingNode, *node );
    if ( fakePivot ) {
        // Save remove pivot action
        ObjectState& state = fakePivot->createState();
        state.addAction( new TrussPivotRemoveAction( *fakePivot ) );
        state.save();
        fakePivot->desist();
    }

    // Reduce search area: find only adjoining pivots
    PivotList pivotList = findAdjoiningPivots( *mergingNode );
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        TrussNode* firstNode = &pivot->getFirstNode();
        TrussNode* lastNode = &pivot->getLastNode();
        if ( firstNode == mergingNode )
        {
            if ( findPivotByNodes ( *node, *lastNode ) ) 
            // remove fake pivot
            {
                // Save remove pivot action
                ObjectState& state = pivot->createState();
                state.addAction( new TrussPivotRemoveAction( *pivot ) );
                state.save();
                pivot->desist();
            }
            else
            {
                // Save action of node changing
                ObjectState& state = pivot->createState();
                state.addAction( 
                    new ConcreteObjectAction<TrussPivot, TrussNode*>(
                                                        *pivot,
                                                        &TrussPivot::setFirstNode,
                                                        &TrussPivot::setFirstNode,
                                                        node,
                                                        firstNode ) );
                state.save();
                pivot->setFirstNode( node );
            }
        }
        else if ( lastNode == mergingNode )
        {
            if ( findPivotByNodes ( *node, *firstNode ) ) 
            // remove fake pivot
            {
                // Save remove pivot action
                ObjectState& state = pivot->createState();
                state.addAction( new TrussPivotRemoveAction( *pivot ) );
                state.save();
                pivot->desist();
            }
            else
            {
                // Save action of node changing
                ObjectState& state = pivot->createState();
                state.addAction( 
                    new ConcreteObjectAction<TrussPivot, TrussNode*>(
                                                        *pivot,
                                                        &TrussPivot::setLastNode,
                                                        &TrussPivot::setLastNode,
                                                        node,
                                                        lastNode ) );
                state.save();
                pivot->setLastNode ( node );
            }
        }
    }

    // Save remove node action
    ObjectState& state = mergingNode->createState();
    state.addAction( new TrussNodeRemoveAction( *mergingNode ) );
    state.save();

    mergingNode->desist();

    mng->endStateBlock();

    rendered(false);
}

void TrussUnitWindow::dividePivot ( TrussPivot& dividualPivot, 
                                    TrussNode& dividingNode )
{
    TrussNode& first = dividualPivot.getFirstNode();
    TrussNode& last = dividualPivot.getLastNode();

    ObjectStateManager* mng = dividualPivot.getStateManager();
    mng->startStateBlock();

    dividualPivot.setHighlighted ( false );

    // Save remove pivot action
    ObjectState& state = dividualPivot.createState();
    state.addAction( new TrussPivotRemoveAction( dividualPivot ) );
    state.save();
    dividualPivot.desist();

    TrussPivot* pivot1 = findPivotByNodes ( first, dividingNode );
    TrussPivot* pivot2 = findPivotByNodes ( last, dividingNode );
    if ( pivot1 )
    {
        // Save remove pivot action
        ObjectState& state = pivot1->createState();
        state.addAction( new TrussPivotRemoveAction( *pivot1 ) );
        state.save();
        pivot1->desist();
    }
    if ( pivot2 )
    {
        // Save remove pivot action
        ObjectState& state = pivot2->createState();
        state.addAction( new TrussPivotRemoveAction( *pivot2 ) );
        state.save();
        pivot2->desist();
    }
    TrussPivot* newlyCreated = 0;

    if ( first.getNumber() < dividingNode.getNumber() )
        newlyCreated = &createPivot( first, dividingNode );
    else
        newlyCreated = &createPivot( dividingNode, first );

    // Save create pivot action
    ObjectState& stateFirst = newlyCreated->createState();
    stateFirst.addAction( new TrussPivotCreateAction( *newlyCreated ) );
    stateFirst.save();

    if ( last.getNumber() < dividingNode.getNumber() )
        newlyCreated = &createPivot( last, dividingNode );
    else
        newlyCreated = &createPivot( dividingNode, last );

    // Save create pivot action
    ObjectState& stateSecond = newlyCreated->createState();
    stateSecond.addAction( new TrussPivotCreateAction( *newlyCreated ) );
    stateSecond.save();
    mng->endStateBlock();

    rendered(false);
}

TrussPivot* TrussUnitWindow::findDividualPivot ( TrussNode& dividingNode ) const
// finds pivot which is intersected by incoming node, but doesn't divided as yet
{
    TrussPivot* dividualPivot = findPivotByCoord ( dividingNode.getPoint(), 
                                                  2 * getPivotPrecision() );
    if ( ! dividualPivot )
        return 0;

    if ( &dividingNode == &dividualPivot->getFirstNode() || 
         &dividingNode == &dividualPivot->getLastNode() )
        return 0;

    return dividualPivot;
}


DoublePointArray TrussUnitWindow::getPivotCrossPoints ( 
                                     const PivotList& nonCrossingPivots ) const
{
    DoublePointArray crossPoints;

    if ( nonCrossingPivots.empty() )
        return crossPoints;

    // find points at which pivots from incoming list intersect with other pivots
    PivotList pivotList = getPivotList ();
    PivotList::reverse_iterator rev_iter = pivotList.rbegin();
    for ( ; rev_iter != pivotList.rend(); ++rev_iter )
    {
        TrussPivot* pivot = *rev_iter;
        const DoublePoint& p11 = pivot->getFirstNode().getPoint();
        const DoublePoint& p12 = pivot->getLastNode().getPoint();

        PivotListConstIter iter = nonCrossingPivots.begin();
        for ( ; iter != nonCrossingPivots.end(); ++iter )
        {   
            TrussPivot* adjPivot = *iter;
            if ( adjPivot == pivot )
                break;  // taken pivot is one of the noncrossing pivots

            const DoublePoint& p21 = adjPivot->getFirstNode().getPoint();
            const DoublePoint& p22 = adjPivot->getLastNode().getPoint();

            DoublePoint crossPoint = getLineSegmentsCrossPoint( p11, p12, 
                                                                p21, p22 );

            // we are interested in cross points between pivots nodes only
            if ( comparePoints( p11, crossPoint, getNodePrecision(false) ) ||
                 comparePoints( p12, crossPoint, getNodePrecision(false) ) ||
                 comparePoints( p21, crossPoint, getNodePrecision(false) ) ||
                 comparePoints( p22, crossPoint, getNodePrecision(false) ) )
                continue;

            if ( crossPoint.x() == -1 )
                continue;   // there are no intersections between these pivots

            if ( crossPoints.contains( crossPoint ) == 0 ) {
                crossPoints.append( crossPoint );
            }
        }
    }

    // find points at which pivots from incoming list intersect with nodes
    NodeList nodeList = getNodeList ();
    NodeListIter iter = nodeList.begin();
    for ( ; iter != nodeList.end(); ++iter )
    {
        TrussNode* node = *iter;
        TrussPivot* pivot = findDividualPivot ( *node );
        // check if node belongs to one of the incoming pivots
        if ( pivot && 
             std::find( nonCrossingPivots.begin(), 
                        nonCrossingPivots.end(), pivot ) != 
             nonCrossingPivots.end() )
        {
            if ( crossPoints.contains( node->getPoint() ) == 0 ) {
                crossPoints.append( node->getPoint() );
            }
        }
    }
    return crossPoints;
}

void TrussUnitWindow::createPivotCrossNodes ( const DoublePointArray& crossPoints )
{
    if ( crossPoints.isEmpty() )
        return;

    TrussNode *crossNode, *node;
    // create nodes at found points
    NodeList crossNodes;
    DoublePointArray::ConstIterator iter = crossPoints.begin();
    for ( ; iter != crossPoints.end(); ++iter )
    {
        crossNode = &createCrossNode ( *iter );
        if ( crossNode )
            crossNodes.push_back ( crossNode );
    }
    // check created nodes for merging
    NodeListIter nodeIter = crossNodes.begin();
    for ( ; nodeIter != crossNodes.end(); ++nodeIter )
    {
        crossNode = *nodeIter;
        // can not allow unfixed node to "eat" a fixed one
        if ( crossNode->getFixation() == Node::Unfixed )
            node = nodesMergingComparison( *crossNode, 
                                           int(1.5 * getNodePrecision()),
                                           false );
        else
            node = nodesMergingComparison( *crossNode, 
                                           int(1.5 * getNodePrecision()),
                                           true );
        if ( node )
            mergeNodes ( crossNode, node );
    }
}

TrussNode& TrussUnitWindow::createCrossNode ( const DoublePoint& crossPoint )
{
    TrussNode* crossNode = findNodeByCoord ( crossPoint, 0 );

    if ( !crossNode )
    {
        crossNode = &createNode ( crossPoint.x(), crossPoint.y() );
        // Save create node action
        ObjectState& state = crossNode->createState();
        TrussNodeCreateAction* action = 
            new TrussNodeCreateAction( *crossNode );
        state.addAction( action );
        state.save();
    }

    TrussPivot* pivot = 0;

    PivotList pivotList = getPivotList ();
    uint i;
    for ( i = 0; i < pivotList.size(); i++ )
    {
        pivot = findDividualPivot ( *crossNode );
        if ( pivot )
            dividePivot ( *pivot, *crossNode );
    }
    return *crossNode;
}

void TrussUnitWindow::updateNodePosition ( TrussNode* selectedNode, 
                                           bool fixationCheck )
// Updates new position of selected node: merge with other node and(or) 
// divide pivot if necessary.
{
    TrussNode* node = nodesMergingComparison( *selectedNode, 
                                              2 * getNodePrecision(), 
                                              fixationCheck );
    if ( node )
        mergeNodes ( selectedNode, node );
    else
    {
        TrussPivot* pivot = findDividualPivot ( *selectedNode );
        if ( pivot )
            dividePivot ( *pivot, *selectedNode );
    }    
}

void TrussUnitWindow::updateAfterNodeManipulation ( TrussNode* selectedNode, 
                                                    bool fixationCheck )
// Analyses positions of selected node and its adjoining pivots
// relative to other truss elements.
{
    int numb = selectedNode->getNumber();

    PivotList adjPivotList = findAdjoiningPivots ( *selectedNode );
    DoublePointArray crossPoints = getPivotCrossPoints ( adjPivotList );
    createPivotCrossNodes ( crossPoints );

    selectedNode = findNodeByNumber( numb );
    if ( !selectedNode )
        return;

    updateNodePosition ( selectedNode, fixationCheck );
}

void TrussUnitWindow::updateAfterPivotManipulation ( TrussPivot* selectedPivot,
                                                     bool fixationCheck )
// Analyses positions of selected pivot with adjoining pivots of its nodes
// relative to other truss elements.
{
    TrussNode& first = selectedPivot->getFirstNode();
    TrussNode& last = selectedPivot->getLastNode();
    DoublePointArray crossPoints;

    PivotList firstAdjPivots = findAdjoiningPivots ( first );
    PivotList lastAdjPivots = findAdjoiningPivots ( last );

    PivotListIter pivotIter = std::find( lastAdjPivots.begin(), 
                                         lastAdjPivots.end(), 
                                         selectedPivot );
    if ( pivotIter != lastAdjPivots.end() )
    {
        lastAdjPivots.erase ( pivotIter );
        crossPoints = getPivotCrossPoints ( lastAdjPivots );
        createPivotCrossNodes ( crossPoints );
    }

    crossPoints = getPivotCrossPoints ( firstAdjPivots );
    createPivotCrossNodes ( crossPoints );

    updateNodePosition ( &first, fixationCheck );
    updateNodePosition ( &last, fixationCheck );
}

color_type TrussUnitWindow::getCanvasColor () const
{
    return canvColor;
}

color_type TrussUnitWindow::getHeadlineFirstColor () const
{
    return headFirstColor;
}

color_type TrussUnitWindow::getHeadlineMiddleColor () const
{
    return headMiddleColor;
}

color_type TrussUnitWindow::getHeadlineLastColor () const
{
    return headLastColor;
}

color_type TrussUnitWindow::getBorderColor () const
{
    return borderColor;
}

void TrussUnitWindow::setCanvasColor ( int r, int g, int b )
{
    canvColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setHeadlineFirstColor ( int r, int g, int b )
{
    headFirstColor = agg::rgba8(r, g, b);
}

void TrussUnitWindow::setHeadlineMiddleColor ( int r, int g, int b )
{
    headMiddleColor = agg::rgba8(r, g, b);
}

void TrussUnitWindow::setHeadlineLastColor ( int r, int g, int b )
{
    headLastColor = agg::rgba8(r, g, b);
}

void TrussUnitWindow::setBorderColor ( int r, int g, int b )
{
    borderColor = agg::rgba(r, g, b);
}

void TrussUnitWindow::setResEllColor ( int r , int g, int b )
{
    resEllColor = agg::rgba(r, g, b);
}

QString TrussUnitWindow::fitTextToWindowSize ( QString str, int lengthLimit, 
                                               glyph_gen& glyph ) const
{
    int textLength; // text length in pixels
    uint i, totalChar = str.length();
    for ( i = 0; i < totalChar; i++ )
    {
        textLength = (int)glyph.width( str.ascii() ); 
        if ( textLength <= lengthLimit )
            break;
        else
        {
            str.replace (  str.length() - 3, 3, "..." );
            str.remove ( str.length() - 4, 1 );
        }
    }
    return str;
}

void TrussUnitWindow::drawTrussArea ( ren_dynarow& baseRend,
                                      scanline_rasterizer& ras, 
                                      textRenderer& textRend, 
                                      solidRenderer& solidRend, 
                                      agg::scanline_p8& sl ) const
{
//  draw coordinate lines with arrow heads
    QPoint leftTopAreaPos ( leftWindowIndent, topWindowIndent ),
           rightBottomAreaPos ( windowSize.width() - rigthWindowIndent, 
                                windowSize.height() - bottomWindowIndent );
    QPoint p1, p2;
    p2 = leftTopAreaPos;
    p1 = rightBottomAreaPos;
    baseRend.copy_bar ( p2.x(), p2.y(), p1.x(), p1.y(), agg::rgba(1,1,1) );
    p2.setY ( p2.y() - arrowHeadIndent );
    p1.setX ( p2.x() );
    p1.setY ( p1.y() + arrowTailIndent );
    drawArrow ( ras, solidRend, sl, p1, p2 );
    p1 = rightBottomAreaPos;
    p1.setX ( p2.x() - arrowTailIndent );
    p2 = rightBottomAreaPos;
    p2.setX (p2.x() + arrowHeadIndent );
    drawArrow ( ras, solidRend, sl, p1, p2 );

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX ( p1.x() - scalePieceLength );
    p1.setY ( p1.y() );
    p2.setY ( p1.y() );
    drawLine ( ras, solidRend, sl, p1, p2 );

    p1 = rightBottomAreaPos;
    p1.setY ( p1.y() + scalePieceLength );
    drawLine ( ras, solidRend, sl, p1, p2 );

//  draw scale strokes and figure it

    double areaLenInPix = rightBottomAreaPos.x() - leftTopAreaPos.x();
    double areaWidInPix = rightBottomAreaPos.y() - leftTopAreaPos.y();

    int strokeNumbX = int(areaLenInPix / 36);
    int strokeNumbY = int(areaWidInPix / 32);

    double scaleFactorXInPix = areaLenInPix / strokeNumbX;
    double scaleFactorYInPix = areaWidInPix / strokeNumbY;

    double scaleFactorXInAbs = getTrussAreaSize().height() / strokeNumbX;
    double scaleFactorYInAbs = getTrussAreaSize().width() / strokeNumbY;

    // left point of the scale stroke
    QPoint strokePnt1( int(leftTopAreaPos.x() - scalePieceLength),
                       int(leftTopAreaPos.y() + scaleFactorYInPix) );
    // right point of the scale stroke
    QPoint strokePnt2( leftTopAreaPos.x(),
                       strokePnt1.y() );
    QPoint textPos ( bordWidth + 3, strokePnt1.y() + 3 );
    QString str;
    int i;
    for (i = 1; i < strokeNumbY; i++ )
    {
        strokePnt1.setY( int(leftTopAreaPos.y() + i * scaleFactorYInPix) );
        strokePnt2.setY( strokePnt1.y() );
        drawLine ( ras, solidRend, sl, strokePnt1, strokePnt2 );
        textPos.setY( strokePnt1.y() + 3 );
        str = QString("%1").arg( getTrussAreaSize().height() - 
                                 i * scaleFactorYInAbs,0,'f',2 );   
        drawText ( textRend, str, agg::rgba(100, 100, 100), textPos ); 
    }

    // top point of the scale stroke
    strokePnt1.setX( int(leftTopAreaPos.x() + scaleFactorXInPix) );
    strokePnt1.setY( rightBottomAreaPos.y() );
    // bottom point of the scale stroke
    strokePnt2.setX( strokePnt1.x() );
    strokePnt2.setY( rightBottomAreaPos.y() + scalePieceLength );

    textPos.setY ( strokePnt2.y() + 10 );
    for (i = 1; i < strokeNumbX; i++ )
    {
        strokePnt1.setX( int(rightBottomAreaPos.x() - i * scaleFactorXInPix) );
        strokePnt2.setX( strokePnt1.x() );
        drawLine ( ras, solidRend, sl, strokePnt1, strokePnt2 );
        textPos.setX( strokePnt1.x() - 12 );
        str = QString("%1").arg( getTrussAreaSize().width() - 
                                 i * scaleFactorXInAbs,0,'f',2 );   
        drawText ( textRend, str, agg::rgba(100, 100, 100), textPos ); 
    }

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;

    p2.setX ( p1.x() - scalePieceLength );
    p1.setY ( int(p1.y() + scaleFactorYInPix) );
    p2.setY ( p1.y() );

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX ( bordWidth + 3 );
    p1.setY ( p1.y() + 3 );
    str = QString("%1").arg( getTrussAreaSize().height(),0,'f',2 );
    drawText  ( textRend, str, agg::rgba(100, 100, 100), p1 );
    p2.setX (p2.x() - 10 );
    p2.setY (p2.y() + 10 + scalePieceLength );
    str = QString("%1").arg( getTrussAreaSize().width(),0,'f',2 );
    drawText  ( textRend, str, agg::rgba(100, 100, 100), p2 );

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX ( p1.x() - scaleTextLeftBottomIndent );
    p1.setY ( p2.y() + scaleTextLeftBottomIndent );
    drawText  ( textRend, "0", agg::rgba(100, 100, 100), p1 );
    
}

void TrussUnitWindow::drawHeadline ( ren_dynarow& baseRend, 
                                     solidRenderer& solidRend, 
                                     scanline_rasterizer& ras,
                                     agg::scanline_p8& sl, 
                                     color_array_type& gradColors ) const
{
    QPoint p1, p2;
    p1.setX ( 3 * bordWidth );
    p1.setY ( bordWidth / 2 );
    p2.setX ( windowSize.width() - 3 * bordWidth );
    p2.setY ( p1.y() + headWidth );

    agg::rounded_rect headline ( p1.x(), p1.y(), p2.x(), p2.y(), 9 );

    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;
    agg::trans_affine mtx;
    interpolator inter ( mtx );
    mtx *= agg::trans_affine_translation( 0, -1 );
    linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, gradColors, 
                                           0, headWidth );
    linear_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path ( headline );
    agg::render_scanlines ( ras, sl, gradRend );

    agg::rounded_rect outline ( p1.x(), p1.y(), p2.x(), p2.y(), headWidth / 2 );
    solidRend.color ( agg::rgba( 30, 20, 10 ) );
    agg::conv_stroke<agg::rounded_rect> stroke( outline );
    stroke.width ( 1.2 );
    ras.add_path ( stroke );
    agg::render_scanlines(ras, sl, solidRend);
}

void TrussUnitWindow::drawCursorCoordinatesField ( ren_dynarow& baseRend,
                                                   textRenderer& textRend,
                                                   glyph_gen& glyph ) const
                                             
{
    if ( isHighlighted() )
        baseRend.clear ( agg::rgba( 25,55,65 ) );
    else
        baseRend.clear ( agg::rgba( 40,65,60 ) );
    
    QPoint textPos ( 0, 9 );
    color_type textColor = agg::rgba( 0, 0, 0 );

    if ( cursorCoord.x() == -1.0 )
    {
        textPos.setX ( 20 );
        drawText ( textRend, "--- : ---", textColor, textPos );
    }
    else
    {
        QString str;
        str = QString( "%1" ).arg( cursorCoord.x(),0,'f',2 );
        str.append(" : ");
        drawText ( textRend, str, textColor, textPos );
        int textLength = (int)glyph.width( str.ascii() ); 
        str = QString( "%1" ).arg( cursorCoord.y(),0,'f',2 );  
        textPos.setX ( textPos.x() + textLength );
        drawText ( textRend, str, textColor, textPos );
    }
}

void TrussUnitWindow::drawNumbersField ( ren_dynarow& baseRend,
                                         textRenderer& textRend ) const
                                             
{
    if ( isHighlighted() )
        baseRend.clear ( agg::rgba( 25,55,65 ) );
    else
        baseRend.clear ( agg::rgba( 40,65,60 ) );
    
    int nodeNumb = 0, pivotNumb = 0;
    PivotList pivotList = getPivotList();
    PivotListIter iter = pivotList.begin();
    for ( ; iter != pivotList.end(); ++iter )
    {
        TrussPivot* pivot = *iter;
        if ( pivot->isHighlighted() )
        {
            pivotNumb = pivot->getNumber();
            break;
        }
    }
    if ( ! pivotNumb )
    {
        NodeList nodeList = getNodeList();
        NodeListIter iter = nodeList.begin();
        for ( ; iter != nodeList.end(); ++iter )
        {
            TrussNode* node = *iter;
            if ( node->isHighlighted() )
            {
                nodeNumb = node->getNumber();
                break;
            }
        }
    }
    if ( ! nodeNumb && ! pivotNumb )
        return;

    QPoint textPos ( 0, 9 );
    color_type textColor = agg::rgba(0, 0, 0);

    if ( pivotNumb )
    {
        drawText ( textRend, "Pivot #", textColor, textPos );

        QString str;
        str = QString("%1").arg( pivotNumb );
        textPos.setX ( 41 );
        drawText ( textRend, str, textColor, textPos );
    }
    else if ( nodeNumb )
    {
        drawText ( textRend, "Node #", textColor, textPos );

        QString str;
        str = QString("%1").arg( nodeNumb );
        textPos.setX ( 41 );
        drawText ( textRend, str, textColor, textPos );
    }    
}

void TrussUnitWindow::drawNodeNumber( TrussNode* node, 
                                      ren_dynarow& baseRend, 
                                      solidRenderer& solidRend, 
                                      scanline_rasterizer& ras, 
                                      agg::scanline_p8& sl ) const
{
    if ( ! node->isVisible() )
        return;

    QPoint nodePos = getWidgetPosFromTrussCoord ( node->getPoint() );
    int rad;
    glyph_gen glyph( 0 );
    textFont numbFont;
    color_type textColor, backColor;
    QPoint textPos, backLeftTopPos, backRightBottomPos;

    if ( ! node->isHighlighted() )
    {
        backColor = agg::rgba(1, 1, 1, 0.7);
        numbFont = agg::gse5x9;
        textColor = agg::rgba(0, 100, 0);
        if ( node->getNumber() < 10 )
        {
            textPos.setX ( nodePos.x() + 1 - windowLeftTopPos.x() );
            textPos.setY ( nodePos.y() - 5 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 6 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        else
        {
            textPos.setX ( nodePos.x() - 1 - windowLeftTopPos.x() );
            textPos.setY ( nodePos.y() - 5 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 1 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 11 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        rad = 3;
    }
    else
    {
        backColor = agg::rgba(1, 1, 1, 0.8);
        numbFont = agg::gse6x12;
        textColor = agg::rgba(0, 100, 0);
        if ( node->getNumber() < 10 )
        {
            textPos.setX ( nodePos.x() + 1 - windowLeftTopPos.x() ) ;
            textPos.setY ( nodePos.y() - 6 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() - 1 );
            backRightBottomPos.setX ( textPos.x() + 7 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        else
        {
            textPos.setX ( nodePos.x() - 1 - windowLeftTopPos.x() ) ;
            textPos.setY ( nodePos.y() - 6 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 13 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        rad = 5;
    }

    agg::rounded_rect backRect ( backLeftTopPos.x(), backLeftTopPos.y(), 
                                 backRightBottomPos.x(), backRightBottomPos.y(), rad );
    ras.add_path ( backRect );
    solidRend.color ( backColor );
    agg::render_scanlines ( ras, sl, solidRend );
    glyph.font ( numbFont );
    textRenderer textRend ( baseRend, glyph );
    QString str;
    str = QString("%1").arg( node->getNumber() );
    drawText ( textRend, str, textColor, textPos );
}

void TrussUnitWindow::drawPivotNumber( TrussPivot* pivot, 
                                       ren_dynarow& baseRend, 
                                       solidRenderer& solidRend, 
                                       scanline_rasterizer& ras, 
                                       agg::scanline_p8& sl ) const
{
    if ( ! pivot->isVisible() || ! pivot->getDrawingStatus() )
        return;

    QPoint p1 = getWidgetPosFromTrussCoord ( pivot->getFirstNode().getPoint() );
    QPoint p2 = getWidgetPosFromTrussCoord ( pivot->getLastNode().getPoint() );

    int rad;
    glyph_gen glyph( 0 );
    textFont numbFont;
    color_type textColor, backColor;
    QPoint textPos, backLeftTopPos, backRightBottomPos;

    if ( ! pivot->isHighlighted() )
    {
        backColor = agg::rgba(1, 1, 1, 0.9);
        numbFont = agg::gse5x9;
        textColor = agg::rgba(30, 0, 0);
        if ( pivot->getNumber() < 10 )
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 2 - windowLeftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 4 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 6 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        else
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 4 - windowLeftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 4 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 1 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 11 );
            backRightBottomPos.setY ( textPos.y() - 9 );
        }
        rad = 3;
    }
    else
    {
        backColor = agg::rgba(1, 1, 1, 0.9);
        numbFont = agg::gse6x12;
        textColor = agg::rgba(30, 0, 0);
        if ( pivot->getNumber() < 10 )
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 3 - windowLeftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 5 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 3 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 7 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        else
        {
            textPos.setX ( abs( p1.x() + p2.x() ) / 2 - 5 - windowLeftTopPos.x() );
            textPos.setY ( abs( p1.y() + p2.y() ) / 2 + 5 - windowLeftTopPos.y() );
            backLeftTopPos.setX ( textPos.x() - 2 );
            backLeftTopPos.setY ( textPos.y() );
            backRightBottomPos.setX ( textPos.x() + 13 );
            backRightBottomPos.setY ( textPos.y() - 12 );
        }
        rad = 5;
    }

    agg::rounded_rect backRect ( backLeftTopPos.x(), backLeftTopPos.y(), 
                                 backRightBottomPos.x(), 
                                 backRightBottomPos.y(), rad );
    ras.add_path ( backRect );
    solidRend.color ( backColor );
    agg::render_scanlines ( ras, sl, solidRend );
    glyph.font ( numbFont );
    textRenderer textRend ( baseRend, glyph );
    QString str;
    str = QString("%1").arg( pivot->getNumber() );
    drawText ( textRend, str, textColor, textPos );
}

void TrussUnitWindow::drawTrussElementsNumbers ( ren_dynarow& baseRend, 
                                                 solidRenderer& solidRend, 
                                                 scanline_rasterizer& ras, 
                                                 agg::scanline_p8& sl ) const
{
    PivotList pivots = getPivotList ();
    PivotListIter pivotIter = pivots.begin ();
    for ( ; pivotIter != pivots.end(); ++pivotIter )
        drawPivotNumber ( *pivotIter, baseRend, solidRend, ras, sl );

    NodeList nodes = getNodeList ();
    NodeListIter nodeIter = nodes.begin ();
    for ( ; nodeIter != nodes.end(); ++nodeIter )
        drawNodeNumber ( *nodeIter, baseRend, solidRend, ras, sl );
}

void TrussUnitWindow::drawEllipseHighlight ( solidRenderer& solidRend, 
                                             scanline_rasterizer& ras, 
                                             agg::scanline_p8& sl,
                                             QPoint pos ) const
{                                         
    int rad = 4;
    agg::ellipse ell;
    solidRend.color ( agg::rgba( 1, 0, 0, 0.6 ) );
    ell.init ( pos.x(), pos.y(), rad, rad, 10 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );
    
    solidRend.color ( agg::rgba( 0, 1, 1, 0.8 ) );
    ell.init ( pos.x(), pos.y(), 3, 3, 10 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussUnitWindow::drawResizeEllipses ( solidRenderer& solidRend, 
                                           scanline_rasterizer& ras, 
                                           agg::scanline_p8& sl ) const
{
    QPoint leftTopPos ( bordWidth, bordWidth ),
           leftBottomPos ( bordWidth, windowSize.height() - bordWidth ),
           rightTopPos ( windowSize.width() - bordWidth, bordWidth ),
           rightBottomPos ( windowSize.width() - bordWidth, 
                            windowSize.height() - bordWidth );

    agg::ellipse ell;
    solidRend.color ( resEllColor );
    ell.init ( leftTopPos.x(), leftTopPos.y(), 
               resEllRad, resEllRad, 10 );
    ras.add_path ( ell );
    ell.init ( rightTopPos.x(), rightTopPos.y(), 
               resEllRad, resEllRad, 10 );
    ras.add_path ( ell );
    ell.init ( leftBottomPos.x(), leftBottomPos.y(), 
               resEllRad, resEllRad, 10 );
    ras.add_path ( ell );
    ell.init ( rightBottomPos.x(), rightBottomPos.y(), 
               resEllRad, resEllRad, 10 );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, solidRend );

    if ( resEll == LeftTop )
        drawEllipseHighlight ( solidRend, ras, sl, leftTopPos );
    else if ( resEll == LeftBottom )
        drawEllipseHighlight ( solidRend, ras, sl, leftBottomPos );
    else if ( resEll == RightTop )
        drawEllipseHighlight ( solidRend, ras, sl, rightTopPos );
    else if ( resEll == RightBottom )
        drawEllipseHighlight ( solidRend, ras, sl, rightBottomPos );
}

void TrussUnitWindow::drawCalcIndicator ( ren_dynarow& baseRend, 
                                          solidRenderer& solidRend,
                                          scanline_rasterizer& ras, 
                                          agg::scanline_p8& sl,
                                          color_array_type& gradColors,
                                          const QPoint& pos ) const
{
    int rad =  headWidth/4 + 2;
    agg::ellipse ell;

    ell.init ( pos.x(), pos.y(), rad + 1, rad + 1, 100 );
    solidRend.color ( agg::rgba( 0, 0, 0, 0.4 ) );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, solidRend );

    agg::trans_affine mtx;

    linear_gradient gradFunc;
    color_type firstColor ( agg::rgba( 0, 0, 0 ) );
    color_type middleColor ( agg::rgba( 1, 15, 25 ) );
    if ( ! isHighlighted() )
        middleColor = agg::rgba( 1, 1, 1 );
    color_type lastColor ( agg::rgba( 0, 0, 0 ) );
    fillColorArray ( gradColors, firstColor, middleColor, lastColor );
    drawGradientEllipse( baseRend, ras, sl, gradFunc, gradColors, mtx, 
                         pos.x(), pos.y(), rad, -6, headWidth + 8 );

    radial_gradient gradRadFunc;
    mtx *= agg::trans_affine_translation( pos.x(), pos.y() );
    mtx.invert ();
    firstColor = agg::rgba8( 155, 155, 255 );
    middleColor = agg::rgba8( 80, 100, 195 );
    lastColor = agg::rgba8( 10, 10, 120 );
    fillColorArray ( gradColors, firstColor, middleColor, lastColor );

    drawGradientEllipse( baseRend, ras, sl, gradRadFunc, gradColors, mtx, 
                         pos.x(), pos.y(), rad - 2, 0, rad - 2 );
}

void TrussUnitWindow::paint ( base_renderer& baseRenderer ) const
{
    if ( !isVisible() )
        return;

    glyph_gen glyph( 0 );
    scanline_rasterizer ras;    
    agg::scanline_p8 sl;

    bool numbersDrawing = true;

    pixf_dynarow windowPixf( *windowBuf ),
                 numbersPixf( *numbersBuf ),
                 coordPixf( *coordBuf ),
                 buttonPixf( *buttonBuf ); 
                 

    if ( ! isRendered() ) 
    { 
        ren_dynarow baseRend( windowPixf );
        baseRend.clear ( agg::rgba (10, 10, 10, 0) );
        solidRenderer solidRend ( baseRend );    
        textRenderer textRend ( baseRend, glyph );


        /*------draw resize ellipses------*/
        if ( ! maximized )
            drawResizeEllipses( solidRend, ras, sl );


        /*------draw window border------*/
        if ( maximized )
            baseRend.copy_bar( 0, 0, windowSize.width(), 
                                     windowSize.height(), borderColor );
        else
        {
            QPoint borderLeftTop( 0, 0 );
            QPoint borderRightBottom( windowSize.width(), 
                                   windowSize.height() );

            agg::rounded_rect border ( borderLeftTop.x(), 
                                       borderLeftTop.y(), 
                                       borderRightBottom.x(), 
                                       borderRightBottom.y(), 
                                       winCornerRadius );
            ras.add_path ( border );
            solidRend.color ( borderColor );
            agg::render_scanlines ( ras, sl, solidRend );
        }


        /*------draw window canvas------*/
        QPoint canvasLeftTop( bordWidth, bordWidth + headWidth );
        QPoint canvasRightBottom( windowSize.width() - bordWidth, 
                                  windowSize.height() - coordBuf->height() - 4 );

        agg::rounded_rect canvas ( canvasLeftTop.x(), 
                                   canvasLeftTop.y(), 
                                   canvasRightBottom.x(), 
                                   canvasRightBottom.y(), 
                                   winCornerRadius / 3 );
        ras.add_path ( canvas );
        solidRend.color ( canvColor );
        agg::render_scanlines ( ras, sl, solidRend );


        /*------draw window headline------*/
        color_array_type gradColors;
        fillColorArray ( gradColors, headFirstColor, 
                         headMiddleColor, headLastColor );
        drawHeadline ( baseRend, solidRend, ras, sl, gradColors );


        /*------draw simple calculation indicator------*/
        QPoint centerPos( 22, bordWidth/2 + headWidth/2 );
        drawCalcIndicator( baseRend, solidRend, ras, sl, 
                           gradColors, centerPos );


        /*------draw window title text and background rounded rectangle------*/
        glyph.font ( headFont );
        int lengthLimit = windowSize.width() / 2 - 20;
        QString title = fitTextToWindowSize ( getTrussName (), lengthLimit, glyph );
        int titleLength = (int)glyph.width( title.ascii() );
        QPoint titlePos( ( windowSize.width() - 2 * bordWidth - 
                           titleLength ) / 2, 14 );

        QPoint rectPos1( titlePos.x() - 15, bordWidth / 2 ),
               rectPos2( titlePos.x() + titleLength + 15, rectPos1.y() + headWidth -1 );

        color_type firstColor ( agg::rgba( 0, 0, 0 ) );
        color_type middleColor ( agg::rgba8( 180, 130, 100 ) );
        if ( ! isHighlighted() )
            middleColor = agg::rgba( 100, 120, 120 );
        color_type lastColor ( agg::rgba( 0, 0, 0 ) );

        fillColorArray ( gradColors, firstColor, middleColor, lastColor );
        agg::trans_affine mtx;
        drawOutlineRoundedRect( baseRend, solidRend, ras, sl, gradColors, 
                                mtx, rectPos1, rectPos2, agg::rgba( 1, 1, 1, 0.6 ),
                                8, 1, 0, headWidth * 2 );

        color_type titleColor = agg::rgba(1, 1, 1);
        drawText ( textRend, title, titleColor, titlePos );


        /*------draw editable area in which canvas truss unit can be painted------*/
        glyph.font ( agg::mcs11_prop_condensed );
        drawTrussArea ( baseRend, ras, textRend, solidRend, sl );


        /*------draw truss unit------*/
        TrussUnit::paint ( baseRend, 
                           getScaleMultiplierX (), 
                           getScaleMultiplierY () );


        /*------draw truss elements numbers above them------*/
        if ( numbersDrawing )
            drawTrussElementsNumbers ( baseRend, solidRend, ras, sl );

        rendered( true );
    }

    if ( ! coordFieldRendered )
    {
        /*------draw coordinates field------*/
        ren_dynarow baseRend ( coordPixf );
        textRenderer textRend ( baseRend, glyph );
        glyph.font ( numbersFont );
        drawCursorCoordinatesField ( baseRend, textRend, glyph );
        coordFieldRendered = true;
    }


    /*------draw truss elements numbers field------*/
    ren_dynarow baseRend = numbersPixf;
    textRenderer textRend ( baseRend, glyph );
    glyph.font ( numbersFont );
    drawNumbersField ( baseRend, textRend );

    if ( ! buttonBufRendered )
    {
        /*------draw buttons------*/
        baseRend = buttonPixf;
        baseRend.clear ( agg::rgba (10, 10, 10, 0) );
        solidRenderer solidRend ( baseRend );   
        hideButton->paint ( baseRend, ras, sl, solidRend );
        rollUpButton->paint ( baseRend, ras, sl, solidRend );
        buttonBufRendered = true;
    }


    /*------blend buffers------*/
    baseRenderer.blend_from ( windowPixf, 0, windowLeftTopPos.x(), 
                              windowLeftTopPos.y(), unsigned(1.0 * 255) );

    baseRenderer.blend_from ( coordPixf, 0, windowLeftTopPos.x() + winCornerRadius, 
                              windowRightBottomPos.y() - coordBuf->height() - 3, 
                              unsigned(1.0 * 255) );

    baseRenderer.blend_from ( numbersPixf, 0, windowRightBottomPos.x() - 80, 
                              windowRightBottomPos.y() - coordBuf->height() - 3, 
                              unsigned(1.0 * 255) );

    baseRenderer.blend_from ( buttonPixf, 0, getButtonBufPos().x(), 
                              getButtonBufPos().y(), unsigned(1.0 * 255) );
}

/****************************************************************************/
