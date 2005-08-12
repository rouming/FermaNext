
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

/*****************************************************************************/

class PaintableTrussElement
{
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool h, bool v, bool e, bool r );

    virtual bool isVisible () const;
    virtual bool isHighlighted () const;
    virtual bool isEnabled () const;
    virtual bool isRendered () const;

public:
    virtual void setVisible ( bool );
    virtual void setHighlighted ( bool );
    virtual void setEnabled ( bool );

protected:
    virtual void rendered ( bool ) const;


// We should use this pretty hack to workaround Qt prohibition
// of using multiple inheritance. So we just declare methods as 
// pure virtual and emit them in the body of the this class.
// We get cheap possibility to have signals and multiple inheritance
// at the same time.
protected:
    virtual void onVisibleChange ( bool ) = 0;
    virtual void onHighlightChange ( bool ) = 0;

private:
    bool visible;
    bool highlighted;
    bool enabled;
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
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );

private:
    static const QString UNNAMED;

    QString trussName;
    QSize trussAreaSize;
};

/*****************************************************************************/

class TrussNode: public Node, public PaintableTrussElement
{    
    Q_OBJECT
public:
    TrussNode ( ObjectStateManager* );
    TrussNode ( int x, int y, ObjectStateManager* );
    TrussNode ( int x, int y, Fixation, ObjectStateManager* );

    void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                    agg::scanline_p8& sl, QPoint point1, QPoint point2,
                    int width, color_type color ) const;
    void drawFixation ( scanline_rasterizer& ras, solidRenderer& solidRend, 
                        agg::scanline_p8& sl, int trussAreaHeight,
                        double scaleMultX, double scaleMultY,
                        int lineWidth, color_type color ) const;

    void drawGradientEllipse ( scanline_rasterizer& ras, ren_dynarow& baseRend, 
                               agg::scanline_p8& sl, int x, int y, int radius, 
                               color_type begin, color_type middle, color_type end ) const;

    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight ) const;

signals:
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
};

/*****************************************************************************/

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
    Q_OBJECT
public:
    TrussPivot ( ObjectStateManager* );
    TrussPivot ( TrussNode&, TrussNode&, ObjectStateManager* );

    void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                    agg::scanline_p8& sl, QPoint point1, QPoint point2,
                    int width, color_type color ) const;

    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight) const;

signals:
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
};

#endif //TRUSSUNIT_H
