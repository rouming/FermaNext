
#include "TrussUnitDesignerWidget.h"
#include <algorithm>

/*****************************************************************************
 * Truss Designer Widget
 *****************************************************************************/

TrussUnitDesignerWidget::TrussUnitDesignerWidget ()
{}

TrussUnit& TrussUnitDesignerWidget::createTrussUnit ()
{
    TrussUnit* truss = new TrussUnit;
    trussUnits.push_back(truss);
    return *truss;
}

bool TrussUnitDesignerWidget::removeTrussUnit ( const TrussUnit& truss )
{
    TrussUnitList::iterator iter = trussUnits.begin();
    for ( ; iter != trussUnits.end(); ++iter )
        if ( (*iter) == &truss ) {
            delete *iter;
            trussUnits.erase(iter);
            return true;
        }            
    return false; 
}

/*****************************************************************************/
// Event handlers
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_rasterizer_outline.h"
#include "agg_trans_affine.h"
#include "agg_conv_stroke.h"
#include "agg_conv_dash.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_conv_marker.h"
#include "agg_conv_shorten_path.h"
#include "agg_conv_marker_adaptor.h"
#include "agg_conv_concat.h"
#include "agg_arrowhead.h"
#include "agg_vcgen_markers_term.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_renderer_scanline.h"
#include "agg_renderer_primitives.h"
#include "agg_span_allocator.h"
#include "agg_span_gradient.h"
#include "agg_span_interpolator_linear.h"
#include "agg_pixfmt_rgb.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"

typedef agg::pixfmt_bgr24 pixfmt;
typedef pixfmt::color_type color_type;
typedef agg::renderer_base<pixfmt> base_renderer;
typedef agg::renderer_primitives<base_renderer> primitives_renderer;

typedef agg::renderer_scanline_aa_solid<base_renderer>  solid_renderer;
typedef agg::renderer_scanline_bin_solid<base_renderer> draft_renderer;

typedef agg::gradient_circle gradient_function;
typedef agg::span_interpolator_linear<> interpolator;
typedef agg::pod_auto_array<color_type, 256> color_array_type;
typedef agg::span_gradient<color_type, 
                           interpolator, 
                           gradient_function, 
                           color_array_type> gradient_span_gen;
typedef agg::span_allocator<color_type> gradient_span_alloc;

typedef agg::renderer_scanline_aa<base_renderer, 
                                  gradient_span_gen> gradient_renderer;

typedef agg::rasterizer_scanline_aa<>                scanline_rasterizer;
typedef agg::rasterizer_outline<primitives_renderer> outline_rasterizer;

int X, Y = 110;

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* pe )
{
   scanline_rasterizer ras;
            pixfmt pixf(getAggRenderingBuffer());
            base_renderer rb(pixf);
            solid_renderer solid(rb);
            draft_renderer draft(rb);
            rb.clear(agg::rgba(10, 10, 10));
            ras.gamma(agg::gamma_none());

            agg::scanline_p8 sl;

        double scaling_koeff_x = 20;
        double scaling_koeff_y = 10;

        double x_start = scaling_koeff_x;
        double x_end   = width() - 15.0;
        double y_start = scaling_koeff_y;
        double y_end   = height() - 10.0;

                agg::ellipse ell(X, Y, 5.0 * 1, 5.0 * 1, 32);
                ras.add_path(ell);
                agg::render_scanlines(ras, sl, solid);



}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* re )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
}

void TrussUnitDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
    X = me->x();
    Y = height() - me->y();
    update();
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
    X = me->x();
    Y = height() - me->y();
    update();

}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* ctrl )
{
}

/*****************************************************************************/
