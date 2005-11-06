
#ifndef CALCDATATOOLBAR_H
#define CALCDATATOOLBAR_H

#include "ToolBarManager.h"
#include "TrussUnitWindow.h"

class CalcDataWidget;
class QIconSet;

class CalcDataToolBar : public ToolBarManager
{
    Q_OBJECT
public:
    CalcDataToolBar ( QMainWindow* parent = 0, const char* name = 0 );
    virtual ~CalcDataToolBar ();

    virtual CalcDataWidget* findCalcDataWidget ( TrussUnitWindow& );

protected:
    virtual void initCalcDataIcons ();
    void clear ();

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );
    virtual void removeTrussUnitWindow ( TrussUnitWindow& );

protected slots:
    virtual void addWidget ( QWidget& );

    virtual void trussUnitWindowRename( const QString& );
    // Actions on some life time signals
    virtual void trussUnitWindowDesist ( StatefulObject& );
    virtual void trussUnitWindowRevive ( StatefulObject& );
    virtual void trussUnitWindowReviveDesist ( StatefulObject&, bool );

private:
    typedef QMap<const TrussUnitWindow*, CalcDataWidget*> CalcDataMap;
    typedef CalcDataMap::iterator CalcDataMapIter;

    CalcDataMap calcDataWidgets;
    QIconSet calcDataIcons;
};

#endif //CALCDATATOOLBARMANAGER_H
