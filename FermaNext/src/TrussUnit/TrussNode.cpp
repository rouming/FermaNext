
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ()
{}

TrussNode::TrussNode ( const TrussNode& n ) :
    Node(n),
    PaintableTrussElement()
{}

int TrussNode::getRadius () const
{
    return radius;
}

int TrussNode::getXWidgetPosition () const
{
    return xPos;
}

int TrussNode::getYWidgetPosition () const
{
    return yPos;
}

void TrussNode::setXWidgetPosition ( int x )
{
    xPos = x;
}

void TrussNode::setYWidgetPosition ( int y )
{
    yPos = y;
}

void TrussNode::setRadius ( int rad )
{
    radius = rad;
}

void TrussNode::paint ( base_renderer& baseRend, solid_renderer& solidRend,
                        text_renderer&, scanline_rasterizer& ras, 
                        agg::scanline_p8& sl, agg::ellipse& ell ) const
{
    int k = 0;
    if ( isHighlighted () )
        k = 1;
    solidRend.color ( agg::rgba(10, 10, 10) );
    ell.init ( xPos, yPos, radius + k + 1, radius + k + 1, 16 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );
    if ( isHighlighted () )
    {
        solidRend.color ( agg::rgba(200, 35, 15, 0.5) );
        ell.init ( xPos, yPos, radius + 5, radius + 5, 16 );
        ras.add_path ( ell );
        agg::render_scanlines ( ras, sl, solidRend );
    }
    ell.init ( xPos, yPos, radius + k, radius + k, 16 );
    gradient_span_alloc gradSpan;
    radial_gradient gradFunc;
    color_array_type gradColors;
    color_type begin ( 250, 250, 230 ); 
    color_type middle ( 220, 190, 180 ); 
    color_type end ( 170 - k * 100, 120 - k * 60, 110 - k * 50 );
    unsigned i;
    for(i = 0; i < 128; ++i)
    {
        gradColors[i] = begin.gradient ( middle, i / 128.0 );
    }
    for(; i < 256; ++i)
    {
        gradColors[i] = middle.gradient ( end, (i - 128) / 128.0 );
    }
    agg::trans_affine mtx;
    mtx *= agg::trans_affine_scaling ( 1 / 2.0 );
    mtx *= agg::trans_affine_translation ( xPos, yPos );
    mtx.invert ();
    interpolator inter(mtx);
    radial_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, 
                                           gradColors, 0.0, 10.0 );
    radial_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, gradRend );
}

/****************************************************************************/
