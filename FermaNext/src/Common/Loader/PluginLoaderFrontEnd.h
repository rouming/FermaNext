
#ifndef PLUGINLOADERFRONTEND_H
#define PLUGINLOADERFRONTEND_H

#include "PluginManager.h"
#include "Global.h"

#define PLUGIN_LOADER_INSTANCE_INIT ferma_next_plugin_loader_instance_init
#define PLUGIN_LOADER_INSTANCE_FINI ferma_next_plugin_loader_instance_fini
#define PLUGIN_LOADER_INSTANCE_INIT_CALL "ferma_next_plugin_loader_instance_init"
#define PLUGIN_LOADER_INSTANCE_FINI_CALL "ferma_next_plugin_loader_instance_fini"

/** 
 * Creates plugin loader instance in native DLL.
 * Loading priority is an output of this call, 
 * i.e. every loader should set this param. 
 * @see PluginLoader
 */
typedef PluginLoader* ( *PluginLoaderInstanceInitCall ) (
                             PluginManager&, 
                             const QString& path,
                             PluginManager::LoadingPriority* );

/** 
 * Destroyes plugin loader instance in native DLL.
 * @see PluginLoader
 */
typedef void ( *PluginLoaderInstanceFiniCall ) ();


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
    static LoaderClass* loader_instance__ = 0; \
    PluginExport PluginLoader* PLUGIN_LOADER_INSTANCE_INIT ( \
           PluginManager& mng, \
           const QString& path, \
           PluginManager::LoadingPriority* priorityOut ) \
    { \
      *priorityOut = priority; \
      loader_instance__ = new LoaderClass( mng, path ); \
      return loader_instance__; \
    } \
    \
    PluginExport void PLUGIN_LOADER_INSTANCE_FINI () \
    { \
      delete loader_instance__; \
      loader_instance__ = 0; \
    }

#endif //PLUGINLOADERFRONTEND_H
