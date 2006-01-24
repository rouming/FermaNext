
#ifndef PLUGINLOADERFRONTEND_H
#define PLUGINLOADERFRONTEND_H

#include "PluginManager.h"
#include "Global.h"

#define PLUGIN_LOADER_INSTANCE ferma_next_plugin_loader_instance
#define PLUGIN_LOADER_INSTANCE_CALL "ferma_next_plugin_loader_instance"

/** 
 * Plugin loader signature in native DLL. 
 * Loading priority is an output of this call, 
 * i.e. every loader should set this param. 
 * @see PluginLoader
 */
typedef PluginLoader* ( *PluginLoaderInstanceCall ) (
                             PluginManager&, PluginManager::LoadingPriority* );

/**
 * Define OS independent macros to export plugin loader instance.
 * Example:
 * @code 
 * // plugin loader export 
 * class NativePluginLoader : public PluginLoader
 * {   
 *   // some native code goes here
 * };
 * FERMA_NEXT_PLUGIN_LOADER(NativePluginLoader);
 * @endcode
 */
#define FERMA_NEXT_PLUGIN_LOADER(LoaderClass, priority) \
    PluginExport PluginLoader* PLUGIN_LOADER_INSTANCE ( \
           PluginManager& mng, PluginManager::LoadingPriority* priorityOut ) \
    { \
      *priorityOut = priority; \
      static LoaderClass loader_instance__( mng ); \
      return &loader_instance__; \
    }

#endif //PLUGINLOADERFRONTEND_H
