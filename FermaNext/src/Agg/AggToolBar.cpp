
#include "AggToolBar.h"

/*****************************************************************************
 * Agg Tool Bar Button
 *****************************************************************************/

AggToolBarButton::AggToolBarButton ( const QString& fname, const QString& l, 
                                     QPoint pos, int w, int h  ) :
    AggButton ( l, pos, w, h ),
    fillCol( agg::rgba( 0, 0, 0, 0.3 ) ),
    lineCol( agg::rgba( 0, 0, 0 ) ), 
    highlightFill( agg::rgba( 0, 0, 0, 0.15 ) ),
    highlightLine( agg::rgba( 0, 0, 0, 0.7 ) )
{
    parseSvg ( pathRend, fname.ascii() );
}

AggToolBarButton::~AggToolBarButton ()
{}

void AggToolBarButton::paint ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                               agg::scanline_p8& sl, solidRenderer& solidRend,
                               agg::trans_affine& mtx, double scaleX, double scaleY ) const
{
    QPoint pos = getPosition ();
    int width = getWidth ();
    int height = getHeight ();

    primitivesRenderer primRend ( baseRend );

    if ( isPressed() )
    {
        primRend.fill_color ( fillCol );
        primRend.line_color ( lineCol );
        primRend.outlined_rectangle ( pos.x() + 1, pos.y() + 1, pos.x() + width - 1, 
                                      pos.y() + height - 1 );
    }   
    else if ( isHighlighted() )
    {
        primRend.fill_color ( highlightFill );
        primRend.line_color ( highlightLine );
        primRend.outlined_rectangle ( pos.x(), pos.y(), pos.x() + width, pos.y() + height );
    }
    QPoint iconPos( int((pos.x() + width/4) / scaleX), 
                    int((pos.y() + height/8) / scaleY) );
    drawSvg ( baseRend, ras, sl, pathRend, solidRend, mtx, iconPos.x(), 
              iconPos.y(), scaleX, scaleY, svgExpand, svgGamma );
}

/*****************************************************************************
 * Agg Tool Bar
 *****************************************************************************/

AggToolBar::AggToolBar( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                        int bordBottom, int separation_ ) :
    visible(true),
    centerPos(pos),
    borderLeft(bordLeft),
    borderRight(bordRight),
    borderTop(bordTop),
    borderBottom(bordBottom),
    separation(separation_),
    // Consider that we may need additional space (bufferEmptyArea)
    // for the different agg effects
    toolBarBuf( new rbuf_dynarow( bordLeft + bordRight + 2 * bufferEmptyArea,
                                  bordTop + bordBottom + bufferEmptyArea ) ),
    renderedFlag(false)
{
    toolBarWidth = bordLeft + bordRight + separation_;
    toolBarHeight = bordTop + bordBottom;
    // Centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight );

    QObject::connect( this, SIGNAL( onChangeToolBarPosition() ),
                      SLOT( clearToolBarRenderedFlag() ) );
}

AggToolBar::~AggToolBar ()
{
    clean();
    delete toolBarBuf;
}

void AggToolBar::clean ()
{
    ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
        delete *iter;
    buttons.clear();
}

AggToolBarButton& AggToolBar::addButton ( const QString& fname, 
                                          const QString& label, 
                                          QPoint leftTopPos, 
                                          uint buttonWidth, uint buttonHeight, 
                                          QObject* widget,  const char* signal,
                                          const char* slot )
{
    AggToolBarButton* button = new AggToolBarButton( fname, label, leftTopPos, 
                                                     buttonWidth, buttonHeight );
    buttons.push_back ( button );

    QObject::connect( button, SIGNAL( onButtonHighlightChange() ),
                      SLOT( clearToolBarRenderedFlag() ) );

    QObject::connect( button, SIGNAL( onButtonPress() ),
                      SLOT( releaseButtons() ) );

    QObject::connect( button, SIGNAL( onChangeButtonState() ),
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

void AggToolBar::removeButton ( const QString& label )
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
            buttons.erase( iter );
            delete button;
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

    // Clear all buttons.
    clean();

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

QPoint AggToolBar::getPosition () const
{
    return toolBarLeftTopPos;
}

void AggToolBar::setPosition ( QPoint newPos )
{
    if ( toolBarLeftTopPos == newPos )
        return;
    toolBarLeftTopPos = newPos;
    emit onChangeToolBarPosition ();
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

int AggToolBar::getWidth () const
{
    return toolBarWidth;
}

int AggToolBar::getHeight () const
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

