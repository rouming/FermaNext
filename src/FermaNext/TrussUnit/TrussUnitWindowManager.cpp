
#include "TrussUnitWindowManager.h"
#include "StatefulObject.h"

#include <QRegExp>

// Indeces of povot nodes
struct PivotNodes
{
    PivotNodes ( uint first_, uint last_ ) :
        first(first_),
        last(last_)
    {}
    uint first;
    uint last;
};

typedef std::vector<PivotNodes> PivotNodesList;
typedef PivotNodesList::iterator PivotNodesListIter;

/*****************************************************************************
 * Truss Unit Window Manager
 *****************************************************************************/

const QString& TrussUnitWindowManager::newFormatExtension ()
{
    static QString extension = ".fnx";
    return extension;
}

const QString& TrussUnitWindowManager::oldFormatExtension ()
{
    static QString extension = ".frm";
    return extension;
}

/*****************************************************************************/

TrussUnitWindowManager::TrussUnitWindowManager ( 
    const TrussMaterialLibrary& lib) :
    materialLib(lib)
{}

TrussUnitWindowManager::~TrussUnitWindowManager ()
{
    clearTrussUnitWindows();
}

void TrussUnitWindowManager::clearTrussUnitWindows ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); )
        iter = removeTrussUnitWindow( iter );
    stateManagerMap.clear();
}

void TrussUnitWindowManager::suspendedClean () 
{
    // Clean desisted truss windows without states
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ) {
        TrussUnitWindow* truss = *iter;
        if ( !truss->isAlive() && truss->countEnabledStates() == 0 ) {
            iter = removeTrussUnitWindow(iter);
        }
        else
            ++iter;
    }
}

void TrussUnitWindowManager::trussWindowAfterRevive ( StatefulObject& st )
{
    try {
        onTrussUnitWindowRevive( dynamic_cast<TrussUnitWindow&>(st) );
    } catch ( ... ) {}
}

void TrussUnitWindowManager::trussWindowAfterDesist ( StatefulObject& st )
{
    try {
        onTrussUnitWindowDesist( dynamic_cast<TrussUnitWindow&>(st) );
    } catch ( ... ) {}
}

TrussUnitWindow& TrussUnitWindowManager::createTrussUnitWindow (
    const QString& name )
{
    return createTrussUnitWindow( false, name );
}

TrussUnitWindow& TrussUnitWindowManager::createTrussUnitWindow (
    bool silence, 
    const QString& name )
{
    // Clean earlier desisted truss windows
    suspendedClean();

    ObjectStateManager* stateManager = new ObjectStateManager;
    TrussUnitWindow* trussWindow = 
        new TrussUnitWindow(name, stateManager, materialLib);
    stateManagerMap[trussWindow] = stateManager;

    QObject::connect( trussWindow, SIGNAL(onAfterRevive(StatefulObject&)), 
                      SLOT(trussWindowAfterRevive(StatefulObject&)) );
    QObject::connect( trussWindow, SIGNAL(onAfterDesist(StatefulObject&)), 
                      SLOT(trussWindowAfterDesist(StatefulObject&)) );
    QObject::connect( this, 
                      SIGNAL(beforeMaterialRemove(const TrussMaterial&)), 
                      trussWindow, 
                      SLOT(removePivotMaterial(const TrussMaterial&)) );
    QObject::connect( trussWindow, 
                      SIGNAL(onVisibleChange(bool)), 
                      SIGNAL(onTrussWindowVisibilityChange(bool)) );
    
    trussWindows.push_back(trussWindow);

    if ( ! silence )
        emit onTrussUnitWindowCreate(*trussWindow);

    return *trussWindow;
}

TrussUnitWindow& TrussUnitWindowManager::createTrussUnitWindowFromFile ( 
    const QString& fileName ) /*throw (ReadFileException,
                                       WrongFormatException)*/
{
    QFile file( fileName );
    if ( !file.open(QIODevice::ReadOnly) )
	    throw ReadFileException();

    QRegExp re("([^/\\\\]*)((\\" + oldFormatExtension() + ")|"
                           "(\\" + newFormatExtension() + "))$");
    re.indexIn(fileName);
    QString trussName = re.cap(1);
    if ( trussName.isEmpty() )        
        throw ReadFileException();

    // Use 'silence' method to create truss and then to remove it if
    // exception has happened.
    TrussUnitWindow& trussWindow = createTrussUnitWindow( true, trussName );
    try {
        if( fileName.contains( newFormatExtension() ) ) 
            loadNewVersion( trussWindow, file );
        else if ( fileName.contains( oldFormatExtension() ) ) 
            loadOldVersion( trussWindow, file );
        else 
            throw WrongFormatException();

    } catch ( WrongFormatException& ) {
        removeTrussUnitWindow(trussWindow);
        throw;
    }
    emit onTrussUnitWindowCreate(trussWindow);
    return trussWindow;
}

bool TrussUnitWindowManager::removeTrussUnitWindow ( 
    TrussUnitWindow& trussWindow )
{
    // Clean earlier desisted truss windows
    suspendedClean();

    WindowListIter iter = std::find( trussWindows.begin(), 
                                     trussWindows.end(), 
                                     &trussWindow );
    if ( iter == trussWindows.end() )
        return false;

    // Desist first
    (*iter)->desist();
    iter = removeTrussUnitWindow( iter );
    if ( iter == trussWindows.end() )
        return false;
    return true;
}

WindowListIter TrussUnitWindowManager::removeTrussUnitWindow ( 
    WindowListIter iter )
{
    if ( iter == trussWindows.end() )
        return trussWindows.end();
    TrussUnitWindow* w = *iter;
    if ( w->isAlive() )
        return trussWindows.end();
    emit onTrussUnitWindowRemove( *w );
    iter = trussWindows.erase(iter);
    ObjectStateManager* stateMng = stateManagerMap[w];
    stateManagerMap.remove(w);
    delete w;
    delete stateMng;
    return iter;
}

WindowList TrussUnitWindowManager::getTrussUnitWindowList () const
{
    WindowList aliveTrussWindows;
    WindowListConstIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( (*iter)->isAlive() )
            aliveTrussWindows.push_back( *iter );

    return aliveTrussWindows;
}

void TrussUnitWindowManager::loadOldVersion ( TrussUnit& truss, QFile& file ) 
    /*throw (WrongFormatException)*/
{
    char line[256];

    file.readLine( line, 256 );
    uint pivotsNum = strtol( line,  NULL, 10 ); //nst1    

    file.readLine( line, 256 );
    uint nodesNum = strtol( line,  NULL, 10 ); //nyz1

    file.readLine( line, 256 );
    //int ny1  = strtol( line,  NULL, 10 );
    file.readLine( line, 256 );

    // FIXME: should we read material from old format?

    //double elasticityModule = strtod( line,  NULL ); //e1

    file.readLine( line, 256 );
    uint loadCasesNum = strtol( line,  NULL, 10 ); //nsn1

    file.readLine( line, 256 );

    // FIXME: should we read material from old format?

    //double workingStress = strtod( line,  NULL ); //sd1

    //TrussMaterial& material = truss.getMaterial();
    //material.setElasticityModule( elasticityModule );
    //material.setWorkingStress( workingStress );


    // Support for old-new format, which contains odd double
    //   if this line represents double -- assuming old-new format
    //   if integer -- old-old format
    QString str( file.readLine() );
    bool oldOldFormat = false;
    str.toInt( &oldOldFormat );
    

    uint i;
    PivotNodesList pivots;

    for ( i = 0; i < pivotsNum; ++i )
    {
        uint firstNodeInd, lastNodeInd;
        if ( ! oldOldFormat || i > 0 ) {
            file.readLine( line, 256 );
            firstNodeInd = strtol( line,  NULL, 10 );
        } else
            firstNodeInd = str.toInt();
    
        file.readLine( line, 256 );
        lastNodeInd = strtol( line,  NULL, 10 );

        pivots.push_back( PivotNodes(firstNodeInd, lastNodeInd) );
    }

    for ( i = 0; i < nodesNum; ++i )
    {
        double x, y;
        file.readLine( line, 256 );
        x = strtod( line,  NULL );
    
        file.readLine( line, 256 );
        y = strtod( line,  NULL );

        truss.createNode( x, y );
    }

    for ( PivotNodesListIter iter = pivots.begin(); 
          iter != pivots.end(); 
          ++iter ) {
        try {
            PivotNodes pivotInd = *iter;            
            truss.createPivot( pivotInd.first - 1, pivotInd.last - 1 );
        } catch ( TrussUnit::NodeIndexOutOfBoundException& ) {
            throw WrongFormatException();
        }
    }

    for ( i = 1; i <= pivotsNum; ++i )
    {
        TrussPivot* pivot = truss.findPivotByNumber( i );
        if ( pivot == 0 )
            throw WrongFormatException();
        file.readLine( line, 256 );
        double thickness = strtod( line,  NULL );
        pivot->setThickness( thickness );
    }
    
    for ( i = 1; i <= nodesNum; ++i )
    {
        file.readLine( line, 256 );
        int xFixation = strtol( line,  NULL, 10 );

        file.readLine( line, 256 );
        int yFixation = strtol( line,  NULL, 10 );

        TrussNode* node = truss.findNodeByNumber( i );
        if ( node == 0 )
            throw WrongFormatException();

        Node::Fixation fix = Node::Unfixed;
        // All fixations of the old format are unusual swapped!
        // So should be inverted!
        if ( !xFixation && !yFixation )
            fix = Node::FixationByXY;
        else if ( !xFixation )
            fix = Node::FixationByX;
        else if ( !yFixation )
            fix = Node::FixationByY;

        node->setFixation( fix );
    }

    TrussUnit::LoadCases& loadCases = truss.getLoadCases();
    for ( i = 0; i < loadCasesNum; ++i )
    {
        TrussUnit::LoadCase* loadCase;
        if ( i == 0 )
            loadCase = loadCases.getCurrentLoadCase();
        else
            loadCase = &loadCases.createLoadCase();

        if ( i == 2 )
            loadCases.setCurrentLoadCase ( *loadCase );

        for ( uint j = 1; j <= nodesNum; ++j )
        {
            file.readLine( line, 256 );
            double xForce = strtod( line,  NULL );
            file.readLine( line, 256 );
            double yForce = strtod( line,  NULL );

            if ( xForce != 0 || yForce != 0 ) {
                TrussNode* node = truss.findNodeByNumber( j );
                if ( node == 0 )
                    throw WrongFormatException();
                loadCase->addLoad ( *node, xForce , yForce );
            }
        }
    }

    TrussDimension& dim = truss.getDimension();

    QString dimensionStr;
   
    file.readLine( line, 256 );
    dimensionStr = line;
    if ( dimensionStr.contains("μμ", Qt::CaseInsensitive) )
        dim.setLengthMeasure( TrussDimension::mm );
    else if ( dimensionStr.contains("ρμ", Qt::CaseInsensitive) )
        dim.setLengthMeasure( TrussDimension::sm );
    else
        dim.setLengthMeasure( TrussDimension::m );        

    file.readLine( line, 256 );
    dimensionStr = line;
    if ( dimensionStr.contains("Ν", Qt::CaseInsensitive) )
        dim.setForceMeasure( TrussDimension::newton );
    else
        dim.setForceMeasure( TrussDimension::kg );
    
    file.readLine( line, 256 );
    double width = strtod( line,  NULL );

    file.readLine( line, 256 );
    double height = strtod( line,  NULL );

    truss.setTrussAreaSize( DoubleSize( width, height ) );
}

void TrussUnitWindowManager::loadNewVersion ( TrussUnit& truss, 
                                              QFile& xmlFile )
    /*throw (WrongFormatException)*/
{
    QDomDocument doc;
    QIODevice* xmlIODev = &xmlFile;
    if ( !doc.setContent( xmlIODev ) ) {
        throw WrongFormatException();
    }

    QDomElement docElem = doc.documentElement();
    if ( docElem.isNull() )
        throw WrongFormatException();

    try { 
        truss.loadFromXML( docElem );
    } catch ( TrussUnit::LoadException& ) {
        throw WrongFormatException();
    }
}

/*****************************************************************************/
