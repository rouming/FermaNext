
#include "PluginLoaderFrontEnd.h"
#include "NativePluginLoader.h"
#include "NativePlugin.h"
#include "PluginManager.h"
#include "DynaLoader.h"
#include "Plugin.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Plugin Loader hook
 *****************************************************************************/

/** 
 * Native loader must have special system priority 
 * to be often loaded before others loaders.
 */
FERMA_NEXT_PLUGIN_LOADER(NativePluginLoader, (PluginManager::LoadingPriority)0)

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("loader.NativePluginLoader") );

/*****************************************************************************
 * Native Plugin Loader
 *****************************************************************************/

NativePluginLoader::NativePluginLoader ( PluginManager& mng, 
                                         const QString& path ) :
    PluginLoader(mng, path)
{}

NativePluginLoader::~NativePluginLoader ()
{
    // Calls unload method of base class
    unloadPlugins();
}

const QString& NativePluginLoader::pluginExtension () const
{
    // Returns OS dependent dyna extension.
    return DynaLoader::libExtension();
}

NativePluginLoader::Status NativePluginLoader::pluginLoaderStatusCode () const
{ return OkStatus; }

QString NativePluginLoader::pluginLoaderStatusMsg () const
{ return QString(); }

Plugin& NativePluginLoader::specificLoadPlugin ( const QString& pathToPlugin )
    /*throw (PluginLoadException)*/
{
    LOG4CXX_DEBUG(logger, "specificLoadPlugin(QString): " + 
                  pathToPlugin.toStdString() );

    DynaLoader* dynaLoader = new DynaLoader;
    NativePlugin* plugin = 0;
    try {
        dynaLoader->loadLibrary( pathToPlugin );
        PluginInstanceInitCall pluginInstanceInitCall = 
            (PluginInstanceInitCall)dynaLoader->getProcAddress( 
                                                   PLUGIN_INSTANCE_INIT_CALL );
        plugin = pluginInstanceInitCall( pluginManager(), pathToPlugin );
    }
    catch ( DynaLoader::LibraryLoadException& e ) {
        LOG4CXX_WARN(logger, "plugins can't be loaded: " + 
                     pathToPlugin.toStdString() + ", can't load library: "+ 
                     e.message().toStdString() );
        delete dynaLoader;
        throw PluginLoadException();
    }
    catch ( DynaLoader::AddressException& e ) {
        LOG4CXX_WARN(logger, "plugin can't be loaded: " + 
                     pathToPlugin.toStdString() + ", can't find address: "+
                     e.message().toStdString() );
        delete dynaLoader;
        throw PluginLoadException();
    }
    catch ( ... ) {
        LOG4CXX_ERROR(logger, "plugin can't be loaded: " + 
                      pathToPlugin.toStdString() + ", unknown exception");
        delete dynaLoader;
        throw PluginLoadException();
    }

    if ( plugin == 0 ) {
        LOG4CXX_WARN(logger, "plugins can't be loaded: " + 
                     pathToPlugin.toStdString() + ", loader pointer is 0");

        delete dynaLoader;
        throw PluginLoadException();
    }

    LOG4CXX_INFO(logger, "plugin loaded: " +  pathToPlugin.toStdString());

    pluginMap[plugin] = dynaLoader;
    return *plugin;
}

void NativePluginLoader::specificUnloadPlugin ( Plugin& plugin )
{
    QString pluginPath = plugin.pluginPath();
    LOG4CXX_DEBUG(logger, "specificUnloadPlugin: " +
                  pluginPath.toStdString());

    try { 
        NativePlugin& nativePlg = dynamic_cast<NativePlugin&>( plugin );
        if ( ! pluginMap.contains( &nativePlg ) ) {
            LOG4CXX_WARN(logger, "plugin doesn't exist: " +
                          pluginPath.toStdString());            
            return;
        }

        DynaLoader* dynaLoader = pluginMap[&nativePlg];
        pluginMap.remove( &nativePlg );
        try { 
            PluginInstanceFiniCall pluginInstanceFiniCall =
                (PluginInstanceFiniCall) dynaLoader->getProcAddress(
                                                   PLUGIN_INSTANCE_FINI_CALL );
            pluginInstanceFiniCall();
        }
        catch ( DynaLoader::AddressException& e ) {
            LOG4CXX_WARN(logger, "plugin can't be correctly unloaded: " + 
                         pluginPath.toStdString() + ", can't find address: "+
                         e.message().toStdString() );
        }       

        delete dynaLoader;

        LOG4CXX_INFO(logger, "plugin unloaded: " +  pluginPath.toStdString());
    }
    catch ( std::exception& ) {
        // Cast exception
        LOG4CXX_WARN(logger, "plugin can't be correctly unloaded: " + 
                     pluginPath.toStdString() + 
                     ", is not a child of base plugin class");
    }
    catch ( ... ) {
        LOG4CXX_ERROR(logger, "plugin can't be loaded: " + 
                      pluginPath.toStdString() + ", unknown exception");
    }
}

void NativePluginLoader::specificUnloadPlugins ()
{
    NativePluginMapIter it = pluginMap.begin();
    for ( ; it != pluginMap.end(); ++it )
        specificUnloadPlugin( *it.key() );
    pluginMap.clear();
}

/*****************************************************************************/
