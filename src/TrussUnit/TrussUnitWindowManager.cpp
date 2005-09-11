
#include "TrussUnitWindowManager.h"
#include <qregexp.h>

/*****************************************************************************
 * Truss Unit Window Manager
 *****************************************************************************/

const QString TrussUnitWindowManager::NEW_EXTENSION = ".fnx";
const QString TrussUnitWindowManager::OLD_EXTENSION = ".frm";

TrussUnitWindowManager::TrussUnitWindowManager ( ObjectStateManager& mng ) :
    stateManager(mng)
{}

TrussUnitWindowManager::~TrussUnitWindowManager ()
{
    clearTrussUnitWindows();
}

void TrussUnitWindowManager::clearTrussUnitWindows ()
{
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )       
      delete *iter;    
    trussWindows.clear();
}

void TrussUnitWindowManager::suspendedClean () 
{
    // Clean desisted truss windows without states
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ) {
        TrussUnitWindow* truss = *iter;
        if ( !truss->isAlive() && truss->countEnabledStates() == 0 ) {
            removeTrussUnitWindow(iter);
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

TrussUnitWindow& TrussUnitWindowManager::createTrussUnitWindow ( const QString& name )
{
    // Clean earlier desisted truss windows
    suspendedClean();

    TrussUnitWindow* trussWindow = new TrussUnitWindow(name, &stateManager);
    QObject::connect( trussWindow, SIGNAL(onAfterRevive(StatefulObject&)), 
                                   SLOT(trussWindowAfterRevive(StatefulObject&)) );
    QObject::connect( trussWindow, SIGNAL(onAfterDesist(StatefulObject&)), 
                                   SLOT(trussWindowAfterDesist(StatefulObject&)) );
    trussWindows.push_back(trussWindow);
    emit onTrussUnitWindowCreate(*trussWindow);
    return *trussWindow;
}

TrussUnitWindow& TrussUnitWindowManager::createTrussUnitWindowFromFile ( 
    const QString& fileName ) throw (ReadFileException,
                                     WrongFormatException)
{
    QFile file( fileName );
    if ( !file.open( IO_ReadOnly ) )
	    throw ReadFileException();

    QRegExp re("([^/\\\\]*)(\\" + OLD_EXTENSION + ")$");
    re.search(fileName);
    QString trussName = re.cap(1);
    if ( trussName.isEmpty() )        
        throw ReadFileException();

    TrussUnitWindow& trussWindow = createTrussUnitWindow(trussName);
    try {
        if( fileName.contains(NEW_EXTENSION) ) 
            load( trussWindow, file );
        else if ( fileName.contains(OLD_EXTENSION) ) 
            loadOldVersion( trussWindow, file );
        else 
            throw WrongFormatException();

    } catch ( WrongFormatException& ) {
        removeTrussUnitWindow(trussWindow);
        throw;
    }
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
    return removeTrussUnitWindow( iter );
}

bool TrussUnitWindowManager::removeTrussUnitWindow ( WindowListIter& iter )
{
    if ( iter == trussWindows.end() )
        return false;
    TrussUnitWindow* w = *iter;
    if ( w->isAlive() )
        return false;
    emit onTrussUnitWindowRemove( *w );
    trussWindows.erase(iter);
    delete w;
    return true;
}

void TrussUnitWindowManager::loadOldVersion ( TrussUnit& truss, QFile& file ) 
                                                            throw (WrongFormatException)
{
    //TODO: convert to some pretty struct all this stuff

    char line[256];
    std::vector<QPoint> pivots;

    file.readLine( line, 256 );
    int pivotsNum = strtol( line,  NULL, 10 ); //nst1    

    file.readLine( line, 256 );
    int nodesNum = strtol( line,  NULL, 10 ); //nyz1

    file.readLine( line, 256 );
    //int ny1  = strtol( line,  NULL, 10 );
    file.readLine( line, 256 );
    double elasticityModule = strtod( line,  NULL ); //e1

    file.readLine( line, 256 );
    int loadCasesNum = strtol( line,  NULL, 10 ); //nsn1

    file.readLine( line, 256 );
    double workingStress = strtod( line,  NULL ); //sd1

    TrussMaterial& material = truss.getMaterial();
    material.setElasticityModule( elasticityModule );
    material.setWorkingStress( workingStress );

    int i;

    for ( i = 0; i < pivotsNum; ++i )
    {
        uint firstNodeInd, lastNodeInd;
        file.readLine( line, 256 );
        firstNodeInd = strtol( line,  NULL, 10 );
    
        file.readLine( line, 256 );
        lastNodeInd = strtol( line,  NULL, 10 );

        pivots.push_back( QPoint(firstNodeInd, lastNodeInd) );
    }

    for ( i = 0; i < nodesNum; ++i )
    {
        int x, y;
        file.readLine( line, 256 );
        x = (int)strtod( line,  NULL );
    
        file.readLine( line, 256 );
        y = (int)strtod( line,  NULL );

        truss.createNode( x, y );
    }


    std::vector<QPoint>::iterator iter;
    for ( iter = pivots.begin(); iter != pivots.end(); ++iter ) {
        try {
            QPoint pivotInd = *iter;            
            truss.createPivot( uint(pivotInd.x()-1), uint(pivotInd.y()-1) );
        } catch ( TrussUnit::NodeIndexOutOfBoundException& ) {
            throw WrongFormatException();    
        }
    }

    for ( i = 0; i < pivotsNum; ++i )
    {
        file.readLine( line, 256 );
        //frm.Fn[i] = strtod( line,  NULL );
    }
    
    for ( i = 1; i <= nodesNum; ++i )
    {
        file.readLine( line, 256 );
        int xFixation = strtol( line,  NULL, 10 );
        //frm.msn[i][0] = strtol( line,  NULL, 10 );
        file.readLine( line, 256 );
        //frm.msn[i][1] = strtol( line,  NULL, 10 );
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

    TrussUnitLoadCases& loadCases = truss.getLoadCases();
    for ( i = 0; i < loadCasesNum; ++i )
    {
        TrussUnitLoadCase& loadCase = loadCases.createLoadCase();
        if ( i == 2 )
            loadCases.setCurrentLoadCase ( loadCase );

        for ( int j = 1; j <= nodesNum; ++j )
        {
            file.readLine( line, 256 );
            double xForce = strtod( line,  NULL );
            file.readLine( line, 256 );
            double yForce = strtod( line,  NULL );

            if ( xForce != 0 || yForce != 0 ) {
                TrussNode* node = truss.findNodeByNumber( j );
                if ( node == 0 )
                    throw WrongFormatException();
                loadCase.addLoad ( *node, xForce , yForce );
            }
        }
    }
   
    file.readLine( line, 256 );
    //frm.s_lin = line;
    
    file.readLine( line, 256 );
    //frm.s_for = line;
    
    file.readLine( line, 256 );
    int width = (int)strtod( line,  NULL );

    file.readLine( line, 256 );
    int height = (int)strtod( line,  NULL );

    truss.setTrussAreaSize( QSize( width, height ) );
}

void TrussUnitWindowManager::load ( TrussUnit&, const QFile& )
                                                   throw (WrongFormatException)
{
    // Implementation is looking forward
}

/*****************************************************************************/
