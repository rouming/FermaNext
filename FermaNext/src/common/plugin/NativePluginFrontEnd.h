/**
 * This is a front end for every dynamic library which 
 * greatly wants to be a FN native plugin :-)
 *
 */

#ifndef PLUGINFRONTEND_H
#define PLUGINFRONTEND_H

#include "NativePlugin.h"

/**
 * Define OS independent macros to provide init and fini functions
 * in dynamic libraries and plugin export macros.
 *
 * NOTE: init and fini macroses should always go after 
 * FERMA_NEXT_PLUGIN macros as in the example below.
 *
 * Example:
 *
 * @code
 * // plugin export 
 * class SomeCalcPlugin : public NativePlugin
 * {   
 *   // some native code goes here
 * };
 * FERMA_NEXT_PLUGIN(SomeCalcPlugin);
 *
 * // init code
 * FERMA_NEXT_PLUGIN_INIT
 * {
 *   // some init code goes here
 * }
 *
 * // fini code
 * FERMA_NEXT_PLUGIN_FINI
 * {
 *   // some fini code goes here
 * }
 * @endcode
 */
#if defined _WIN32 || defined WIN32
  #include <windows.h>

  #define FERMA_NEXT_PLUGIN_INIT \
    void init_plugin__ (); \
    class caller_init__ \
    { public: caller_init__() { initCaller__ = init_plugin__; } }; \
    caller_init__ init__; \
    void init_plugin__ ()

  #define FERMA_NEXT_PLUGIN_FINI \
    void fini_plugin__ (); \
    class caller_fini__ \
    { public: caller_fini__() { finiCaller__ = fini_plugin__; } }; \
    caller_fini__ fini__; \
    void fini_plugin__ ()

  #define FERMA_NEXT_PLUGIN(PluginClass) \
    PluginExport NativePlugin* PLUGIN_INSTANCE ( PluginManager& mng,   \
                                                 const QString& path ) \
    { \
      static PluginClass plugin_instance__( mng, path ); \
      return &plugin_instance__; \
    } \
    \
    typedef void (* InitFiniCall__) (); \
    template <typename Call> \
    class caller__ \
    { \
    public: \
      caller__ () : call(0) {} \
      caller__& operator= ( const Call& call_ ) \
      { call = call_; return *this; } \
      void operator() () { if ( call ) call(); } \
    private: \
      Call call; \
    }; \
    caller__<InitFiniCall__> initCaller__; \
    caller__<InitFiniCall__> finiCaller__; \
    \
    BOOL WINAPI DllMain( \
      HINSTANCE hinstDLL, \
      DWORD fdwReason,    \
      LPVOID lpvReserved  \
    ) \
    { \
      if ( fdwReason == DLL_PROCESS_ATTACH ) \
        initCaller__(); \
      else if ( fdwReason == DLL_PROCESS_DETACH ) \
        finiCaller__(); \
      return TRUE; \
    }
#else
  #define FERMA_NEXT_PLUGIN_INIT \
    __attribute__((constructor)) void init_plugin__ ()
  #define FERMA_NEXT_PLUGIN_FINI \
    __attribute__((destructor)) void fini_plugin__ ()

  #define FERMA_NEXT_PLUGIN(PluginClass) \
    PluginExport NativePlugin* PLUGIN_INSTANCE ( PluginManager& mng,   \
                                                 const QString& path ) \
    { \
      static PluginClass plugin_instance__( mng, path ); \
      return &plugin_instance__; \
    }
#endif

#endif //PLUGINFRONTEND_H
