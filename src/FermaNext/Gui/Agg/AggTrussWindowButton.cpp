
#include "AggTrussWindowButton.h"

/*****************************************************************************
 * Agg Truss Window Button
 *****************************************************************************/

AggTrussWindowButton::AggTrussWindowButton ( const QPoint& pos, 
                                             const QString& fname ) :
    windowHighlighted( true ),
    edgingCol( agg::rgba( 0, 0, 0, 0.7 ) ),
    normalCol( agg::rgba( 20, 60, 80, 0.5 ) ),
    highlightCol( agg::rgba( 1, 1, 1, 0.5 ) ), 
    pressedCol( agg::rgba( 65, 90, 110, 0.7 ) ),
    lineCol( agg::rgba( 10, 10, 10 ) )
{
    parseSvg( pathRend, fname.toLocal8Bit() );
    setPosition( pos );
    setWidth( Global::headWidth - 6 );
    setHeight( Global::headWidth - 6 );
}

AggTrussWindowButton::~AggTrussWindowButton ()
{}

/*
    Gets flag type to draw button highlight in colors, 
    corresponding to the truss unit window current 
    highlight status.
*/
void AggTrussWindowButton::setButtonHighlightType ( bool type )
{
    windowHighlighted = type;
}

void AggTrussWindowButton::paint ( ren_dynarow& baseRend ) const
{
    scanline_rasterizer ras;
    agg::scanline_p8 sl; 
    solidRenderer solidRend( baseRend );

    double scaleX = 0.13, scaleY = 0.13;
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
        barMiddleColor = agg::rgba( 1, 1, 1 );

    color_type barLastColor ( agg::rgba( 0, 0, 0 ) );

    if ( ! isPressed() )
    {
        agg::rounded_rect button ( topPos.x(), topPos.y(), bottomPos.x(), 
                                   bottomPos.y(), width / 4 );
        ras.add_path ( button );
        
        if ( isHighlighted() )
            if ( ! windowHighlighted )
                barMiddleColor = agg::rgba( 30, 30, 1 );
            else
                barMiddleColor = agg::rgba( 50, 1, 1 );
    }
    else
    {
        agg::rounded_rect button ( topPos.x(), topPos.y() + 2, bottomPos.x(), 
                                   bottomPos.y() + 1, width / 4 );
        ras.add_path ( button );
    }

    agg::trans_affine mtx;
    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;
    color_array_type gradColors;

    mtx *= agg::trans_affine_translation( 0, 1 );
    interpolator inter ( mtx );
    fillColorArray( gradColors, barFirstColor, barMiddleColor, barLastColor );
    linear_gradient_span_gen gradSpanGen ( gradSpan, inter, gradFunc, 
                                           gradColors, -height, 2*height );
    linear_gradient_renderer gradRend ( baseRend, gradSpanGen );

    agg::render_scanlines( ras, sl, gradRend );
    if( isPressed() )
    {
        scaleX = 0.12; 
        scaleY = 0.12;
        mtx *= agg::trans_affine_translation( 0, 11 );
    }
    QPoint iconPos( int(( topPos.x() + width / 6 ) / scaleX), 
                    int(( topPos.y() + 1 ) / scaleY) );
    drawSvg ( baseRend, ras, sl, pathRend, solidRend, mtx, 
              iconPos.x(), iconPos.y(), scaleX, scaleY, 
              Global::svgExpand, Global::svgGamma );
}

