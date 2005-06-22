
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode () :
    radius(5)
{}

int TrussNode::getRadius () const
{
    return radius;
}

QPoint TrussNode::getNodeWidgetPosition () const
{
    return widgetPosition;
}

void TrussNode::setNodeWidgetPosition ( QPoint point )
{
    widgetPosition = point;
}

void TrussNode::setRadius ( int rad )
{
    radius = rad;
}

void TrussNode::paint ( base_renderer& baseRend ) const
{
    solid_renderer solidRend ( baseRend );
    glyph_gen glyph(0);
    text_renderer textRend ( baseRend, glyph );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;
    glyph.font ( agg::verdana17_bold );

    int highlightKoeff = 0;
    if ( isHighlighted () )
    {
        highlightKoeff = 1;
    }
    solidRend.color ( agg::rgba(10, 10, 10) );
    ell.init ( widgetPosition.x(), widgetPosition.y(), 
               radius + highlightKoeff + 1, radius + highlightKoeff + 1, 16 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );
    if ( isHighlighted () )
    {
        solidRend.color ( agg::rgba(200, 135, 15, 0.45) );
        ell.init ( widgetPosition.x(), widgetPosition.y(), radius + 5, radius + 5, 16 );
        ras.add_path ( ell );
        agg::render_scanlines ( ras, sl, solidRend );
    }
    ell.init ( widgetPosition.x(), widgetPosition.y(), radius + highlightKoeff, 
                                                       radius + highlightKoeff, 16 );
    gradient_span_alloc gradSpan;
    radial_gradient gradFunc;
    color_array_type gradColors;
    color_type begin ( 255, 255, 255 ); 
    color_type middle ( 230, 200, 195 ); 
    color_type end ( 130 - highlightKoeff * 100, 
                     90 - highlightKoeff * 60, 
                     70 - highlightKoeff * 50 );
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
    mtx *= agg::trans_affine_translation ( widgetPosition.x(), widgetPosition.y() );
    mtx.invert ();
    interpolator inter(mtx);
    radial_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, 
                                           gradColors, 0.0, 10.0 );
    radial_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, gradRend );
}

/****************************************************************************/