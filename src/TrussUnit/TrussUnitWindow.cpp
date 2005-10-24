
#include "TrussUnitWindow.h"
#include "TrussUnitActions.h"
#include "qtimer.h"

/*****************************************************************************
 * Truss Unit Window
 *****************************************************************************/

/* 
   Constructs a new truss unit window with the title name.
*/
TrussUnitWindow::TrussUnitWindow ( const QString& name, 
                                   ObjectStateManager* mng ) :
    TrussUnit( name, mng ),
    windowSize( defaultWidth, defaultHeight ),
    coordFieldSize( 49, 12 ),
    cursorCoord( -1.0, -1.0 ),
    buttonBufRendered(false),
    coordFieldRendered(false),
    headFont( agg::verdana14_bold ),
    numbersFont( agg::verdana13 ),
    windowBuf( new rbuf_dynarow( defaultWidth, defaultHeight ) ),
    trussBuf( new rbuf_dynarow( defaultBufWidth, defaultBufHeight ) ),
    coordBuf( new rbuf_dynarow( 100, 10 ) ),
    numbersBuf( new rbuf_dynarow( 60, 10 ) ),
    buttonBuf( new rbuf_dynarow( 36, 16 ) ),
    hideButton( new TrussUnitWindowButton( QPoint( 15, 1 ),
                imagesSvgPath() + "/closeIcon.svg" ) ),
    rollUpButton( new TrussUnitWindowButton( QPoint( 1, 1 ),
                  imagesSvgPath() + "/rollUpIcon.svg" ) ),
    currentPressedButton( 0 ),
    currentHintedButton( 0 ),
    timer( new QTimer( this ) ),
    maximized(false)
{
    const QPoint& pixAreaSize = getTrussAreaRightBottomPos() -
                                getTrussAreaLeftTopPos();
    setTrussAreaSizeInPix( QSize(pixAreaSize.x(),pixAreaSize.y()) );

    hideButton->setHint( "Hide truss unit" );
    rollUpButton->setHint( "Maximize truss unit" );

    QObject::connect( hideButton, SIGNAL( onChangeButtonState() ),
                                  SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( rollUpButton, SIGNAL( onChangeButtonState() ),
                                    SLOT( clearButtonBufRenderedFlag() ) );

    QObject::connect( this, SIGNAL( onHighlightChange( bool ) ),
                      hideButton, SLOT( setButtonHighlightType( bool ) ) );

    QObject::connect( this, SIGNAL( onHighlightChange( bool ) ),
                      rollUpButton, SLOT( setButtonHighlightType( bool ) ) );

    QObject::connect( timer, SIGNAL( timeout() ),
                             SLOT( setWindowButtonHinted() ) );

    setCanvasColor( 8, 10, 12 );
    setHighlighted( false );    
}

/* 
   Destroys truss unit window. 
*/
TrussUnitWindow::~TrussUnitWindow ()
{
    delete windowBuf;
    delete trussBuf;
    delete coordBuf;
    delete numbersBuf;
    delete buttonBuf;
    delete hideButton;
    delete rollUpButton;
    delete timer;
}

/* 
   Sets rendered flag of the button buffer to FALSE, so it 
   will be repainted, when the paint method will be called.
*/
void TrussUnitWindow::clearButtonBufRenderedFlag ()
{
    buttonBufRendered = false;
}

/* 
   Returns left top position of the truss unit window 
   relative to main agg rendering buffer. 
*/
QPoint TrussUnitWindow::getWindowLeftTopPos () const
{
    return windowLeftTopPos;
}

/* 
   Returns right bottom position of the truss unit window 
   relative to main agg rendering buffer. 
*/
QPoint TrussUnitWindow::getWindowRightBottomPos () const
{
    return windowRightBottomPos;
}

/* 
   Returns left top position of the editable truss area.
   Truss units are painted within it. 
*/
QPoint TrussUnitWindow::getTrussAreaLeftTopPos () const
{
    QPoint point;
    point.setX ( windowLeftTopPos.x() + leftWindowIndent );
    point.setY ( windowLeftTopPos.y() + topWindowIndent );
    return point;
}

/* 
   Returns right bottom position of the editable truss area.
   Truss units are painted within it. 
*/
QPoint TrussUnitWindow::getTrussAreaRightBottomPos () const
{
    QPoint point;
    point.setX ( windowLeftTopPos.x() + windowSize.width() - rigthWindowIndent );
    point.setY ( windowLeftTopPos.y() + windowSize.height() - bottomWindowIndent );
    return point;
}

/* 
   Sets window left top position relative to main agg 
   rendering buffer .
*/
void TrussUnitWindow::setWindowPosition ( QPoint pos )
{
    QPoint oldPos = windowLeftTopPos;
    windowLeftTopPos = pos;
    windowRightBottomPos.setX ( windowLeftTopPos.x() + windowSize.width() );
    windowRightBottomPos.setY ( windowLeftTopPos.y() + windowSize.height() );
    emit onMove( oldPos, pos );
    setTrussPosition( getTrussAreaLeftTopPos() );
}

/* 
   Returns button buffer left top position relative to main agg 
   rendering buffer.
*/
QPoint TrussUnitWindow::getButtonBufPos () const
{
    return QPoint( windowRightBottomPos.x() - 46, 
                   windowLeftTopPos.y() + 4 );
}

/* 
   Returns current size of the truss unit window.
*/
const QSize& TrussUnitWindow::getWindowSize () const
{
    return windowSize;
}

/* 
   Sets a new size of the truss unit window.
*/
void TrussUnitWindow::setWindowSize ( int width, int height )
{
    QSize oldSize = windowSize;
    windowSize.setWidth ( width );
    windowSize.setHeight ( height );
    rendered(false);
    emit onResize( oldSize, windowSize );
}

/* 
   Resizes truss unit window. New size is described by 
   leftTop and rightBottom points.
*/
void TrussUnitWindow::resize ( QPoint leftTop, QPoint rightBottom )
{
    int dx = rightBottom.x() - leftTop.x();
    int dy = rightBottom.y() - leftTop.y();
    windowBuf->init( dx, dy );
    setWindowSize( dx, dy );

    const QPoint& pixAreaSize = getTrussAreaRightBottomPos() -
                                getTrussAreaLeftTopPos();
    trussBuf->init( pixAreaSize.x() + 2 * trussBufIndent, 
                    pixAreaSize.y() + 2 * trussBufIndent );

    setTrussAreaSizeInPix( QSize(pixAreaSize.x(),pixAreaSize.y()) );

    setWindowPosition( leftTop );
}

/* 
   Check if window is maximized. Returns TRUE if it so.
*/
bool TrussUnitWindow::isMaximized () const
{
    return maximized;
}

/*
    Sets size of the maximized window depending on current
    Designer Widget geometry size.
*/
void TrussUnitWindow::setMaxSize ( int x, int y )
{
    maxSize = QSize( x - bordWidth, y - bordWidth );
}

/*
    Maximizes truss unit window. Saves its current size and
    resizes it to maxSize.
*/
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

    rollUpButton->setHint( "Minimize truss unit" );
    maximized = true;
}

/*
    Minimizes truss unit window. Sets its size to saved 
    minLeftTopPos and minRightBottomPos.
*/
void TrussUnitWindow::minimize ()
{
    removeButtonsHighlight();
    resize ( minLeftTopPos, minRightBottomPos );

    rollUpButton->setHint( "Maximize truss unit" );
    maximized = false;
}

/*
    Hides truss unit window.
*/
void TrussUnitWindow::hide ()
{
    setVisible( false );
}

/*
    Returns last saved truss coordinates of the mouse cursor.
*/
DoublePoint TrussUnitWindow::getCursorCoord () const
{
    return cursorCoord;
}

/*
    Sets new truss coordinates of the mouse cursor 
    above the truss area.
*/
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

/* 
    Sets new truss coordinates of the mouse cursor above 
    the truss area. Assuming this coords are from truss 
    node position, so we don't want to convert them from 
    widget position.
*/
void TrussUnitWindow::setCursorCoord ( const DoublePoint& p )
{
    if ( cursorCoord == p )
        return;
    cursorCoord = p;
    coordFieldRendered = false;
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
         y <= windowLeftTopPos.y() + headWidth + bordWidth / 2 &&
         ! inButtonsRect( x, y ) )
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
        (y - (windowLeftTopPos.y() + bordWidth)) ) <= resEllRad )
    {
        return true;
    }
    else if ( sqrt( (x - (windowLeftTopPos.x() + bordWidth)) * 
            (x - (windowLeftTopPos.x() + bordWidth)) + 
            (y - (windowRightBottomPos.y() - bordWidth)) * 
            (y - (windowRightBottomPos.y() - bordWidth)) ) <= resEllRad )
    {
        return true;
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
        (y - (windowLeftTopPos.y() + bordWidth)) ) <= resEllRad )
    {
        return true;
    }
    else if ( sqrt( (x - (windowRightBottomPos.x() - bordWidth)) * 
            (x - (windowRightBottomPos.x() - bordWidth)) +
            (y - (windowRightBottomPos.y() - bordWidth)) * 
            (y - (windowRightBottomPos.y() - bordWidth)) ) <= resEllRad )
    {
        return true;
    }
    return false;
}

void TrussUnitWindow::setWindowButtonHinted ()
{
    hinted = true;
    if( currentHintedButton )
        emit onHintShowsUp( currentHintedButton->getHint(), 
                            hintCurrentPos, false );
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

bool TrussUnitWindow::inButtonsRect ( int x, int y ) const
{
    QPoint rectLeftTopPnt = getButtonBufPos();
    QPoint rectRightBottomPnt( rectLeftTopPnt.x() + 2 + 
                               rollUpButton->getWidth() +
                               hideButton->getWidth(),
                               rectLeftTopPnt.y() + 1 +
                               rollUpButton->getHeight() ); 

    if ( x > getButtonBufPos().x() && x < rectRightBottomPnt.x() &&
         y > getButtonBufPos().y() && y < rectRightBottomPnt.y() )
         return true;
    return false;
}

void TrussUnitWindow::checkMouseMoveEvent ( int x, int y, bool mousePressed )
{
    if ( timer->isActive() )
        timer->stop();

    removeButtonsHighlight();

    if ( inHideButtonRect( x, y ) )
    {
        hideButton->setHighlighted( true );
        if( mousePressed && hideButton == currentPressedButton )
        {
            hideButton->setPressed( true );
        }
        else if ( hinted )
        {
            if ( hideButton != currentHintedButton )
            {
                currentHintedButton = hideButton;
                hintCurrentPos = QPoint( x, y );
                emit onHintShowsUp( currentHintedButton->getHint(), 
                                    hintCurrentPos, false );
            }
        }
        else
        {
            currentHintedButton = hideButton;
            hintCurrentPos = QPoint( x, y );
            timer->start( 1000, false );
        }
    }
    else if ( inRollUpButtonRect( x, y ) )
    {
        rollUpButton->setHighlighted( true );
        if( mousePressed && currentPressedButton == rollUpButton )
        {
            rollUpButton->setPressed( true );
        }
        else if ( hinted )
        {
            if ( rollUpButton != currentHintedButton )
            {
                currentHintedButton = rollUpButton;
                hintCurrentPos = QPoint( x, y );
                emit onHintShowsUp( currentHintedButton->getHint(), 
                                    hintCurrentPos, false );
            }
        }
        else
        {
            currentHintedButton = rollUpButton;
            hintCurrentPos = QPoint( x, y );
            timer->start( 1000, false );
        }
    }
    else if ( ! inButtonsRect( x, y ) )
    {
        if ( hinted )
        {
            hinted = false;
            currentHintedButton = 0;
            emit onHintHides( false );
        }
    }
}

void TrussUnitWindow::checkMousePressEvent ( int x, int y )
{
    releaseButtons();

    if ( hinted )
    {
        hinted = false;
        currentHintedButton = 0;
        emit onHintHides( false );
    }

    if ( inHideButtonRect( x, y ) )
    {
        hideButton->setPressed( true );
        currentPressedButton = hideButton;
    }
    else if ( inRollUpButtonRect( x, y ) )
    {
        rollUpButton->setPressed( true );
        currentPressedButton = rollUpButton;
    }
}

void TrussUnitWindow::checkMouseReleaseEvent ( int x, int y )
{
    if ( inHideButtonRect( x, y ) )
    {
        if ( hideButton->isPressed() )
        {
            hideButton->setPressed( false );
            currentPressedButton = 0;
            hide();
        }
    }
    else if ( inRollUpButtonRect( x, y ) )
    {
        if ( rollUpButton->isPressed() )
        {
            rollUpButton->setPressed( false );
            currentPressedButton = 0;
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
// Draw coordinate lines with arrow heads
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

// Draw scale strokes and figure it
    double areaLenInPix = rightBottomAreaPos.x() - leftTopAreaPos.x();
    double areaWidInPix = rightBottomAreaPos.y() - leftTopAreaPos.y();

    int strokeNumbX = int(areaLenInPix / 36);
    int strokeNumbY = int(areaWidInPix / 32);

    double scaleFactorXInPix = areaLenInPix / strokeNumbX;
    double scaleFactorYInPix = areaWidInPix / strokeNumbY;

    double scaleFactorXInAbs = getTrussAreaSize().width() / strokeNumbX;
    double scaleFactorYInAbs = getTrussAreaSize().height() / strokeNumbY;

    // sign Y-axis
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

    // sign X-axis
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

    // draw area boundary values
    strokePnt1.setX ( bordWidth + 3 );
    strokePnt1.setY ( leftTopAreaPos.y() + 3 );
    str = QString("%1").arg( getTrussAreaSize().height(),0,'f',2 );
    drawText  ( textRend, str, agg::rgba(100, 100, 100), strokePnt1 );

    strokePnt2.setX (rightBottomAreaPos.x() - 10 );
    strokePnt2.setY (rightBottomAreaPos.y() + 10 + scalePieceLength );
    str = QString("%1").arg( getTrussAreaSize().width(),0,'f',2 );
    drawText  ( textRend, str, agg::rgba(100, 100, 100), strokePnt2 );

    strokePnt1.setX ( leftTopAreaPos.x() - scaleTextLeftBottomIndent );
    strokePnt1.setY ( rightBottomAreaPos.y() + scaleTextLeftBottomIndent );
    drawText  ( textRend, "0", agg::rgba(100, 100, 100), strokePnt1 );
    
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

    // draw edging
    linear_gradient gradFunc;
    color_type firstColor ( agg::rgba( 0, 0, 0 ) );
    color_type middleColor ( agg::rgba( 1, 15, 25 ) );
    if ( ! isHighlighted() )
        middleColor = agg::rgba( 1, 1, 1 );
    color_type lastColor ( agg::rgba( 0, 0, 0 ) );
    fillColorArray ( gradColors, firstColor, middleColor, lastColor );
    drawGradientEllipse( baseRend, ras, sl, gradFunc, gradColors, mtx, 
                         pos.x(), pos.y(), rad, -6, headWidth + 8 );

    // draw indicator
    radial_gradient gradRadFunc;
    mtx *= agg::trans_affine_translation( pos.x(), pos.y() );
    mtx.invert ();
    if ( ! isCalculated() )
    {
        firstColor = agg::rgba8( 155, 155, 255 );
        middleColor = agg::rgba8( 80, 100, 195 );
        lastColor = agg::rgba8( 10, 10, 120 );
    }
    else
    {
        firstColor = agg::rgba8( 55, 255, 55 );
        middleColor = agg::rgba8( 95, 170, 85 );
        lastColor = agg::rgba8( 10, 100, 10 );
    }
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

    pixf_dynarow windowPixf( *windowBuf ),
                 trussPixf( *trussBuf ),
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
               rectPos2( titlePos.x() + titleLength + 15, 
                         rectPos1.y() + headWidth -1 );

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

        rendered( true );
    }

    /*------draw truss unit------*/
    ren_dynarow baseRend( trussPixf );
    TrussUnit::paint ( baseRend );

    /*------draw truss elements numbers field------*/
    baseRend = numbersPixf;
    textRenderer textRend ( baseRend, glyph );
    glyph.font ( numbersFont );
    drawNumbersField ( baseRend, textRend );

    if ( ! coordFieldRendered )
    {
        /*------draw coordinates field------*/
        baseRend = coordPixf;
        textRenderer textRend ( baseRend, glyph );
        glyph.font ( numbersFont );
        drawCursorCoordinatesField ( baseRend, textRend, glyph );
        coordFieldRendered = true;
    }

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

    const QPoint& areaLeftTop = getTrussAreaLeftTopPos();

    baseRenderer.blend_from ( trussPixf, 0, areaLeftTop.x() - trussBufIndent,
                              areaLeftTop.y() - trussBufIndent, 
                              unsigned(1.0 * 255) );

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
