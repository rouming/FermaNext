
#ifndef CALCDATATOOLBAR_H
#define CALCDATATOOLBAR_H

#include "ToolBarManager.h"
#include "TrussUnitWindow.h"

class QIconSet;

class CalcDataToolBar : public ToolBarManager
{
    Q_OBJECT
public:
    CalcDataToolBar ( QMainWindow* parent = 0, const char* name = 0 );
    virtual ~CalcDataToolBar ();

protected:
    virtual void initCalcDataIcons ();
    void clear ();

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );
    virtual void removeTrussUnitWindow ( const TrussUnitWindow& );

protected slots:
    virtual void addWidget ( QWidget& );

private:
    typedef QMap<const TrussUnitWindow*, QWidget*> CalcDataMap;
    typedef CalcDataMap::iterator CalcDataMapIter;

    CalcDataMap calcDataWidgets;
    QIconSet calcDataIcons;
};

#endif //CALCDATATOOLBARMANAGER_H
