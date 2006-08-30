
#include <algorithm>

#include "PluginManager.h"
#include "TrussResultsManager.h"
#include "TrussUnitWindowManager.h"

/*****************************************************************************
 * Truss Results Manager
 *****************************************************************************/

TrussResultsManager::TrussResultsManager ( const PluginManager& pMng,
                                           const TrussUnitWindowManager& wMng) :
    plgMng( pMng ),
    windowMng( wMng )
{
    connect( &plgMng, SIGNAL(onAfterPluginLoad(Plugin&)),
                        SLOT(afterPluginWasLoaded(Plugin&)) );
    connect( &plgMng, SIGNAL(onBeforePluginUnload(Plugin&)),
                        SLOT(beforePluginWasUnloaded(Plugin&)) );
    connect( &windowMng, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
                           SLOT(removeSolutionResults(TrussUnitWindow&)) );
}

TrussResultsManager::~TrussResultsManager ()
{
    clean();
}

void TrussResultsManager::afterPluginWasLoaded ( Plugin& plg )
{
    connect( &plg, SIGNAL(afterExecution(Plugin&, Plugin::ExecutionResult)),
                     SLOT(parseExecutionResults(Plugin&, 
                                                Plugin::ExecutionResult)) );
}

void TrussResultsManager::beforePluginWasUnloaded ( Plugin& plg )
{
    disconnect( &plg, SIGNAL(afterExecution(Plugin&, Plugin::ExecutionResult)),
                  this, SLOT(parseExecutionResults(Plugin&, 
                                                   Plugin::ExecutionResult)) );
}

TrussSolutionResults& TrussResultsManager::createTrussSolutionResults ()
{
    TrussSolutionResults* res = new TrussSolutionResults( windowMng );
    trussResultsList.push_back( res );
    return *res;
}

TrussSolutionResults& TrussResultsManager::parseExecutionResults ( 
                                                Plugin& plg, 
                                                Plugin::ExecutionResult exRes )
                                            /*throw (ResultsReadingException)*/
{
    QDomDocument doc;
    if ( ! doc.setContent( exRes.data ) )
        throw ResultsReadingException();

    QDomElement pluginResElem = doc.documentElement();
    if ( pluginResElem.isNull() )
        throw ResultsReadingException();

    if ( ! pluginResElem.hasAttribute( "trussUUID" ) )
        throw WrongResultsFormat();
    
    QString trussID = pluginResElem.attribute( "trussUUID" );

    WindowList windows = windowMng.getTrussUnitWindowList();
    TrussUnitWindow* w = 0;
    foreach ( w, windows )
        if ( w->getUUID() == trussID )
            break;

    if ( ! w )
        throw ResultsReadingException();

    TrussSolutionResults* trussResults = getResultsForTrussUnit( trussID );
    if ( trussResults && ! w->isCalculated() ) {
        delete trussResults;
        trussResults = &createTrussSolutionResults();
        trussResults->setTrussUnit( trussID );
    } 
    else if ( ! trussResults ) {
        trussResults = &createTrussSolutionResults();
        trussResults->setTrussUnit( trussID );
    }

    PluginResults& pluginRes = trussResults->createPluginResults();

    try { pluginRes.loadAttributesFromXML( pluginResElem, false ); }
    catch ( ... ) {
        throw ResultsReadingException();
    }
    w->setCalculatedStatus( true );
    return *trussResults;
}

TrussSolutionResults* TrussResultsManager::getResultsForTrussUnit ( 
                                               const QString& trussUUID ) const
{
    TrussResultsListConstIter iter = trussResultsList.begin();
    for ( ; iter < trussResultsList.end(); ++iter ) {
        TrussSolutionResults* res = *iter;
        if ( res->getTrussUnitUUID() == trussUUID )
            return res;
    }
    return 0;
}

TrussResultsList TrussResultsManager::getResultsList () const
{
    return trussResultsList;
}

void TrussResultsManager::clean ()
{
    TrussResultsListIter iter = trussResultsList.begin();
    for ( ; iter != trussResultsList.end(); ++iter )
        delete *iter;
    trussResultsList.clear();
}

void TrussResultsManager::removeSolutionResults ( TrussUnitWindow& w )
{
    TrussSolutionResults* res = getResultsForTrussUnit( w.getUUID() );
    if ( ! res )
        return;

    TrussResultsListIter iter = std::find( trussResultsList.begin(), 
                                           trussResultsList.end(),
                                           res );
    if ( iter == trussResultsList.end() )
        return;

    trussResultsList.erase( iter );    
}

