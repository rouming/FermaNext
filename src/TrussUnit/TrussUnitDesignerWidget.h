
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnit.h"

class TrussUnitDesignerWidget : public AggQWidget
{
public:
    typedef std::vector<TrussUnit*>  TrussUnitList;

    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual TrussUnit& createTrussUnit ();
    virtual bool removeTrussUnit ( const TrussUnit& );
	virtual TrussUnit* whoIsInRect ( double x, double y );
	virtual void initTruss();
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
};

#endif //TRUSSDESIGNERWIDGET_H
