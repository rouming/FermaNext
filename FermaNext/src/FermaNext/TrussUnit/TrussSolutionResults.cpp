
#include "TrussSolutionResults.h"
#include "TrussUnitCopy.h"
#include "TrussUnitWindowManager.h"
#include "Plugin.h"

/*****************************************************************************
 * Node Results
 *****************************************************************************/

void LoadCaseResults::NodeResults::loadFromXML (
                                   const QDomElement& nodeResElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( nodeResElem );
    loadAttributesFromXML( nodeResElem, true );
}

void LoadCaseResults::NodeResults::loadAttributesFromXML (
                                   const QDomElement& nodeResElem, bool ) 
    /*throw (LoadException)*/
{
    if ( ! nodeResElem.hasAttribute("dispX") )
        throw LoadException();
    QString valueStr = nodeResElem.attribute( "dispX" );
    bool valid;
    dispX = valueStr.toDouble( &valid );
    if ( ! valid )
        throw LoadException();

    if ( ! nodeResElem.hasAttribute("dispY") )
        throw LoadException();
    valueStr = nodeResElem.attribute( "dispY" );
    dispY = valueStr.toDouble( &valid );
    if ( ! valid )
        throw LoadException();
    
    if ( ! nodeResElem.hasAttribute("nodeNumber") )
        throw LoadException();
    valueStr = nodeResElem.attribute( "nodeNumber" );
    nodeNumb = valueStr.toInt( &valid );
    if ( ! valid )
        throw LoadException();
}

QDomElement LoadCaseResults::NodeResults::saveToXML ( QDomDocument& doc )
{
    QDomElement nodeResElem = XMLSerializableObject::saveToXML( doc );
    nodeResElem.setTagName( "NodeResults" );

    nodeResElem.setAttribute( "dispX", dispX );
    nodeResElem.setAttribute( "dispY", dispY );
    nodeResElem.setAttribute( "nodeNumber", nodeNumb );

    return nodeResElem;
}

/*****************************************************************************
 * Pivot Results
 *****************************************************************************/

void LoadCaseResults::PivotResults::loadFromXML (
                                   const QDomElement& pivotResElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( pivotResElem );
    loadAttributesFromXML( pivotResElem, true );
}

void LoadCaseResults::PivotResults::loadAttributesFromXML (
                                   const QDomElement& pivotResElem, bool ) 
    /*throw (LoadException)*/
{
    if ( ! pivotResElem.hasAttribute("stress") )
        throw LoadException();
    QString valueStr = pivotResElem.attribute( "stress" );
    bool valid;
    stress = valueStr.toDouble( &valid );
    if ( ! valid )
        throw LoadException();

    if ( ! pivotResElem.hasAttribute("requiredThickness") )
        throw LoadException();
    valueStr = pivotResElem.attribute( "requiredThickness" );
    requiredThick = valueStr.toDouble( &valid );
    if ( ! valid )
        throw LoadException();

    if ( ! pivotResElem.hasAttribute("pivotNumber") )
        throw LoadException();
    valueStr = pivotResElem.attribute( "pivotNumber" );
    pivotNumb = valueStr.toInt( &valid );
    if ( ! valid )
        throw LoadException();
}

QDomElement LoadCaseResults::PivotResults::saveToXML ( QDomDocument& doc )
{
    QDomElement pivotResElem = XMLSerializableObject::saveToXML( doc );
    pivotResElem.setTagName( "PivotResults" );
    pivotResElem.setAttribute( "stress", stress );
    pivotResElem.setAttribute( "requiredThickness", requiredThick );
    pivotResElem.setAttribute( "pivotNumber", pivotNumb );
    return pivotResElem;
}

/*****************************************************************************
 * Load Case Results
 *****************************************************************************/

LoadCaseResults::LoadCaseResults () :
    loadCaseNumb( 0 )
{}

LoadCaseResults::~LoadCaseResults ()
{
    clean();
}

LoadCaseResults::NodeResults& LoadCaseResults::addNodeResults ( double dispX, 
                                                                double dispY, 
                                                                int numb )
{
    NodeResults* nodeRes = new NodeResults( dispX, dispY, numb );
    nodeResultsList.push_back( nodeRes );
    return *nodeRes;
}

LoadCaseResults::PivotResults& LoadCaseResults::addPivotResults ( double stress, 
                                                                double reqThick, 
                                                                int numb )
{
    PivotResults* pivotRes = new PivotResults( stress, reqThick, numb );
    pivotResultsList.push_back( pivotRes );
    return *pivotRes;
}

DoublePoint LoadCaseResults::getDisplacement ( int indx, bool& valid ) const
{
    if ( nodeResultsList.empty() || indx < 0 || 
         indx >= nodeResultsList.size() ) {
        valid = false;
        return DoublePoint( 0, 0 );
    }
    NodeResults* nodeRes = 0;
    foreach ( nodeRes, nodeResultsList )
        if ( nodeRes->nodeNumb == indx + 1 ) {
            valid = true;
            return DoublePoint( nodeRes->dispX, nodeRes->dispY );
        }
    valid = false;
    return DoublePoint( 0, 0 );
}

double LoadCaseResults::getStress ( int indx, bool& valid ) const
{
    if ( pivotResultsList.empty() || indx < 0 || 
         indx >= pivotResultsList.size() ) {
        valid = false;
        return 0;
    }
    PivotResults* pivotRes = 0;
    foreach ( pivotRes, pivotResultsList )
        if ( pivotRes->pivotNumb == indx + 1 ) {
            valid = true;
            return pivotRes->stress;
        }
    valid = false;
    return 0;
}

double LoadCaseResults::getRequiredThickness ( int indx, bool& valid ) const
{
    if ( pivotResultsList.empty() || indx < 0 || 
         indx >= pivotResultsList.size() ) {
        valid = false;
        return 0;
    }
    PivotResults* pivotRes = 0;
    foreach ( pivotRes, pivotResultsList )
        if ( pivotRes->pivotNumb == indx + 1 ) {
            valid = true;
            return pivotRes->requiredThick;
        }
    valid = false;
    return 0;
}

int LoadCaseResults::countDisplacements () const
{
    return nodeResultsList.size();
}

int LoadCaseResults::countStresses () const
{
    return pivotResultsList.size();
}

void LoadCaseResults::setLoadCaseNumber ( int numb )
{
    loadCaseNumb = numb;
}

int LoadCaseResults::getLoadCaseNumber() const
{
    return loadCaseNumb;
}

void LoadCaseResults::clean ()
{
    NodeResultsListIter nIter = nodeResultsList.begin();
    for ( ; nIter != nodeResultsList.end(); ++nIter )
        delete *nIter;
    nodeResultsList.clear();

    PivotResultsListIter pIter = pivotResultsList.begin();
    for ( ; pIter != pivotResultsList.end(); ++pIter )
        delete *pIter;
    pivotResultsList.clear();
}

void LoadCaseResults::loadFromXML ( const QDomElement& loadCaseResElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( loadCaseResElem );
    loadAttributesFromXML( loadCaseResElem, true );   
}

void LoadCaseResults::loadAttributesFromXML ( const QDomElement& loadCaseResElem, 
                                              bool uuidLoad )
{
    // Destroy existent results
    clean();

    /** 
     * Set load case number 
     ***********************/
    if ( ! loadCaseResElem.hasAttribute("loadCaseNumber") )
        throw LoadException();
    QString valueStr = loadCaseResElem.attribute( "loadCaseNumber" );
    bool valid;
    int loadCaseNumb = valueStr.toInt( &valid );
    if ( ! valid )
        throw LoadException();
    setLoadCaseNumber( loadCaseNumb );

    /** 
     * Fill node results
     *********************/
    QDomNodeList nodes = 
            loadCaseResElem.elementsByTagName( "NodeResults" );
    for ( int i = 0; i < nodes.count(); ++i ) {
        QDomNode node = nodes.item( i );
        if ( ! node.isElement() )
            throw LoadException();
        QDomElement nodeResElem = node.toElement();

        // fill with default zero values
        NodeResults& nRes = addNodeResults( 0, 0, 0 );
        if ( uuidLoad )
            nRes.loadFromXML( nodeResElem );
        else
            nRes.loadAttributesFromXML( nodeResElem, false );
    }

    /** 
     * Fill pivot results
     **********************/
    QDomNodeList pivots = 
            loadCaseResElem.elementsByTagName( "PivotResults" );
    for ( int i = 0; i < pivots.count(); ++i ) {
        QDomNode pivot = pivots.item( i );
        if ( ! pivot.isElement() )
            throw LoadException();
        QDomElement pivotResElem = pivot.toElement();

        // fill with default zero values
        PivotResults& pRes = addPivotResults( 0, 0, 0 );
        if ( uuidLoad )
            pRes.loadFromXML( pivotResElem );
        else
            pRes.loadAttributesFromXML( pivotResElem, false );
    } 
}

QDomElement LoadCaseResults::saveToXML ( QDomDocument& doc )
{
    QDomElement loadCaseResElem = XMLSerializableObject::saveToXML( doc );
    loadCaseResElem.setTagName( "LoadCaseResults" );

    /**
     * Save load case number
     ************************/
    loadCaseResElem.setAttribute( "loadCaseNumber", getLoadCaseNumber() );

    /**
     * Save node results
     *********************/
    NodeResults* nodeRes = 0;
    foreach ( nodeRes, nodeResultsList )
        loadCaseResElem.appendChild( nodeRes->saveToXML( doc ) );

    /**
     * Save pivot results
     *********************/
    PivotResults* pivotRes = 0;
    foreach ( pivotRes, pivotResultsList )
        loadCaseResElem.appendChild( pivotRes->saveToXML( doc ) );

    return loadCaseResElem;
}

/*****************************************************************************
 * Plugin Results
 *****************************************************************************/

PluginResults::PluginResults () :
    forceWeight( 0 )
{}

PluginResults::~PluginResults ()
{
    clean();
}

LoadCaseResults& PluginResults::createLoadCaseResults ()
{
    LoadCaseResults* loadCaseRes = new LoadCaseResults;
    loadCaseResults.push_back( loadCaseRes );
    return *loadCaseRes;
}

const LoadCaseResults* PluginResults::getLoadCaseResults ( int numb ) const
{
    // load case numbers start with 1
    if ( numb <= 0 )
        return 0;

    LoadCaseResultsList::const_iterator iter = loadCaseResults.begin();
    for ( ; iter != loadCaseResults.end(); ++iter ) {
        LoadCaseResults* res = *iter;
        if ( res->getLoadCaseNumber() == numb )
            return res;
    }
    return 0;
}

void PluginResults::setPluginName ( const QString& name )
{
    pluginName = name;
}

const QString& PluginResults::getPluginName () const
{
    return pluginName;
}

void PluginResults::setForceWeight ( double value )
{
    forceWeight = value;
}

double PluginResults::getForceWeight () const
{
    return forceWeight;
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

void PluginResults::loadFromXML ( const QDomElement& plgResElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( plgResElem );
    loadAttributesFromXML( plgResElem, true );
}

void PluginResults::loadAttributesFromXML ( const QDomElement& plgResElem, 
                                            bool uuidLoad )
{
    // Destroy existent load case results
    clean();

    /** 
     * Set plugin name 
     *******************/
    if ( ! plgResElem.hasAttribute("pluginName") )
        throw LoadException();
    setPluginName( plgResElem.attribute( "pluginName" ) );

    /** 
     * Set force weight value 
     *************************/
    if ( ! plgResElem.hasAttribute("forceWeight") )
        throw LoadException();
    QString valueStr = plgResElem.attribute( "forceWeight" );
    bool valid;
    double fWeight = valueStr.toDouble( &valid );
    if ( ! valid )
        throw LoadException();
    setForceWeight( fWeight );
    
    /** 
     * Create load cases results
     ****************************/
    QDomNodeList loadCaseResElements = 
            plgResElem.elementsByTagName( "LoadCaseResults" );
    for ( int i = 0; i < loadCaseResElements.count(); ++i ) {
        QDomNode loadCaseResNode = loadCaseResElements.item( i );
        if ( ! loadCaseResNode.isElement() )
            throw LoadException();
        QDomElement loadCaseResElem = loadCaseResNode.toElement();

        LoadCaseResults& loadCaseRes = createLoadCaseResults();
        if ( uuidLoad )
            loadCaseRes.loadFromXML( loadCaseResElem );
        else
            loadCaseRes.loadAttributesFromXML( loadCaseResElem, false );
    }    
}


QDomElement PluginResults::saveToXML ( QDomDocument& doc )
{
    QDomElement pluginResultsElem = XMLSerializableObject::saveToXML( doc );
    pluginResultsElem.setTagName( "PluginResults" );

    /**
     * Save plugin name
     ********************/
    pluginResultsElem.setAttribute( "pluginName", getPluginName() );

    /**
     * Save force weight
     ********************/
    pluginResultsElem.setAttribute( "forceWeight", getForceWeight() );

    /**
     * Save load case results
     *************************/
    LoadCaseResults* res = 0;
    foreach ( res, loadCaseResults )
        pluginResultsElem.appendChild( res->saveToXML( doc ) );
    
    return pluginResultsElem;
}

/*****************************************************************************
 * Truss Solution Results
 *****************************************************************************/

TrussSolutionResults::TrussSolutionResults ( const TrussUnitWindowManager& mng ) :
    windowMng( mng ),
    trussUUID( QString() ),
    trussCopy( new TrussUnitCopy )
{}

TrussSolutionResults::~TrussSolutionResults ()
{
    clean();
    delete trussCopy;
}

PluginResults& TrussSolutionResults::createPluginResults ()
{
    PluginResults* plgRes = new PluginResults();
    pluginResults.push_back( plgRes );
    return *plgRes;
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

void TrussSolutionResults::setTrussUnit ( const QString& trussUnitUUID )
{
    trussUUID = trussUnitUUID;

    WindowList windows = windowMng.getTrussUnitWindowList();
    TrussUnitWindow* w = 0;
    foreach ( w, windows )
        if ( w->getUUID() == trussUUID )
            break;
    if ( ! w )
        return;

    trussCopy->loadTrussUnitData( *w );
}

const QString& TrussSolutionResults::getTrussUnitUUID () const
{
    return trussUUID;
}

TrussUnitCopy& TrussSolutionResults::getTrussUnitCopy () const
{
    return *trussCopy;
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

void TrussSolutionResults::loadFromXML ( const QDomElement& trussResElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( trussResElem );

    // Destroy existent plugin results
    clean();

    /** 
     * Init truss unit copy 
     ***********************/
    if ( ! trussResElem.hasAttribute( "trussUUID" ) )
        throw LoadException();
    setTrussUnit( trussResElem.attribute( "trussUUID" ) );

    /** 
     * Set plugin results
     **********************/
    QDomNodeList plgResList = trussResElem.elementsByTagName( "PluginResults" );
    for ( int i = 0; i < plgResList.count(); ++i ) {
        QDomNode plgRes = plgResList.item( i );
        if ( ! plgRes.isElement() )
            throw LoadException();
        QDomElement plgResElem = plgRes.toElement();

        PluginResults& pluginRes = createPluginResults();
        pluginRes.loadFromXML( plgResElem );
    }
}

QDomElement TrussSolutionResults::saveToXML ( QDomDocument& doc )
{
    QDomElement trussResElem = XMLSerializableObject::saveToXML( doc );
    trussResElem.setTagName( "TrussSolutionResults" );

    /**
     * Save truss UUID
     *******************/
    trussResElem.setAttribute( "trussUUID", getTrussUnitUUID() );

    /**
     * Save plugin results
     ***********************/    
    PluginResults* res = 0;
    foreach ( res, pluginResults )
        trussResElem.appendChild( res->saveToXML( doc ) );
    
    return trussResElem;
}

/*****************************************************************************/
