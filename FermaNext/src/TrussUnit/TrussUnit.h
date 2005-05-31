
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
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

typedef agg::rasterizer_scanline_aa<>                       scanline_rasterizer;
typedef agg::gamma_lut<agg::int8u, agg::int8u, 8, 8>        gamma_lut_type;
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
typedef agg::renderer_scanline_aa<base_renderer, 
                                  radial_gradient_span_gen> radial_gradient_renderer;
typedef agg::renderer_scanline_aa<base_renderer, 
                                  linear_gradient_span_gen> linear_gradient_renderer;
typedef agg::glyph_raster_bin<agg::rgba8> glyph_gen;
typedef agg::renderer_raster_htext_solid<base_renderer, 
                                         glyph_gen>         text_renderer;


class TrussNode;
class TrussPivot;

class PaintableTrussElement
{    
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool h, bool v );
    virtual void setVisible ( bool );
    virtual void setHighlighted ( bool );
    virtual bool isVisible () const;
    virtual bool isHighlighted () const;
    virtual void paint ( base_renderer& baseRend ) const = 0;
private:
    bool visible;
    bool highlighted; 
};

class TrussUnit : public Truss<TrussNode, TrussPivot>,                  
                  public PaintableTrussElement
{
    Q_OBJECT
private:    
    // constructor is closed. TrussUnitManager should manage trusses;
    friend class TrussUnitManager;
    TrussUnit ( const QString& name );

public:        
    virtual ~TrussUnit ();

    int getNodesRadius () const;
    int getPivotsWidth () const;
    const QString& getTrussName () const;
    const QSize& getArea ();
  
    void paint ( base_renderer& baseRend ) const;

public slots:
    void setTrussName ( const QString& name );
    void setNodesRadius ( int rad );
    void setPivotsWidth ( int wid );
    void setArea ( const QSize& );

signals:
    void onTrussNameChange ( const QString& old, const QString& n );
    void onAreaChange ( const QSize& );
    void onNodesRadiusChange ( int );
    void onPivotsWidthChange ( int );

private:
    static const QString UNNAMED;

    int nodesRadius, pivotsWidth;
    QString trussName;
    QSize area;
};

class TrussNode: public Node, public PaintableTrussElement
{    
public:
    TrussNode ();
    TrussNode ( const TrussNode& );
    QPoint getNodeWidgetPosition () const;
    void setNodeWidgetPosition ( QPoint point );
    int getRadius () const;
    void setRadius ( int rad );
    void paint ( base_renderer& baseRend ) const;
private:
    int radius;
    QPoint widgetPosition;
};

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
public:
    TrussPivot ();
    TrussPivot ( TrussNode&, TrussNode& );
    TrussPivot ( const TrussPivot& );
    int getPivotWidth ();
    void setPivotWidth ( int w );
    void drawLine ( scanline_rasterizer& ras, solid_renderer& solidRend,
                    agg::scanline_p8& sl, QPoint point1, QPoint point2,
                    int width, color_type color ) const;
    void paint ( base_renderer& baseRend ) const;
private:
    int width;
};

#endif //TRUSSUNIT_H
