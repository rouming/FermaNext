/**
 * This is a front end for every dynamic library which 
 * greatly wants to be a FN native plugin :-)
 *
 */

#ifndef PLUGINFRONTEND_H
#define PLUGINFRONTEND_H

#include "NativePlugin.h"

/**
 * Define OS independent plugin export macros.
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
 * @endcode
 */

#define FERMA_NEXT_PLUGIN(PluginClass) \
    static PluginClass* plugin_instance__ = 0; \
    PluginExport NativePlugin* PLUGIN_INSTANCE_INIT ( PluginManager& mng,   \
                                                      const QString& path ) \
    { \
      plugin_instance__ = new PluginClass( mng, path ); \
      return plugin_instance__; \
    } \
    \
    PluginExport void PLUGIN_INSTANCE_FINI () \
    { \
      delete plugin_instance__; \
      plugin_instance__ = 0; \
    }

/** Easy way to get plugin instance from plugin. */
#define PLUGIN_INSTANCE plugin_instance__

#endif //PLUGINFRONTEND_H
