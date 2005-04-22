
#include "TrussUnitDesignerWidget.h"
#include <algorithm>
#include <qsize.h>

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


int X1 = 50, Y1 = 50;
int X2 = 400, Y2 = 400;
int clickX, clickY;
bool init = true;
TrussUnit* selectedTruss;

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

TrussUnit* TrussUnitDesignerWidget::whoIsInRect ( double x, double y )
{
	TrussUnitList::reverse_iterator rev_iter = trussUnits.rbegin();
	for ( ; rev_iter != trussUnits.rend(); ++rev_iter )
		if ( (*rev_iter)->inCanvasRect(x, y) || (*rev_iter)->inHeadlineRect(x, y))
		{
			if (rev_iter == trussUnits.rend())
			{
				return *rev_iter;
			}
			TrussUnit* tmpTruss = *rev_iter;
			trussUnits.push_back(tmpTruss);
			TrussUnitList::iterator iter = trussUnits.begin();
			for ( ; iter != trussUnits.end(); ++iter )
				if ( (*iter)->inCanvasRect(x, y) && *iter == tmpTruss )
				{
					trussUnits.erase(iter);
					if ( tmpTruss->inHeadlineRect(x, y) )
						return tmpTruss;
					return 0;
				}
		}

	return 0;
}

void TrussUnitDesignerWidget::initTruss ()
{
	flipY( true );
	uint i;
	for(i = 0; i <3; i++)
	{
		createTrussUnit();
	}
	TrussUnitList::iterator iter = trussUnits.begin();
	for ( ; iter != trussUnits.end(); ++iter )
	{
		(*iter)->setPosition ( X1, Y1, X2, Y2 );
		(*iter)->setHeadlineWidth ( 20.0 );
		(*iter)->setBorderWidth ( 3.0 );
		(*iter)->setBorderColor ( 50, 50, 50 );
		(*iter)->setHeadlineColor ( X1, X2, Y1 );
		(*iter)->setCanvasColor ( 20, 20, 20 );
		X1 = X1 + 180;
		X2 = X2 + 180;
	}
	init = false;
}

void TrussUnitDesignerWidget::onDraw ()
{
    pixfmt pixf(getAggRenderingBuffer());
    base_renderer rb(pixf);
    rb.clear(agg::rgba(10, 10, 10));

	TrussUnitList::iterator iter = trussUnits.begin();
	for ( ; iter != trussUnits.end(); ++iter )
		(*iter)->paint(getAggRenderingBuffer());
  }


/*****************************************************************************/
// Event handlers

void TrussUnitDesignerWidget::aggPaintEvent ( QPaintEvent* pe )
{
	if(init)
		initTruss();
	onDraw();
}

void TrussUnitDesignerWidget::aggResizeEvent ( QResizeEvent* re )
{
}

void TrussUnitDesignerWidget::aggKeyPressEvent ( QKeyEvent* ke )
{
}

void TrussUnitDesignerWidget::aggMouseMoveEvent ( QMouseEvent* me )
{
	if ( selectedTruss )
	{
		double dx, dy;
		dx = me->x() - clickX;
		dy = flipY() ? height() - me->y() - clickY : me->y() - clickY;
		X1 = selectedTruss->getX1() + dx;
		Y1 = selectedTruss->getY1() + dy;
		X2 = selectedTruss->getX2() + dx;
		Y2 = selectedTruss->getY2() + dy;
		selectedTruss->setPosition ( X1, Y1, X2, Y2 );
		update();
		clickX += dx;
		clickY += dy;
	}
}

void TrussUnitDesignerWidget::aggMouseReleaseEvent ( QMouseEvent* me )
{
	selectedTruss = NULL;
}

void TrussUnitDesignerWidget::aggMousePressEvent ( QMouseEvent* me )
{
	clickX = me->x();
	clickY = flipY() ? height() - me->pos().y() : me->pos().y();
	selectedTruss = whoIsInRect ( clickX, clickY );
	update();
}

void TrussUnitDesignerWidget::aggCtrlChangedEvent ( const agg::ctrl* ctrl )
{
}

/*****************************************************************************/
