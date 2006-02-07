
#ifndef NATIVEPLUGIN_H
#define NATIVEPLUGIN_H

#include "Plugin.h"
#include "Global.h"

class PluginManager;
class NativePlugin;

#define PLUGIN_INSTANCE_INIT ferma_next_plugin_instance_init
#define PLUGIN_INSTANCE_FINI ferma_next_plugin_instance_fini
#define PLUGIN_INSTANCE_INIT_CALL "ferma_next_plugin_instance_init"
#define PLUGIN_INSTANCE_FINI_CALL "ferma_next_plugin_instance_fini"

/** Creates plugin instance in native DLL. */
typedef NativePlugin* ( *PluginInstanceInitCall ) ( PluginManager&, 
                                                    const QString& path );
/** Destroyes plugin instance in native DLL. */
typedef void ( *PluginInstanceFiniCall ) ();

/** 
 * Basic native plugin class.
 */
class NativePlugin : public Plugin
{
 public:
    NativePlugin ( PluginManager& mng, const QString& path ) :
        Plugin( mng, path )
    {}    
};

#endif //NATIVEPLUGIN_H
