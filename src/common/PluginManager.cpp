
#include "PluginManager.h"
#include "PluginLoader.h"
#include <qdir.h>
#include <qregexp.h>

/*****************************************************************************
 * Plugin Manager
 *****************************************************************************/

const QString PluginManager::PluginExtension = "plg";

PluginManager::~PluginManager ()
{
    clean();
}

void PluginManager::clean ()
{
    // Free all plugins first
    PluginsMapIter plgIter = plugins.begin();
    for ( ; plgIter != plugins.end(); ++plgIter )
        delete plgIter.data();
    plugins.clear();

    PreloadedDynaLibsIter libsIter = dynaLibs.begin();
    for ( ; libsIter != dynaLibs.end(); ++libsIter )
        delete *libsIter;
    dynaLibs.clear();
}

void PluginManager::loadPlugins ( const QString& path )
{
    // Clean first
    clean();

    uint i = 0;

    //
    // Preload all dynamic libs before plugin loading. 
    // This should be done because some plugins can require 
    // other dynamic libs, which must be loaded first.
    //
    QDir preloadedLibsDir( path, "*." + DynaLoader::LibExtension,
                           QDir::Name | QDir::IgnoreCase, 
                           QDir::Files | QDir::Readable );
    for ( i = 0; i < preloadedLibsDir.count(); ++i ) {
        QString dir = preloadedLibsDir[i];
        // Preload only simple dynamic libs. Ignore FermaNext plugins.
        if ( dir.contains( QRegExp( QString(".*\\.") + 
                                    PluginExtension + "\\." + 
                                    DynaLoader::LibExtension + "$" ) ) )
            continue;
        try {
            DynaLoader* dynaLib = 
                new DynaLoader( path + QDir::separator() + dir );
            dynaLibs.push_back( dynaLib );
        } catch ( ... ) {}
    }

    //
    // Load all FermaNext plugins from plugin dir.
    //
    QDir plgDir( path, "*." + PluginExtension + "." + DynaLoader::LibExtension,
                 QDir::Name | QDir::IgnoreCase, 
                 QDir::Files | QDir::Readable );
    for ( i = 0; i < plgDir.count(); i++ ) {
        QString plg = plgDir[i];
        PluginLoader* loader = 0;
        try {
            loader = new PluginLoader( path + QDir::separator() + plg );
            Plugin& pluginInstance = loader->pluginInstance();
            PluginHandle handle = pluginInstance.pluginChecksum();
            if ( plugins.contains(handle) )
                delete plugins[handle];
            plugins[handle] = loader;
        } catch ( ... ) { 
            delete loader;
        }
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
