
#ifndef PLUGIN_H
#define PLUGIN_H

#ifdef __cplusplus
  #define ExternC extern "C"
#else
  #define ExternC extern
#endif

#if defined WINDOWS || defined WIN32
  #define PluginExport ExternC __declspec( dllexport )
  #define PluginImport ExternC __declspec( dllimport )
  #define StandardCall __stdcall
#else
  #define PluginExport ExternC
  #define PluginImport ExternC
  #define StandardCall
#endif

class Plugin;
class TrussTopology;
class CalcData;

// Plugin signature.
typedef Plugin& ( StandardCall *PluginInstanceCall ) ();

enum PluginType { 
                  CALCULATION_PLUGIN  = 0x0001, 
                  OPTIMIZATION_PLUGIN = 0x0010
                };

// Plain Old Data for plugin info.
// Plugin name should be short and unique in system context.
struct PluginInfo
{
    const char* name;
    const char* description;
};

// Main plugin class
class Plugin
{
public:
    virtual PluginInfo& pluginInfo () const = 0;
    virtual PluginType pluginType () const = 0;
    virtual bool dependsOn ( PluginType ) const = 0;
    //    virtual void calculate ( TrussTopology&, CalcData& ) const = 0;
};

// Calculation plugin
class CalculationPlugin : public Plugin
{
public:
    PluginType pluginType () 
    { return CALCULATION_PLUGIN; }
};

// Optimization plugin
class OptimizationPlugin : public Plugin
{
public:
    PluginType pluginType ()
    { return OPTIMIZATION_PLUGIN; }
};

#define PLUGIN_INSTANCE ferma_next_plugin_instance
#define PLUGIN_INSTANCE_CALL "ferma_next_plugin_instance"


// Define OS independent macros to provide init and fini functions
// in dynamic libraries.
// Example:
// 
// Init code: 
// FERMA_NEXT_PLUGIN_INIT
// {
//   /* some init code goes here */
// }
//
// Fini code: 
// FERMA_NEXT_PLUGIN_FINI
// {
//   /* some fini code goes here */
// }
//
#if defined WINDOWS || defined WIN32
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

  #define FERMA_NEXT_PLUGIN(info) \
    PluginExport PluginInfo& PLUGIN_INFO () \
    { return info; } \
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

  #define FERMA_NEXT_PLUGIN(info) \
    PluginExport PluginInfo& StandardCall PLUGIN_INFO () \
    { return info; }
#endif


#endif //PLUGIN_H
