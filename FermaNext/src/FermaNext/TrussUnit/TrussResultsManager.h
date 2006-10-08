
#ifndef TRUSSRESULTSMANAGER_H
#define TRUSSRESULTSMANAGER_H

#include <QDomElement>
#include <QList>

#include "Plugin.h"
#include "TrussSolutionResults.h"

class TrussUnit;
class TrussUnitWindow;
class TrussUnitWindowManager;

typedef QList<TrussSolutionResults*> TrussResultsList;
typedef TrussResultsList::iterator TrussResultsListIter;
typedef TrussResultsList::const_iterator TrussResultsListConstIter;

class TrussResultsManager : public QObject
{
    Q_OBJECT
public:
    TrussResultsManager ( const PluginManager&, 
                          const TrussUnitWindowManager& );
    ~TrussResultsManager ();

    TrussSolutionResults& createSolutionResults ();
    TrussResultsList getResultsList () const;
    TrussSolutionResults* getResultsForTrussUnit ( 
                                           const QString& trussUUID ) const;
    bool parseExecutionResults ( Plugin::ExecutionResult, QString& errMsg );
    
protected:
    void clean ();
    
protected slots:
    void removeSolutionResults ( TrussSolutionResults& );
    void removeSolutionResults ( TrussUnitWindow& );

private:
    TrussResultsList trussResultsList;
    const PluginManager& plgMng;
    const TrussUnitWindowManager& windowMng;
};

#endif //TRUSSRESULTSMANAGER_H
