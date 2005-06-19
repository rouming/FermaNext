
#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"
#include <vector>
#include <qstring.h>
#include <qpoint.h> 

#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgb.h"
#include "agg_gamma_lut.h"
#include "agg_ellipse.h"
#include "agg_rounded_rect.h"
#include "agg_conv_stroke.h"
#include "agg_conv_contour.h"
#include "agg_conv_marker.h"
#include "agg_conv_shorten_path.h"
#include "agg_conv_marker_adaptor.h"
#include "agg_conv_concat.h"
#include "agg_arrowhead.h"
#include "agg_vcgen_markers_term.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_embedded_raster_fonts.h"
#include "agg_rendering_buffer_dynarow.h"
#include "agg_pixfmt_rgba.h"
#include <agg_rendering_buffer.h>

typedef agg::rasterizer_scanline_aa<>                       scanline_rasterizer;
typedef agg::gamma_lut<agg::int8u, agg::int8u, 8, 8>        gamma_lut_type;
typedef agg::pixfmt_rgb24                                   pixfmt;
typedef pixfmt::color_type                                  color_type;
typedef agg::glyph_raster_bin<agg::rgba8> glyph_gen;

typedef agg::rendering_buffer_dynarow<4> rbuf_dynarow;

//typedef agg::pixfmt_custom_rbuf_rgba<pixfmt, rbuf_dynarow> pixf_dynarow;
typedef agg::pixfmt_custom_rbuf_rgba<agg::blender_bgra32, rbuf_dynarow> pixf_dynarow;
typedef agg::renderer_base<pixf_dynarow> ren_dynarow;


typedef agg::renderer_base<pixfmt>                          base_renderer;
//typedef agg::renderer_scanline_aa_solid<base_renderer>      solid_renderer;
//typedef agg::renderer_raster_htext_solid<base_renderer, glyph_gen> text_renderer;

typedef agg::renderer_scanline_aa_solid<ren_dynarow>  solidRenderer;
typedef agg::renderer_raster_htext_solid<ren_dynarow, glyph_gen> textRenderer;


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
typedef agg::renderer_scanline_aa<base_renderer, 
                                  radial_gradient_span_gen> radial_gradient_renderer;
typedef agg::renderer_scanline_aa<base_renderer, 
                                  linear_gradient_span_gen> linear_gradient_renderer;


typedef agg::renderer_scanline_aa<ren_dynarow, linear_gradient_span_gen> linearGradientRenderer;


typedef agg::rendering_buffer          rendering_buffer;


class TrussUnitWindow : public TrussUnit
{
public:     
    TrussUnitWindow ( const QString& name );
    virtual ~TrussUnitWindow ();

    QPoint getPoint1 () const;
    QPoint getPoint2 () const;
    QPoint getTrussAreaPoint1 () const;
    QPoint getTrussAreaPoint2 () const;
    QPoint transCoord ( QPoint point, bool flipY );
    int getHeadlineWidth () const;
    int getBorderWidth () const;
    int getMinResizeVal () const;
    bool inWindowRect ( int x, int y );
    bool inCanvasRect ( int  x, int  y );
    bool inHeadlineRect ( int  x, int  y );
    bool inHorResizeRect ( int x, int y );
    bool inVerResizeRect ( int x, int y );
    bool inBDiagResizeRect ( int x, int y );
    bool inFDiagResizeRect ( int x, int y );
    bool inNodeRadius ( int x, int y );
    bool isPivotSelected ( int x, int y );
    TrussNode* findNodeByCoord ( int x, int y );
    TrussPivot* findPivotByCoord ( int x, int y );
    void setPosition ( QPoint point1, QPoint point2 );    
    void setTrussAreaWindowIndent ( int indent );
    void setTrussNodesPosition ();
    void setNodeHighlight ( int x, int y );
    void setPivotHighlight ( int x, int y );
    void removeNodeHighlight ();
    void removePivotHighlight ();

    color_type getCanvasColor () const;
	color_type getHeadlineColor () const;
	color_type getBorderColor () const;
    void setCoordinateLineWidth ( int width );
    void setHeadlineWidth ( int  width );
    void setBorderWidth ( int  width );
    void setMinResizeVal ( int value );
    void setCanvasColor ( int r, int g, int b );
    void setHeadlineColor ( int r, int g, int b );
    void setBorderColor ( int r, int g, int b );
    void setResEllRad ( int radius );
    void setWinCornerRadius ( int radius );
    void setResEllColor ( int r, int g, int b );

    void drawText ( ren_dynarow& baseRend, textRenderer& textRend,
                    const QString& str, color_type c, 
                    QPoint point, bool flipY ) const;
    void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                    agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const;
    void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                     agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const;
    void drawOutlineRoundedRect ( solidRenderer& solidRend, 
                                  scanline_rasterizer& ras,
                                  agg::scanline_p8& sl, 
                                  QPoint point1, QPoint point2, 
                                  int cornerRadius,
                                  color_type color) const;
    void drawTrussArea ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                         textRenderer& textRend, solidRenderer& solidRend, 
                         agg::scanline_p8& sl ) const;
    void drawHeadline ( ren_dynarow& baseRend, solidRenderer& solidRend,
                        scanline_rasterizer& ras, agg::scanline_p8& sl, 
                        gradient_span_alloc& gradSpan, linear_gradient& gradFunc, 
                        color_array_type& gradColors, agg::trans_affine& mtx ) const;
    void paint ( base_renderer& baseRend ) const;
private:
    rbuf_dynarow* rbuf;
    int  headW, bordW, resEllRad, winCornerRadius, 
         minResizeVal, lineWidth, trussAreaIndent;
    QPoint _point1, _point2;
    QPoint leftTopPos, rightBottomPos;
    color_type canvColor, headColor, borderColor, resEllColor;    
};

#endif //TRUSSUNITWINDOW_H
