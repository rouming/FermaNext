
#include "TrussUnitWindowManager.h"
#include <qregexp.h>

/*****************************************************************************
 * Truss Unit Window Manager
 *****************************************************************************/

const QString TrussUnitWindowManager::NEW_EXTENSION = ".fnx";
const QString TrussUnitWindowManager::OLD_EXTENSION = ".frm";


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

TrussUnitWindow& TrussUnitWindowManager::createTrussUnitWindow ( const QString& name )
{
    TrussUnitWindow* trussWindow = new TrussUnitWindow(name);
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
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( (*iter) == &trussWindow ) {
            TrussUnitWindow* trussWindow = *iter;
            emit onTrussUnitWindowRemove(*trussWindow);
            trussWindows.erase(iter);
            delete trussWindow;
            return true;
        }            
    return false; 
}

void TrussUnitWindowManager::loadOldVersion ( TrussUnit& truss, QFile& file ) 
                                                            throw (WrongFormatException)
{
    char line[256];
    std::vector<QPoint> pivots;

    file.readLine( line, 256 );
    int pivotsNum = strtol( line,  NULL, 10 ); //nst1    

    file.readLine( line, 256 );
    int nodesNum = strtol( line,  NULL, 10 ); //nyz1

    file.readLine( line, 256 );
    //int ny1  = strtol( line,  NULL, 10 );
    file.readLine( line, 256 );
    //int e1   = strtod( line,  NULL );
    file.readLine( line, 256 );
    //int nsn1 = strtol( line,  NULL, 10 );
    file.readLine( line, 256 );
    //int sd1  = strtod( line,  NULL );

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

/*
    //TODO: convert to some pretty struct

    for ( i = 0; i < frm.nst1; ++i )
    {
        file.readLine( line, 256 );
        frm.Fn[i] = strtod( line,  NULL );
    }
    
    for ( i = 0; i < 9; ++i )
    {
        frm.msn[i][0] = 0;
        frm.msn[i][1] = 0;
    }

    for ( i = 0; i < frm.nyz1; ++i )
    {
        file.readLine( line, 256 );
        frm.msn[i][0] = strtol( line,  NULL, 10 );
    
        file.readLine( line, 256 );
        frm.msn[i][1] = strtol( line,  NULL, 10 );
    }

    for ( i = 0; i < frm.nyz1; ++i )
    {
        if ( frm.msn[i][0] == 0 )
            frm.msn[i][0] = 1;
        else
            frm.msn[i][0] = 0;
    
        if ( frm.msn[i][1] == 0 )
            frm.msn[i][1] = 1;
        else
        frm.msn[i][1] = 0;
    }

    for ( i = 0; i < frm.nsn1; ++i )
    {
        for ( int j = 0; j < frm.nyz1 * 2; ++j )
        {
            file.readLine( line, 256 );
            frm.Pn[j][i] = strtod( line,  NULL );
        }
    }

    file.readLine( line, 256 );
    frm.s_lin = line;
    
    file.readLine( line, 256 );
    frm.s_for = line;
    
    file.readLine( line, 256 );
    frm.region_x = strtod( line,  NULL );

    file.readLine( line, 256 );
    frm.region_y = strtod( line,  NULL );


    for ( int i = 0; i < frm.nyz1; ++i )
    {
        node n( frm.corn[i][0], frm.corn[i][1] );
        nodes.push_back( n );
    }

    for ( i = 0; i < frm.nst1; ++i )
    {           
        node& n1 = nodes.at( frm.iTopN[i][0] - 1 );
        node& n2 = nodes.at( frm.iTopN[i][1] - 1 );

        line p( n1.x, n1.y, n2.x, n2.y );                 
        pivots.push_back( p );        
    }
*/
}

void TrussUnitWindowManager::load ( TrussUnit& truss, const QFile& )
                                                            throw (WrongFormatException)
{
    // Implementation is looked forward
}

/*****************************************************************************/
