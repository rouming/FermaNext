
#include "TrussUnitToolBar.h"

/*****************************************************************************
 * Agg Tool Bar Hide Button
 *****************************************************************************/

AggToolBarHideButton::AggToolBarHideButton () :
    AggButton ( QPoint(0,0), buttonWidth * 3, 6 ),
    fillCol( agg::rgba( 65, 90, 110, 0.5 ) ),
    lineCol( agg::rgba( 10, 10, 10 ) ), 
    highlightFill( agg::rgba( 1, 1, 1, 0.5 ) )
{}

AggToolBarHideButton::~AggToolBarHideButton ()
{}

void AggToolBarHideButton::paint ( ren_dynarow& baseRend, 
                                   scanline_rasterizer&,
                                   agg::scanline_p8&, 
                                   solidRenderer& ) const
{
    QPoint pos = getPosition ();
    int width = getWidth ();
    int height = getHeight ();

    agg::trans_affine mtx;
    primitivesRenderer primRend ( baseRend );

    if ( ! isHighlighted() )
        primRend.fill_color ( fillCol );
    else 
        primRend.fill_color ( highlightFill );

    primRend.line_color ( lineCol );
    primRend.outlined_rectangle ( pos.x(), pos.y(), pos.x() + width, pos.y() + height );

    QPoint pos1 ( pos.x() + width / 2 - 4, pos.y() + 2 );
    QPoint pos2 ( pos.x() + width / 2 + 4, pos.y() + 4 );
    if ( ! isHighlighted() )
        primRend.fill_color ( lineCol );
    else 
        primRend.fill_color (  agg::rgba( 120, 120, 120 ) );
    primRend.solid_rectangle ( pos1.x(), pos1.y(), pos2.x(), pos2.y() );

/*
    agg::rounded_rect button ( pos.x(), pos.y(), pos.x() + width, pos.y() + height, 
                               height / 2 );

    ras.add_path ( button );

    if ( ! isHighlighted() )
        solidRend.color ( fillCol );
    else 
        solidRend.color ( highlightFill );

    agg::render_scanlines( ras, sl, solidRend );
    QPoint pos2 ( pos.x() + width, pos.y() + height );
    drawOutlineRoundedRect ( solidRend, ras, sl, pos, pos2, 
                             height / 2, lineCol );
*/
}

/*****************************************************************************
 * Truss Unit Tool Bar
 *****************************************************************************/

TrussUnitToolBar::TrussUnitToolBar  ( QPoint pos, int bordLeft, int bordRight, 
                                      int bordTop, int bordBottom, int separ, 
                                      int rad ) :
    AggToolBar( pos, bordLeft, bordRight, bordTop, bordBottom, separ ),
    cornerRadius(rad),
    hideButton(0),
    enabled(true),
    timer( new QTimer( this ) ),
    // gradient colors
    barFirstColor ( agg::rgba( 35, 50, 60, 0.8 ) ),
    barMiddleColor ( agg::rgba( 20, 60, 80, 0.8 ) ),
    barLastColor ( agg::rgba( 20, 60, 80, 0.8 ) )
{
    initHideButton();
    QObject::connect( timer, SIGNAL( timeout() ),
                      SLOT( changeToolBarPosition() ) );    
}

TrussUnitToolBar::~TrussUnitToolBar ()
{
    delete timer;
    delete hideButton;
}

AggToolBarButton& TrussUnitToolBar::addButton ( const QString& fn, 
                                                const QString& l, 
                                                QPoint pos, 
                                                uint w,
                                                uint h,
                                                QObject* r,  
                                                const char* sig,
                                                const char* sl )
{
    AggToolBarButton& b = AggToolBar::addButton( fn, l, pos, w, h, r, sig, sl);
    if ( hideButton )
        hideButton->setPosition( hideButtonPos() );
    return b;
}

QPoint TrussUnitToolBar::hideButtonPos ()
{
    if ( hideButton == 0 )
        return QPoint(0,0);
    return QPoint( (getWidth() - hideButton->getWidth()) / 2 + bufferEmptyArea,
                   bufferEmptyArea - hideButton->getHeight() / 2 );
}

void TrussUnitToolBar::initHideButton ()
{
    hideButton = new AggToolBarHideButton ();
    hideButton->setPosition( hideButtonPos() );
    
    QObject::connect( hideButton, SIGNAL( onButtonHighlightChange() ),
                      SLOT( clearToolBarRenderedFlag() ) );

    QObject::connect( hideButton, SIGNAL( onChangeButtonState() ),
                      SLOT( clearToolBarRenderedFlag() ) );

    QObject::connect( hideButton, SIGNAL( onButtonPress() ),
                      SLOT( hideToolBar() ) );

    QObject::connect( hideButton, SIGNAL( onButtonRelease() ),
                      SLOT( showToolBar() ) );

    setRendered( false );
}

void TrussUnitToolBar::showToolBar ()
{
    removeButtonHighlight();
    enabled = false;
    setVisible ( true );
    timer->start( 20, false );
}

void TrussUnitToolBar::hideToolBar ()
{
    removeButtonHighlight();
    enabled = false;
    topPos = getPosition ();
    timer->start( 20, false );
}

void TrussUnitToolBar::changeToolBarPosition ()
{
    QPoint pos = getPosition();
    if ( hideButton->isPressed() )
    {
        pos.setY ( pos.y() + 2 );
        if ( pos.y() == getCenterPoint().y() - hideButton->getHeight() )
        {
            timer->stop();
            setVisible ( false );
            enabled = true;
        }
    }
    else
    {
        pos.setY ( pos.y() - 2 );
        if ( pos.y() == topPos.y() )
        {
            timer->stop();
            enabled = true;
        }
    }
    setPosition ( pos );
}

void TrussUnitToolBar::removeButtonHighlight ()
{
    if ( hideButton )
        hideButton->setHighlighted( false );

    ButtonList buttons = getButtonList();
    if ( buttons.empty() )
        return;

    ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
        (*iter)->setHighlighted( false );
}

QPoint TrussUnitToolBar::getDynarowBufPos ( int x, int y ) const
{
    // get inner buffer coords from widget coords
    QPoint pos = getPosition ();
    pos.setX ( x - pos.x() + bufferEmptyArea );
    pos.setY ( y - pos.y() + bufferEmptyArea );
    return pos;
}

bool TrussUnitToolBar::inToolBarRect ( int x, int y ) const
{
    // consider that buttons know only about dynarow buffer coords
    QPoint bufPos = getDynarowBufPos ( x, y );

    //TODO: define more accurately rounded boundaries of the tool bar
    QPoint pos = getPosition ();
    if ( x > pos.x() && x < pos.x() + getWidth() &&
         y > pos.y() && y < pos.y() + getHeight() ||
         hideButton && hideButton->inButtonRect( bufPos.x(), bufPos.y() ) )
         return true;
    return false;
}

void TrussUnitToolBar::checkMouseMoveEvent ( int x, int y )
{
    if ( ! enabled )
        return;

    removeButtonHighlight();

    AggToolBarButton* button = getSelectedButton( x, y );
    if ( button )
        button->setHighlighted( true );

    // consider that buttons know only about dynarow buffer coords
    QPoint pos = getDynarowBufPos ( x, y );

    if ( hideButton && hideButton->inButtonRect( pos.x(), pos.y() ) )
        hideButton->setHighlighted( true );
}

void TrussUnitToolBar::checkMousePressEvent ( int x, int y )
{
    if ( ! enabled )
        return;

    AggToolBarButton* button = getSelectedButton( x, y );
    if ( button )
        button->setPressed( true );

    // consider that buttons know only about dynarow buffer coords
    QPoint pos = getDynarowBufPos ( x, y );

    if ( hideButton && hideButton->inButtonRect( pos.x(), pos.y() ) )
    {
        if ( hideButton->isPressed() )
            hideButton->setPressed( false );
        else
            hideButton->setPressed( true );
    }
}

AggToolBarButton* TrussUnitToolBar::getSelectedButton ( int x, int y ) const
{
    ButtonList buttons = getButtonList ();
    if ( buttons.empty() )
        return 0;

    // consider that buttons know only about dynarow buffer coords
    QPoint bufPos = getDynarowBufPos ( x, y );

    AggToolBar::ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
    {
        AggToolBarButton* button = *iter;
        if ( button->inButtonRect( bufPos.x(), bufPos.y() ) )
            return button;
    }
    return 0;
}

void TrussUnitToolBar::drawButtons ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                                     agg::scanline_p8& sl, solidRenderer& solidRend ) const
{
    if ( hideButton )
        hideButton->paint( baseRend, ras, sl, solidRend );

    if ( ! isVisible() )
        return;

    ButtonList buttons = getButtonList ();
    if ( buttons.empty() )
        return;

    agg::trans_affine mtx;

    AggToolBar::ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
    {
        AggToolBarButton* button = *iter;
        button->paint ( baseRend, ras, sl, solidRend, mtx, 0.2, 0.2 );
    }
}

void TrussUnitToolBar::paint ( base_renderer& baseRenderer ) const
{
    pixf_dynarow toolBarPixf( getRenderingBuffer() ); 
    ren_dynarow baseRend( toolBarPixf );
    solidRenderer solidRend ( baseRend );

    if ( ! isRendered () )
    {
        baseRend.clear ( agg::rgba( 150, 150, 150, 0 ) );

        scanline_rasterizer ras;
        agg::scanline_p8 sl;

        if ( isVisible() )
        {
            agg::trans_affine mtx;
            gradient_span_alloc gradSpan;
            linear_gradient gradFunc;
            color_array_type gradColors;

            mtx *= agg::trans_affine_translation( 0, -10 );
            interpolator inter ( mtx );

            agg::rounded_rect bar ( bufferEmptyArea, bufferEmptyArea, 
                                    bufferEmptyArea + getWidth(), 
                                    bufferEmptyArea + getRenderingBuffer().height(), 
                                    cornerRadius );
            ras.add_path ( bar );

            unsigned i;
            for(i = 0; i < 128; ++i)
            {
                gradColors[i] = barFirstColor.gradient ( barMiddleColor, i / 128.0 );
            }
            for(; i < 256; ++i)
            {
                gradColors[i] = barMiddleColor.gradient ( barLastColor, (i - 128) / 128.0 );
            }
            linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, gradColors, 
                                                   bufferEmptyArea, 
                                                   bufferEmptyArea + getHeight() );
            linear_gradient_renderer gradRend ( baseRend, gradSpanGen );

            agg::render_scanlines( ras, sl, gradRend );
        }

        drawButtons ( baseRend, ras, sl, solidRend );

        setRendered ( true );
    }

    QPoint pos = getPosition ();
    baseRenderer.blend_from ( toolBarPixf, 0, pos.x() - bufferEmptyArea, 
                              pos.y() - bufferEmptyArea, unsigned(1.0 * 255) );

}

/*****************************************************************************/

