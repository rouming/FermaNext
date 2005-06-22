
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include <vector>
#include <qstring.h>
#include <qpoint.h> 

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

typedef agg::rendering_buffer_dynarow<4>                                    rbuf_dynarow;
typedef agg::pixfmt_custom_rbuf_rgba<agg::blender_bgra32, rbuf_dynarow>     pixf_dynarow;
typedef agg::renderer_base<pixf_dynarow>                                    ren_dynarow;

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
//    virtual void paint ( base_renderer& baseRend ) const = 0;
private:
    bool visible;
    bool highlighted; 
};

/*****************************************************************************/

class TrussUnit : public Truss<TrussNode, TrussPivot>,                  
                  public PaintableTrussElement
{
    Q_OBJECT
public:        
    TrussUnit ( const QString& name );
    virtual ~TrussUnit ();

    int getNodesRadius () const;
    int getPivotsWidth () const;
    const QString& getTrussName () const;
    const QSize& getArea () const;
  
    void paint ( ren_dynarow& baseRend, QPoint leftTopPos ) const;

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
    rbuf_dynarow* rbuf;
    static const QString UNNAMED;

    int nodesRadius, pivotsWidth;
    QString trussName;
    QSize area;
};

/*****************************************************************************/

class TrussNode: public Node, public PaintableTrussElement
{    
public:
    TrussNode ();

    QPoint getNodeWidgetPosition () const;
    void setNodeWidgetPosition ( QPoint point );
    int getRadius () const;
    void setRadius ( int rad );
    void paint ( ren_dynarow& baseRend ) const;
private:
    int radius;
    QPoint widgetPosition;
};

/*****************************************************************************/

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
public:
    TrussPivot ();
    TrussPivot ( TrussNode&, TrussNode& );

    int getPivotWidth () const;
    void setPivotWidth ( int w );
    void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                    agg::scanline_p8& sl, QPoint point1, QPoint point2,
                    int width, color_type color ) const;
    void paint ( ren_dynarow& baseRend ) const;
private:
    int width;
};

#endif //TRUSSUNIT_H
