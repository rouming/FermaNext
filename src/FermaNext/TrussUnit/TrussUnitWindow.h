
#ifndef TRUSSUNITWINDOW_H
#define TRUSSUNITWINDOW_H

#include "TrussUnit.h"

#include <QTimer>

class QWidget;
class AggTrussWindowButton;

class TrussUnitWindow : public TrussUnit
{
    Q_OBJECT
public:
    TrussUnitWindow ( const QString& name, ObjectStateManager*,
                      const TrussMaterialLibrary& );
    virtual ~TrussUnitWindow ();

    // Set owner of this window. 
    // e.g. window uses owner for correct maximizing.
    virtual void setWindowOwner ( QWidget* owner );

    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    virtual QDomElement saveToXML ( QDomDocument& );

    // Manage window position
    virtual void setWindowPosition ( QPoint pos );
    // Returns false if window position was not set, true otherwise
    virtual bool hasPosition () const;
    virtual QPoint getWindowLeftTopPos () const;
    virtual QPoint getWindowRightBottomPos () const;
    virtual QPoint getTrussAreaLeftTopPos () const;
    virtual QPoint getTrussAreaRightBottomPos () const;

    // Get/set window size
    virtual void setWindowSize ( int w, int h );
    virtual void setWindowSize ( const QSize& size );
    virtual const QSize& getWindowSize () const;
    // If window has its owner, it returns window size to
    // which it can be maximized. Or 0,0 is returned.
    virtual QSize getMaximizedWindowSize () const;
    // Resize and then move window
    virtual void resize ( QPoint leftTop, QPoint rightBottom );

    virtual bool isMaximized () const;
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

    virtual AggTrussWindowButton* getButtonByCoord ( int xGlobal, 
                                                     int yGlobal ) const;
   
    virtual void setHighlighted ( bool );
    virtual void removeButtonsHighlight ();
    virtual void releaseButtons ();
    virtual void clearButtonHint ();

    virtual void checkMouseMoveEvent ( int x, int y, bool buttonPressed );
    virtual void checkMousePressEvent ( int x, int y );
    virtual void checkMouseReleaseEvent ( int x, int y );

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
    void initWindowButtons ();

    virtual QPoint getButtonBufPos () const;
    virtual bool inButtonBufRect ( int x, int y ) const;
    
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
    void onClearButtonRenderedFlag ();

protected slots:
    void clearWindowRenderedFlag ();
    void clearButtonBufRenderedFlag ();
    void setWindowButtonHinted ();
    void setProportionalSize ();

private:
    // Owner of this window
    QWidget* windowOwner;

    // Window has position
    bool positionIsSet;

    // Window left top position in designer widget coordinates
    QPoint windowLeftTopPos;

    // Window size in pixels
    QSize windowSize;

    // Current cursor coordinate in truss coordinate system
    DoublePoint cursorCoord;

    // Render flags 
    mutable bool buttonBufRendered, coordFieldRendered;

    // Window palette colors
    color_type canvColor, headFirstColor, headMiddleColor, 
               headLastColor, borderColor, resEllColor;

    // Window fonts
    textFont headFont, numbersFont;

    // Window blending buffers
    rbuf_dynarow *windowBuf, *trussBuf, *coordBuf, 
                 *numbersBuf, *buttonBuf;

    // Window button list
    QList<AggTrussWindowButton*> windowButtons;

    // Currently pressed window button
    AggTrussWindowButton *currentPressedButton;

    // Currently hinted window button
    AggTrussWindowButton *currentHintedButton;

    // Hint timer
    QTimer *timer;

    // Current hint position
    QPoint hintCurrentPos;

    // Is some window button currently hinted
    bool hinted;

    // Is window maximized
    bool maximized;

    // Saved window geometry for minimization
    QPoint minLeftTopPos, minRightBottomPos;
};

#endif //TRUSSUNITWINDOW_H
