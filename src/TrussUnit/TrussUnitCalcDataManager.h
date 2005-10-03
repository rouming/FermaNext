
#ifndef TRUSSUNITCALCDATAMANAGER_H
#define TRUSSUNITCALCDATAMANAGER_H

#include "TrussCalcData.h"
#include <vector>
#include <qmap.h>
#include <qdatetime.h>

class TrussUnit;
class TrussTopology;

class TrussUnitCalcDataManager 
{
protected:
    class CalcCortegeList
    {
    protected:
        class CalcCortege
        {
            PluginHandle plugin;
            TrussTopology& topology;
            TrussCalcData calcData;
            QDateTime calcDateTime;
        };

    public:
        CalcCortegeList () :
            upToDate(true)
        {}        
        ~CalcCortegeList ()
        {}
            
        bool isUpToDate () const
        { return upToDate; }
        bool setUpToDate ( bool upToDate_ )
        { upToDate = upToDate_; }

        CalcData& createCalcCortege ( PluginInfo& plugin,
                                      TrussTopology& topology )
        {}
                                      
        CalcCortegeList getCortegeList ();
        CalcCortegeList findCalcCortege ( PluginInfo& );

    private:
        typedef std:::vector<CalcCortege*> CalcCortegeList;
        typedef CalcCortegeList::iterator CalcCortegeListIter;

        CalcCortegeList corteges;
        bool upToDate;
    };

public:

private:
    typedef CortegeList::const_iterator CortegeListConstIter;
    typedef QMap<TrussUnit*, CalcCortegeList*> CalculationMap;
    typedef CalculationMap::Iterator CalculationMapIter;

    CalculationMap calculations;
};

#endif //TRUSSUNITCALCDATAMANAGER_H
