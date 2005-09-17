
#include "AggToolBar.h"

/*****************************************************************************
 * Agg Tool Bar Button
 *****************************************************************************/

AggToolBarButton::AggToolBarButton ( QString fname, QString l, QPoint pos, 
                                     int w, int h  ) :
    leftTopPos ( pos ),
    width ( w ),
    height ( h ),
    label ( l ),
    pressed ( false ),
    highlighted ( false )
{
    parseSvg ( pathRend, fname.ascii() );
}

AggToolBarButton::~AggToolBarButton ()
{}

void AggToolBarButton::setLabel ( QString l )
{
    label = l;
}

QString AggToolBarButton::getLabel () const
{
    return label;
}

void AggToolBarButton::setHint ( QString hint_ )
{
    label = hint_;
}

QString AggToolBarButton::getHint () const
{
    return hint;
}

void AggToolBarButton::setPosition ( QPoint newPos )
{
    leftTopPos = newPos;
}

QPoint AggToolBarButton::getPosition () const
{
    return leftTopPos;
}

void AggToolBarButton::setWidth ( int w )
{
    width = w;
}

int AggToolBarButton::getWidth () const
{
    return width;
}

void AggToolBarButton::setHeight ( int h )
{
    height = h;
}

int AggToolBarButton::getHeight () const
{
    return height;
}

void AggToolBarButton::setHighlighted ( bool h_ )
{
    if ( highlighted == h_ )
        return;
    highlighted = h_;
    emit onButtonHighlightChange ();
}

bool AggToolBarButton::isHighlighted () const
{
    return highlighted;
}

bool AggToolBarButton::inButtonRect ( int x, int y ) const
{
    if ( x > leftTopPos.x() && x < leftTopPos.x() + width &&
         y > leftTopPos.y() && y < leftTopPos.y() + height )
        return true;
    return false;
}

bool AggToolBarButton::isPressed () const
{
    return pressed;
}

void AggToolBarButton::setPressed ( bool status )
{
    if ( pressed == status )
        return;

    if ( status )
        emit onButtonPress();

    pressed = status;
    emit buttonIsPressed();
}

void AggToolBarButton::pressButton ()
{
    setPressed ( true );
}

void AggToolBarButton::paint ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, solidRenderer& solidRend,
                               agg::trans_affine& mtx, double scaleX, double scaleY ) const
{
    if ( isPressed() )
    {
        primitivesRenderer primRend ( baseRend );
        primRend.fill_color ( agg::rgba( 0, 0, 0, 0.3 ) );
        primRend.line_color ( agg::rgba( 0, 0, 0 ) );
        primRend.outlined_rectangle ( leftTopPos.x() + 1, leftTopPos.y() + 1, 
                                      leftTopPos.x() + width - 1, 
                                      leftTopPos.y() + height - 1 );
    }   
    else if ( isHighlighted() )
    {
        primitivesRenderer primRend ( baseRend );
        primRend.fill_color ( agg::rgba( 0, 0, 0, 0.15 ) );
        primRend.line_color ( agg::rgba( 0, 0, 0, 0.7 ) );
        primRend.outlined_rectangle ( leftTopPos.x(), leftTopPos.y(), 
                                      leftTopPos.x() + width, leftTopPos.y() + height );
    }
    QPoint iconPos ( ( leftTopPos.x() + width / 4 ) / scaleX,
                     ( leftTopPos.y() + height / 8 ) / scaleY );
    drawSvg ( baseRend, ras, sl, pathRend, solidRend, mtx, iconPos.x(), 
              iconPos.y(), scaleX, scaleY, svgExpand, svgGamma );
}

/*****************************************************************************
 * Agg Tool Bar
 *****************************************************************************/

AggToolBar::AggToolBar( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                        int bordBottom, int separation_ ) :
    renderedFlag ( false ),
    borderLeft ( bordLeft ),
    borderRight ( bordRight ),
    borderTop ( bordTop ),
    borderBottom ( bordBottom ),
    separation( separation_ ),
    visible ( true ),
    centerPos ( pos ),
    // Consider that we may need additional space (bufferEmptyArea)
    // for the different agg effects
    toolBarBuf ( new rbuf_dynarow( bordLeft + bordRight + 2 * bufferEmptyArea, 
                                   bordTop + bordBottom + bufferEmptyArea ) )
{
    toolBarWidth = bordLeft + bordRight + separation_;
    toolBarHeight = bordTop + bordBottom;
    // Centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight );  
}

AggToolBar::~AggToolBar ()
{
    delete toolBarBuf;
}

AggToolBarButton& AggToolBar::addButton ( QString fname, QString label, QPoint leftTopPos, 
                                          int buttonWidth, int buttonHeight, 
                                          QObject* widget, char* signal, char* slot )
{
    AggToolBarButton* button = new AggToolBarButton ( fname, label, leftTopPos, 
                                                      buttonWidth, buttonHeight );
    buttons.push_back ( button );

    QObject::connect( button, SIGNAL( onButtonHighlightChange() ),
                      SLOT( clearToolBarRenderedFlag() ) );

    QObject::connect( button, SIGNAL( onButtonPress() ),
                      SLOT( releaseButtons() ) );

    QObject::connect( button, SIGNAL( buttonIsPressed() ),
                      SLOT( clearToolBarRenderedFlag() ) );

    if ( signal && widget )
        QObject::connect( widget, signal, button, SLOT( pressButton() ) );

    if ( slot && widget )
        QObject::connect( button, SIGNAL( onButtonPress() ), widget, slot );

    if ( buttonHeight > toolBarHeight - borderTop - borderBottom )
        toolBarHeight = buttonHeight + borderTop + borderBottom;

    toolBarWidth = toolBarWidth + buttonWidth + separation;

    // Expand buffer for the new buttons if necessary
    if ( toolBarBuf->width() < toolBarWidth + 2 * bufferEmptyArea )
        toolBarBuf->init( toolBarWidth + 2 * bufferEmptyArea, 
                          toolBarHeight + bufferEmptyArea );

    // Renew tool bar widget position and centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight ); 

    setRendered( false );
    return *button;
}

void AggToolBar::removeButton ( QString label )
{
    if ( buttons.empty() )
        return;

    int delWidth;
    ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
    {
        AggToolBarButton* button = *iter;
        if ( button->getLabel() == label )
        {
            delWidth = button->getWidth();
            buttons.erase ( iter );
        }
    }
    toolBarHeight = findMaxButtonHeight ();
    toolBarWidth = toolBarWidth - delWidth - separation;

    // Compress buffer if its up to twice as much as an actual tool bar width
    // with additional area
    if ( toolBarBuf->width() > toolBarWidth * 2 + 2 * bufferEmptyArea )
        toolBarBuf->init( toolBarWidth, toolBarHeight + bufferEmptyArea );

    // Renew tool bar widget position and centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight );

    setRendered( false );
}

int AggToolBar::findMaxButtonHeight () const
{
    if ( buttons.empty() )
        return 0;

    int height = 0;
    ButtonListConstIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
    {
        int newHeight = (*iter)->getHeight();
        if ( newHeight > height )
            height = newHeight;
    }
    return height;
}

void AggToolBar::clearToolBar ()
{
    if ( buttons.empty() )
        return;

    buttons.clear();
    toolBarBuf->init( borderLeft + borderRight + separation + 2 * bufferEmptyArea, 
                      toolBarBuf->height() );

    // Renew tool bar widget position and centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight );

    setRendered( false );
}

AggToolBar::ButtonList AggToolBar::getButtonList () const
{
    return buttons;
}

void AggToolBar::removeButtonHighlight ()
{
    if ( buttons.empty() )
        return;

    ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
        (*iter)->setHighlighted( false );
}

void AggToolBar::releaseButtons ()
{
    if ( buttons.empty() )
        return;

    ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
        (*iter)->setPressed( false );
}

void AggToolBar::setCenterPoint ( QPoint pos )
{
    if ( centerPos == pos )
        return;

    centerPos = pos;
    // Renew tool bar widget position and centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight );  
    setRendered( false );
}

QPoint AggToolBar::getCenterPoint () const
{
    return centerPos;
}

QPoint AggToolBar::getLeftTopPos () const
{
    return toolBarLeftTopPos;
}

void AggToolBar::setButtonSeparation ( int s )
{
    if ( separation == s ) 
        return;

    toolBarWidth = toolBarWidth - ( buttons.size() + 1 ) * separation + 
                   ( buttons.size() + 1 ) * s;

    // Expand or compress buffer if necessary
    if ( toolBarBuf->width() < toolBarWidth || 
         toolBarBuf->width() > toolBarWidth * 2 + 2 * bufferEmptyArea )
        toolBarBuf->init( toolBarWidth, toolBarBuf->height() );

    separation = s;
    setRendered( false );
}

int AggToolBar::getButtonSeparation () const
{
    return separation;
}

void AggToolBar::setBorderLeft ( int width )
{
    if ( borderLeft == width ) 
        return;
    borderLeft = width;
    setRendered( false );
}

int AggToolBar::getBorderLeft () const
{
    return borderLeft;
}

void AggToolBar::setBorderRight ( int width )
{
    if ( borderRight == width ) 
        return;
    borderRight = width;
    setRendered( false );
}

int AggToolBar::getBorderRight () const
{
    return borderRight;
}

void AggToolBar::setBorderBottom ( int width )
{
    if ( borderBottom == width ) 
        return;
    borderBottom = width;
    setRendered( false );
}

int AggToolBar::getBorderBottom () const
{
    return borderBottom;
}

void AggToolBar::setBorderTop ( int width )
{
    if ( borderTop == width ) 
        return;
    borderTop = width;
    setRendered( false );
}

int AggToolBar::getBorderTop () const
{
    return borderTop;
}

int AggToolBar::getToolBarWidth () const
{
    return toolBarWidth;
}

int AggToolBar::getToolBarHeight () const
{
    return toolBarHeight;
}

rbuf_dynarow& AggToolBar::getRenderingBuffer () const
{
    return *toolBarBuf;
}

bool AggToolBar::isVisible () const
{
    return visible;
}

void AggToolBar::setVisible ( bool status )
{
    if ( visible == status ) 
        return;
    visible = status;
    setRendered( false );
}

bool AggToolBar::isRendered () const
{
    return renderedFlag;
}

void AggToolBar::setRendered ( bool status ) const
{
    if ( renderedFlag == status )
        return;

    renderedFlag = status;
}

void AggToolBar::clearToolBarRenderedFlag ()
{
    renderedFlag = false;
    emit onRenderingStatusCleared ();
}

/*****************************************************************************/

