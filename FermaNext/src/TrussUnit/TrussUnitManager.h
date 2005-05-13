
#ifndef TRUSSUNITMANAGER_H
#define TRUSSUNITMANAGER_H

#include <vector>
#include <qobject.h>

#include "TrussUnit.h"

typedef std::vector<TrussUnit*>  TrussUnitList;
typedef std::vector<TrussUnit*>::iterator  TrussUnitListIter;

class TrussUnitManager : public QObject
{
    Q_OBJECT
public:    
    virtual ~TrussUnitManager ();

protected:
    virtual void clear ();

public slots:
    virtual TrussUnit& createTrussUnit ();
    virtual bool removeTrussUnit ( const TrussUnit& );

signals:
    void onTrussUnitCreate ( TrussUnit& );
    void onTrussUnitRemove ( const TrussUnit& );

private:
    TrussUnitList trussUnits;
};

#endif //TRUSSUNITMANAGER_H
