
#ifndef TRUSSUNITCALCDATAMANAGER_H
#define TRUSSUNITCALCDATAMANAGER_H

#include <vector>
#include <qmap.h>
#include <qdatetime.h>

class TrussUnit;
class TrussTopology;

class TrussUnitCalcDataManager 
{
protected:
    class Cortege
    {
        PluginInfo plugin;
        TrussTopology& topology;
        CalcData calcData;
        QDateTime calcDateTime;                
    };

    class CalcCortege
    {
    private:
        typedef std:::vector<Cortege*> CortegeList;
        typedef CortegeList::iterator CortegeListIter;        

        CortegeList corteges;
        bool isUpToDate;
    };

public:

private:
    typedef CortegeList::const_iterator CortegeListConstIter;
    typedef QMap<TrussUnit*, CalcCortege*> CalculationMap;
    typedef CalculationMap::Iterator CalculationMapIter;

    CalculationMap calculations;
};

#endif //TRUSSUNITCALCDATAMANAGER_H
