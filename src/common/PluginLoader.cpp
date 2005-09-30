
#include "PluginLoader.h"
#include "Plugin.h"

/*****************************************************************************
 * Plugin Loader
 *****************************************************************************/

PluginLoader::PluginLoader ( const QString& pluginName ) 
    throw (PluginLoadException)
{
    try {
        dynaLoader.loadLibrary( pluginName );
        PluginInstanceCall pluginInstanceCall = (PluginInstanceCall)
                   dynaLoader.getProcAddress( PLUGIN_INSTANCE_CALL );
        plugin = pluginInstanceCall();
        if ( plugin == 0 )
            throw PluginLoadException();
    }
    catch ( ... )
    {
        if ( dynaLoader.isLoaded() )
            dynaLoader.freeLibrary();
        throw PluginLoadException();
    }
}

PluginLoader::~PluginLoader ()
{
    dynaLoader.freeLibrary();
}

Plugin& PluginLoader::pluginInstance ()
{ 
    return *plugin;
}

const Plugin& PluginLoader::pluginInstance () const
{ 
    return *plugin;
}

/*****************************************************************************/
