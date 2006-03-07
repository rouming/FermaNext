
#ifndef SUBSIDIARYCONSTANTS_H
#define SUBSIDIARYCONSTANTS_H

#include <QString>

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
                 resEllRad,
                 snapPixels;

extern const bool flipY;

/************************************
 * Truss Unit Tool Bar Consts
 ************************************/

extern const int bufferEmptyArea, 
                 buttonHeight,
                 buttonWidth,
                 pixHideNumb;

extern const double svgGamma,
                    svgExpand;

/************************************
 * Main subs
 ************************************/

QChar pathSeparator ();

QString imagesPath ();
QString imagesSvgPath ();
QString imagesCursorsPath ();
QString pluginsPath ();
QString pluginLoadersPath ();
QString configFileName ();

// Sage return of the directory that 
// contains the application executable.
QString applicationDirPath ();

// Convert absolute path to absolute
QString filePathToRelative ( const QString& fname, const QString& dir);

// Convert relative path to absolute
QString filePathToAbsolute ( const QString& fname, const QString& dir);

#endif //SUBSIDIARYCONSTANTS_H
