
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include "SubsidiaryConstants.h"

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
typedef agg::pixfmt_custom_rbuf_rgba<agg::blender_rgba32, rbuf_dynarow>     pixf_dynarow;
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
    PaintableTrussElement ( bool h, bool v, bool r );

    virtual bool isVisible () const;
    virtual bool isHighlighted () const;
    virtual bool isRendered () const;

    virtual void setVisible ( bool );
    virtual void setHighlighted ( bool );

protected:
    virtual void rendered ( bool ) const;

private:
    bool visible;
    bool highlighted;
    mutable bool renderedFlag;
};

/*****************************************************************************/

class TrussUnit : public Truss<TrussNode, TrussPivot>,                  
                  public PaintableTrussElement
{
    Q_OBJECT
public:        
    TrussUnit ( const QString& name, ObjectStateManager* mng );
    virtual ~TrussUnit ();

    const QString& getTrussName () const;
    const QSize& getTrussAreaSize () const;

    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight ) const;

public slots:
    void setTrussName ( const QString& name );
    void setTrussAreaSize ( const QSize& );

protected slots:
    void trussUnitStateIsChanged ();

signals:
    void onTrussNameChange ( const QString& old, const QString& n );
    void onAreaChange ( const QSize& );

private:
    static const QString UNNAMED;

    QString trussName;
    QSize trussAreaSize;
};

/*****************************************************************************/

class TrussNode: public Node, public PaintableTrussElement
{    
public:
    TrussNode ( ObjectStateManager* );
    TrussNode ( int x, int y, ObjectStateManager* );
    TrussNode ( int x, int y, Fixation, ObjectStateManager* );

    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight ) const;
};

/*****************************************************************************/

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
public:
    TrussPivot ();
    TrussPivot ( TrussNode&, TrussNode& );

    void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                    agg::scanline_p8& sl, QPoint point1, QPoint point2,
                    int width, color_type color ) const;
    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight) const;
};

#endif //TRUSSUNIT_H
