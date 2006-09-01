
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

    class ResultsReadingException {};
    class WrongResultsFormat {};

    TrussSolutionResults& createTrussSolutionResults ();
    TrussResultsList getResultsList () const;
    TrussSolutionResults* getResultsForTrussUnit ( 
                                           const QString& trussUUID ) const;
protected:
    TrussSolutionResults& parseExecutionResults ( Plugin::ExecutionResult );
                                       /*throw (ResultsReadingException)*/;
    void clean ();

public slots:
    void pluginWasExecuted ( Plugin&, Plugin::ExecutionResult );
    
protected slots:
    void removeSolutionResults ( TrussUnitWindow& );
    void afterPluginWasLoaded ( Plugin& );
    void beforePluginWasUnloaded ( Plugin& );

private:
    TrussResultsList trussResultsList;
    const PluginManager& plgMng;
    const TrussUnitWindowManager& windowMng;
};

#endif //TRUSSRESULTSMANAGER_H
