
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

extern const int nodesRadius,
                 pivotsWidth,
                 defaultBufWidth,
                 defaultBufHeight,
                 trussBufIndent;

/************************************
 * Truss Unit Window Consts
 ************************************/

extern const int defaultWidth,
                 defaultHeight,
                 leftWindowIndent,
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
                 resEllRad;

extern const bool flipY;

/************************************
 * Truss Unit Tool Bar Consts
 ************************************/

extern const uint bufferEmptyArea, 
                  buttonHeight,
                  buttonWidth,
                  pixHideNumb;

extern const double svgGamma,
                    svgExpand;

/************************************
 * Main subs
 ************************************/

QString imagesPath ();
QString imagesSvgPath (); 
QString pluginsPath ();
QString configFileName ();

// Sage return of the directory that 
// contains the application executable.
QString applicationDirPath ();

#endif //SUBSIDIARYCONSTANTS_H
