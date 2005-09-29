
#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"
#include "TrussUnitWindowButton.h"

typedef QValueList<DoublePoint> DoublePointArray;

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
    virtual DoublePoint getTrussCoordFromWidgetPos ( int x, int y ) const;
    virtual DoublePoint getTrussCoordFromWidgetPos ( QPoint pos ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( double x, double y ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( const DoublePoint& ) const;
    virtual void setWindowPosition ( QPoint pos );
    virtual void resize ( QPoint leftTop, QPoint rightBottom );
    virtual void setCursorCoord ( const QPoint& );
    // Assuming this coords are from truss node position, 
    // so we don't want to convert them from widget position
    virtual void setCursorCoord ( const DoublePoint& );
    virtual DoublePoint getCursorCoord () const;
    virtual const QSize& getWindowSize () const;

    virtual void setMaxSize ( int x, int y );
    virtual bool isMaximized () const;

    virtual bool inWindowRect ( int x, int y ) const;
    virtual bool inCanvasRect ( int  x, int  y ) const;
    virtual bool inHeadlineRect ( int  x, int  y ) const;
    virtual bool inTrussAreaRect ( int x, int y ) const;
    virtual bool inHorResizeRect ( int x, int y ) const;
    virtual bool inVerResizeRect ( int x, int y ) const;
    virtual bool inBDiagResizeRect ( int x, int y );
    virtual bool inFDiagResizeRect ( int x, int y );
    virtual bool inHideButtonRect ( int x, int y ) const;
    virtual bool inRollUpButtonRect ( int x, int y ) const;

    virtual QPoint getButtonBufPos () const;

    virtual void checkMouseMoveEvent ( int x, int y );
    virtual void checkMousePressEvent ( int x, int y );
    virtual void checkMouseReleaseEvent ( int x, int y );

    virtual void setHighlighted ( bool );
    virtual void setResizeEllipseHighlighted ( selectedEllipse ellipseType );
    virtual void setFocusOnNode ( TrussNode* selectedNode );
    virtual void setFocusOnPivot ( TrussPivot* selectedPivot );
    virtual void removeNodesHighlight ();
    virtual void removePivotsHighlight ();
    virtual void removeButtonsHighlight ();
    virtual void releaseButtons ();

    virtual double getNodePrecision ( bool inPix = true ) const;
    virtual double getPivotPrecision () const;

    virtual TrussNode* findNodeByWidgetPos ( const QPoint& pos, 
                                             double precision ) const;
    virtual TrussNode* findNodeByWidgetPos ( const QPoint& pos ) const;
    virtual TrussNode* findNodeByWidgetPos ( int x, int y, 
                                             double precision ) const;
    virtual TrussNode* findNodeByWidgetPos ( int x, int y ) const;

    virtual TrussPivot* findPivotByCoord ( const DoublePoint& coord, 
                                           double precision ) const;
    virtual TrussPivot* findPivotByCoord ( const DoublePoint& coord ) const;

    virtual TrussPivot* findPivotByWidgetPos ( const QPoint& pos, 
                                               double precision ) const;
    virtual TrussPivot* findPivotByWidgetPos ( const QPoint& pos ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y, 
                                               double precision ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y ) const;

    virtual void moveTrussNode ( int x, int y, TrussNode* node );

    virtual void moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                  QPoint firstNodeClickDist, 
                                  QPoint lastNodeClickDist );

    virtual TrussNode* nodesMergingComparison ( TrussNode& comparableNode, 
                                                int precision, 
                                                bool fixationCheck );

    virtual void mergeNodes ( TrussNode* mergingNode, TrussNode* node );

    virtual void dividePivot ( TrussPivot& dividualPivot, TrussNode& dividingNode );

    virtual TrussPivot* findDividualPivot ( TrussNode& dividingNode ) const;

    virtual DoublePointArray getPivotCrossPoints ( 
                                    const PivotList& nonCrossingPivots ) const;

    virtual TrussNode& createCrossNode ( const DoublePoint& crossPoint );

    virtual void createPivotCrossNodes ( const DoublePointArray& );

    virtual void updateNodePosition ( TrussNode* selectedNode, 
                                      bool fixationCheck );

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

    virtual void paint ( base_renderer& baseRend ) const;

protected:
    virtual double getScaleMultiplierX () const;
    virtual double getScaleMultiplierY () const;

    virtual void setWindowSize ( int w, int h );

    virtual void hide ();
    virtual void maximize ();
    virtual void minimize ();

    virtual QString fitTextToWindowSize ( QString str, int lengthLimit, 
                                          glyph_gen& glyph ) const;

    virtual void drawTrussArea ( ren_dynarow& baseRend, 
                                 scanline_rasterizer& ras,
                                 textRenderer& textRend, 
                                 solidRenderer& solidRend, 
                                 agg::scanline_p8& sl ) const;

    virtual void drawHeadline ( ren_dynarow& baseRend, 
                                solidRenderer& solidRend,
                                scanline_rasterizer& ras, 
                                agg::scanline_p8& sl, 
                                gradient_span_alloc& gradSpan, 
                                linear_gradient& gradFunc, 
                                color_array_type& gradColors, 
                                agg::trans_affine& mtx ) const;

    virtual void drawCursorCoordinatesField ( ren_dynarow& baseRend,
                                              textRenderer& textRend,
                                              glyph_gen& glyph ) const;

    virtual void drawNumbersField ( ren_dynarow& baseRend, 
                                    textRenderer& textRend ) const;

    virtual void drawNodeNumber ( TrussNode* node, 
                                  ren_dynarow& baseRend, 
                                  solidRenderer& solidRend, 
                                  scanline_rasterizer& ras, 
                                 agg::scanline_p8& sl ) const;

    virtual void drawPivotNumber ( TrussPivot* pivot, 
                                   ren_dynarow& baseRend, 
                                   solidRenderer& solidRend, 
                                   scanline_rasterizer& ras, 
                                   agg::scanline_p8& sl ) const;

    virtual void drawTrussElementsNumbers ( ren_dynarow& baseRend, 
                                            solidRenderer& solidRend, 
                                            scanline_rasterizer& ras, 
                                            agg::scanline_p8& sl ) const;

    virtual void drawEllipseHighlight ( solidRenderer& solidRend, 
                                        scanline_rasterizer& ras, 
                                        agg::scanline_p8& sl, 
                                        QPoint pos ) const;

signals:
    void onResize ( QSize oldS, QSize newS );
    void onMove ( QPoint oldP, QPoint newP );
    void onTrussUnitWindowHide ();
    void onTrussUnitWindowRollUp ();

protected slots:
    virtual void clearButtonBufRenderedFlag ();

private:
    selectedEllipse resEll;
    textFont headFont, numbersFont;
    rbuf_dynarow *windowBuf, *coordBuf, *numbersBuf, *buttonBuf;
    QPoint windowLeftTopPos, windowRightBottomPos; 
    DoublePoint cursorCoord;
    QSize windowSize, coordFieldSize;
    color_type canvColor, headFirstColor, headMiddleColor, 
               headLastColor, borderColor, resEllColor;
    TrussUnitWindowButton *hideButton, *rollUpButton;
    mutable bool buttonBufRendered;

    // minimize/maximize subsidiaries
    bool maximized;
    QSize maxSize;
    QPoint minLeftTopPos, minRightBottomPos;
};

#endif //TRUSSUNITWINDOW_H
