
#include "PluginLoader.h"
#include "Plugin.h"

/*****************************************************************************
 * Plugin Loader
 *****************************************************************************/

PluginLoader::PluginLoader ( const QString& pluginName ) 
    throw (PluginLoadException) 
    try :  dynaLoader( pluginName )
{
    PluginInstanceCall pluginInstanceCall = 
         (PluginInstanceCall)dynaLoader.getProcAddress( PLUGIN_INSTANCE_CALL );
    plugin = pluginInstanceCall();
    if ( plugin == 0 )
        throw PluginLoadException();
}
catch ( ... )
{
    throw PluginLoadException();
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
