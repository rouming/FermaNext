
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

const int leftWindowIndent = 38,
          rigthWindowIndent = 28,
          topWindowIndent = 52,
          bottomWindowIndent = 40,
          scaleTextLeftBottomIndent = 17,
          arrowTailIndent = 10,
          arrowHeadIndent = 16,
          scalePieceLength = 8,
          bordWidth = 4,
          headWidth = 24,
          coordLineWidth = 1,
          resizeLimit = 200,
          winCornerRadius = 25,
          resEllRad = 3,
          nodesRadius = 4,
          pivotsWidth = 2;

const bool flipY = true;


/************************************
 * Desginer Consts
 ************************************/

// Empty
 
 /************************************
 * Main subs
 ************************************/

QString imagesPath () 
{ 
    return applicationDirPath() + "/" + "images"; 
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
