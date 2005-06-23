
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
public:     
    TrussUnitWindow ( const QString& name );
    virtual ~TrussUnitWindow ();

    QPoint getWindowLeftTopPos () const;
    QPoint getWindowRightBottomPos () const;
    QPoint getTrussAreaLeftTopPos () const;
    QPoint getTrussAreaRightBottomPos () const;
    QPoint transCoord ( QPoint point, bool flipY );
    rbuf_dynarow* getRBufDynarow ();
    int getHeadlineWidth () const;
    int getBorderWidth () const;
    int getMinResizeValue () const;
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
    color_type getCanvasColor () const;
	color_type getHeadlineColor () const;
	color_type getBorderColor () const;

    void setWindowPosition ( QPoint pos );    
    void setTrussAreaWindowIndent ( int indent );
    void setTrussNodesPosition ();
    void setNodeHighlight ( int x, int y );
    void setPivotHighlight ( int x, int y );
    void removeNodeHighlight ();
    void removePivotHighlight ();
    void setCoordinateLineWidth ( int width );
    void setHeadlineWidth ( int  width );
    void setBorderWidth ( int  width );
    void setMinResizeValue ( int value );
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
    int  headWidth, bordWidth, resEllRad, winCornerRadius, 
         minResizeVal, lineWidth, trussAreaIndent;
    QPoint windowLeftTopPos, windowRightBottomPos;
    QSize windowSize;
    color_type canvColor, headColor, borderColor, resEllColor;
    mutable bool isRendered;
};

#endif //TRUSSUNITWINDOW_H
