
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ( ObjectStateManager* mng ) :
    Node(mng)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removeNodeHighlight()) );   
}

TrussNode::TrussNode ( double x, double y, ObjectStateManager* mng ) :
    Node(x, y, mng)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removeNodeHighlight()) );   
}

TrussNode::TrussNode ( double x, double y, Fixation fix, 
                       ObjectStateManager* mng ) :
    Node(x, y, fix, mng)
{
    QObject::connect( this, SIGNAL(onAfterDesist( StatefulObject& )),
                            SLOT(removeNodeHighlight()) );   
}

void TrussNode::removeNodeHighlight ()
{
    setHighlighted ( false );
}

void TrussNode::drawFixation ( scanline_rasterizer& ras, solidRenderer& solidRend, 
                               agg::scanline_p8& sl, double trussAreaHeight,
                               double scaleMultX, double scaleMultY,
                               int lineWidth, color_type color ) const
{
    DoublePoint nodeCoord = getPoint();
    QPoint leftPnt, rightPnt, nodePos;
    nodePos.setX ( int( nodeCoord.x() * scaleMultX ) + leftWindowIndent );
    nodePos.setY ( flipY ? int( ( trussAreaHeight - nodeCoord.y() ) * scaleMultY ) + 
                  topWindowIndent : int( nodeCoord.y() * scaleMultY + topWindowIndent ) );

    if ( getFixation() == FixationByX )
    {
        leftPnt.setX ( nodePos.x() - 4 );
        leftPnt.setY ( nodePos.y() - 8 );
        rightPnt.setX ( nodePos.x() - 4 );
        rightPnt.setY ( nodePos.y() + 8 );
        drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );
    
        leftPnt.setX ( leftPnt.x() - 4 );
        leftPnt.setY ( leftPnt.y() - 4 );
        rightPnt.setX ( rightPnt.x() - 1 );
        rightPnt.setY ( leftPnt.y() + 3 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setY ( rightPnt.y() + 4 );
            leftPnt.setY ( leftPnt.y() + 4 );
            drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth - 1, color );
        }
    }

    if ( getFixation() == FixationByY )
    {
        leftPnt.setX ( nodePos.x() - 8 );
        leftPnt.setY ( nodePos.y() + 4 );
        rightPnt.setX ( nodePos.x() + 9 );
        rightPnt.setY ( nodePos.y() + 4 );
        drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );
    
        leftPnt.setX ( nodePos.x() - 11 );
        leftPnt.setY ( nodePos.y() + 8 );
        rightPnt.setX ( nodePos.x() - 8 );
        rightPnt.setY ( nodePos.y() + 5 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setX ( rightPnt.x() + 4 );
            leftPnt.setX ( leftPnt.x() + 4 );
            drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth - 1, color );
        }
    }

    if ( getFixation() == FixationByXY )
    {
        leftPnt.setX ( nodePos.x() - 4 );
        leftPnt.setY ( nodePos.y() + 9 );
        rightPnt.setX ( nodePos.x() + 4 );
        rightPnt.setY ( nodePos.y() + 9 );
        drawLine ( ras, solidRend, sl, nodePos, leftPnt, lineWidth, color );
        drawLine ( ras, solidRend, sl, nodePos, rightPnt, lineWidth, color );
    
        leftPnt.setX ( leftPnt.x() - 4 );
        rightPnt.setX ( rightPnt.x() + 5 );
        drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth, color );

        rightPnt.setX ( leftPnt.x() );
        rightPnt.setY ( leftPnt.y() );
        leftPnt.setX ( leftPnt.x() - 4 );
        leftPnt.setY ( leftPnt.y() + 4 );
        uint i;
        for ( i = 0; i < 4; i++ )
        {
            rightPnt.setX ( rightPnt.x() + 4 );
            leftPnt.setX ( leftPnt.x() + 4 );
            drawLine ( ras, solidRend, sl, leftPnt, rightPnt, lineWidth - 1, color );
        }
    }
}

void TrussNode::drawGradientEllipse ( scanline_rasterizer& ras, 
                                      ren_dynarow& baseRend, 
                                      agg::scanline_p8& sl, double x, double y,
                                      int radius, color_type begin, 
                                      color_type middle, color_type end ) const
{
    agg::ellipse ell;
    ell.init ( x, y, radius, radius, 16 );
    gradient_span_alloc gradSpan;
    radial_gradient gradFunc;
    color_array_type gradColors;

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


void TrussNode::paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                       double trussAreaHeight ) const
{
    if ( !isVisible() )
        return;

    DoublePoint pos = getPoint();
    int x = (pos.x() * scaleMultX) + leftWindowIndent;
    int y = flipY ? 
              ( trussAreaHeight - pos.y() ) * scaleMultY + topWindowIndent :
                pos.y() * scaleMultY + topWindowIndent;

    solidRenderer solidRend ( baseRend );
    scanline_rasterizer   ras;
    agg::scanline_p8     sl;
    agg::ellipse ell;

    int highlightKoeff = 0;
    if ( isHighlighted () )
    {
        highlightKoeff = 1;
    }

//  draw node' outline
    solidRend.color ( agg::rgba(10, 10, 10) );
    ell.init ( x, y, nodesRadius + highlightKoeff + 1, 
                     nodesRadius + highlightKoeff + 1, 16 );
    ras.add_path ( ell );
    agg::render_scanlines ( ras, sl, solidRend );

//  draw fixation
    if ( getFixation () )
        drawFixation ( ras, solidRend, sl, trussAreaHeight, 
                       scaleMultX, scaleMultY, 2, agg::rgba(0, 0, 35) ); 

    if ( isHighlighted () )
    {
        // draw translucent highlight
        solidRend.color ( agg::rgba(200, 135, 15, 0.45) );
        ell.init ( x, y, nodesRadius + 5, nodesRadius + 5, 16 );
        ras.add_path ( ell );
        agg::render_scanlines ( ras, sl, solidRend );
    }

//  draw node
    if ( getFixation() == Unfixed )
    {
        color_type begin ( 255, 255, 255 ); 
        color_type middle ( 230, 200, 195 ); 
        color_type end ( 130 - highlightKoeff * 100, 
                        90 - highlightKoeff * 60, 
                        70 - highlightKoeff * 50 );
        drawGradientEllipse ( ras, baseRend, sl, x, y, nodesRadius + highlightKoeff, 
                             begin, middle, end );
    }
    else 
    {
        color_type begin ( 255, 255, 255 ); 
        color_type middle ( 80, 100, 195 ); 
        color_type end ( 0, 0, 80 );
        drawGradientEllipse ( ras, baseRend, sl, x, y, nodesRadius + highlightKoeff, 
                             begin, middle, end );
    }
}

/****************************************************************************/
