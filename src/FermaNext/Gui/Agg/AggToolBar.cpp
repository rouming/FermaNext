
#include <algorithm>

#include <QBasicTimer>
#include <QTimer>

#include "AggToolBar.h"
#include "AggComboBox.h"

/*****************************************************************************
 * Agg Tool Bar Button
 *****************************************************************************/

AggToolBarButton::AggToolBarButton ( const QString& iconPath ) :
    fillCol( agg::rgba( 0, 0, 0, 0.3 ) ),
    lineCol( agg::rgba( 0, 0, 0 ) ), 
    highlightFill( agg::rgba( 0, 0, 0, 0.15 ) ),
    highlightLine( agg::rgba( 0, 0, 0, 0.7 ) )
{
    parseSvg( pathRend, iconPath.toLocal8Bit() );
    setWidth( Global::buttonWidth );
    setHeight( Global::buttonHeight );
}

AggToolBarButton::~AggToolBarButton ()
{}

void AggToolBarButton::paint ( ren_dynarow& baseRend ) const
{
    scanline_rasterizer ras;
    agg::scanline_p8 sl; 
    solidRenderer solidRend( baseRend );
    agg::trans_affine mtx;

    QPoint pos = getPosition ();
    int width = getWidth ();
    int height = getHeight ();

    primitivesRenderer primRend ( baseRend );

    if ( isPressed() )
    {
        primRend.fill_color ( fillCol );
        primRend.line_color ( lineCol );
        primRend.outlined_rectangle ( pos.x() + 1, pos.y() + 1, 
                                      pos.x() + width - 1, 
                                      pos.y() + height - 1 );
    }   
    else if ( isHighlighted() )
    {
        primRend.fill_color ( highlightFill );
        primRend.line_color ( highlightLine );
        primRend.outlined_rectangle ( pos.x(), pos.y(), 
                                      pos.x() + width, 
                                      pos.y() + height );
    }

    double scaleX = 0.2, scaleY = 0.2;
    QPoint iconPos( int((pos.x() + width/4) / scaleX), 
                    int((pos.y() + height/8) / scaleY) );

    drawSvg ( baseRend, ras, sl, pathRend, solidRend, mtx, 
              iconPos.x(), iconPos.y(), scaleX, scaleY, 
              Global::svgExpand, Global::svgGamma );
}

/*****************************************************************************
 * Agg Tool Bar Hide Button
 *****************************************************************************/

AggToolBarHideButton::AggToolBarHideButton () :
    fillCol( agg::rgba( 65, 90, 110, 0.5 ) ),
    lineCol( agg::rgba( 10, 10, 10 ) ), 
    highlightFill( agg::rgba( 1, 1, 1, 0.5 ) )
{
    setHint( "Hide tool bar" );
    setWidth( Global::buttonWidth * 3 );
    setHeight( 6 );  
}

AggToolBarHideButton::~AggToolBarHideButton ()
{}

void AggToolBarHideButton::paint ( ren_dynarow& baseRend ) const
{
    QPoint pos = getPosition ();
    int width = getWidth ();
    int height = getHeight ();

    scanline_rasterizer ras;
    agg::scanline_p8 sl; 
    solidRenderer solidRend( baseRend );
    agg::trans_affine mtx;
    primitivesRenderer primRend ( baseRend );

    // draw button
    if ( ! isHighlighted() )
        primRend.fill_color ( fillCol );
    else 
        primRend.fill_color ( highlightFill );

    primRend.line_color ( lineCol );
    primRend.outlined_rectangle ( pos.x(), pos.y(), 
                                  pos.x() + width, 
                                  pos.y() + height );

    // draw button icon
    QPoint pos1 ( pos.x() + width / 2 - 4, pos.y() + 2 );
    QPoint pos2 ( pos.x() + width / 2 + 4, pos.y() + 4 );
    if ( ! isHighlighted() )
        primRend.fill_color ( lineCol );
    else 
        primRend.fill_color (  agg::rgba( 120, 120, 120 ) );
    primRend.solid_rectangle ( pos1.x(), pos1.y(), pos2.x(), pos2.y() );
}

/*****************************************************************************
 * Agg Tool Bar
 *****************************************************************************/

AggToolBar::AggToolBar( QWidget* parentWidget ) :
    toolBarBuf( new rbuf_dynarow( 0, 0 ) ),
    currentHintedButton( 0 ),
    hideButton( 0 ),
    thread( new AggPaintThread( *parentWidget ) ),
    hintTimer( new QTimer( this ) ),
    animTimer( new QBasicTimer ),
    designerWidget( parentWidget ),
    pixNumb( 0 ),
    toolBarWidth( 0 ),
    toolBarHeight( 0 ),
    centerPos( QPoint( 0, 0 ) ),
    borderLeft( 15 ),
    borderRight( 15 ),
    borderTop( 8 ),
    borderBottom( 5 ),
    cornerRadius( 30 ),
    separator( 5 ),
    rendered( false ),
    visible( true ),
    enabled( true ),
    toolBarIsHinted( false )
{
    Q_ASSERT( designerWidget != 0 );

    initHideButton();

    resize( borderLeft + borderRight, borderBottom + borderTop );
    
    thread->setFrameDelayMsec( 1 );
    thread->setFrameRate( Global::pixHideNumb );

    connect( hintTimer, SIGNAL( timeout() ),
                          SLOT( setToolBarHinted() ) );

    connect( hintTimer, SIGNAL( timeout() ),
             designerWidget, SLOT( update() ) );

    connect( this, SIGNAL( onRenderingStatusCleared() ),
             designerWidget, SLOT( update() ) );
}

AggToolBar::~AggToolBar ()
{
    clean();

    if ( thread->isRunning() )
        thread->wait();
    delete thread;

    delete hintTimer;
    delete animTimer;
    delete hideButton;
    delete toolBarBuf;
}

void AggToolBar::initHideButton ()
{
    hideButton = new AggToolBarHideButton();

    connect( hideButton, SIGNAL( onChangeButtonState() ),
                           SLOT( clearToolBarRenderedFlag() ) );

    connect( hideButton, SIGNAL( onButtonPress() ),
                           SLOT( hideToolBar() ) );

    connect( hideButton, SIGNAL( onButtonRelease() ),
                           SLOT( showToolBar() ) );
}

void AggToolBar::clean ()
{
    foreach ( AggButton* b, buttons )
        delete b;
    buttons.clear();

    foreach ( AggComboBox* cb, comboBoxes )
        delete cb;
    comboBoxes.clear();
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
    
    clearToolBarRenderedFlag();
}

void AggToolBar::setCenterPosition ( QPoint pos )
{
    if ( centerPos == pos )
        return;

    QPoint diff = centerPos - pos;
    centerPos = pos;
    setPosition( toolBarLeftTopPos - diff );
}

void AggToolBar::resize ( int w, int h )
{
    // Consider that we may need additional space (Global::bufferEmptyArea)
    // for the different agg effects
    toolBarBuf->init( w  + 2 * Global::bufferEmptyArea, 
                      h  + Global::bufferEmptyArea );

    clearToolBarRenderedFlag();
}

int AggToolBar::getWidth () const
{
    return toolBarWidth;
}

int AggToolBar::getHeight () const
{
    return toolBarHeight;
}

QWidget* AggToolBar::parentWidget () const
{
    return designerWidget;
}

QPoint AggToolBar::fromGlobalPos ( int xpos, int ypos ) const
{
    QPoint pos;
    pos.setX( xpos - toolBarLeftTopPos.x() + Global::bufferEmptyArea );
    pos.setY( ypos - toolBarLeftTopPos.y() + Global::bufferEmptyArea );
    return pos;
}

bool AggToolBar::inToolBarRect ( int x, int y, bool bordCheck /* = false */ ) const
{
    // consider that buttons know only about dynarow buffer coords
    QPoint bufPos = fromGlobalPos( x, y );

    //TODO: define more accurately rounded boundaries of the tool bar
    if ( bordCheck )
    {
        if ( x > toolBarLeftTopPos.x() + borderLeft && 
             x < toolBarLeftTopPos.x() + toolBarWidth - borderRight - separator &&
             y > toolBarLeftTopPos.y() + borderTop && 
             y < toolBarLeftTopPos.y() + toolBarHeight - borderBottom ||
             hideButton->inButtonRect( bufPos.x(), bufPos.y() ) )
            return true;
        else
            return false;
    }
    else 
    {
        if ( x > toolBarLeftTopPos.x() && 
             x < toolBarLeftTopPos.x() + toolBarWidth &&
             y > toolBarLeftTopPos.y() && 
             y < toolBarLeftTopPos.y() + toolBarHeight ||
             hideButton->inButtonRect( bufPos.x(), bufPos.y() ) )
            return true;
        else
            return false;
    }
}

void AggToolBar::updateToolBarGeometry ()
{
    toolBarWidth = borderLeft + borderRight; 
    int maxButtonHeight = 0;
    foreach ( AggButton* b, buttons ) {
        toolBarWidth = toolBarWidth + b->getWidth() + separator;

        if ( maxButtonHeight < b->getHeight() )
            maxButtonHeight = b->getHeight();
    }

    toolBarHeight = maxButtonHeight + borderTop + borderBottom;

    resize( toolBarWidth, toolBarHeight );    

    // Renew tool bar widget position and centering tool bar
    toolBarLeftTopPos.setX( centerPos.x() - toolBarWidth/2 );
    toolBarLeftTopPos.setY( centerPos.y() - toolBarHeight ); 

    // Update hide button position
    int xPos = ( toolBarWidth - hideButton->getWidth() ) / 2 + 
               Global::bufferEmptyArea;
    int yPos = Global::bufferEmptyArea - hideButton->getHeight() / 2;
    hideButton->setPosition( QPoint( xPos, yPos ) );
}

QPoint AggToolBar::getPositionForNewButton () const
{
    
    int xPos = toolBarBuf->width() - Global::bufferEmptyArea - 
               borderLeft;
    int yPos = Global::bufferEmptyArea + borderTop;

    return QPoint( xPos, yPos );
}

void AggToolBar::addButton ( AggButton& button )
{
    buttons.push_back( &button );

    connect( &button, SIGNAL( onButtonPress() ),
                        SLOT( releaseButtons() ) );

    connect( &button, SIGNAL( onChangeButtonState() ),
                        SLOT( clearToolBarRenderedFlag() ) );

    button.setPosition( getPositionForNewButton() );

    updateToolBarGeometry();
}

void AggToolBar::removeButton ( AggButton& btn )
{
    if ( buttons.empty() )
        return;

    int delWidth = 0;

    ButtonListIter iter = std::find( buttons.begin(),
                                     buttons.end(),
                                     &btn );
    if ( iter == buttons.end() )
        return;

    buttons.erase( iter );
    delete *iter;

    updateToolBarGeometry();
}

void AggToolBar::removeButton ( int indx )
{
    if ( indx >= buttons.size() )
        return;

    removeButton( *buttons.at( indx ) );
}

AggButton* AggToolBar::getButton ( int indx ) const
{
    if ( indx >= buttons.size() || indx < 0 )
        return 0;

    ButtonListConstIter iter = std::find( buttons.begin(),
                                          buttons.end(),
                                          buttons.at( indx ) );
    if ( iter == buttons.end() )
        return 0;

    return *iter;
}

AggButton* AggToolBar::getButton ( int xPos, int yPos ) const
{
    QPoint bufPos = fromGlobalPos( xPos, yPos );

    foreach ( AggButton* button, buttons )
        if ( button->inButtonRect( bufPos.x(), bufPos.y() ) )
            return button;

    if ( hideButton->inButtonRect( bufPos.x(), bufPos.y() ) )
        return hideButton;

    return 0;
}

void AggToolBar::addComboBox ( AggComboBox& combo )
{
    comboBoxes.append( &combo );
}

AggComboBox* AggToolBar::getComboBox ( int idx ) const
{
    try {
        return comboBoxes.at( idx );
    } 
    catch (...) {
        return 0;
    }
}

AggToolBar::ButtonList AggToolBar::getButtonList () const
{
    return buttons;
}

void AggToolBar::removeButtonHighlight ()
{
    hideButton->setHighlighted( false );

    foreach ( AggButton* b, buttons )
        b->setHighlighted( false );
}

void AggToolBar::releaseButtons ()
{
    foreach ( AggButton* b, buttons )
        b->setPressed( false );
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

    clearToolBarRenderedFlag();
}

bool AggToolBar::isEnabled () const
{
    return enabled;
}

void AggToolBar::setEnabled ( bool status )
{
    if ( enabled == status )
        return;

    enabled = status;
}

bool AggToolBar::isRendered () const
{
    return rendered;
}

void AggToolBar::clearToolBarRenderedFlag ()
{
    if ( ! rendered )
        return;

    rendered = false;
    emit onRenderingStatusCleared ();
}

void AggToolBar::showToolBar ()
{
    if ( animTimer->isActive() )
        animTimer->stop(); 

    removeButtonHighlight();

    pixNumb = toolBarHeight - hideButton->getHeight();

    enabled = false;
    visible = true;

    animTimer->start( 1, this );

    emit onAnimationPlays( true );
}

void AggToolBar::hideToolBar ()
{
    if ( animTimer->isActive() )
        animTimer->stop();

    removeButtonHighlight();

    pixNumb = toolBarHeight - hideButton->getHeight();

    enabled = false;

    animTimer->start( 1, this );

    emit onAnimationPlays( true );
}

void AggToolBar::timerEvent ( QTimerEvent* )
{
    Q_ASSERT( designerWidget != 0 );

    int indent = 2;
    QPoint pos = toolBarLeftTopPos;

    if ( hideButton->isPressed() )
    {
        pixNumb -= Global::pixHideNumb;
        pos.setY( pos.y() + Global::pixHideNumb );

        if ( pixNumb <= 0 ) {
            pos.setY( designerWidget->height() - 
                      hideButton->getHeight() + indent );

            animTimer->stop(); 

            visible = false;
            enabled = true;

            emit onAnimationPlays( false );
        }
    }
    else
    {
        pixNumb -= Global::pixHideNumb;
        pos.setY( pos.y() - Global::pixHideNumb );

        if ( pixNumb <= 0 ) {
            pos.setY( designerWidget->height() - 
                      getHeight() + indent );

            animTimer->stop(); 

            enabled = true;

            emit onAnimationPlays( false );
        }
    }

    setPosition( pos );
}

void AggToolBar::setToolBarHinted ()
{
    toolBarIsHinted = true;
    if ( currentHintedButton )
        emit onHintShowsUp( currentHintedButton->getHint(), 
                            hintCurrentPos, true );
}

void AggToolBar::clearButtonHint ()
{
    if ( hintTimer->isActive() )
        hintTimer->stop();

    if ( toolBarIsHinted ) {
        toolBarIsHinted = false;
        currentHintedButton = 0;
        emit onHintHides( false );
    }
}

void AggToolBar::mouseMove ( int x, int y )
{
    if ( hintTimer->isActive() )
        hintTimer->stop();

    if ( ! enabled )
        return;

    removeButtonHighlight();

    AggButton* button = getButton( x, y );
    if ( button && button->isEnabled() )
    {
        button->setHighlighted( true );

        if ( toolBarIsHinted )
        {
            if ( button != currentHintedButton )
            {
                currentHintedButton = button;
                hintCurrentPos = QPoint( x, y );
                emit onHintShowsUp( currentHintedButton->getHint(), 
                                    hintCurrentPos, false );
            }
        }   
        else
        {
            currentHintedButton = button;
            hintCurrentPos = QPoint( x, y );
            hintTimer->start( 1000 );
        }
    }
    else
    {
        if ( currentHintedButton != 0 )
        {
            currentHintedButton = 0;
            emit onHintHides( false );
        }
    }

    if ( ! inToolBarRect( x, y, true ) )
        clearButtonHint();
}

void AggToolBar::mousePress ( int x, int y )
{
    if ( ! enabled )
        return;

    clearButtonHint ();

    AggButton* button = getButton( x, y );
    if ( ! button )
        return;

    if ( button != hideButton ) {
        if ( button->isEnabled() )
            button->setPressed( true );
    }
    else if ( ! hideButton->isPressed() )
        button->setPressed( true );
    else
        button->setPressed( false );
}

void AggToolBar::drawButtons ( ren_dynarow& baseRend ) const
{
    hideButton->paint( baseRend );

    if ( ! isVisible() )
        return;

    foreach ( AggButton* button, buttons )
        button->paint( baseRend );
}

void AggToolBar::paint ( base_renderer& baseRenderer ) const
{
    pixf_dynarow toolBarPixf( *toolBarBuf ); 
    ren_dynarow baseRend( toolBarPixf );

    if ( ! isRendered () )
    {
        baseRend.clear( agg::rgba( 150, 150, 150, 0 ) );

        scanline_rasterizer ras;
        agg::scanline_p8 sl;

        if ( isVisible() )
        {
            agg::trans_affine mtx;
            gradient_span_alloc gradSpan;
            linear_gradient gradFunc;
            color_array_type gradColors;

            color_type barFirstColor( agg::rgba( 35, 50, 60, 0.8 ) );
            color_type barMiddleColor( agg::rgba( 20, 60, 80, 0.8 ) );
            color_type barLastColor( agg::rgba( 20, 60, 80, 0.8 ) );

            mtx *= agg::trans_affine_translation( 0, -10 );
            interpolator inter( mtx );

            agg::rounded_rect bar( Global::bufferEmptyArea, 
                                   Global::bufferEmptyArea, 
                                   Global::bufferEmptyArea + toolBarWidth, 
                                   Global::bufferEmptyArea + 
                                   toolBarBuf->height(), 
                                   cornerRadius );
            ras.add_path( bar );

            fillColorArray( gradColors, barFirstColor, barMiddleColor, 
                            barLastColor );

            linear_gradient_span_gen gradSpanGen( gradSpan, inter, 
                                                  gradFunc, gradColors, 
                                                  Global::bufferEmptyArea, 
                                                  Global::bufferEmptyArea + 
                                                  toolBarHeight );

            linear_gradient_renderer gradRend( baseRend, gradSpanGen );

            agg::render_scanlines( ras, sl, gradRend );
        }

        drawButtons( baseRend );

        rendered = true;
    }

    QPoint blendPos( toolBarLeftTopPos.x() - Global::bufferEmptyArea, 
                     toolBarLeftTopPos.y() - Global::bufferEmptyArea );

    blendBuffer( baseRenderer, toolBarPixf, blendPos );

}

/*****************************************************************************/

