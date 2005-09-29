
#include "TrussUnitWindowButton.h"

/*****************************************************************************
 * Truss Unit Window Button
 *****************************************************************************/

TrussUnitWindowButton::TrussUnitWindowButton ( const QPoint& pos, 
                                               const QString& fname ) :
    AggButton ( pos, headWidth - 11, headWidth - 11 ),
    edgingCol( agg::rgba( 0, 0, 0, 0.7 ) ),
    normalCol( agg::rgba( 20, 60, 80, 0.5 ) ),
    highlightCol( agg::rgba( 1, 1, 1, 0.5 ) ), 
    pressedCol( agg::rgba( 65, 90, 110, 0.7 ) ),
    lineCol( agg::rgba( 10, 10, 10 ) ),
    windowHighlighted( true )
{
    parseSvg ( pathRend, fname.ascii() );    
}

TrussUnitWindowButton::~TrussUnitWindowButton ()
{}

void TrussUnitWindowButton::setWindowHighlightStatus ( bool status )
{
    windowHighlighted = status;
}

void TrussUnitWindowButton::paint ( ren_dynarow& baseRend, 
                                    scanline_rasterizer& ras,
                                    agg::scanline_p8& sl, 
                                    solidRenderer& solidRend ) const
{
    double scaleX = 0.15, scaleY = 0.15;
    int width = getWidth ();
    int height = getHeight ();
    QPoint topPos = getPosition ();
    QPoint bottomPos ( topPos.x() + width, topPos.y() + height );

    agg::rounded_rect edging ( topPos.x() - 1, topPos.y(), bottomPos.x() + 1, 
                               bottomPos.y() + 2, width / 4 );
    ras.add_path ( edging );
    solidRend.color ( edgingCol );
    agg::render_scanlines( ras, sl, solidRend );

    color_type barFirstColor ( agg::rgba( 0, 0, 0 ) );

    color_type barMiddleColor ( agg::rgba( 1, 15, 25 ) );
    if ( ! windowHighlighted )
        barMiddleColor = agg::rgba( 25, 35, 25 );

    color_type barLastColor ( agg::rgba( 0, 0, 0 ) );

    if ( ! isPressed() )
    {
        agg::rounded_rect button ( topPos.x(), topPos.y(), bottomPos.x(), 
                                   bottomPos.y(), width / 4 );
        ras.add_path ( button );
        
        if ( isHighlighted() )
            if ( ! windowHighlighted )
                barMiddleColor = agg::rgba( 1, 1, 1 );
            else
                barMiddleColor = agg::rgba( 70, 1, 1 );
    }
    else
    {
        agg::rounded_rect button ( topPos.x() + 1, topPos.y() + 2, bottomPos.x() - 1, 
                                   bottomPos.y(), width / 4 );
        ras.add_path ( button );
    }

    agg::trans_affine mtx;
    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;
    color_array_type gradColors;

    mtx *= agg::trans_affine_translation( 0, 1 );
    interpolator inter ( mtx );
    for( uint i = 0; i < 128; ++i)
    {
        gradColors[i] = barFirstColor.gradient ( barMiddleColor, i / 128.0 );
    }
    for(; i < 256; ++i)
    {
        gradColors[i] = barMiddleColor.gradient ( barLastColor, (i - 128) / 128.0 );
    }
    linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, gradColors, 
                                           -height, 2*height );
    linear_gradient_renderer gradRend ( baseRend, gradSpanGen );

    agg::render_scanlines( ras, sl, gradRend );

    QPoint iconPos( int(( topPos.x() + width / 6 ) / scaleX), 
                    int(( topPos.y() - 1 ) / scaleY) );
    drawSvg ( baseRend, ras, sl, pathRend, solidRend, mtx, iconPos.x(), 
              iconPos.y(), scaleX, scaleY, svgExpand, svgGamma );
}

