
#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"

class TrussUnitWindow : public TrussUnit
{
    Q_OBJECT
public:
    enum selectedEllipse { None = 0, LeftTop, LeftBottom, RightTop, RightBottom };

    TrussUnitWindow ( const QString& name, ObjectStateManager* );
    virtual ~TrussUnitWindow ();

    virtual QPoint getWindowLeftTopPos () const;
    virtual QPoint getWindowRightBottomPos () const;
    virtual QPoint getTrussAreaLeftTopPos () const;
    virtual QPoint getTrussAreaRightBottomPos () const;
    virtual QPoint getTrussCoordFromWidgetPos ( int x, int y ) const;
    virtual QPoint getTrussCoordFromWidgetPos ( QPoint pos ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( int x, int y ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( QPoint coord ) const;
    virtual void setWindowPosition ( QPoint pos );
    virtual void resize ( QPoint leftTop, QPoint rightBottom );
    virtual void setCursorCoord ( QPoint coord );
    virtual QPoint getCursorCoord () const;
    virtual const QSize& getWindowSize () const;

    virtual bool inWindowRect ( int x, int y ) const;
    virtual bool inCanvasRect ( int  x, int  y ) const;
    virtual bool inHeadlineRect ( int  x, int  y ) const;
    virtual bool inTrussAreaRect ( int x, int y ) const;
    virtual bool inHorResizeRect ( int x, int y ) const;
    virtual bool inVerResizeRect ( int x, int y ) const;
    virtual bool inBDiagResizeRect ( int x, int y );
    virtual bool inFDiagResizeRect ( int x, int y );

    virtual void setHighlighted ( bool );
    virtual void setResizeEllipseHighlighted ( selectedEllipse ellipseType );
    virtual void setFocusOnNode ( TrussNode* selectedNode );
    virtual void setFocusOnPivot ( TrussPivot* selectedPivot );
    virtual void removeNodesHighlight ();
    virtual void removePivotsHighlight ();

    virtual TrussNode* findNodeByWidgetPos ( int x, int y, int presicion ) const;
    virtual TrussNode* findNodeByWidgetPos ( QPoint pos, int presicion ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y, int presicion ) const;
    virtual TrussPivot* findPivotByWidgetPos ( QPoint pos, int presicion ) const;
    virtual int getNodeFindingPrecision () const;
    virtual int getPivotFindingPrecision () const;
    virtual void moveTrussNode ( int x, int y, TrussNode* node );
    virtual void moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                  QPoint firstNodeClickDist, QPoint lastNodeClickDist );
    virtual TrussNode* nodesMergingComparison ( TrussNode& comparableNode, int precision,
                                                bool fixationCheck );
    virtual void mergeNodes ( TrussNode* mergingNode, TrussNode* node );
    virtual void dividePivot ( TrussPivot& dividualPivot, TrussNode& dividingNode );
    virtual TrussPivot* findDividualPivot ( TrussNode& dividingNode ) const;
    virtual QPoint getLineSegmentsCrossPoint ( QPoint p11, QPoint p12, 
                                               QPoint p21, QPoint p22 ) const;
    virtual QRect getPivotsArea ( PivotList pivots ) const;
    virtual QPointArray getPivotCrossPoints ( PivotList nonCrossingPivots ) const;
    virtual TrussNode& createCrossNode ( QPoint crossPoint );
    virtual void createPivotCrossNodes ( QPointArray crossPoints );
    virtual void updateNodePosition ( TrussNode* selectedNode, bool fixationCheck );
    virtual void updateAfterNodeManipulation ( TrussNode* selectedNode, 
                                              bool fixationCheck );
    virtual void updateAfterPivotManipulation ( TrussPivot* selectedPivot, 
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
    virtual void drawNodeNumber ( TrussNode* node, ren_dynarow& baseRend, 
                                 solidRenderer& solidRend, scanline_rasterizer& ras, 
                                 agg::scanline_p8& sl ) const;
    virtual void drawPivotNumber ( TrussPivot* pivot, ren_dynarow& baseRend, 
                                   solidRenderer& solidRend, scanline_rasterizer& ras, 
                                   agg::scanline_p8& sl ) const;
    virtual void drawTrussElementsNumbers ( ren_dynarow& baseRend, 
                                            solidRenderer& solidRend, 
                                            scanline_rasterizer& ras, 
                                            agg::scanline_p8& sl ) const;
    virtual void drawEllipseHighlight ( solidRenderer& solidRend, scanline_rasterizer& ras, 
                                        agg::scanline_p8& sl, QPoint pos ) const;
    virtual void paint ( base_renderer& baseRend ) const;

signals:
    void onResize ( QSize oldS, QSize newS );
    void onMove ( QPoint oldP, QPoint newP );

protected:
    virtual double getScaleMultiplierX () const;
    virtual double getScaleMultiplierY () const;
    virtual void setWindowSize ( int w, int h );

private:
    int nodeFindingPrecision, pivotFindingPrecision;
    selectedEllipse resEll;
    textFont headFont, numbersFont;
    rbuf_dynarow *windowBuf, *coordBuf, *numbersBuf;
    QPoint windowLeftTopPos, windowRightBottomPos, cursorCoord;
    QSize windowSize, coordFieldSize;
    color_type canvColor, headFirstColor, headMiddleColor, headLastColor, 
               borderColor, resEllColor;    
};

#endif //TRUSSUNITWINDOW_H
