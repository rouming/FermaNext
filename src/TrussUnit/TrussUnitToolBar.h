
#ifndef TRUSSUNITTOOLBAR_H
#define TRUSSUNITTOOLBAR_H

#include "AggToolBar.h"
#include "qthread.h"
#include "qwidget.h"

/*****************************************************************************/

class ToolBarThread : public QObject, public QThread
{
    Q_OBJECT
public:
    ToolBarThread ( QWidget& p );
    virtual ~ToolBarThread ();

    virtual void setSleepTime ( int timeMsec );
    virtual void setAttemtsNumber ( int numb );

    virtual void run ();

signals:
    void onToolBarMove ();

private:
    int attemtsNumb, msecToSleep;
    QWidget& widgetToPaint; 
};

/*****************************************************************************/

class AggToolBarHideButton : public AggButton
{
public:
    AggToolBarHideButton ();
    virtual ~AggToolBarHideButton ();
    virtual void paint ( ren_dynarow& baseRend, scanline_rasterizer& ras,
                         agg::scanline_p8& sl, solidRenderer& solidRend  ) const;
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
                                          QObject* widget, 
                                          const char* signal, 
                                          const char* slot );


    virtual bool inToolBarRect ( int x, int y ) const;

    virtual void checkMouseMoveEvent ( int x, int y );
    virtual void checkMousePressEvent ( int x, int y );

    virtual void paint ( base_renderer& baseRenderer ) const;

protected slots:
    void hideToolBar ();
    void showToolBar ();
    void moveToolBar ();

protected:
    virtual QPoint getDynarowBufPos ( int x, int y ) const;
    virtual QPoint hideButtonPos ();
    virtual void initHideButton ();
    virtual void removeButtonHighlight ();
    virtual AggToolBarButton* getSelectedButton ( int x, int y ) const;
    virtual void drawButtons ( ren_dynarow& baseRend, 
                               scanline_rasterizer& ras,
                               agg::scanline_p8& sl, 
                               solidRenderer& solidRend ) const;
private:
    int pixNumb;
    int cornerRadius;
    AggToolBarHideButton* hideButton;
    bool enabled;
    ToolBarThread* thread;
    color_type barFirstColor, barMiddleColor, barLastColor, selectionColor;
};

#endif //TRUSSUNITTOOLBAR_H

