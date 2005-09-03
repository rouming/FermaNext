
#ifndef SUBSIDIARYCONSTANTS_H
#define SUBSIDIARYCONSTANTS_H

#include <qstring.h>

/************************************
 * Main Consts 
 ************************************/

extern const QString untitledWorkspaceName;

/************************************
 * Truss Unit Consts
 ************************************/

extern const int leftWindowIndent,
                 rigthWindowIndent,
                 topWindowIndent,
                 bottomWindowIndent,
                 scaleTextLeftBottomIndent,
                 arrowTailIndent,
                 arrowHeadIndent,
                 scalePieceLength,
                 bordWidth,
                 headWidth,
                 coordLineWidth,
                 resizeLimit,
                 winCornerRadius,
                 resEllRad,
                 nodesRadius,
                 pivotsWidth;

extern const bool flipY;

/************************************
 * Desginer Consts
 ************************************/

/************************************
 * Main subs
 ************************************/

QString imagesPath ();
QString pluginsPath ();
QString configFileName ();

// Sage return of the directory that 
// contains the application executable.
QString applicationDirPath ();

#endif //SUBSIDIARYCONSTANTS_H
