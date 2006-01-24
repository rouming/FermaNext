
#ifndef GLOBAL_H
#define GLOBAL_H

#include <qglobal.h>
#include <qmetaobject.h>

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

#endif //GLOBAL_H
