
#ifndef AGGSUBSIDIARY_H
#define AGGSUBSIDIARY_H

#include "SubsidiaryConstants.h"
#include "agg_basics.h"
#include "agg_conv_marker.h"
#include "agg_conv_shorten_path.h"
#include "agg_conv_marker_adaptor.h"
#include "agg_conv_concat.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_smooth_poly1.h"
#include "agg_path_storage.h"
#include "agg_vertex_sequence.h"
#include "agg_vcgen_markers_term.h"
#include "agg_vcgen_smooth_poly1.h"
#include "agg_arrowhead.h"
#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_embedded_raster_fonts.h"
#include "agg_rounded_rect.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_pixfmt_rgba.h"
#include "agg_ellipse.h"
#include "agg_conv_stroke.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_rendering_buffer_dynarow.h"
#include "agg_svg_parser.h"
#include "agg_renderer_primitives.h"

/*****************************************************************************/

typedef agg::rendering_buffer_dynarow<4>                    rbuf_dynarow;
typedef agg::pixfmt_custom_rbuf_rgba<agg::blender_rgba32, 
                                          rbuf_dynarow>     pixf_dynarow;
typedef agg::renderer_base<pixf_dynarow>                    ren_dynarow;
typedef agg::renderer_scanline_aa_solid<ren_dynarow>        solidRenderer;
typedef agg::rasterizer_scanline_aa<>                       scanline_rasterizer;
typedef agg::pixfmt_rgb24                                   pixfmt;
typedef pixfmt::color_type                                  color_type;
typedef agg::renderer_base<pixfmt>                          base_renderer;
typedef agg::renderer_scanline_aa_solid<base_renderer>      solid_renderer;
typedef agg::gradient_circle                                radial_gradient;
typedef agg::gradient_y                                     linear_gradient;
typedef agg::span_interpolator_linear<>                     interpolator;
typedef agg::pod_auto_array<color_type, 256>                color_array_type;
typedef agg::span_gradient<color_type, interpolator, 
                           radial_gradient, 
                           color_array_type>                radial_gradient_span_gen;
typedef agg::span_gradient<color_type, interpolator, 
                           linear_gradient, 
                           color_array_type>                linear_gradient_span_gen;

typedef agg::span_allocator<color_type>                     gradient_span_alloc;

typedef agg::renderer_scanline_aa<ren_dynarow, 
                                  radial_gradient_span_gen> radial_gradient_renderer;
typedef agg::renderer_scanline_aa<ren_dynarow, 
                                  linear_gradient_span_gen> linear_gradient_renderer;

typedef agg::glyph_raster_bin<agg::rgba8>                   glyph_gen;
typedef agg::renderer_raster_htext_solid<ren_dynarow, 
                                         glyph_gen>         textRenderer;
typedef const agg::int8u*                                   textFont;
typedef agg::svg::path_renderer                             pathRenderer;
typedef agg::renderer_primitives<ren_dynarow>               primitivesRenderer;

/*****************************************************************************/

class QString;
class QPoint;
class QSize;

/*****************************************************************************/

struct line
{
    double x1, y1, x2, y2;
    int f;

    line ( double x1_, double y1_, double x2_, double y2_ );
    void rewind ( unsigned );
    unsigned vertex ( double* x, double* y );
};

/*****************************************************************************/

struct arrow
{
    typedef agg::conv_stroke<line, agg::vcgen_markers_term>                     stroke_type;
    typedef agg::conv_marker<stroke_type::marker_type, agg::arrowhead>          marker_type;
    typedef agg::conv_concat<stroke_type, marker_type>                          concat_type;

    stroke_type    s;
    agg::arrowhead ah;
    marker_type    m;
    concat_type    c;

    arrow ( line& l, double w );
    arrow ( line& l, double w, int downLen, int upLen, int sideWid, int sideLen );
    void rewind ( unsigned id );
    unsigned vertex ( double* x, double* y );
};

/*****************************************************************************/

void drawText ( textRenderer& textRend, const QString& str, 
                color_type c, const QPoint& point );

void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                agg::scanline_p8& sl, const QPoint& point1, 
                const QPoint& point2, int width, color_type col );

void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                agg::scanline_p8& sl, const QPoint& point1, 
                const QPoint& point2 );

void drawCurve ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, const QPoint& point1, 
                 const QPoint& point2, const QPoint& point3, 
                 int width, color_type col );

void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, const QPoint& tail, 
                 const QPoint& head, color_type color,
                 int downLen, int upLen, int sideWid, int sideLen );

void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                 agg::scanline_p8& sl, const QPoint& point1, 
                 const QPoint& point2 );

void fillColorArray ( color_array_type& colArray, color_type first, 
                      color_type middle, color_type last );

void drawOutlineRoundedRect ( ren_dynarow& baseRend,
                              solidRenderer& solidRend, 
                              scanline_rasterizer& ras,
                              agg::scanline_p8& sl,
                              color_array_type& gradColors,
                              agg::trans_affine& mtx,
                              const QPoint& point1, 
                              const QPoint& point2, 
                              color_type outlineCol, 
                              int cornerRadius, 
                              int lineWidth, 
                              int bound1, int bound2 );

void drawOutlineRoundedRect ( solidRenderer& solidRend, 
                              scanline_rasterizer& ras,
                              agg::scanline_p8& sl,
                              const QPoint& point1, 
                              const QPoint& point2, 
                              color_type outlineCol,
                              color_type rectCol,  
                              int cornerRadius, 
                              int lineWidth );

void drawGradientEllipse ( ren_dynarow& baseRend, 
                           scanline_rasterizer& ras,
                           agg::scanline_p8& sl, 
                           radial_gradient& gradFunc,
                           color_array_type gradColors,
                           agg::trans_affine& mtx, 
                           int x, int y, int radius, 
                           int bound1, int bound2 );

void drawGradientEllipse ( ren_dynarow& baseRend, 
                           scanline_rasterizer& ras,
                           agg::scanline_p8& sl, 
                           linear_gradient& gradFunc,
                           color_array_type gradColors,
                           agg::trans_affine& mtx, 
                           int x, int y, int radius, 
                           int bound1, int bound2 );

void parseSvg ( pathRenderer& pathRend, const char* fname );

void drawSvg ( ren_dynarow& baseRend, scanline_rasterizer& ras, 
               agg::scanline_p8& sl,  pathRenderer& pathRend, 
               solidRenderer& solidRend, agg::trans_affine mtx, 
               int dx, int dy, double scaleX, double scaleY, double expand, 
               double gamma );

#endif //AGGSUBSIDIARY_H
