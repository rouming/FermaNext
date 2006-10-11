
#include <algorithm>
#include <QMessageBox>

#include "PluginManager.h"
#include "TrussResultsManager.h"
#include "TrussUnitWindowManager.h"

/*****************************************************************************
 * Truss Results Manager
 *****************************************************************************/

TrussResultsManager::TrussResultsManager ( const PluginManager& pMng,
                                           const TrussUnitWindowManager& wMng):
    plgMng( pMng ),
    windowMng( wMng )
{
    connect( &windowMng, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
                           SLOT(removeSolutionResults(TrussUnitWindow&)) );
}

TrussResultsManager::~TrussResultsManager ()
{
    clean();
}

TrussSolutionResults& TrussResultsManager::createSolutionResults ()
{
    TrussSolutionResults* res = new TrussSolutionResults( windowMng );
    trussResultsList.push_back( res );
    return *res;
}

bool TrussResultsManager::parseExecutionResults ( 
                                              Plugin::ExecutionResult exRes,
                                              QString& errMsg )
{
    QDomDocument doc;

    if ( exRes.status == Plugin::InternalErrStatus ) {
        errMsg = exRes.data;
        return false;       
    }

    if ( ! doc.setContent( exRes.data ) ) {
        errMsg = QString( tr("Wrong XML format of the results") );
        return false;
    }

    QDomElement pluginResElem = doc.documentElement();
    if ( pluginResElem.isNull() ) {
        errMsg = QString( tr("Wrong XML format of the results") );
        return false;
    }

    if ( ! pluginResElem.hasAttribute( "trussUUID" ) ) {
        errMsg = QString( tr("Wrong XML format of the results: truss UUID wasn't set") );
        return false;
    }
    
    QString trussID = pluginResElem.attribute( "trussUUID" );

    WindowList windows = windowMng.getTrussUnitWindowList();
    TrussUnitWindow* w = 0;
    foreach ( w, windows )
        if ( w->getUUID() == trussID )
            break;

    if ( ! w ) {
        errMsg = QString( tr("Truss with the given UUID wasn't found") );
        return false;
    }

    // Create new solution results
    TrussSolutionResults* newTrussResults = &createSolutionResults();
    PluginResults& pluginRes = newTrussResults->createPluginResults();

    // Reading results XML
    try { pluginRes.loadAttributesFromXML( pluginResElem, false ); }
    catch ( ... ) {
        // Remove if there was error while reading XML
        removeSolutionResults( *newTrussResults );
        
        errMsg = QString( tr("Error while reading XML of the results") );
        return false;
    }

    // Remove old results
    TrussSolutionResults* trussResults = getResultsForTrussUnit( trussID );
    if ( trussResults )
        removeSolutionResults( *trussResults );

    newTrussResults->setTrussUnit( trussID );
    w->setCalculatedStatus( true );
    
    return true;
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

TrussUnitWindow* TrussResultsManager::findTrussByResults ( 
                                           const TrussSolutionResults& res ) const
{
    WindowList wList = windowMng.getTrussUnitWindowList();
    foreach ( TrussUnitWindow* w, wList )
        if ( w->getUUID() == res.getTrussUnitUUID() )
            return w;

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

void TrussResultsManager::removeSolutionResults ( TrussSolutionResults& res )
{
    TrussResultsListIter iter = std::find( trussResultsList.begin(), 
                                           trussResultsList.end(),
                                           &res );
    if ( iter == trussResultsList.end() )
        return;

    trussResultsList.erase( iter );    
}

void TrussResultsManager::removeSolutionResults ( TrussUnitWindow& w )
{
    TrussSolutionResults* res = getResultsForTrussUnit( w.getUUID() );
    if ( ! res )
        return;

    removeSolutionResults( *res );    
}

