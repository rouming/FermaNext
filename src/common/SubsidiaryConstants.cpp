
#include "SubsidiaryConstants.h"
#include <qapplication.h>
#include <qfile.h>

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
          resEllRad = 3;

const bool flipY = true;

/************************************
 * Truss Unit Tool Bar Consts
 ************************************/

const uint bufferEmptyArea = 30,
           buttonHeight = 25,
           buttonWidth = 25,
           pixHideNumb = 2;

const double svgGamma = 1.0,
             svgExpand = 0.3;
 
 /************************************
 * Main subs
 ************************************/

QString imagesPath () 
{ 
    return applicationDirPath() + "/" + "images"; 
}

QString imagesSvgPath ()
{
    return applicationDirPath() + "/" + "images/svg";
}

QString pluginsPath () 
{ 
    return applicationDirPath() + "/" + "plugins";
}

QString configFileName ()
{
    return applicationDirPath() + "/" + "config.xml";
}

QString applicationDirPath ()
{
    if ( qApp ) 
        return qApp->applicationDirPath();
    else 
        return ".";
}

/*****************************************************************************/
