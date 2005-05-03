
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnit.h"

class TrussUnitDesignerWidget : public AggQWidget
{
public:
    bool init; //temp
    typedef std::vector<TrussUnit*>  TrussUnitList;

    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual TrussUnit& createTrussUnit ();
    virtual bool removeTrussUnit ( const TrussUnit& );
    virtual bool isHorResize ( int x, int y );
    virtual bool isVerResize ( int x, int y );
    virtual bool isBDiagResize ( int x, int y );
    virtual bool isFDiagResize ( int x, int y );
    virtual TrussUnit* whoIsInRect ( int x, int y );
	virtual void initTruss(); //temp
	virtual void onDraw();

    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );

private:
    TrussUnitList trussUnits;
    TrussUnit* selectedTruss;
    enum winBehaviours { nothing = 0, onDrag, onHorResize, onVerResize, 
        onBDiagResize, onFDiagResize };
    int behaviour;
    int clickX, clickY;
};

#endif //TRUSSDESIGNERWIDGET_H
