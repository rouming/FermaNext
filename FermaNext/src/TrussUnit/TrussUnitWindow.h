
#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"
#include "TrussUnitWindowButton.h"

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

    virtual void setWindowPosition ( QPoint pos );

    virtual const QSize& getWindowSize () const;
    virtual void resize ( QPoint leftTop, QPoint rightBottom );

    virtual bool isMaximized () const;
    virtual void setMaxSize ( int x, int y );
    virtual void maximize ( bool saveOldSize = true );
    virtual void minimize ();

    virtual DoublePoint getCursorCoord () const;
    virtual void setCursorCoord ( const QPoint& );
    virtual void setCursorCoord ( const DoublePoint& );

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

    virtual void checkMouseMoveEvent ( int x, int y, bool mousePressed );
    virtual void checkMousePressEvent ( int x, int y );
    virtual void checkMouseReleaseEvent ( int x, int y );

    virtual void setHighlighted ( bool );
    virtual void removeButtonsHighlight ();
    virtual void releaseButtons ();

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
    virtual void setWindowSize ( int w, int h );
    virtual QPoint getButtonBufPos () const;
    virtual bool inButtonsRect ( int x, int y ) const;
    virtual void hide ();
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
                                color_array_type& gradColors ) const;

    virtual void drawCursorCoordinatesField ( ren_dynarow& baseRend,
                                              textRenderer& textRend,
                                              glyph_gen& glyph ) const;

    virtual void drawNumbersField ( ren_dynarow& baseRend, 
                                    textRenderer& textRend ) const;

    virtual void drawResizeEllipses ( solidRenderer& solidRend, 
                                      scanline_rasterizer& ras, 
                                      agg::scanline_p8& sl ) const;

    virtual void drawCalcIndicator ( ren_dynarow& baseRend, 
                                     solidRenderer& solidRend,
                                     scanline_rasterizer& ras, 
                                     agg::scanline_p8& sl, 
                                     color_array_type& gradColors,
                                     const QPoint& pos ) const;
signals:
    void onResize ( QSize oldS, QSize newS );
    void onMove ( QPoint oldP, QPoint newP );
    void onTrussUnitWindowHide ();
    void onTrussUnitWindowRollUp ();
    void onHintShowsUp ( const QString& hint, const QPoint pos, bool smooth );
    void onHintHides ( bool smooth );

protected slots:
    virtual void clearButtonBufRenderedFlag ();
    virtual void setWindowButtonHinted ();

private:
    QPoint windowLeftTopPos, windowRightBottomPos;
    QSize windowSize, coordFieldSize;
    DoublePoint cursorCoord;
    mutable bool buttonBufRendered, coordFieldRendered;
    color_type canvColor, headFirstColor, headMiddleColor, 
           headLastColor, borderColor, resEllColor;
    textFont headFont, numbersFont;
    rbuf_dynarow *windowBuf, *trussBuf, *coordBuf, 
                 *numbersBuf, *buttonBuf;
    TrussUnitWindowButton *hideButton, *rollUpButton;
    TrussUnitWindowButton *currentPressedButton;
    // hints subsidiaries
    TrussUnitWindowButton *currentHintedButton;
    QTimer *timer;
    QPoint hintCurrentPos;
    bool hinted;
    // minimize/maximize subsidiaries
    bool maximized;
    QSize maxSize;
    QPoint minLeftTopPos, minRightBottomPos;
};

#endif //TRUSSUNITWINDOW_H
