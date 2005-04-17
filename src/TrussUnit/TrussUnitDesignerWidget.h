
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnit.h"

class TrussUnitDesignerWidget : public AggQWidget
{
public:
    typedef std::vector<TrussUnit*>  TrussUnitList;

    TrussUnitDesignerWidget ();
    virtual TrussUnit& createTrussUnit ();
    virtual bool removeTrussUnit ( const TrussUnit& );

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
