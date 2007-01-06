
#include <QTimer>
#include <QWidget>

#include "TrussUnitWindow.h"
#include "TrussUnitActions.h"
#include "AggTrussWindowButton.h"

/*****************************************************************************
 * Truss Unit Window
 *****************************************************************************/

/* 
   Constructs a new truss unit window with the title name.
*/
TrussUnitWindow::TrussUnitWindow ( const QString& name, 
                                   ObjectStateManager* mng,
                                   const TrussMaterialLibrary& mLib ) :
    TrussUnit( name, mng, mLib ),
    windowOwner(0),
    positionIsSet( false ),
    windowSize( Global::defaultWidth, Global::defaultHeight ),
    cursorCoord( -1.0, -1.0 ),
    buttonBufRendered(false),
    coordFieldRendered(false),
    headFont( agg::verdana14_bold ),
    numbersFont( agg::verdana13 ),
    windowBuf( new rbuf_dynarow( Global::defaultWidth, 
                                 Global::defaultHeight ) ),
    trussBuf( new rbuf_dynarow( Global::defaultBufWidth, 
                                Global::defaultBufHeight ) ),
    coordBuf( new rbuf_dynarow( 100, 10 ) ),
    numbersBuf( new rbuf_dynarow( 60, 10 ) ),
    buttonBuf( new rbuf_dynarow( 0, 0 ) ),
    currentPressedButton( 0 ),
    currentHintedButton( 0 ),
    timer( new QTimer( this ) ),
    maximized( false ), 
    hinted( false )
{
    const QPoint& pixAreaSize = getTrussAreaRightBottomPos() -
                                getTrussAreaLeftTopPos();

    setTrussAreaSizeInPix( QSize(pixAreaSize.x(),pixAreaSize.y()) );
    
    connect( this, SIGNAL( onAreaChange(const DoubleSize&) ),
                   SLOT( clearWindowRenderedFlag() ) );

    connect( this, SIGNAL( onCalculationStatusChange(bool) ),
                   SLOT( clearWindowRenderedFlag() ) );

    // Clear rendered flag of the parent truss unit. Used for force updating 
    // truss units from the outside
    connect( this, SIGNAL( onForceRedrawTrussUnit() ),
                   SLOT( clearRenderedFlag() ) );

    initWindowButtons();

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

    foreach ( AggTrussWindowButton* b, windowButtons )
        delete b;
    windowButtons.clear();
    
    delete timer;
}

void TrussUnitWindow::initWindowButtons ()
{
    AggTrussWindowButton* hideButton = 
                new AggTrussWindowButton( QPoint( 30, 1 ),
                    Global::imagesSvgPath() + "/closeIcon.svg",
                    WindowButtonType::HideButton );

    AggTrussWindowButton* rollUpButton = 
                  new AggTrussWindowButton( QPoint( 15, 1 ),
                      Global::imagesSvgPath() + "/rollUpIcon.svg",
                      WindowButtonType::RollUpButton );

    AggTrussWindowButton* propSizeButton = 
                    new AggTrussWindowButton( QPoint( 1, 1 ),
                        Global::imagesSvgPath() + "/rollUpIcon.svg",
                        WindowButtonType::PropSizeButton );   

    windowButtons.append( hideButton );
    windowButtons.append( rollUpButton );
    windowButtons.append( propSizeButton );

    buttonBuf->init( 48, 16 );

    hideButton->setHint( tr( "Hide truss unit" ) );
    rollUpButton->setHint( tr( "Maximize truss unit" ) );
    propSizeButton->setHint( tr( "Proportional size" ) );

    connect( this, SIGNAL( onAreaChange(const DoubleSize&) ),
                   SLOT( clearWindowRenderedFlag() ) );

    connect( this, SIGNAL( onCalculationStatusChange(bool) ),
                   SLOT( clearWindowRenderedFlag() ) );

    connect( hideButton, SIGNAL( onChangeButtonState() ),
                         SLOT( clearButtonBufRenderedFlag() ) );

    connect( rollUpButton, SIGNAL( onChangeButtonState() ),
                           SLOT( clearButtonBufRenderedFlag() ) );

    connect( propSizeButton, SIGNAL( onChangeButtonState() ),
                             SLOT( clearButtonBufRenderedFlag() ) );

    connect( this, SIGNAL( onHighlightChange( bool ) ),
             hideButton, SLOT( setButtonHighlightType( bool ) ) );

    connect( this, SIGNAL( onHighlightChange( bool ) ),
             rollUpButton, SLOT( setButtonHighlightType( bool ) ) );

    connect( this, SIGNAL( onHighlightChange( bool ) ),
             propSizeButton, SLOT( setButtonHighlightType( bool ) ) );

    connect( this, SIGNAL( onHighlightChange( bool ) ),
             propSizeButton, SLOT( setButtonHighlightType( bool ) ) );

    connect( propSizeButton, SIGNAL( onButtonRelease() ),
             this, SLOT( setProportionalSize() ) );
    
    // Setup timer for button tool tips
    connect( timer, SIGNAL( timeout() ),
                    SLOT( setWindowButtonHinted() ) );
}

void TrussUnitWindow::setWindowOwner ( QWidget* owner )
{
    windowOwner = owner;
}

void TrussUnitWindow::redrawTrussUnit ()
{
    emit onForceRedrawTrussUnit();
}

/** 
 * Loads truss window from XML format
 */
void TrussUnitWindow::loadFromXML ( const QDomElement& elem ) 
    /*throw (LoadException)*/
{
    TrussUnit::loadFromXML( elem );

    /** 
     * Set window size
     *******************/
    if ( ! elem.hasAttribute( "windowWidth" ) )
        throw LoadException();
    if ( ! elem.hasAttribute( "windowHeight" ) )
        throw LoadException();

    bool ok;
    int width = elem.attribute( "windowWidth" ).toInt( &ok );
    if ( !ok ) throw LoadException();

    int height = elem.attribute( "windowHeight" ).toInt( &ok );
    if ( !ok ) throw LoadException();

    setWindowSize( width, height );

    /** 
     * Set window pisition
     ***********************/
    if ( ! elem.hasAttribute( "windowXPosition" ) )
        throw LoadException();
    if ( ! elem.hasAttribute( "windowYPosition" ) )
        throw LoadException();

    int xPos = elem.attribute( "windowXPosition" ).toInt( &ok );
    if ( !ok ) throw LoadException();

    int yPos = elem.attribute( "windowYPosition" ).toInt( &ok );
    if ( !ok ) throw LoadException();

    setWindowPosition( QPoint( xPos, yPos ) );

    /**
     * Set visible
     *****************/
    if ( elem.hasAttribute( "windowVisible" ) ) {
        QString isVisible = elem.attribute( "windowVisible" );
        if ( isVisible == "Yes" )
            setVisible( true );
        else if ( isVisible == "No" )
            setVisible( false );
        else
            throw LoadException();
    }

    /**
     * Set maximized
     *****************/
    if ( elem.hasAttribute( "windowMaximized" ) ) {
        QString isMaximized = elem.attribute( "windowMaximized" );
        if ( isMaximized == "Yes" )
            maximize( true );
        else if ( isMaximized != "No" )
            throw LoadException();
    }
}

/** 
 * Saves truss window to XML format
 */
QDomElement TrussUnitWindow::saveToXML ( QDomDocument& doc )
{
    QDomElement trussElem = TrussUnit::saveToXML( doc );
    trussElem.setTagName( "TrussUnitWindow" );

    /** 
     * Save window size and window position
     ***************************************/
    QSize size;
    QPoint pos;
    if ( isMaximized() ) {
        size = QSize( minRightBottomPos.x() - minLeftTopPos.x(), 
                      minRightBottomPos.y() - minLeftTopPos.y() );
        pos = minLeftTopPos;
    } else {
        size = getWindowSize();
        pos = getWindowLeftTopPos();
    }
    trussElem.setAttribute( "windowWidth", size.width() );
    trussElem.setAttribute( "windowHeight", size.height() );
    trussElem.setAttribute( "windowXPosition", pos.x() );
    trussElem.setAttribute( "windowYPosition", pos.y() );

    /**
     * Save visible
     *****************/
    trussElem.setAttribute( "windowVisible", (isVisible() ? "Yes" : "No") );

    /**
     * Save maximized
     *****************/
    trussElem.setAttribute( "windowMaximized", (isMaximized() ?"Yes" : "No") );

    return trussElem;
}

/* 
   Sets rendered flag of the window buffer to FALSE, so it 
   will be repainted, when the paint method will be called.
*/
void TrussUnitWindow::clearWindowRenderedFlag ()
{
    rendered( false );
}

/* 
   Sets rendered flag of the button buffer to FALSE, so it 
   will be repainted, when the paint method will be called.
*/
void TrussUnitWindow::clearButtonBufRenderedFlag ()
{
    buttonBufRendered = false;
    emit onClearButtonRenderedFlag();
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
    return QPoint( windowLeftTopPos.x() + windowSize.width(),
                   windowLeftTopPos.y() + windowSize.height() );
}

/* 
   Returns left top position of the editable truss area.
   Truss units are painted within it. 
*/
QPoint TrussUnitWindow::getTrussAreaLeftTopPos () const
{
    QPoint point;
    point.setX( windowLeftTopPos.x() + Global::leftWindowIndent );
    point.setY( windowLeftTopPos.y() + Global::topWindowIndent );
    return point;
}

/* 
   Returns right bottom position of the editable truss area.
   Truss units are painted within it. 
*/
QPoint TrussUnitWindow::getTrussAreaRightBottomPos () const
{
    QPoint point;
    point.setX( windowLeftTopPos.x() + windowSize.width() - 
                Global::rigthWindowIndent );
    point.setY( windowLeftTopPos.y() + windowSize.height() - 
                Global::bottomWindowIndent );
    return point;
}

/* 
   Sets window left top position relative to main agg 
   rendering buffer .
*/
void TrussUnitWindow::setWindowPosition ( QPoint pos )
{
    positionIsSet = true;
    QPoint oldPos = windowLeftTopPos;
    windowLeftTopPos = pos;
    setTrussPosition( getTrussAreaLeftTopPos() );
    emit onMove( oldPos, pos );
}

/**
 * Returns true if position was set
 */
bool TrussUnitWindow::hasPosition () const
{
    return positionIsSet;
}

/* 
   Returns button buffer left top position relative to main agg 
   rendering buffer.
*/
QPoint TrussUnitWindow::getButtonBufPos () const
{
    int buttonsWidth = 0;
    foreach ( AggTrussWindowButton* b, windowButtons )
        buttonsWidth = buttonsWidth + b->getWidth();

    int xIndent = 22, yIndent = 4;
    return QPoint( getWindowRightBottomPos().x() - buttonsWidth - xIndent, 
                   windowLeftTopPos.y() + yIndent );
}

/* 
   Returns current size of the truss unit window.
*/
const QSize& TrussUnitWindow::getWindowSize () const
{
    return windowSize;
}

/**
  * If window has its owner, it returns window size to
  * which it can be maximized. Or 0,0 is returned.
  */
QSize TrussUnitWindow::getMaximizedWindowSize () const
{
    if ( windowOwner )
        return QSize( windowOwner->width() - Global::bordWidth,
                      windowOwner->height() - Global::bordWidth );
    return QSize(0,0); 
}

/* 
   Sets a new size of the truss unit window.
*/
void TrussUnitWindow::setWindowSize ( int width, int height )
{
    setWindowSize( QSize(width, height) );
}

void TrussUnitWindow::setWindowSize ( const QSize& size )
{
    windowBuf->init( size.width(), size.height() );
    QSize oldSize = windowSize;
    windowSize = size;

    QPoint pixAreaSize = getTrussAreaRightBottomPos() -
                         getTrussAreaLeftTopPos();
    trussBuf->init( pixAreaSize.x() + 2 * Global::trussBufIndent, 
                    pixAreaSize.y() + 2 * Global::trussBufIndent );

    setTrussAreaSizeInPix( QSize(pixAreaSize.x(),pixAreaSize.y()) );

    clearWindowRenderedFlag();
    emit onResize( oldSize, windowSize );
}

/* 
   Resizes truss unit window and moves it to the leftTop pos.
*/
void TrussUnitWindow::resize ( QPoint leftTop, QPoint rightBottom )
{
    int dx = rightBottom.x() - leftTop.x();
    int dy = rightBottom.y() - leftTop.y();
    setWindowSize( dx, dy );
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
    Maximizes truss unit window. Saves its current size and
    resizes it to maxSize.
*/
void TrussUnitWindow::maximize ( bool saveOldSize )
{
    if ( windowOwner == 0 )
        return;

    removeButtonsHighlight();

    if ( saveOldSize )
    {
        // save old window position for future minimization
        minLeftTopPos = windowLeftTopPos;
        minRightBottomPos = getWindowRightBottomPos();
    }

    QSize maximizedSize = getMaximizedWindowSize();
    resize( QPoint( 0, 0 ), 
            QPoint( maximizedSize.width(), maximizedSize.height() ) );

    foreach ( AggTrussWindowButton* button, windowButtons )
        if ( button->getButtonType() == WindowButtonType::RollUpButton ) {
            button->setHint( "Minimize truss unit" );
            break;
        }

    maximized = true;
}

/*
    Minimizes truss unit window. Sets its size to saved 
    minLeftTopPos and minRightBottomPos.
*/
void TrussUnitWindow::minimize ()
{
    removeButtonsHighlight();
    resize( minLeftTopPos, minRightBottomPos );

    foreach ( AggTrussWindowButton* button, windowButtons )
        if ( button->getButtonType() == WindowButtonType::RollUpButton ) {
            button->setHint( "Maximize truss unit" );
            break;
        }

    maximized = false;
}

/*
    Hides truss unit window.
*/
void TrussUnitWindow::hide ()
{
    setVisible( false );
}

void TrussUnitWindow::setProportionalSize ()
{
    int w = windowSize.width(), h = windowSize.height();

    if ( h < w )  {
        w = h * getTrussAreaSize().width() / getTrussAreaSize().height();
        qWarning( "h = %d", h );

        // Check if size is smaller then minimum window size 
        if ( w < Global::resizeLimit ) {
            double sizeCoeff = double( Global::resizeLimit ) / w;
            qWarning( "sizeCoeff = %f", sizeCoeff );
            w = Global::resizeLimit;
            h = int( h * sizeCoeff );
        }
    }
    else {
        h = w * getTrussAreaSize().height() / getTrussAreaSize().width();

        // Check if size is smaller then minimum window size 
        if ( h < Global::resizeLimit ) {
            double sizeCoeff =  double( Global::resizeLimit ) / h ;
            h = Global::resizeLimit;
            w = int( w * sizeCoeff );
        }
    }

    setWindowSize( w, h );
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
    QPoint windowRightBottomPos = getWindowRightBottomPos();
    
    if ( x >= windowLeftTopPos.x() && x <= windowRightBottomPos.x() && 
         y >= windowLeftTopPos.y() && y <= windowRightBottomPos.y() )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inCanvasRect ( int x, int y ) const
{
    QPoint windowRightBottomPos = getWindowRightBottomPos();

    if ( x >= windowLeftTopPos.x() + Global::bordWidth && 
        x <= windowRightBottomPos.x() - Global::bordWidth && 
        y >= windowLeftTopPos.y() + Global::bordWidth + Global::headWidth && 
        y <= windowRightBottomPos.y() - Global::bordWidth )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inHeadlineRect ( int x, int y ) const
{
    if ( maximized )
        return false;

    QPoint windowRightBottomPos = getWindowRightBottomPos();

    if ( x >= windowLeftTopPos.x() + 3 * Global::bordWidth && 
         x <= windowRightBottomPos.x() - 3 * Global::bordWidth && y >= 
         windowLeftTopPos.y() + Global::bordWidth && 
         y <= windowLeftTopPos.y() + Global::headWidth + 
              Global::bordWidth / 2 &&
         ! inButtonBufRect( x, y ) )
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

    QPoint windowRightBottomPos = getWindowRightBottomPos();

    if ( x >= windowLeftTopPos.x() + 1 && 
         x <= windowLeftTopPos.x() + Global::bordWidth && 
         y >= windowLeftTopPos.y() + Global::winCornerRadius && 
         y <= windowRightBottomPos.y() - Global::winCornerRadius ||
         x >= windowRightBottomPos.x() - Global::bordWidth && 
         x <= windowRightBottomPos.x() - 1 && 
         y >= windowLeftTopPos.y() + Global::winCornerRadius && 
         y <= windowRightBottomPos.y() - Global::winCornerRadius )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inVerResizeRect ( int x, int y ) const
{
    if ( maximized )
        return false;

    QPoint windowRightBottomPos = getWindowRightBottomPos();

    if ( x >= windowLeftTopPos.x() + Global::winCornerRadius && 
         x <= windowRightBottomPos.x() - Global::winCornerRadius && 
         y >= windowLeftTopPos.y() && 
         y <= windowLeftTopPos.y() + Global::bordWidth / 2 ||
         x >= windowLeftTopPos.x() + Global::winCornerRadius && 
         x <= windowRightBottomPos.x() - Global::winCornerRadius && 
         y >= windowRightBottomPos.y() - Global::bordWidth && 
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

    QPoint windowRightBottomPos = getWindowRightBottomPos();

    if ( sqrt( double((x - (windowRightBottomPos.x() - Global::bordWidth)) * 
                      (x - (windowRightBottomPos.x() - Global::bordWidth)) +
                      (y - (windowLeftTopPos.y() + Global::bordWidth)) * 
                      (y - (windowLeftTopPos.y() + Global::bordWidth)) )) 
         <= Global::resEllRad )
    {
        return true;
    }
    else if ( sqrt(double((x -(windowLeftTopPos.x() + Global::bordWidth)) * 
                          (x -(windowLeftTopPos.x() + Global::bordWidth)) + 
                          (y -(windowRightBottomPos.y() - Global::bordWidth))*
                          (y -(windowRightBottomPos.y() - Global::bordWidth))))
              <= Global::resEllRad )
    {
        return true;
    }
    return false;
}

bool TrussUnitWindow::inFDiagResizeRect ( int x, int y )
{
    if ( maximized )
        return false;

    QPoint windowRightBottomPos = getWindowRightBottomPos();
    
    if ( sqrt( double((x - (windowLeftTopPos.x() + Global::bordWidth)) * 
                      (x - (windowLeftTopPos.x() + Global::bordWidth)) +
                      (y - (windowLeftTopPos.y() + Global::bordWidth)) * 
                      (y - (windowLeftTopPos.y() + Global::bordWidth))) ) 
         <= Global::resEllRad )
    {
        return true;
    }
    else if ( sqrt(double((x -(windowRightBottomPos.x() - Global::bordWidth))*
                          (x -(windowRightBottomPos.x() - Global::bordWidth)) +
                          (y -(windowRightBottomPos.y() - Global::bordWidth)) *
                          (y -(windowRightBottomPos.y() - Global::bordWidth))))
              <= Global::resEllRad )
    {
        return true;
    }
    return false;
}


AggTrussWindowButton* TrussUnitWindow::getButtonByCoord ( int xGlobal, 
                                                          int yGlobal ) const
{   
    QPoint widgetPos( xGlobal, yGlobal );

    // inner buffer coordinates
    QPoint bufPos = widgetPos - getButtonBufPos();
    foreach ( AggTrussWindowButton* b, windowButtons )
        if ( b->inButtonRect( bufPos.x(), bufPos.y() ) )
            return b;
    return 0;
}

bool TrussUnitWindow::inButtonBufRect ( int x, int y ) const
{
    int buttonsWidth = 0;
    foreach ( AggTrussWindowButton* b, windowButtons )
        buttonsWidth = buttonsWidth + b->getWidth();

    QPoint rectLeftTopPnt = getButtonBufPos();
    QPoint rectRightBottomPnt( rectLeftTopPnt.x() + 2 + buttonsWidth,
                               rectLeftTopPnt.y() + 1 +
                               Global::windowButtonHeight ); 

    if ( x > getButtonBufPos().x() && x < rectRightBottomPnt.x() &&
         y > getButtonBufPos().y() && y < rectRightBottomPnt.y() )
         return true;
    return false;
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
        setResEllColor( 90, 90, 90 );
        setHeadlineFirstColor( 180, 130, 150 );
        setHeadlineMiddleColor( 230, 210, 200 );
        setHeadlineLastColor( 150, 90, 110 );        
    }
    TrussUnit::setHighlighted(h);
    coordFieldRendered = false;
    buttonBufRendered = false;
}

void TrussUnitWindow::setWindowButtonHinted ()
{
    hinted = true;
    if( currentHintedButton )
        emit onHintShowsUp( currentHintedButton->getHint(), 
                            hintCurrentPos, true );
}

void TrussUnitWindow::removeButtonsHighlight ()
{
    foreach ( AggTrussWindowButton* b, windowButtons )
        b->setHighlighted( false );
}

void TrussUnitWindow::releaseButtons ()
{
    foreach ( AggTrussWindowButton* b, windowButtons )
        b->setPressed( false );
}

void TrussUnitWindow::clearButtonHint ()
{
    if ( timer->isActive() )
        timer->stop();

    if ( hinted )
    {
        hinted = false;
        currentHintedButton = 0;
        emit onHintHides( false );
    }
}

void TrussUnitWindow::checkMouseMoveEvent ( int x, int y, bool buttonPressed )
{
    if ( timer->isActive() )
        timer->stop();

    releaseButtons();
    removeButtonsHighlight();

    AggTrussWindowButton* selectedButton = getButtonByCoord( x, y );

    if ( selectedButton )
    {
        selectedButton->setHighlighted( true );

        if( buttonPressed && selectedButton == currentPressedButton )
            selectedButton->setPressed( true );

        else if ( hinted )
        {
            if ( selectedButton != currentHintedButton )
            {
                currentHintedButton = selectedButton;
                hintCurrentPos = QPoint( x, y );
                emit onHintShowsUp( currentHintedButton->getHint(), 
                                    hintCurrentPos, false );
            }
        }
        else
        {
            currentHintedButton = selectedButton;
            hintCurrentPos = QPoint( x, y );
            timer->start( 1000 );
        }
    }
    else if ( ! inButtonBufRect( x, y ) )
        clearButtonHint();
}

void TrussUnitWindow::checkMousePressEvent ( int x, int y )
{
    releaseButtons();
    clearButtonHint();

    AggTrussWindowButton* selectedButton = getButtonByCoord( x, y );

    if ( selectedButton ) {
        selectedButton->setPressed( true );
        currentPressedButton = selectedButton;
    }
}

void TrussUnitWindow::checkMouseReleaseEvent ( int x, int y )
{
    AggTrussWindowButton* selectedButton = getButtonByCoord( x, y );
    if ( ! selectedButton )
        return;

    if ( selectedButton->getButtonType() == 
                            WindowButtonType::HideButton )
    {
        if ( selectedButton->isPressed() )
        {
            selectedButton->setPressed( false );
            currentPressedButton = 0;
            hide();
        }
    }
    else if ( selectedButton->getButtonType() == 
                                WindowButtonType::RollUpButton )
    {   
        if ( selectedButton->isPressed() )
        {
            selectedButton->setPressed( false );
            currentPressedButton = 0;
            if ( maximized )
                minimize ();
            else
                maximize ();
        }
    }
    else if ( selectedButton->getButtonType() == 
                                WindowButtonType::PropSizeButton ) 
    {
        if ( selectedButton->isPressed() )
        {
            selectedButton->setPressed( false );
            currentPressedButton = 0;
        }
    }
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
        textLength = (int)glyph.width( str.toAscii().data() ); 
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
    DoublePoint leftTopAreaPos( Global::leftWindowIndent, 
                                Global::topWindowIndent );
    DoublePoint rightBottomAreaPos( windowSize.width() - 
                                    Global::rigthWindowIndent, 
                                    windowSize.height() - 
                                    Global::bottomWindowIndent);
    DoublePoint p1, p2;
    p2 = leftTopAreaPos;
    p1 = rightBottomAreaPos;
    baseRend.copy_bar( (int)p2.x(), (int)p2.y(), 
                       (int)p1.x(), (int)p1.y(), agg::rgba(1,1,1) );
    p2.setY( p2.y() - Global::arrowHeadIndent );
    p1.setX( p2.x() );
    p1.setY( p1.y() + Global::arrowTailIndent );
    drawArrow( ras, solidRend, sl, p1, p2 );
    p1 = rightBottomAreaPos;
    p1.setX( p2.x() - Global::arrowTailIndent );
    p2 = rightBottomAreaPos;
    p2.setX(p2.x() + Global::arrowHeadIndent );
    drawArrow( ras, solidRend, sl, p1, p2 );

    p1 = leftTopAreaPos;
    p2 = rightBottomAreaPos;
    p1.setX( p1.x() - Global::scalePieceLength );
    p1.setY( p1.y() );
    p2.setY( p1.y() );
    drawLine( ras, solidRend, sl, p1, p2 );

    p1 = rightBottomAreaPos;
    p1.setY( p1.y() + Global::scalePieceLength );
    drawLine( ras, solidRend, sl, p1, p2 );

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
    DoublePoint strokePnt1( leftTopAreaPos.x() - Global::scalePieceLength,
                            leftTopAreaPos.y() + scaleFactorYInPix );
    // right point of the scale stroke
    DoublePoint strokePnt2( leftTopAreaPos.x(), strokePnt1.y() );
    DoublePoint textPos( Global::bordWidth + 3, strokePnt1.y() + 3 );
    QString str;
    int i;
    for (i = 1; i < strokeNumbY; i++ )
    {
        strokePnt1.setY( leftTopAreaPos.y() + i * scaleFactorYInPix );
        strokePnt2.setY( strokePnt1.y() );
        drawLine ( ras, solidRend, sl, strokePnt1, strokePnt2 );
        textPos.setY( strokePnt1.y() + 3 );
        str = QString("%1").arg( getTrussAreaSize().height() - 
                                 i * scaleFactorYInAbs,0,'f',2 );   
        drawText ( textRend, str, agg::rgba(100, 100, 100), textPos ); 
    }

    // sign X-axis
    // top point of the scale stroke
    strokePnt1.setX( leftTopAreaPos.x() + scaleFactorXInPix );
    strokePnt1.setY( rightBottomAreaPos.y() );
    // bottom point of the scale stroke
    strokePnt2.setX( strokePnt1.x() );
    strokePnt2.setY( rightBottomAreaPos.y() + Global::scalePieceLength );

    textPos.setY ( strokePnt2.y() + 10 );
    for (i = 1; i < strokeNumbX; i++ )
    {
        strokePnt1.setX( rightBottomAreaPos.x() - i * scaleFactorXInPix );
        strokePnt2.setX( strokePnt1.x() );
        drawLine( ras, solidRend, sl, strokePnt1, strokePnt2 );
        textPos.setX( strokePnt1.x() - 12 );
        str = QString("%1").arg( getTrussAreaSize().width() - 
                                 i * scaleFactorXInAbs,0,'f',2 );   
        drawText( textRend, str, agg::rgba(100, 100, 100), textPos ); 
    }

    // draw area boundary values
    strokePnt1.setX( Global::bordWidth + 3 );
    strokePnt1.setY( leftTopAreaPos.y() + 3 );
    str = QString("%1").arg( getTrussAreaSize().height(),0,'f',2 );
    drawText( textRend, str, agg::rgba(100, 100, 100), strokePnt1 );

    strokePnt2.setX(rightBottomAreaPos.x() - 10 );
    strokePnt2.setY(rightBottomAreaPos.y() + 10 + Global::scalePieceLength );
    str = QString("%1").arg( getTrussAreaSize().width(),0,'f',2 );
    drawText( textRend, str, agg::rgba(100, 100, 100), strokePnt2 );

    strokePnt1.setX( leftTopAreaPos.x() - Global::scaleTextLeftBottomIndent );
    strokePnt1.setY( rightBottomAreaPos.y() + 
                      Global::scaleTextLeftBottomIndent );
    drawText( textRend, "0", agg::rgba(100, 100, 100), strokePnt1 );
    
}

void TrussUnitWindow::drawHeadline ( ren_dynarow& baseRend, 
                                     solidRenderer& solidRend, 
                                     scanline_rasterizer& ras,
                                     agg::scanline_p8& sl, 
                                     color_array_type& gradColors ) const
{
    QPoint p1, p2;
    p1.setX( 3 * Global::bordWidth );
    p1.setY( Global::bordWidth / 2 );
    p2.setX( windowSize.width() - 3 * Global::bordWidth );
    p2.setY( p1.y() + Global::headWidth );

    agg::rounded_rect headline( p1.x(), p1.y(), p2.x(), p2.y(), 9 );

    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;
    agg::trans_affine mtx;
    interpolator inter ( mtx );
    mtx *= agg::trans_affine_translation( 0, -1 );
    linear_gradient_span_gen gradSpanGen( gradSpan, inter, gradFunc, 
                                           gradColors, 0, Global::headWidth );
    linear_gradient_renderer gradRend( baseRend, gradSpanGen );
    ras.add_path( headline );
    agg::render_scanlines( ras, sl, gradRend );

    agg::rounded_rect outline( p1.x(), p1.y(), p2.x(), p2.y(), 
                               Global::headWidth / 2 );
    solidRend.color( agg::rgba( 30, 20, 10 ) );
    agg::conv_stroke<agg::rounded_rect> stroke( outline );
    stroke.width( 1.2 );
    ras.add_path( stroke );
    agg::render_scanlines(ras, sl, solidRend);
}

void TrussUnitWindow::drawCursorCoordinatesField ( ren_dynarow& baseRend,
                                                   textRenderer& textRend,
                                                   glyph_gen& glyph ) const
                                             
{
    if ( isHighlighted() )
        baseRend.clear( agg::rgba( 25,55,65 ) );
    else
        baseRend.clear( agg::rgba( 40,65,60 ) );
    
    DoublePoint textPos( 0.0, 9.0 );
    color_type textColor = agg::rgba( 0, 0, 0 );

    if ( cursorCoord.x() == -1.0 )
    {
        textPos.setX( 20 );
        drawText( textRend, "--- : ---", textColor, textPos );
    }
    else
    {
        QString str;
        str = QString( "%1" ).arg( cursorCoord.x(),0,'f',2 );
        str.append(" : ");
        drawText( textRend, str, textColor, textPos );
        int textLength = (int)glyph.width( str.toAscii().data() ); 
        str = QString( "%1" ).arg( cursorCoord.y(),0,'f',2 );  
        textPos.setX ( textPos.x() + textLength );
        drawText( textRend, str, textColor, textPos );
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

    DoublePoint textPos( 0.0, 9.0 );
    color_type textColor = agg::rgba( 0, 0, 0 );

    if ( pivotNumb )
    {
        drawText( textRend, "Pivot #", textColor, textPos );

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
    QPoint leftTopPos( Global::bordWidth, Global::bordWidth );
    QPoint leftBottomPos( Global::bordWidth, 
                          windowSize.height() - Global::bordWidth );
    QPoint rightTopPos( windowSize.width() - Global::bordWidth, 
                        Global::bordWidth );
    QPoint rightBottomPos( windowSize.width() - Global::bordWidth, 
                           windowSize.height() - Global::bordWidth );

    agg::ellipse ell;
    solidRend.color( resEllColor );
    ell.init( leftTopPos.x(), leftTopPos.y(), 
              Global::resEllRad, Global::resEllRad, 10 );
    ras.add_path( ell );
    ell.init( rightTopPos.x(), rightTopPos.y(), 
              Global::resEllRad, Global::resEllRad, 10 );
    ras.add_path ( ell );
    ell.init( leftBottomPos.x(), leftBottomPos.y(), 
              Global::resEllRad, Global::resEllRad, 10 );
    ras.add_path ( ell );
    ell.init( rightBottomPos.x(), rightBottomPos.y(), 
              Global::resEllRad, Global::resEllRad, 10 );
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
    int rad =  Global::headWidth/4 + 2;
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
                         pos.x(), pos.y(), rad, -6, Global::headWidth + 8 );

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
    fillColorArray( gradColors, firstColor, middleColor, lastColor );

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
        baseRend.clear( agg::rgba (10, 10, 10, 0) );
        solidRenderer solidRend( baseRend );    
        textRenderer textRend( baseRend, glyph );

        /*------draw resize ellipses------*/
        if ( ! maximized )
            drawResizeEllipses( solidRend, ras, sl );

        /*------draw window border------*/
        if ( maximized )
            baseRend.copy_bar( 0, 0, windowSize.width(), 
                                     windowSize.height(), borderColor );
        else
        {
            DoublePoint borderLeftTop( 0, -1 );
            DoublePoint borderRightBottom( windowSize.width(), 
                                           windowSize.height() );
            DoublePoint shadowLeftTop( borderLeftTop.x() + 1, 
                                       borderLeftTop.y() + 1 );

            color_type shadowColor = agg::rgba( 0, 0, 0 );
            if ( isHighlighted() )
                shadowColor = agg::rgba( 120, 0, 0 );

            drawOutlineRoundedRect( solidRend, ras, sl,
                                    shadowLeftTop, borderRightBottom, 
                                    shadowColor, borderColor,
                                    Global::winCornerRadius, 1 );
            drawOutlineRoundedRect( solidRend, ras, sl,
                                    borderLeftTop, borderRightBottom, 
                                    agg::rgba( 1, 1, 1, 0.6 ), borderColor,
                                    Global::winCornerRadius, 1 );
        }

        /*------draw window canvas------*/
        QPoint canvasLeftTop( Global::bordWidth, 
                              Global::bordWidth + Global::headWidth );
        QPoint canvasRightBottom( windowSize.width() - Global::bordWidth, 
                                  windowSize.height() - coordBuf->height()-5 );

        color_type shadowColor = agg::rgba( 0, 0, 0, 0.2 );
        if ( isHighlighted() )
            shadowColor = agg::rgba( 120, 0, 0, 0.3 );
        agg::rounded_rect shadow ( canvasLeftTop.x(), 
                                   canvasLeftTop.y(), 
                                   canvasRightBottom.x(), 
                                   canvasRightBottom.y(), 
                                   Global::winCornerRadius / 3 );
        ras.add_path ( shadow );
        solidRend.color ( shadowColor );
        agg::render_scanlines ( ras, sl, solidRend );

        DoublePoint lineLeftPnt( canvasLeftTop.x() + Global::winCornerRadius / 3, 
                                 canvasRightBottom.y() + 1 ),
                    lineRightPnt( canvasRightBottom.x() - 
                                  Global::winCornerRadius / 3, 
                                  canvasRightBottom.y() + 1 );
        drawLine( ras, solidRend, sl, lineLeftPnt, lineRightPnt, 
                  1, agg::rgba( 1, 1, 1, 0.7 ) );

        agg::rounded_rect canvas ( canvasLeftTop.x() + 1, 
                                   canvasLeftTop.y() + 1, 
                                   canvasRightBottom.x(), 
                                   canvasRightBottom.y(), 
                                   Global::winCornerRadius / 3 );
        ras.add_path ( canvas );
        solidRend.color ( canvColor );
        agg::render_scanlines ( ras, sl, solidRend );

        /*------draw window headline------*/
        color_array_type gradColors;
        fillColorArray( gradColors, headFirstColor, 
                        headMiddleColor, headLastColor );
        drawHeadline( baseRend, solidRend, ras, sl, gradColors );

        /*------draw simple calculation indicator------*/
        QPoint centerPos( 22, Global::bordWidth/2 + Global::headWidth/2 );
        drawCalcIndicator( baseRend, solidRend, ras, sl, 
                           gradColors, centerPos );

        /*------draw window title text and background rounded rectangle------*/
        glyph.font ( headFont );
        double lengthLimit = windowSize.width() / 2 - 38;
        QString title = fitTextToWindowSize( getTrussName (), 
                                             (int)lengthLimit, glyph );
        int titleLength = (int)glyph.width( title.toAscii().data() );
        DoublePoint titlePos( ( windowSize.width() - 2 * Global::bordWidth - 
                                titleLength ) / 2, 14 );

        DoublePoint rectPos1( titlePos.x() - 15, Global::bordWidth / 2 ),
                    rectPos2( titlePos.x() + titleLength + 15, 
                              rectPos1.y() + Global::headWidth -1 );

        color_type firstColor( agg::rgba( 0, 0, 0 ) );
        color_type middleColor( agg::rgba8( 180, 130, 100 ) );
        if ( ! isHighlighted() )
            middleColor = agg::rgba( 100, 120, 120 );
        color_type lastColor( agg::rgba( 0, 0, 0 ) );

        fillColorArray( gradColors, firstColor, middleColor, lastColor );
        agg::trans_affine mtx;
        drawOutlineRoundedRect( baseRend, solidRend, ras, sl, gradColors, 
                                mtx, rectPos1, rectPos2, 
                                agg::rgba( 1, 1, 1, 0.6 ),
                                8, 1, 0, Global::headWidth * 2 );

        color_type titleColor = agg::rgba(1, 1, 1);
        drawText( textRend, title, titleColor, titlePos );

        /*----draw editable area in which canvas truss unit can be painted---*/
        glyph.font( agg::mcs11_prop_condensed );
        drawTrussArea( baseRend, ras, textRend, solidRend, sl );

        rendered( true );
    }

    /*------draw truss unit------*/
    ren_dynarow baseRend( trussPixf );
    TrussUnit::paint( baseRend );
/*
    //------draw truss elements numbers field------
    baseRend = numbersPixf;
    textRenderer textRend ( baseRend, glyph );
    glyph.font ( numbersFont );
    drawNumbersField ( baseRend, textRend );

    if ( ! coordFieldRendered )
    {
        //------draw coordinates field------
        baseRend = coordPixf;
        textRenderer textRend ( baseRend, glyph );
        glyph.font ( numbersFont );
        drawCursorCoordinatesField ( baseRend, textRend, glyph );
        coordFieldRendered = true;
    }
*/
    if ( ! buttonBufRendered )
    {
        /*------draw buttons------*/
        baseRend = buttonPixf;
        baseRend.clear( agg::rgba (10, 10, 10, 0) );
        solidRenderer solidRend( baseRend ); 
        
        foreach ( AggTrussWindowButton* b, windowButtons )
            b->paint( baseRend );

        buttonBufRendered = true;
    }

    /*------blend buffers------*/
    baseRenderer.blend_from( windowPixf, 0, windowLeftTopPos.x(), 
                             windowLeftTopPos.y(), unsigned(1.0 * 255) );

    const QPoint& areaLeftTop = getTrussAreaLeftTopPos();

    baseRenderer.blend_from( trussPixf, 0, 
                             areaLeftTop.x() - Global::trussBufIndent,
                             areaLeftTop.y() - Global::trussBufIndent, 
                             uint(1.0 * 255) );
/*
    baseRenderer.blend_from ( coordPixf, 0, 
                              windowLeftTopPos.x() + Global::winCornerRadius, 
                              windowRightBottomPos.y() - coordBuf->height() -3,
                              uint(1.0 * 255) );

    baseRenderer.blend_from ( numbersPixf, 0, windowRightBottomPos.x() - 80, 
                              windowRightBottomPos.y() - coordBuf->height() -3,
                              uint(1.0 * 255) );
*/
    baseRenderer.blend_from( buttonPixf, 0, getButtonBufPos().x(), 
                             getButtonBufPos().y(), uint(1.0 * 255) );
}

/****************************************************************************/
