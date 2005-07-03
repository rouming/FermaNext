
#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"
#include <vector>
#include <qstring.h>
#include <qpoint.h> 

#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_embedded_raster_fonts.h"
#include "agg_conv_marker.h"
#include "agg_conv_shorten_path.h"
#include "agg_conv_marker_adaptor.h"
#include "agg_conv_concat.h"
#include "agg_vcgen_markers_term.h"
#include "agg_arrowhead.h"
#include "agg_rounded_rect.h"

typedef agg::glyph_raster_bin<agg::rgba8>                           glyph_gen;
typedef agg::renderer_raster_htext_solid<ren_dynarow, glyph_gen>    textRenderer;


class TrussUnitWindow : public TrussUnit
{
    Q_OBJECT
public:     
    TrussUnitWindow ( const QString& name, ObjectStateManager* );
    virtual ~TrussUnitWindow ();

    virtual void setHighlighted ( bool );

    virtual QPoint getWindowLeftTopPos () const;
    virtual QPoint getWindowRightBottomPos () const;
    virtual QPoint getTrussAreaLeftTopPos () const;
    virtual QPoint getTrussAreaRightBottomPos () const;
    virtual const QSize& getWindowSize () const;    
    virtual double getScaleMultiplierX () const;
    virtual double getScaleMultiplierY () const;

    virtual bool inWindowRect ( int x, int y ) const;
    virtual bool inCanvasRect ( int  x, int  y ) const;
    virtual bool inHeadlineRect ( int  x, int  y ) const;
    virtual bool inHorResizeRect ( int x, int y ) const;
    virtual bool inVerResizeRect ( int x, int y ) const;
    virtual bool inBDiagResizeRect ( int x, int y ) const;
    virtual bool inFDiagResizeRect ( int x, int y ) const;
    virtual bool inNodeRadius ( int x, int y ) const;
    virtual bool isPivotSelected ( int x, int y ) const;

    virtual TrussNode* findNodeByWidgetPos ( int x, int y ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y ) const;

    virtual color_type getCanvasColor () const;
	virtual color_type getHeadlineFirstColor () const;
    virtual color_type getHeadlineMiddleColor () const;
    virtual color_type getHeadlineLastColor () const;
	virtual color_type getBorderColor () const;

    virtual void setWindowSize ( int w, int h );
    virtual void setWindowPosition ( QPoint pos );
    virtual void setNodeHighlight ( int x, int y );
    virtual void setPivotHighlight ( int x, int y );
    virtual void removeNodeHighlight ();
    virtual void removePivotHighlight ();

    virtual void setCanvasColor ( int r, int g, int b );
    virtual void setHeadlineFirstColor ( int r, int g, int b );
    virtual void setHeadlineMiddleColor ( int r, int g, int b );
    virtual void setHeadlineLastColor ( int r, int g, int b );
    virtual void setBorderColor ( int r, int g, int b );
    virtual void setResEllColor ( int r, int g, int b );

    virtual void resize ( QPoint leftTop, QPoint rightBottom );

    virtual void drawText ( ren_dynarow& baseRend, textRenderer& textRend,
                            const QString& str, color_type c, QPoint point ) const;
    virtual void drawLine ( scanline_rasterizer& ras, solidRenderer& solidRend,
                            agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const;
    virtual void drawArrow ( scanline_rasterizer& ras, solidRenderer& solidRend,
                             agg::scanline_p8& sl, QPoint point1, QPoint point2 ) const;
    virtual void drawOutlineRoundedRect ( solidRenderer& solidRend, 
                                         scanline_rasterizer& ras,
                                         agg::scanline_p8& sl, 
                                         QPoint point1, QPoint point2, 
                                         int cornerRadius,
                                         color_type color) const;
    virtual void drawTrussArea ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                                 textRenderer& textRend, solidRenderer& solidRend, 
                                 agg::scanline_p8& sl ) const;
    virtual void drawHeadline ( ren_dynarow& baseRend, solidRenderer& solidRend,
                                scanline_rasterizer& ras, agg::scanline_p8& sl, 
                                gradient_span_alloc& gradSpan, linear_gradient& gradFunc, 
                                color_array_type& gradColors, agg::trans_affine& mtx ) const;
    virtual void paint ( base_renderer& baseRend ) const;

signals:
    void onResize ( QSize oldS, QSize newS );
    void onMove ( QPoint oldP, QPoint newP );

private:
    rbuf_dynarow* rbuf;
    QPoint windowLeftTopPos, windowRightBottomPos;
    QSize windowSize;
    color_type canvColor, headFirstColor, headMiddleColor, headLastColor, 
               borderColor, resEllColor;    
};

#endif //TRUSSUNITWINDOW_H
