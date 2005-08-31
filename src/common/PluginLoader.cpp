
#include "PluginLoader.h"
#include <qdir.h>

/*****************************************************************************
 * Plugin Loader
 *****************************************************************************/

PluginLoader::~PluginLoader ()
{
    clean();
}

void PluginLoader::clean ()
{
    PluginsMapIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        delete iter.data();
    }
    plugins.clear();
}

void PluginLoader::loadPlugins ( const QString& path )
{
    clean();
    QDir dir( path, "*." + PLUGIN_EXTENSION, QDir::Name | QDir::IgnoreCase, 
              QDir::Files | QDir::Readable | QDir::Executable );
    for ( uint i = 0; i < dir.count(); i++ ) {
        FermaNextPlugin* plugin = new FermaNextPlugin;
        try { 
            DynaLoader& loader = plugin->loader;
            loader.loadLibrary( path + "/" + dir[i] );
            PluginInfoCall pluginInfoCall = loader.getAddress<PluginInfoCall>( 
                                                           PLUGIN_INFO_CALL );
            PluginInfo& pluginInfo = pluginInfoCall();
            plugin->info = pluginInfo;
            plugins[i] = plugin;
        } catch ( ... ) {
            delete plugin;
        }
    }
}

void PluginLoader::unloadPlugins ()
{
    clean();
}

void PluginLoader::unloadPlugin ( const PluginHandle& h )
{
    if ( plugins.contains(h) ) {
        delete plugins[h];
        plugins.erase(h);
    }
}

FermaNextPluginInfo& PluginLoader::findPluginInfo ( const PluginHandle& h ) 
    throw (FindException)
{
    if ( ! plugins.contains(h) )
        throw FindException();
    FermaNextPlugin* plugin = plugins[h];
    return plugin->info;
}

PluginHandleList PluginLoader::loadedPlugins () const
{
    PluginHandleList handles;
    PluginsMapConstIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        handles.push_back( iter.key() );
    }    
    return handles;
}

QStringList PluginLoader::loadedPluginsNames () const
{
    QStringList names;
    PluginsMapConstIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        names.push_back( iter.data()->info.name );
    }        
    return names;
}

PluginHandleList PluginLoader::loadedPluginsOfType ( const PluginType& type ) const
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
