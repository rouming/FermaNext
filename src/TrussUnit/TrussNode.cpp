
#include "TrussUnit.h"

/*****************************************************************************
 * Truss Node
 *****************************************************************************/

TrussNode::TrussNode ( ObjectStateManager* mng ) :
    Node(mng)
{}

TrussNode::TrussNode ( int x, int y, ObjectStateManager* mng ) :
    Node(x, y, mng)
{}

TrussNode::TrussNode ( int x, int y, Fixation fix, ObjectStateManager* mng ) :
    Node(x, y, fix, mng)
{}

struct line
{
    double x1, y1, x2, y2;
    int f;

    line(double x1_, double y1_, double x2_, double y2_) : 
        x1(x1_), y1(y1_), x2(x2_), y2(y2_), f(0) {}

    void rewind(unsigned) { f = 0; }
    unsigned vertex(double* x, double* y)
    {
        if(f == 0) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
        if(f == 1) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
        return agg::path_cmd_stop;
    }
};

void TrussNode::drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                          agg::scanline_p8& sl, QPoint point1, QPoint point2,
                          int width, color_type color ) const
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( width ); 
    ras.add_path ( stroke );
    solidRend.color ( color );
    agg::render_scanlines ( ras, sl, solidRend );
}

void TrussNode::drawFixation ( scanline_rasterizer& ras, solidRenderer& solidRend, 
                               agg::scanline_p8& sl, int trussAreaHeight,
                               double scaleMultX, double scaleMultY,
                               int lineWidth, color_type color ) const
{
    QPoint leftPnt, rightPnt, nodePos = getPoint();
    nodePos.setX ( int(nodePos.x() * scaleMultX) + leftWindowIndent );
    nodePos.setY ( flipY ? int(( trussAreaHeight - nodePos.y() ) * scaleMultY) + 
                  topWindowIndent : int(nodePos.y() * scaleMultY) + topWindowIndent );

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

void TrussNode::drawGradientEllipse ( scanline_rasterizer& ras, ren_dynarow& baseRend, 
                                     agg::scanline_p8& sl, int x, int y, int radius, 
                                     color_type begin, color_type middle, 
                                     color_type end ) const
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
                       int trussAreaHeight ) const
{
    if ( !isVisible() )
        return;

    QPoint pos = getPoint();
    int x = int(pos.x() * scaleMultX) + leftWindowIndent;
    int y = flipY ? int(( trussAreaHeight - pos.y() ) * scaleMultY) + topWindowIndent :
                    int(pos.y() * scaleMultY) + topWindowIndent;

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
