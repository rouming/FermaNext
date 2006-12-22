
#include <QString>
#include <QPoint>
#include <QSize>

#include "AggSubsidiary.h"
#include "Geometry.h"

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
    s.width( w ); 
    ah.head( 4, 1, 4, 6 );
    s.shorten( w * 2.0 );
}

arrow::arrow ( line& l, double w, double downLen, 
               double upLen, double sideWid, double sideLen ) : 
    s( l ),
    ah(),
    m( s.markers(), ah ),
    c( s, m )
{
    s.width( w ); 
    ah.head( downLen, upLen, sideWid, sideLen );
    s.shorten( w * 2.0 );
}

void arrow::rewind ( unsigned id ) 
{ 
    c.rewind(id); 
}
    
unsigned arrow::vertex ( double* x, double* y ) 
{ 
    return c.vertex(x, y); 
}

/*****************************************************************************
 * Dashed Stroke
 *****************************************************************************/

template<class Source> struct DashStroke
{
    typedef agg::conv_dash<Source>       DashType;
    typedef agg::conv_stroke<DashType>   StrokeType;

    DashType      dash;
    StrokeType    stroke;

    DashStroke( Source& src, double dashLen, double gapLen, double width ) : 
        dash( src ),
        stroke( dash )
    {
        dash.add_dash( dashLen, gapLen );
        stroke.width( width );
    }

    void rewind( unsigned path_id ) { stroke.rewind( path_id ); }
    unsigned vertex( double* x, double* y ) { return stroke.vertex( x, y ); }
};

/*****************************************************************************/

QString fitTextToWidth ( QString str, int pixWidth, glyph_gen& glyph )
{
    int strLength = str.length();
    for ( int i = 0; i < strLength; ++i )
    {
        int textLength = (int)glyph.width( str.toAscii().data() ); 

        if ( textLength <= pixWidth )
            break;

        else {
            str.replace (  str.length() - 3, 3, "..." );
            str.remove ( str.length() - 4, 1 );
        }
    }
    return str;
}

void drawText ( textRenderer& textRend, const QString& str, color_type col, 
                const DoublePoint& point )
{
    textRend.color( col );    
    textRend.render_text( point.x(), point.y(), str.toAscii().data(), 
                          Global::flipY );
}

void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                agg::scanline_p8& sl, const DoublePoint& point1, 
                const DoublePoint& point2, double width, color_type col )
{
    line newLine( point1.x(), point1.y(), point2.x(), point2.y() );
    agg::conv_stroke<line> stroke ( newLine );
    stroke.width( width ); 
    ras.add_path( stroke );
    solidRend.color( col );
    agg::render_scanlines( ras, sl, solidRend );
}

void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                agg::scanline_p8& sl, const DoublePoint& point1, 
                const DoublePoint& point2 )
{
    drawLine( ras, solidRend, sl, point1, point2, 
              Global::coordLineWidth, agg::rgba(90, 90, 90) );
}

void drawDashLine ( ren_dynarow& base, DoublePoint point1,
                    DoublePoint point2, color_type col, double width,
                    double dashLen, double dashGap, bool draft /* = false */ )
{
    line newLine( point1.x(), point1.y(), point2.x(), point2.y() );
    DashStroke<line> dashLine( newLine, dashLen, dashGap, width );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    ras.add_path( dashLine );
    if ( ! draft ) {
        solidRenderer rend( base );
        rend.color( col );
        agg::render_scanlines( ras, sl, rend );
    }
    else {
        draft_renderer rend( base );
        rend.color( col );
        agg::render_scanlines( ras, sl, rend );
    }
    
}

void drawCurve ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, const DoublePoint& point1, 
                 const DoublePoint& point2, const DoublePoint& point3, 
                 double width, color_type col )
{
    agg::curve3 newCurve( point1.x(), point1.y(), 
                          point2.x(), point2.y(),
                          point3.x(), point3.y() );
    agg::conv_stroke<agg::curve3> stroke( newCurve );
    stroke.width( width ); 
    ras.add_path( stroke );
    solidRend.color( col );
    agg::render_scanlines( ras, sl, solidRend );
}

void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, const DoublePoint& tail, 
                 const DoublePoint& head,  color_type color, 
                 double downLen, double upLen, double sideWid, 
                 double sideLen )
{
    line newLine ( tail.x(), tail.y(), head.x(), head.y() );
    arrow newArrow ( newLine, Global::coordLineWidth, 
                     downLen, upLen, sideWid, sideLen );
    ras.add_path ( newArrow );
    solidRend.color ( color );
    agg::render_scanlines ( ras, sl, solidRend );
}

void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, const DoublePoint& tail, 
                 const DoublePoint& head )
{
    drawArrow( ras, solidRend, sl, tail, head, 
               agg::rgba(90, 90, 90), 4, 1, 4, 6 );
}

void fillColorArray ( color_array_type& colArray, color_type first, 
                      color_type middle, color_type last )
{
    uint i = 0;
    for( ; i < 128; ++i)
        colArray[i] = first.gradient ( middle, i / 128.0 );
    for( ; i < 256; ++i)
        colArray[i] = middle.gradient ( last, (i - 128) / 128.0 );
}

void drawOutlineRoundedRect ( ren_dynarow& baseRend,
                              solidRenderer& solidRend, 
                              scanline_rasterizer& ras,
                              agg::scanline_p8& sl, 
                              color_array_type& gradColors,
                              agg::trans_affine& mtx,
                              const DoublePoint& point1, 
                              const DoublePoint& point2, 
                              color_type outlineCol, 
                              double cornerRadius, 
                              double lineWidth, 
                              double bound1, double bound2 )
{
    gradient_span_alloc gradSpan;
    linear_gradient gradFunc;

    agg::rounded_rect outline( point1.x(), point1.y(), 
                               point2.x(), point2.y(), cornerRadius );
    ras.add_path( outline );
    solidRend.color( outlineCol );
    agg::render_scanlines( ras, sl, solidRend );

    agg::rounded_rect rect( point1.x() + lineWidth, point1.y() + lineWidth, 
                            point2.x() - lineWidth, point2.y() - lineWidth, 
                            cornerRadius );
    ras.add_path( rect );

    interpolator inter( mtx );
    linear_gradient_span_gen gradSpanGen( gradSpan, inter, gradFunc, 
                                          gradColors, bound1, bound2 );
    linear_gradient_renderer gradRend( baseRend, gradSpanGen );
    agg::render_scanlines( ras, sl, gradRend );
}

void drawOutlineRoundedRect ( solidRenderer& solidRend, 
                              scanline_rasterizer& ras,
                              agg::scanline_p8& sl,
                              const DoublePoint& point1, 
                              const DoublePoint& point2,
                              color_type outlineCol, 
                              color_type rectCol,
                              double cornerRadius, 
                              double lineWidth )
{
    agg::rounded_rect outline( point1.x(), point1.y(), 
                               point2.x(), point2.y(), cornerRadius );
    ras.add_path( outline );
    solidRend.color( outlineCol );
    agg::render_scanlines( ras, sl, solidRend );

    agg::rounded_rect rect( point1.x() + lineWidth, point1.y() + lineWidth, 
                            point2.x() - lineWidth, point2.y() - lineWidth, 
                            cornerRadius );
    ras.add_path( rect );
    solidRend.color( rectCol );
    agg::render_scanlines( ras, sl, solidRend );
}

void drawGradientEllipse ( ren_dynarow& baseRend, 
                           scanline_rasterizer& ras,
                           agg::scanline_p8& sl, 
                           radial_gradient& gradFunc,
                           color_array_type gradColors,
                           agg::trans_affine& mtx, 
                           double x, double y, double radius, 
                           double bound1, double bound2 )
{
    agg::ellipse ell;
    ell.init( x, y, radius, radius, 16 );
    gradient_span_alloc gradSpan;
    interpolator inter( mtx );
    radial_gradient_span_gen gradSpanGen( gradSpan, inter, gradFunc, 
                                          gradColors, bound1, bound2 );
    radial_gradient_renderer gradRend( baseRend, gradSpanGen );
    ras.add_path( ell );
    agg::render_scanlines( ras, sl, gradRend );
}

void drawGradientEllipse ( ren_dynarow& baseRend, 
                           scanline_rasterizer& ras,
                           agg::scanline_p8& sl, 
                           linear_gradient& gradFunc,
                           color_array_type gradColors,
                           agg::trans_affine& mtx, 
                           double x, double y, double radius, 
                           double bound1, double bound2 )
{
    agg::ellipse ell;
    ell.init( x, y, radius, radius, 16 );
    gradient_span_alloc gradSpan;
    interpolator inter( mtx );
    linear_gradient_span_gen gradSpanGen( gradSpan, inter, gradFunc, 
                                          gradColors, bound1, bound2 );
    linear_gradient_renderer gradRend( baseRend, gradSpanGen );
    ras.add_path( ell );
    agg::render_scanlines( ras, sl, gradRend );
}

void drawTriangle ( ren_dynarow& baseRend, DoublePoint p1, DoublePoint p2, 
                    DoublePoint p3, color_type fillCol, color_type lineCol )
{
    solidRenderer solidRend( baseRend );
    scanline_rasterizer ras;
    agg::scanline_p8 sl;
    agg::trans_affine mtx;
    agg::path_storage path;

    path.move_to( p1.x(), p1.y() );
    path.line_to( p2.x(), p2.y() );
    path.line_to( p3.x(), p3.y() );
    path.close_polygon();

    solidRend.color( fillCol );
    ras.add_path( path );
    agg::render_scanlines( ras, sl, solidRend );

    drawLine( ras, solidRend, sl, p1, p2, 1.0, lineCol );
    drawLine( ras, solidRend, sl, p1, p2, 1.0, lineCol );
    drawLine( ras, solidRend, sl, p2, p3, 1.0, lineCol );
    
    //primitivesRenderer primRend( baseRend );
    //outline_rasterizer rasOutline( primRend );
    //primRend.line_color( lineCol );
    //rasOutline.add_path( path );    
}

void parseSvg ( pathRenderer& pathRend, const char* fname )
{
    agg::svg::parser svgParcer( pathRend );
    svgParcer.parse( fname );
    pathRend.arrange_orientations();
}

void drawSvg ( ren_dynarow& baseRend, scanline_rasterizer& ras, 
               agg::scanline_p8& sl, pathRenderer& pathRend, 
               solidRenderer& solidRend, agg::trans_affine mtx, double dx, 
               double dy,double scaleX, double scaleY, double expand, 
               double gamma )
{
    mtx *= agg::trans_affine_translation( dx, dy );
    mtx *= agg::trans_affine_scaling( scaleX, scaleY );
    pathRend.expand( expand );
    ras.gamma( agg::gamma_power( gamma ) );
    pathRend.render( ras, sl, solidRend, mtx, baseRend.clip_box(), 1.0 );
}

void blendBuffer ( base_renderer& baseRenderer, 
                   pixf_dynarow& pixf, QPoint p )
{
    baseRenderer.blend_from( pixf, 0, p.x(), p.y(), uint(1.0 * 255) );
}

agg::rgba8 aggColor ( uint r, uint g, uint b, uint a )
{
    return agg::rgba8( b, g, r, a );
}

/*****************************************************************************/
