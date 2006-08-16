
#include "TrussSolutionResults.h"
#include "TrussUnitCopy.h"
#include "Plugin.h"

/*****************************************************************************
 * Load Case Results
 *****************************************************************************/

LoadCaseResults::LoadCaseResults ( int numb ) :
    loadCaseNumb( numb )
{}

LoadCaseResults::~LoadCaseResults ()
{
    clean();
}

DoublePoint LoadCaseResults::getDisplacement( int indx, bool* valid ) const
{
    if ( displacements.empty() || indx < 0 || indx >= displacements.size() ) {
        *valid = false;
        return DoublePoint( 0, 0 );
    }
    *valid = true;
    return *displacements.at( indx );
}

double LoadCaseResults::getStress( int indx, bool* valid ) const
{
    if ( stresses.empty() || indx < 0 || indx >= displacements.size() ) {
        *valid = false;
        return 0;
    }
    *valid = true;
    return stresses.at( indx );
}

void LoadCaseResults::addDisplacement ( double xDisp, double yDisp, int indx )
{
    displacements.insert( indx, new DoublePoint( xDisp, yDisp) );
}

void LoadCaseResults::addStress ( double stress, int indx )
{
    stresses.insert( indx, stress );
}

void LoadCaseResults::removeDisplacement ( int indx )
{
    if ( displacements.empty() || indx < 0 || indx >= displacements.size() )
        return;

    displacements.removeAt( indx );
}

void LoadCaseResults::removeStress ( int indx )
{
    if ( stresses.empty() || indx < 0 || indx >= stresses.size() )
        return;

    stresses.removeAt( indx );
}

int LoadCaseResults::countDisplacements () const
{
    return displacements.size();
}

int LoadCaseResults::countStresses () const
{
    return stresses.size();
}

void LoadCaseResults::clean ()
{
    DispListIter iter = displacements.begin();
    for ( ; iter != displacements.end(); ++iter )
        delete *iter;
    displacements.clear();
}

int LoadCaseResults::getLoadCaseNumber() const
{
    return loadCaseNumb;
}

void LoadCaseResults::loadFromXML ( const QDomElement& trussResultsElem ) 
    /*throw (LoadException)*/
{}

QDomElement LoadCaseResults::saveToXML ( QDomDocument& doc )
{
    QDomElement pluginResultsElem = XMLSerializableObject::saveToXML( doc );
    pluginResultsElem.setTagName( "PluginResults" );
    return pluginResultsElem;
}

/*****************************************************************************
 * Plugin Results
 *****************************************************************************/

PluginResults::PluginResults ( const QString& name ) :
    pluginName( name )
{}

PluginResults::~PluginResults ()
{
    clean();
}

void PluginResults::addLoadCaseResults ( LoadCaseResults& res )
{
    loadCaseResults.push_back( &res );
}

const LoadCaseResults* PluginResults::getLoadCaseResults ( int numb ) const
{
    // load case numbers start with 1
    if ( numb <= 0 )
        return 0;

    LoadCaseResultsList::const_iterator iter = loadCaseResults.begin();
    for ( ; iter != loadCaseResults.end(); ++iter )
        if ( (*iter)->getLoadCaseNumber() == numb )
            return *iter;
    return 0;
}

int PluginResults::countLoadCaseResults () const
{
    return loadCaseResults.size();
}

void PluginResults::clean ()
{
    LoadCaseResultsListIter iter = loadCaseResults.begin();
    for ( ; iter != loadCaseResults.end(); ++iter )
        delete *iter;
    loadCaseResults.clear();
}

const QString& PluginResults::getPluginName () const
{
    return pluginName;
}

void PluginResults::loadFromXML ( const QDomElement& trussResultsElem ) 
    /*throw (LoadException)*/
{}

QDomElement PluginResults::saveToXML ( QDomDocument& doc )
{
    QDomElement pluginResultsElem = XMLSerializableObject::saveToXML( doc );
    pluginResultsElem.setTagName( "PluginResults" );
    return pluginResultsElem;
}

/*****************************************************************************
 * Truss Solution Results
 *****************************************************************************/

TrussSolutionResults::TrussSolutionResults ( const TrussUnit& truss ) :
    trussUnit( truss ),
    trussCopy( new TrussUnitCopy )
{
    trussCopy->loadTrussUnitData( truss ); 
}

TrussSolutionResults::~TrussSolutionResults ()
{
    clean();
}

void TrussSolutionResults::addPluginResults ( const PluginResults& results )
{
    if ( ! trussUnit.isCalculated() )
        clean();
    pluginResults.push_back( &results );
}

const TrussUnit& TrussSolutionResults::getTrussUnit () const
{
    return trussUnit;
}

TrussUnitCopy& TrussSolutionResults::getTrussUnitCopy () const
{
    return *trussCopy;
}

const PluginResults* TrussSolutionResults::getPluginResults( int indx ) const
{
    if ( indx >= pluginResults.size() )
        return 0;

    try {
        return pluginResults.at( indx );
    }
    catch ( ... ) {
        return 0;
    }
}

int TrussSolutionResults::countResults () const
{
    return pluginResults.size();
}

void TrussSolutionResults::clean ()
{
    PluginResultsListIter iter = pluginResults.begin();
    for ( ; iter != pluginResults.end(); ++iter )
        delete *iter;
    pluginResults.clear();
}

void TrussSolutionResults::loadFromXML ( const QDomElement& trussResultsElem ) 
    /*throw (LoadException)*/
{}

QDomElement TrussSolutionResults::saveToXML ( QDomDocument& doc )
{
    QDomElement trussResultsElem = XMLSerializableObject::saveToXML( doc );
    trussResultsElem.setTagName( "TrussSolutionResults" );
    return trussResultsElem;
}

/*****************************************************************************/
