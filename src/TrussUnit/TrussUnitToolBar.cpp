
#include "TrussUnitToolBar.h"

/*****************************************************************************
 * Truss Unit Tool Bar
 *****************************************************************************/

TrussUnitToolBar::TrussUnitToolBar  ( QPoint pos, int bordLeft, int bordRight, 
                                      int bordTop, int bordBottom, int separation, 
                                      int rad ) :
    AggToolBar( pos, bordLeft, bordRight, bordTop, bordBottom, separation ),
    cornerRadius ( rad ),
    // gradient colors
    barFirstColor ( agg::rgba( 35, 50, 60, 0.8 ) ),
    barMiddleColor ( agg::rgba( 20, 60, 80, 0.8 ) ),
    barLastColor ( agg::rgba( 20, 60, 80, 0.8 ) )
{}

TrussUnitToolBar::~TrussUnitToolBar ()
{}

bool TrussUnitToolBar::inToolBarRect ( int x, int y ) const
{
    //TODO: define more accurately rounded boundaries of the tool bar
    QPoint pos = getLeftTopPos ();
    if ( x > pos.x() && x < pos.x() + getToolBarWidth() &&
         y > pos.y() && y < pos.y() + getToolBarHeight() )
         return true;
    return false;
}

void TrussUnitToolBar::checkMouseMoveEvent ( int x, int y )
{
    removeButtonHighlight();

    if ( ! inToolBarRect ( x, y ) )
        return; 

    AggToolBarButton* button = getSelectedButton( x, y );
    if ( button )
        button->setHighlighted( true );
}

void TrussUnitToolBar::checkMousePressEvent ( int x, int y )
{
    AggToolBarButton* button = getSelectedButton( x, y );
    if ( button )
        button->setPressed( true );
}

AggToolBarButton* TrussUnitToolBar::getSelectedButton ( int x, int y ) const
{
    ButtonList buttons = getButtonList ();
    if ( buttons.empty() )
        return 0;

    QPoint pos = getLeftTopPos ();
    // consider that buttons know only about dynarow buffer coords
    int bufX = x - pos.x() + bufferEmptyArea;
    int bufY = y - pos.y() + bufferEmptyArea;

    AggToolBar::ButtonListIter iter = buttons.begin();
    for ( ; iter != buttons.end(); ++iter )
    {
        AggToolBarButton* button = *iter;
        if ( button->inButtonRect( bufX, bufY ) )
            return button;
    }
    return 0;
}

void TrussUnitToolBar::drawButtons ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                                     agg::scanline_p8& sl, solidRenderer& solidRend ) const
{
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
        agg::trans_affine mtx;
        gradient_span_alloc gradSpan;
        linear_gradient gradFunc;
        color_array_type gradColors;

        mtx *= agg::trans_affine_translation( 0, -10 );
        interpolator inter ( mtx );

        agg::rounded_rect bar ( bufferEmptyArea, bufferEmptyArea, 
                                bufferEmptyArea + getToolBarWidth(), 
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
                                               bufferEmptyArea + getToolBarHeight() );
        linear_gradient_renderer gradRend ( baseRend, gradSpanGen );

        agg::render_scanlines( ras, sl, gradRend );
        
        drawButtons ( baseRend, ras, sl, solidRend );

        setRendered ( true );
    }

    QPoint pos = getLeftTopPos ();
    baseRenderer.blend_from ( toolBarPixf, 0, pos.x() - bufferEmptyArea, 
                              pos.y() - bufferEmptyArea, unsigned(1.0 * 255) );

}

/*****************************************************************************/

