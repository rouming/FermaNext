
#include "PluginLoaderFrontEnd.h"
#include "NativePluginLoader.h"
#include "NativePlugin.h"
#include "PluginManager.h"
#include "DynaLoader.h"
#include "Plugin.h"

/*****************************************************************************
 * Plugin Loader hook
 *****************************************************************************/

/** 
 * Native loader must have special system priority 
 * to be often loaded before others loaders.
 */
FERMA_NEXT_PLUGIN_LOADER(NativePluginLoader, (PluginManager::LoadingPriority)0)

/*****************************************************************************
 * Native Plugin Loader
 *****************************************************************************/

NativePluginLoader::NativePluginLoader ( PluginManager& mng ) :
    PluginLoader(mng)        
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
    throw (PluginLoadException)
{
    DynaLoader* dynaLoader = new DynaLoader;
    NativePlugin* plugin = 0;
    try {
        dynaLoader->loadLibrary( pathToPlugin );
        PluginInstanceInitCall pluginInstanceInitCall = 
            (PluginInstanceInitCall)dynaLoader->getProcAddress( 
                                                   PLUGIN_INSTANCE_INIT_CALL );
        plugin = pluginInstanceInitCall( pluginManager(), pathToPlugin );
    }
    catch ( DynaLoader::LibraryLoadException& ) {
        delete dynaLoader;
        // TODO: log this exception
        throw PluginLoadException();
    }
    catch ( DynaLoader::AddressException& ) {
        delete dynaLoader;
        // TODO: log this exception
        throw PluginLoadException();
    }
    catch ( ... ) {
        // Unknown plugin exception
        // TODO: log this exception
        throw PluginLoadException();
    }

    if ( plugin == 0 ) {
        delete dynaLoader;
        throw PluginLoadException();
    }
    pluginMap[plugin] = dynaLoader;
    return *plugin;
}

void NativePluginLoader::specificUnloadPlugin ( Plugin& plugin )
{
    try { 
        NativePlugin& nativePlg = dynamic_cast<NativePlugin&>( plugin );
        if ( ! pluginMap.contains( &nativePlg ) )
            return;

        DynaLoader* dynaLoader = pluginMap[&nativePlg];
        pluginMap.remove( &nativePlg );
        try { 
            PluginInstanceFiniCall pluginInstanceFiniCall =
                (PluginInstanceFiniCall) dynaLoader->getProcAddress(
                                                   PLUGIN_INSTANCE_FINI_CALL );
            pluginInstanceFiniCall();
        }
        catch ( DynaLoader::AddressException& ) {
            // TODO: log this exception            
        }

        delete dynaLoader;
    }
    catch ( std::exception& ) {
        // cast exception
        //TODO: log this exception
    }
    catch ( ... ) {
        // Unknown plugin exception
        //TODO: log this exception
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
