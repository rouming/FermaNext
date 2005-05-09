
#include "TrussUnitManager.h"

/*****************************************************************************
 * Truss Unit Manager
 *****************************************************************************/

TrussUnitManager::~TrussUnitManager ()
{
    clear();
}

void TrussUnitManager::clear ()
{
    TrussUnitListIter iter = trussUnits.begin();
    for ( ; iter != trussUnits.end(); ++iter )       
      delete *iter;    
    trussUnits.clear();
}

TrussUnit& TrussUnitManager::createTrussUnit ()
{
    TrussUnit* truss = new TrussUnit;
    trussUnits.push_back(truss);
    emit onTrussUnitCreate(*truss);
    return *truss;
}

bool TrussUnitManager::removeTrussUnit ( const TrussUnit& truss )
{
    TrussUnitListIter iter = trussUnits.begin();
    for ( ; iter != trussUnits.end(); ++iter )
        if ( (*iter) == &truss ) {
            TrussUnit* truss = *iter;
            emit onTrussUnitRemove(*truss);
            trussUnits.erase(iter);
            delete truss;
            return true;
        }            
    return false; 
}

/*****************************************************************************/
