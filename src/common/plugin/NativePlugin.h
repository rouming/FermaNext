
#ifndef NATIVEPLUGIN_H
#define NATIVEPLUGIN_H

#include "Plugin.h"
#include "Global.h"

class PluginManager;
class NativePlugin;

#define PLUGIN_INSTANCE ferma_next_plugin_instance
#define PLUGIN_INSTANCE_CALL "ferma_next_plugin_instance"

/** Plugin signature in native DLL. */
typedef NativePlugin* ( *PluginInstanceCall ) ( PluginManager&, 
                                                const QString& path );

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
