
#include "SubsidiaryConstants.h"
#include <qapplication.h>
#include <qfile.h>
#include <qdir.h>
#include <qregexp.h>

/************************************
 * Main Consts 
 ************************************/

const QString untitledWorkspaceName = QObject::tr("Unnamed workspace");

/************************************
 * Truss Unit Consts
 ************************************/

const int nodesRadius = 4,
          pivotsWidth = 2,
          trussBufIndent = 24,
          defaultBufWidth = 204 + trussBufIndent,
          defaultBufHeight = 188 + trussBufIndent;

/************************************
 * Truss Unit Window Consts
 ************************************/

const int defaultWidth = 250,
          defaultHeight = 250,
          leftWindowIndent = 42,
          rigthWindowIndent = 28,
          topWindowIndent = 48,
          bottomWindowIndent = 38,
          scaleTextLeftBottomIndent = 17,
          arrowTailIndent = 10,
          arrowHeadIndent = 16,
          scalePieceLength = 6,
          bordWidth = 4,
          headWidth = 18,
          coordLineWidth = 1,
          resizeLimit = 200,
          winCornerRadius = 24,
          resEllRad = 3,
          snapPixels = 8;

const bool flipY = true;

/************************************
 * Truss Unit Tool Bar Consts
 ************************************/

const int bufferEmptyArea = 30,
          buttonHeight = 25,
          buttonWidth = 25,
          pixHideNumb = 1;

const double svgGamma = 1.0,
             svgExpand = 0.3;
 
 /************************************
 * Main subs
 ************************************/

char pathSeparator ()
{ return QDir::separator(); }

QString imagesPath () 
{ 
    return applicationDirPath() + pathSeparator() + "images"; 
}

QString imagesSvgPath ()
{
    return applicationDirPath() + pathSeparator() + "images"  + 
                                  pathSeparator() + "svg";
}

QString imagesCursorsPath ()
{
    return applicationDirPath() + pathSeparator() + "images" +
                                  pathSeparator() + "cursors";
}

QString pluginsPath () 
{ 
    return applicationDirPath() + pathSeparator() + "plugins";
}

QString pluginLoadersPath ()
{
    return pluginsPath() + pathSeparator() + "loaders";
}

QString configFileName ()
{
    return applicationDirPath() + pathSeparator() + "config.xml";
}

QString applicationDirPath ()
{
    if ( qApp ) 
        return qApp->applicationDirPath();
    else 
        return ".";
}

QString filePathToRelative ( const QString& fname, const QString& dir )
{
    QString path = fname;
    QString basePath = dir;

    QRegExp windowsDiskExp( "^([A-Za-z]:)(.*)" );

    path.replace('\\', '/');
    basePath.replace('\\', '/');

    if ( windowsDiskExp.exactMatch( path ) || 
         windowsDiskExp.exactMatch( basePath ) ) {
        windowsDiskExp.search( path );
        QString pathDisk = windowsDiskExp.cap(1);

        windowsDiskExp.search( basePath );
        QString basePathDisk = windowsDiskExp.cap(1);

        if ( pathDisk != basePathDisk )
            return path;
    } else {
        path.remove( 0, 1 );
        basePath.remove( 0, 1 );
    }

    if ( basePath.right(1) != "/" ) 
        basePath.append( "/" );
    
    int pos = 0;
    int pos1 = 0;
    
    for ( ;; ) {
        pos = path.find( "/" );
        pos1 = basePath.find( "/" );
        if ( pos < 0 || pos1 < 0 ) 
            break;
        if ( path.left( pos+1 ) == basePath.left( pos1+1 ) ) {
            path.remove( 0, pos+1 );
            basePath.remove( 0, pos1+1 );
        }
        else
            break;
    }

    if ( basePath == "/" ) 
        basePath = "";

    int level = basePath.contains( "/" );

    for ( int i=0; i<level; i++ )
        path = "../" + path;

    return path;
}

QString filePathToAbsolute ( const QString& fname, const QString& dir )
{
    int pos;
    QString cutname = fname;
    QString cutdir = dir;

    QRegExp windowsDiskExp( "^([A-Za-z]:)(.*)" );

    cutname.replace('\\', '/');
    cutdir.replace('\\', '/');

    if ( windowsDiskExp.exactMatch( cutname ) &&
         windowsDiskExp.exactMatch( cutdir ) ) {
        windowsDiskExp.search( cutname );
        QString cutnameDisk = windowsDiskExp.cap(1);

        windowsDiskExp.search( cutdir );
        QString cutdirDisk = windowsDiskExp.cap(1);

        if ( cutnameDisk != cutdirDisk )
            return cutname;
    }

    while ( (pos = cutname.find("../")) >=0 ) {
        cutname.remove( 0, pos+3 );
        cutdir.remove( cutdir.length()-1, 1 );
        cutdir.remove( cutdir.findRev('/')+1 , 1000);
    }
    if ( cutdir.right(1) != "/" )
        cutdir.append('/');

    return cutdir + cutname;
}

/*****************************************************************************/
