
#ifndef TRUSSUNITTOOLBAR_H
#define TRUSSUNITTOOLBAR_H

#include "AggToolBar.h"
#include <QWidget>

class AggToolBarHideButton : public AggButton
{
public:
    AggToolBarHideButton ();
    virtual ~AggToolBarHideButton ();

    virtual void paint ( ren_dynarow& baseRend, 
                         scanline_rasterizer& ras,
                         agg::scanline_p8& sl, 
                         solidRenderer& solidRend  ) const;
private:
    color_type fillCol, lineCol, highlightFill;
};

/*****************************************************************************/

class TrussUnitToolBar : public AggToolBar
{
    Q_OBJECT
public:
    TrussUnitToolBar ( QPoint pos, int bordLeft, int bordRight, int bordTop, 
                       int bordBottom, int separation, int rad, QWidget* wid );
    ~TrussUnitToolBar ();

    virtual AggToolBarButton& addButton ( const QString& fname, 
                                          const QString& label, 
                                          QPoint leftTopPos, 
                                          uint width, uint height, 
                                          QWidget* widget, 
                                          const char* signal, 
                                          const char* slot );


    virtual bool inToolBarRect ( int x, int y, bool bordCheck = false ) const;

    virtual void checkMouseMoveEvent ( int x, int y );
    virtual void checkMousePressEvent ( int x, int y );

    virtual void paint ( base_renderer& baseRenderer ) const;

signals:
    void onHintShowsUp ( const QString& hint, const QPoint pos, bool smooth );
    void onHintHides ( bool smooth );

protected slots:
    void hideToolBar ();
    void showToolBar ();
    void moveToolBar ();
    void setToolBarHinted ();

protected:
    virtual QPoint getDynarowBufPos ( int x, int y ) const;
    virtual QPoint hideButtonPos ();
    virtual void initHideButton ();
    virtual void removeButtonHighlight ();
    virtual AggButton* getSelectedButton ( int x, int y ) const;
    virtual void clearButtonHint ();
    virtual void drawButtons ( ren_dynarow& baseRend, 
                               scanline_rasterizer& ras,
                               agg::scanline_p8& sl, 
                               solidRenderer& solidRend ) const;
private:
    bool enabled;
    int cornerRadius;
    int pixNumb;
    QPoint hintCurrentPos;
    AggButton* currentHintedButton;
    AggToolBarHideButton* hideButton;
    AggPaintThread* thread;
    QTimer* timer;
    color_type barFirstColor, barMiddleColor, 
               barLastColor, selectionColor;
};

#endif //TRUSSUNITTOOLBAR_H

