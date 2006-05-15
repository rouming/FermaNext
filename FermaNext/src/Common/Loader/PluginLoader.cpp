
#include "PluginLoader.h"
#include "Plugin.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("common.PluginManager") );

/*****************************************************************************
 * Plugin Loader
 *****************************************************************************/

PluginLoader::PluginLoader ( PluginManager& plgMng, const QString& path_ ) :
    pluginMng(plgMng),
    path(path_),
    status(OkStatus)
{}

PluginLoader::~PluginLoader ()
{}

const QString& PluginLoader::pluginLoaderPath () const
{ return path; }

Plugin& PluginLoader::loadPlugin ( const QString& pathToPlugin )
    /*throw (PluginLoadException, PluginIsAlreadyLoadedException)*/
{
    LOG4CXX_DEBUG(logger, "loadPlugin: " + pathToPlugin.toStdString() );

    if ( pluginMap.contains( pathToPlugin ) ) {
        LOG4CXX_WARN(logger, "plugin can't be loaded: " + 
                     pathToPlugin.toStdString() + ", plugin is already loaded" );
        throw PluginIsAlreadyLoadedException();
    }
    try { 
        Plugin& plugin = specificLoadPlugin( pathToPlugin );
        pluginMap[pathToPlugin] = &plugin;
        return plugin;
    } catch ( PluginLoader::PluginLoadException& ) {
        LOG4CXX_WARN(logger, "plugin can't be loaded: " + 
                     pathToPlugin.toStdString() + ", specific crash" );
        throw;
    }
}

void PluginLoader::unloadPlugin ( const QString& pathToPlugin )
{
    LOG4CXX_DEBUG(logger, "unloadPlugin(QString): " + pathToPlugin.toStdString() );

    if ( ! pluginMap.contains( pathToPlugin ) )
        return;
    specificUnloadPlugin( *pluginMap[pathToPlugin] );
    pluginMap.remove( pathToPlugin );
    LOG4CXX_INFO(logger, "plugin unloaded: " + pathToPlugin.toStdString() );
}

void PluginLoader::unloadPlugin ( Plugin& plugin )
{
    QString pathToPlugin = plugin.pluginPath();
    LOG4CXX_DEBUG(logger, "unloadPlugin(Plugin): " + pathToPlugin.toStdString() );

    PluginMapIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it ) {
        if ( it.value() == &plugin ) {                
            specificUnloadPlugin( plugin );
            pluginMap.remove( it.key() );
            LOG4CXX_INFO(logger, "plugin unloaded: " + pathToPlugin.toStdString() );
            return;
        }
    }
}

void PluginLoader::unloadPlugins ()
{
    LOG4CXX_DEBUG(logger, "unloadPlugins");
    PluginMapIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it ) {
        Plugin& plugin = *it.value();
        QString pathToPlugin = plugin.pluginPath();
        specificUnloadPlugin( plugin );
        LOG4CXX_INFO(logger, "plugin unloaded: " + pathToPlugin.toStdString() );
    }
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
