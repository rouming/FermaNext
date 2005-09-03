
#include "AggSubsidiary.h"

/*****************************************************************************
 * line
 *****************************************************************************/

line::line ( double x1_, double y1_, double x2_, double y2_ ) : 
    x1( x1_ ), y1( y1_ ), 
    x2( x2_ ), y2( y2_ ), 
    f( 0 ) 
{}

void line::rewind ( unsigned )
{ f = 0; }

unsigned line::vertex ( double* x, double* y )
{
    if ( f == 0 ) { ++f; *x = x1; *y = y1; return agg::path_cmd_move_to; }
    if ( f == 1 ) { ++f; *x = x2; *y = y2; return agg::path_cmd_line_to; }
    return agg::path_cmd_stop;
}

/*****************************************************************************
 * arrow
 *****************************************************************************/

arrow::arrow ( line& l, double w ) : 
    s( l ),
    ah(),
    m( s.markers(), ah ),
    c( s, m )
{
    s.width(w); 
    ah.head(4, 4, 4, 6);
    s.shorten(w * 2.0);
}

void arrow::rewind ( unsigned id ) 
{ 
    c.rewind(id); 
}
    
unsigned arrow::vertex ( double* x, double* y ) 
{ 
    return c.vertex(x, y); 
}

/*****************************************************************************/

void drawText ( textRenderer& textRend, const QString& str, color_type col, 
                QPoint point )
{
    textRend.color( col );    
    textRend.render_text( point.x(), point.y(), str.ascii(), flipY );
}

void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                agg::scanline_p8& sl, QPoint point1, QPoint point2,
                int width, color_type col )
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width ( width ); 
    ras.add_path ( stroke );
    solidRend.color ( col );
    agg::render_scanlines ( ras, sl, solidRend );
}

void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                agg::scanline_p8& sl, QPoint point1, QPoint point2 )
{
    drawLine ( ras, solidRend, sl, point1, point2, 
               coordLineWidth, agg::rgba(90, 90, 90) );
}

void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, QPoint point1, QPoint point2 )
{
    line newLine ( point1.x(), point1.y(), point2.x(), point2.y() );
    arrow newArrow ( newLine, coordLineWidth );
    ras.add_path ( newArrow );
    solidRend.color ( agg::rgba(90, 90, 90) );
    agg::render_scanlines ( ras, sl, solidRend );
}

void drawOutlineRoundedRect ( solidRenderer& solidRend, scanline_rasterizer& ras,
                              agg::scanline_p8& sl, QPoint point1, QPoint point2, 
                              int cornerRadius, color_type color)
{
    agg::rounded_rect rectangle ( point1.x(), point1.y(), 
                                  point2.x(), point2.y(), cornerRadius );
    solidRend.color ( color );
    agg::conv_stroke<agg::rounded_rect> stroke(rectangle);
    stroke.width ( 1.0 );
    ras.add_path ( stroke );
    agg::render_scanlines(ras, sl, solidRend);
}
