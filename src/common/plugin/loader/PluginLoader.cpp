
#include "PluginLoader.h"
#include "Plugin.h"

/*****************************************************************************
 * Plugin Loader
 *****************************************************************************/

PluginLoader::PluginLoader ( PluginManager& plgMng ) :
    pluginMng(plgMng), status(OkStatus)
{}

PluginLoader::~PluginLoader ()
{}

Plugin& PluginLoader::loadPlugin ( const QString& pathToPlugin )
    throw (PluginLoadException, PluginIsAlreadyLoadedException)
{
    if ( pluginMap.contains( pathToPlugin ) )
        throw PluginIsAlreadyLoadedException();
    Plugin& plugin = specificLoadPlugin( pathToPlugin );
    pluginMap[pathToPlugin] = &plugin;
    return plugin;
}

void PluginLoader::unloadPlugin ( const QString& pathToPlugin )
{
    if ( ! pluginMap.contains( pathToPlugin ) )
        return;
    specificUnloadPlugin( *pluginMap[pathToPlugin] );
    pluginMap.remove( pathToPlugin );
}

void PluginLoader::unloadPlugin ( Plugin& plugin )
{
    PluginMapIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it ) {
        if ( it.value() == &plugin ) {                
            specificUnloadPlugin( plugin );
            pluginMap.remove( it.key() );
            return;
        }
    }
}

void PluginLoader::unloadPlugins ()
{
    PluginMapIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it )
        specificUnloadPlugin( *it.value() );
    pluginMap.clear();
}

PluginList PluginLoader::loadedPlugins () const
{
    PluginList plugins;
    PluginMapConstIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it )
        plugins.push_back( it.value() );
    return plugins;
}

bool PluginLoader::pluginIsLoaded ( const Plugin& plugin ) const
{
    PluginMapConstIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it ) {
        if ( it.value() == &plugin )
            return true;
    }
    return false;
}

size_t PluginLoader::countLoadedPlugins () const
{
    return pluginMap.size();
}

QString PluginLoader::pathToPlugin ( const Plugin& plugin ) const
{
    PluginMapConstIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it ) {
        if ( it.value() == &plugin )
            return it.key();
    }
    return QString();
}

void PluginLoader::setStatus ( Status s )
{ 
    status = s; 
}

PluginManager& PluginLoader::pluginManager ()
{
    return pluginMng;
}

/*****************************************************************************/
