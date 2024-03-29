
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QRegExp>

#include "Global.h"
#include "Config.h"

#ifdef Q_OS_WIN
 #include <windows.h>
#else
  #include <unistd.h>
#endif

namespace Global {

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
          defaultBufHeight = 188 + trussBufIndent,
          maxLoadCaseNumber = 3;

const double areaMaxDimension = 999.99,
             pivotThickLimit = 999.99,
             defaultThickness = 0.1;

/************************************
 * Truss Material Consts
 ************************************/

const double maxElasticityModule = 999999999.99,
             maxWorkingStress = 9999999.99,
             maxMaterialDensity = 999.99;

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
          windowButtonWidth = headWidth - 6,
          windowButtonHeight = windowButtonWidth, 
          snapPixels = 8;

const bool flipY = true;

/************************************
 * Truss Deform Picture Consts
 ************************************/

const int defaultCanvasWidth = 400,
          defaultCanvasHeight = 400,
          canvasHorIndent = 0,
          canvasVertIndent = 0,
          areaBufferIndent = 60;

/************************************
 * Truss Unit Tool Bar Consts
 ************************************/

const int bufferEmptyArea = 30,
          buttonHeight = 25,
          buttonWidth = 25,
          pixHideNumb = 3;

const double svgGamma = 1.0,
             svgExpand = 0.3;
 
/************************************
 * Geometry/Property Window Consts
 ************************************/

const int tableRowHeight = 17,
          nodeColumnWidth = 45,
          loadColumnWidth = 76,
          thicknessColumnWidth = 62,
          materialColumnWidth = 90;

// fixation check box side length 
const int checkBoxLen = 12;

// fixation check box indent from the cell border
const int checkBoxIndent = 1;

/************************************
 * HTML Consts
 ************************************/
const int htmlRowHeight = 25, 
          htmlColWidth = 54;

const QString htmlTitleBgColor = "#C1D2EC",
              htmlHeaderBgColor = "#EFF0F5",
              htmlSubHeaderBgColor = "#EBEBEB",
              htmlBorderColor = "#BCC6DE",
              htmlCellHighlightColor = "#FFCECE";

/************************************
 * User Editable Consts 
 ************************************/

bool showNodeNumbers = true,
     showPivotNumbers = true,
     showFixations = true,
     showLoads = true;

 /************************************
 * Main subs
 ************************************/

void sleepMsecs ( quint32 msecs )
{
#ifdef Q_OS_WIN
    Sleep(msecs);
#else
    usleep(msecs * 1000);
#endif    
}

QChar pathSeparator ()
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

QString pluginParamsPath () 
{ 
    return pluginsPath() + pathSeparator() + "params";
}

QString pluginLoadersPath ()
{
    return pluginsPath() + pathSeparator() + "loaders";
}

QString configFileName ()
{
    return applicationDirPath() + pathSeparator() + "config.xml";
}

QString loggingFileName ()
{
    return applicationDirPath() + pathSeparator() + "log4cxx.properties";
}

Config& config ()
{
    return Config::instance( configFileName() );
}

QString applicationVersion ()
{
    QString version;
#if (defined DEBUG || defined _DEBUG)
    version = QObject::tr("debug ");
#endif
    version += QString("%1, %2").arg(applicationVersionNumber()).arg(__DATE__);
    return version;
}

QString applicationVersionNumber ()
{
#ifndef FERMA_VERSION
#error Macros 'FERMA_VERSION' is required.
#endif
    return QString( TO_STR(FERMA_VERSION) );
}

QString applicationName ()
{
    QString appPath = QCoreApplication::applicationFilePath();
    QRegExp rx("(.*)/([^/]+)");
    if ( rx.indexIn(appPath) != -1 )
        return rx.cap(2);
    else
        return appPath;
}

QString applicationDirPath ()
{
    return QCoreApplication::applicationDirPath();
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
        windowsDiskExp.indexIn( path );
        QString pathDisk = windowsDiskExp.cap(1);

        windowsDiskExp.indexIn( basePath );
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
        pos = path.indexOf( "/" );
        pos1 = basePath.indexOf( "/" );
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

    int level = basePath.count( "/" );

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
        windowsDiskExp.indexIn( cutname );
        QString cutnameDisk = windowsDiskExp.cap(1);

        windowsDiskExp.indexIn( cutdir );
        QString cutdirDisk = windowsDiskExp.cap(1);

        if ( cutnameDisk != cutdirDisk )
            return cutname;
    }

    while ( (pos = cutname.indexOf("../")) >=0 ) {
        cutname.remove( 0, pos+3 );
        cutdir.remove( cutdir.length()-1, 1 );
        cutdir.remove( cutdir.lastIndexOf('/')+1 , 1000);
    }
    if ( cutdir.right(1) != "/" )
        cutdir.append('/');

    return cutdir + cutname;
}

} //namespace Global

/*****************************************************************************/
