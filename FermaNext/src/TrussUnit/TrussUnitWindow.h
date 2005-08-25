#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"
#include <vector>
#include <qstring.h>
#include <qpoint.h> 

#include "agg_basics.h"
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
typedef const agg::int8u*                                           textFont;


class TrussUnitWindow : public TrussUnit
{
    Q_OBJECT
public:     
    TrussUnitWindow ( const QString& name, ObjectStateManager* );
    virtual ~TrussUnitWindow ();

    virtual QPoint getWindowLeftTopPos () const;
    virtual QPoint getWindowRightBottomPos () const;
    virtual QPoint getTrussAreaLeftTopPos () const;
    virtual QPoint getTrussAreaRightBottomPos () const;
    virtual QPoint getTrussCoordFromWidgetPos ( int x, int y ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( int x, int y ) const;
    virtual void setWindowPosition ( QPoint pos );
    virtual void resize ( QPoint leftTop, QPoint rightBottom );
    virtual void setCursorCoord ( QPoint coord );
    virtual const QSize& getWindowSize () const;

    virtual bool inWindowRect ( int x, int y ) const;
    virtual bool inCanvasRect ( int  x, int  y ) const;
    virtual bool inHeadlineRect ( int  x, int  y ) const;
    virtual bool inTrussAreaRect ( int x, int y ) const;
    virtual bool inHorResizeRect ( int x, int y ) const;
    virtual bool inVerResizeRect ( int x, int y ) const;
    virtual bool inBDiagResizeRect ( int x, int y ) const;
    virtual bool inFDiagResizeRect ( int x, int y ) const;

    virtual void setHighlighted ( bool );
    virtual void setFocusOnNode ( TrussNode* selectedNode );
    virtual void setFocusOnPivot ( TrussPivot* selectedPivot );
    virtual void removeNodesHighlight ();
    virtual void removePivotsHighlight ();

    virtual TrussNode* findNodeByWidgetPos ( int x, int y ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y, int presicion ) const;
    virtual void moveTrussNode ( int x, int y, TrussNode* node );
    virtual void moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                  QPoint firstNodeClickDist, QPoint lastNodeClickDist );

    virtual TrussNode* nodesMergingComparison ( TrussNode& comparableNode, int precision,
                                                bool fixationCheck );
    virtual void mergeNodes ( TrussNode* mergingNode, TrussNode* node );

    virtual void dividePivot ( TrussPivot& dividualPivot, TrussNode& dividingNode );

    virtual QPoint getLineSegmentsIntersectionPoint ( QPoint p11, QPoint p12, 
                                                      QPoint p21, QPoint p22 ) const;
    virtual void checkNodePosition ( TrussNode* selectedNode, bool fixationCheck );
    virtual void createPivotCrossPoints ( TrussPivot* selectedPivot, bool fixationCheck );

    virtual void checkAfterNodeManipulation ( TrussNode* selectedNode, 
                                              bool fixationCheck );
    virtual void checkAfterPivotManipulation ( TrussPivot* selectedPivot, 
                                               bool fixationCheck );

    virtual color_type getCanvasColor () const;
    virtual color_type getHeadlineFirstColor () const;
    virtual color_type getHeadlineMiddleColor () const;
    virtual color_type getHeadlineLastColor () const;
    virtual color_type getBorderColor () const;

    virtual void setCanvasColor ( int r, int g, int b );
    virtual void setHeadlineFirstColor ( int r, int g, int b );
    virtual void setHeadlineMiddleColor ( int r, int g, int b );
    virtual void setHeadlineLastColor ( int r, int g, int b );
    virtual void setBorderColor ( int r, int g, int b );
    virtual void setResEllColor ( int r, int g, int b );

    virtual QString fitTextToWindowSize ( QString str, int lengthLimit, 
                                          glyph_gen& glyph ) const;

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
    virtual void drawCursorCoordinatesField ( ren_dynarow& baseRend,
                                              textRenderer& textRend ) const;
    virtual void drawNumbersField ( ren_dynarow& baseRend, textRenderer& textRend ) const;
    virtual void paint ( base_renderer& baseRend ) const;

signals:
    void onResize ( QSize oldS, QSize newS );
    void onMove ( QPoint oldP, QPoint newP );

protected:
    virtual double getScaleMultiplierX () const;
    virtual double getScaleMultiplierY () const;
    virtual void setWindowSize ( int w, int h );

private:
    textFont headFont, numbersFont;
    rbuf_dynarow *windowBuf, *coordBuf, *numbersBuf;
    QPoint windowLeftTopPos, windowRightBottomPos, cursorCoord;
    QSize windowSize, coordFieldSize;
    color_type canvColor, headFirstColor, headMiddleColor, headLastColor, 
               borderColor, resEllColor;    
};

#endif //TRUSSUNITWINDOW_H
