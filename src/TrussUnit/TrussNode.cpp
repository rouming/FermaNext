
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ()
{}

void TrussNode::paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                       int trussAreaHeight ) const
{
    int x = getX() * scaleMultX + leftWindowIndent;
    int y = flipY ? ( trussAreaHeight - getY() ) * scaleMultY + topWindowIndent :
                    getY() * scaleMultY + topWindowIndent;

    solidRenderer solidRend ( baseRend );

    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;

    int highlightKoeff = 0;
    if ( isHighlighted () )
    {
        highlightKoeff = 1;
    }
    solidRend.color ( agg::rgba(10, 10, 10) );
    ell.init ( x, y, nodesRadius + highlightKoeff + 1, nodesRadius + highlightKoeff + 1, 16 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );
    if ( isHighlighted () )
    {
        solidRend.color ( agg::rgba(200, 135, 15, 0.45) );
        ell.init ( x, y, nodesRadius + 5, nodesRadius + 5, 16 );
        ras.add_path ( ell );
        agg::render_scanlines ( ras, sl, solidRend );
    }
    ell.init ( x, y, nodesRadius + highlightKoeff, nodesRadius + highlightKoeff, 16 );
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
    mtx *= agg::trans_affine_translation ( x, y );
    mtx.invert ();
    interpolator inter(mtx);
    radial_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, 
                                           gradColors, 0.0, 10.0 );
    radial_gradient_renderer gradRend ( baseRend, gradSpanGen );
    ras.add_path(ell);
    agg::render_scanlines ( ras, sl, gradRend );
}

/****************************************************************************/
