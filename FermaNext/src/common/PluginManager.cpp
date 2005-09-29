
#include "PluginManager.h"
#include <qdir.h>

/*****************************************************************************
 * Plugin Manager
 *****************************************************************************/

PluginManager::~PluginManager ()
{
    clean();
}

void PluginManager::clean ()
{
    PluginsMapIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        delete iter.data();
    }
    plugins.clear();
}

void PluginManager::loadPlugins ( const QString& path )
{
    clean();
    QDir dir( path, "*." + PluginLoader::libExtension(),
              QDir::Name | QDir::IgnoreCase, 
              QDir::Files | QDir::Readable );
    for ( uint i = 0; i < dir.count(); i++ ) {
        /*
        FermaNextPlugin* plugin = new FermaNextPlugin;
        try {
            PluginLoader& loader = plugin->loader;
            loader.loadLibrary( path + "/" + dir[i] );
            PluginInfoCall pluginInstanceCall = 
             (PluginInstanceCall)loader.getProcAddress( PLUGIN_INSTANCE_CALL );
            Plugin& pluginInstance = pluginInstanceCall();
            plugin->info = pluginInstance;
            plugins[i] = plugin;
        } catch ( ... ) {
            delete plugin;
        }
        */
    }
}

void PluginManager::unloadPlugins ()
{
    clean();
}

void PluginManager::unloadPlugin ( const PluginHandle& h )
{
    if ( plugins.contains(h) ) {
        delete plugins[h];
        plugins.erase(h);
    }
}

FermaNextPluginInfo& PluginManager::findPluginInfo ( const PluginHandle& h ) 
    throw (FindException)
{
    if ( ! plugins.contains(h) )
        throw FindException();
    FermaNextPlugin* plugin = plugins[h];
    return plugin->info;
}

PluginHandleList PluginManager::loadedPlugins () const
{
    PluginHandleList handles;
    PluginsMapConstIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        handles.push_back( iter.key() );
    }    
    return handles;
}

QStringList PluginManager::loadedPluginsNames () const
{
    QStringList names;
    PluginsMapConstIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        names.push_back( iter.data()->info.name );
    }        
    return names;
}

PluginHandleList PluginManager::loadedPluginsOfType ( const PluginType& type ) const
{
    PluginHandleList handles;
    PluginsMapConstIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        if ( iter.data()->info.type == type )
            handles.push_back( iter.key() );
    }    
    return handles;
}

/*****************************************************************************/
