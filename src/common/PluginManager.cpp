
#include "PluginManager.h"
#include "PluginLoader.h"
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
    QDir dir( path, "*." + DynaLoader::libExtension(),
              QDir::Name | QDir::IgnoreCase, 
              QDir::Files | QDir::Readable );
    for ( uint i = 0; i < dir.count(); i++ ) {
        try {
            PluginLoader* loader = new PluginLoader( path + "/" + dir[i] );
            Plugin& pluginInstance = loader->pluginInstance();
            PluginHandle handle = pluginInstance.pluginChecksum();
            plugins[handle] = loader;
        } catch ( ... ) { }
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

Plugin& PluginManager::findPlugin ( const PluginHandle& h ) 
    throw (FindException)
{
    if ( ! plugins.contains(h) )
        throw FindException();
    PluginLoader* loader = plugins[h];
    return loader->pluginInstance();
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
        names.push_back( iter.data()->pluginInstance().pluginInfo().name );
    }        
    return names;
}

PluginHandleList PluginManager::loadedPluginsOfType ( 
                                                 const PluginType& type ) const
{
    PluginHandleList handles;
    PluginsMapConstIter iter = plugins.begin();
    for ( ; iter != plugins.end(); ++iter ) {
        if ( iter.data()->pluginInstance().pluginType() == type )
            handles.push_back( iter.key() );
    }    
    return handles;
}

/*****************************************************************************/
