
#ifndef GLOBAL_H
#define GLOBAL_H

/*****************************************************************************/

#define ExternC extern "C"

#if defined _WIN32 || defined WIN32
  #define PluginExport ExternC __declspec( dllexport )
  #define PluginImport ExternC __declspec( dllimport )
  #define StandardCall __stdcall
#else
  #define PluginExport ExternC
  #define PluginImport ExternC
  #define StandardCall
#endif

/*****************************************************************************/

#include <QString>

class Config;

namespace Global {

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

extern const double areaMaxDimension,
                    pivotThickLimit,
                    defaultThickness;

/************************************
 * Truss Material Consts
 ************************************/

extern const double maxElasticityModule,
                    maxWorkingStress,
                    maxMaterialDensity;

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
 * Truss Deform Picture Consts
 ************************************/

extern const int defaultCanvasWidth,
                 defaultCanvasHeight,
                 canvasHorIndent,
                 canvasVertIndent,
                 areaBufferIndent;

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
 * Geometry/Property Window Consts
 ************************************/

extern const int tableRowHeight,
                 nodeColumnWidth,
                 loadColumnWidth,
                 thicknessColumnWidth,
                 materialColumnWidth,
                 checkBoxLen,
                 checkBoxIndent;

/************************************
 * HTML Consts
 ************************************/
extern const int htmlRowHeight, 
                 htmlColWidth;

extern const QString htmlTitleBgColor,
                     htmlHeaderBgColor,
                     htmlSubHeaderBgColor,
                     htmlBorderColor;

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
QString loggingFileName ();
Config& config ();

// Returns application version in format: 
//    for release build: 'x.x.x, MMM DD YYYY'
//    for debug   build: 'debug x.x.x, MMM DD YYYY'
QString applicationVersion ();

// Returns application name
QString applicationName ();

// Sage return of the directory that 
// contains the application executable.
QString applicationDirPath ();

// Convert absolute path to absolute
QString filePathToRelative ( const QString& fname, const QString& dir);

// Convert relative path to absolute
QString filePathToAbsolute ( const QString& fname, const QString& dir);

} //namespace Global

#endif //GLOBAL_H
