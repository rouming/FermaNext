
#include "PluginLoaderFrontEnd.h"
#include "PluginManager.h"
#include "DynaLoader.h"
#include "Config.h"
#include "Log4CXX.h"

#include <algorithm>
#include <QDir>
#include <QRegExp>

/*****************************************************************************
 * Logger
 *****************************************************************************/

using log4cxx::LoggerPtr;
using log4cxx::Logger;
static LoggerPtr logger( Logger::getLogger("common.PluginManager") );

/*****************************************************************************
 * Plugin Manager
 *****************************************************************************/

namespace SortHelper {

// Plugin sorter by path
bool sortPluginsByPath ( Plugin* plg1, Plugin* plg2 )
{
    Q_ASSERT(plg1);
    Q_ASSERT(plg2);
    return plg1->pluginPath() < plg2->pluginPath();
}

// Plugin loader sorter by path
static bool sortPluginLoadersByPath ( PluginLoader* ldr1, PluginLoader* ldr2 )
{
    Q_ASSERT(ldr1);
    Q_ASSERT(ldr2);
    return ldr1->pluginLoaderPath() < ldr2->pluginLoaderPath();
}

} //namespace SortHelper

/*****************************************************************************/

const QString& PluginManager::systemPluginExtension ()
{
    static QString extension = "plg";
    return extension;
}

const QString& PluginManager::systemPluginLoaderExtension ()
{
    static QString extension = "ldr";
    return extension;
}

/*****************************************************************************/

PluginManager::PluginManager ( Config& config ) :
    cfg(config)
{}

PluginManager::~PluginManager ()
{
    unregisterPluginLoaders();
}

Config& PluginManager::config ()
{ return cfg; }

void PluginManager::preparePluginExecutionContext ( 
    const PluginExecutionTree& tree )
    /*throw (PluginExecutionContext::ContextIsEmptyException)*/
{ execContext = PluginExecutionContext( tree ); }

PluginExecutionContext& PluginManager::currentExecutionContext ()
{ return execContext; }

const PluginExecutionContext& PluginManager::currentExecutionContext () const
{ return execContext; }

void PluginManager::registerPluginLoaders ( const QString& path )
{
    LOG4CXX_DEBUG(logger, "registerPluginLoaders");

    QDir ldrDir( path, "*." + systemPluginLoaderExtension() + "." +
                       DynaLoader::libExtension(),
                 QDir::Name | QDir::IgnoreCase, 
                 QDir::Files | QDir::Readable );

    // Notify everyone about amount of loaders
    emit onPluginLoadersRegistration( ldrDir.count() );

    // Try to register loaders from path
    for ( uint i = 0; i < ldrDir.count(); ++i ) {
        QString loaderPath = path + QDir::separator() + ldrDir[i];
        try { registerPluginLoader( loaderPath ); }
        catch ( RegisterPluginLoaderException& ) {}
    }
}

void PluginManager::unregisterPluginLoaders ()
{
    LOG4CXX_DEBUG(logger, "unregisterPluginLoaders");

    // Firstly unload all plugins.
    unloadPlugins();

    // Unregister all plugins loaders regardless of their status.
    PluginLoaderList loaders = pluginLoaders( false );

    // Notify everyone about amount of loaders
    emit onPluginLoadersUnregistration( loaders.size() );

    PluginLoaderListConstIter it = loaders.begin();
    for ( ; it != loaders.end(); ++it ) {
        PluginLoader* loader = *it;
        unregisterPluginLoader( *loader );
    }
}

void PluginManager::registerPluginLoader ( const QString& pathToLoaderLib )
    /*throw (RegisterPluginLoaderException)*/
{
    LOG4CXX_DEBUG(logger, "registerPluginLoader: " +
                  pathToLoaderLib.toStdString() );

    DynaLoader* nativeLib = new DynaLoader;
    LoadingPriority priority;
    PluginLoader* loader = 0;
    try {
        emit onBeforePluginLoaderRegistration( pathToLoaderLib );
        nativeLib->loadLibrary( pathToLoaderLib );
        PluginLoaderInstanceInitCall loaderInstanceInitCall = 
            (PluginLoaderInstanceInitCall)nativeLib->getProcAddress( 
                                           PLUGIN_LOADER_INSTANCE_INIT_CALL );
        loader = loaderInstanceInitCall( *this, pathToLoaderLib, &priority );
    }
    catch ( DynaLoader::LibraryLoadException& e ) {
        LOG4CXX_WARN(logger, "loader can't be registered: " + 
                     pathToLoaderLib.toStdString() + ", can't load library: "+ 
                     e.message().toStdString() );

        delete nativeLib;
        throw RegisterPluginLoaderException();
    }
    catch ( DynaLoader::AddressException& e ) {
        LOG4CXX_WARN(logger, "loader can't be registered: " + 
                     pathToLoaderLib.toStdString() + ", can't find address: "+
                     e.message().toStdString() );

        delete nativeLib;
        throw RegisterPluginLoaderException();
    }
    catch ( ... ) {
        LOG4CXX_ERROR(logger, "loader can't be registered: " + 
                      pathToLoaderLib.toStdString() + ", unknown exception");
        throw RegisterPluginLoaderException();
    }

    if ( loader == 0 ) {
        LOG4CXX_WARN(logger, "loader can't be registered: " + 
                     pathToLoaderLib.toStdString() + ", loader pointer is 0");

        delete nativeLib;
        throw RegisterPluginLoaderException();
    }

    LOG4CXX_INFO(logger, "loader registered: " + 
                 pathToLoaderLib.toStdString());

    PluginLoaderList* loaders = 0;
    if ( ! loadersMap.contains( priority ) ) {
        loaders = new PluginLoaderList;
        loadersMap[priority] = loaders;
    } else {
        loaders = loadersMap[priority];
    }
    loaders->push_back( loader );
    loadersLibMap[loader] = nativeLib;
    emit onAfterPluginLoaderRegistration( *loader, priority );
}

void PluginManager::unregisterPluginLoader ( PluginLoader& loader )
{
    // For logging
    QString loaderPath = loader.pluginLoaderPath();
    LOG4CXX_DEBUG(logger, "unregisterPluginLoader: " +
                  loaderPath.toStdString());

    PluginLoadersMapIter loaderMapIt = loadersMap.begin();
    for ( ; loaderMapIt != loadersMap.end(); ++loaderMapIt ) {
        PluginLoaderList* loaders = loaderMapIt.value();
        PluginLoaderListIter it = std::find( loaders->begin(), loaders->end(),
                                             &loader );
        // Continue loop if nothing has been found
        if ( it == loaders->end() )
            continue;

        // Notify everyone
        emit onBeforePluginLoaderUnregistration( loader );

        // Unload all plugins of plugin loader
        PluginList plugins = loader.loadedPlugins();
        PluginListIter plgIt = plugins.begin();
        for ( ; plgIt != plugins.end(); ++plgIt ) {
            Plugin* plg = *plgIt;
            unloadPlugin( *plg );
        }
        // Remove loader
        loaders->erase(it);

        // Free loaders list if it is empty
        if ( loaders->size() == 0 ) {
            delete loaders;
            loadersMap.erase( loaderMapIt );
        }

        // Free native lib
        DynaLoader* nativeLib = loadersLibMap[&loader];
        loadersLibMap.remove( &loader );
        try { 
            PluginLoaderInstanceFiniCall pluginLoaderInstanceFiniCall =
                (PluginLoaderInstanceFiniCall) nativeLib->getProcAddress(
                                            PLUGIN_LOADER_INSTANCE_FINI_CALL );
            pluginLoaderInstanceFiniCall();
        }
        catch ( DynaLoader::AddressException& ) {
            LOG4CXX_WARN(logger, "can't find address for fini call of loader: "
                         + loaderPath.toStdString());
        }

        LOG4CXX_INFO(logger, "loader unloaded: "
                     + loaderPath.toStdString());

        delete nativeLib;

        emit onAfterPluginLoaderUnregistration( loaderPath );

        return;
    }    
}

RequiredPluginsMap PluginManager::getDependencies ( Plugin* plugin )
{
    Q_ASSERT(plugin);
    RequiredPluginsMap requiredPlugins;
    const QStringList& types = plugin->requiredPluginTypes();
    foreach ( QString requiredType, types ) {
        PluginList plugins = loadedPluginsOfType( requiredType );
        requiredPlugins[requiredType] = plugins;
    }
    return requiredPlugins;
}

void PluginManager::loadPlugins ( const QString& path )
{
    LOG4CXX_DEBUG(logger, "loadPlugins: " + path.toStdString());

    bool pluginsReload = false;
    QHash<QString, QString> uuidsCache;

    if ( pluginsPath == path && ! path.isEmpty() ) {
        // If want to reload, save UUIDs. We should restore them.
        pluginsReload = true;
        foreach ( Plugin* plugin, plugins.keys() )
            uuidsCache[ plugin->pluginPath() ] = plugin->getUUID();
    }

    // Firstly unload all plugins
    unloadPlugins();

    // Set new path
    pluginsPath = path;

    // Preload all dynamic system libs before plugin loading. 
    // This should be done because some plugins can require 
    // other system dynamic libs, which must be loaded first.
    //
    QDir preloadedLibsDir( path, "*." + DynaLoader::libExtension(),
                           QDir::Name | QDir::IgnoreCase, 
                           QDir::Files | QDir::Readable );

    for ( uint i = 0; i < preloadedLibsDir.count(); ++i ) {
        QString dir = preloadedLibsDir[i];
        LOG4CXX_INFO(logger, "system lib: " + dir.toStdString());

        // Preload only simple dynamic system libs. Ignore FN plugins.
        if ( dir.contains( QRegExp( ".*\\." +  systemPluginExtension() + 
                                    "\\.\\w+$" ) ) )
            continue;
        QString systemLibPath = path + QDir::separator() + dir;
        try {        
            LOG4CXX_INFO(logger, "preload system lib: " + 
                         systemLibPath.toStdString());
            DynaLoader* systemLib = new DynaLoader( systemLibPath );
            systemLibs.push_back( systemLib );
        } catch ( ... ) {
            LOG4CXX_WARN(logger, "can't preload system lib: " + 
                         systemLibPath.toStdString());
        }
    }

    typedef QList<QString> PluginPathList;
    typedef PluginPathList::ConstIterator PluginPathListConstIter;
    typedef QMap<QString, PluginLoader*> PluginPathMap;

    // Save all paths of plugins
    PluginPathList pluginPathList;
    PluginPathMap pluginPathMap;

    // Get all FermaNext plugins paths in priority order..
    // Loaders should be with Ok status
    PluginLoaderList loaders = pluginLoaders();
    PluginLoaderListConstIter loaderIt = loaders.begin();
    for ( ; loaderIt != loaders.end(); ++loaderIt ) {
        PluginLoader* loader = *loaderIt;
        const QString& pluginExt = loader->pluginExtension();
        
        QDir plgDir( path, "*." + systemPluginExtension() + "." + pluginExt,
                     QDir::Name | QDir::IgnoreCase, 
                     QDir::Files | QDir::Readable );
        for ( uint i = 0; i < plgDir.count(); ++i ) {
            QString pathToPlugin = path + QDir::separator() + plgDir[i];
            pluginPathList.push_back( pathToPlugin );
            pluginPathMap[ pathToPlugin ] = loader;
        }
    }

    // Emit signal about number of plugins which are going to be loaded.
    emit onBeforePluginsLoad( pluginPathList.size() );

    // Load all plugins by stored plugins paths in priority order..
    PluginPathListConstIter pathIt = pluginPathList.begin();
    for ( ; pathIt != pluginPathList.end(); ++pathIt ) {
        const QString& path = *pathIt;
        try {
            LOG4CXX_INFO(logger, "try to load plugin: " + path.toStdString());
            emit onBeforePluginLoad( path );
            PluginLoader* loader = pluginPathMap[ path ];
            Plugin& plg = loader->loadPlugin( path );
            // Set UUID if plugins are reloading
            if ( pluginsReload && uuidsCache.contains(path) )
                plg.setUUID( uuidsCache[path] );

            // Connect to deligate signals
            QObject::connect( &plg, 
                              SIGNAL(beforeExecution(Plugin&)), 
                              SIGNAL(onBeforePluginExecution(Plugin&)) );

            QObject::connect( &plg, 
                              SIGNAL(afterExecution(Plugin&, 
                                                    Plugin::ExecutionResult)),
                              SIGNAL(onAfterPluginExecution(Plugin&,
                                                    Plugin::ExecutionResult)));

            plugins[&plg] = loader;
            emit onAfterPluginLoad( plg );
            LOG4CXX_INFO(logger, "plugin loaded: " + path.toStdString());
        }
        catch ( PluginLoader::PluginIsAlreadyLoadedException& ) {
            LOG4CXX_WARN(logger, "plugin can't be loaded: " + 
                         path.toStdString() + ", plugin is already loaded");
        }
        catch ( PluginLoader::PluginLoadException& ) {
            LOG4CXX_WARN(logger, "plugin can't be loaded: " + 
                         path.toStdString());
        }
    }

    // Emit signal about number of plugins 
    // which are sucessfuly loaded.
    emit onAfterPluginsLoad( plugins.size() );

    //TODO: DO NOT FORGET TO RESOLVE DEPENDENCIES
}

void PluginManager::unloadPlugins ()
{
    LOG4CXX_DEBUG(logger, "unloadPlugins");

    // Unload all plugins regardless of their status
    PluginList loadedPlgs = loadedPlugins( false );
    int pluginsSize = loadedPlgs.size();
    int sysLibsSize = systemLibs.size();

    // Clear plugins path
    pluginsPath.clear();

    // Check if nothing to do
    if ( pluginsSize == 0 && sysLibsSize == 0 )
        return;

    emit onBeforePluginsUnload( pluginsSize );

    // Firstly unload all FN plugins
    PluginListIter it = loadedPlgs.begin();
    for ( ; it != loadedPlgs.end(); ++it ) {
        Plugin* plg = *it;
        unloadPlugin( *plg );
    }

    // Secondly unload system preloaded libs
    PreloadedSystemLibsIter libsIter = systemLibs.begin();
    for ( ; libsIter != systemLibs.end(); ++libsIter )
        delete *libsIter;
    systemLibs.clear();

    emit onAfterPluginsUnload( pluginsSize );
}

void PluginManager::reloadPlugins ()
{
    // Use copy
    QString path = pluginsPath;
    loadPlugins( path );
}

void PluginManager::unloadPlugin ( Plugin& plg )
{
    LOG4CXX_DEBUG(logger, "unloadPlugin: " + plg.pluginPath().toStdString());

    if ( ! plugins.contains( &plg ) )
        return;
    PluginLoader* loader = plugins[&plg];
    QString pluginPath = plg.pluginPath();
    emit onBeforePluginUnload( plg );
    loader->unloadPlugin( plg );
    plugins.remove( &plg );
    emit onAfterPluginUnload( pluginPath );

    //TODO: DO NOT FORGET DEPENDENCIES
}

Plugin* PluginManager::findPluginByUUID ( const QString& uuid, 
                                          bool onlyOk ) const
{
    PluginList allPlugins = loadedPlugins( onlyOk );
    foreach ( Plugin* plugin, allPlugins )
        if ( plugin->getUUID() == uuid )
            return plugin;
    return 0;
}

PluginList PluginManager::loadedPlugins ( bool onlyOk ) const
{
    PluginList pluginList;
    PluginsMapConstIter it = plugins.begin();    
    for ( ; it != plugins.end(); ++it ) {
        Plugin* plg = it.key();
        if ( !onlyOk || plg->pluginStatusCode() == Plugin::OkStatus )
            pluginList.push_back( plg );
    }
    qSort( pluginList.begin(), pluginList.end(), 
           SortHelper::sortPluginsByPath );
    return pluginList;
}

PluginList PluginManager::loadedPluginsOfType ( const QString& plgType,
                                                bool onlyOk ) const
{
    QString type = plgType;
    // Escape all dots
    type.replace( ".", "\\." );
    // Add dots to asterics
    type.replace( "*", ".*" );
    QRegExp regExp( "^" + type + "$", Qt::CaseInsensitive ); 

    PluginList pluginsOfType;
    PluginList plugins = loadedPlugins( onlyOk );
    PluginListConstIter it = plugins.begin();
    for ( ; it != plugins.end(); ++it ) {
        Plugin* plg = *it;
        const PluginInfo& plgInfo = plg->pluginInfo();
        if ( regExp.indexIn( plgInfo.type ) != -1 ) {
            pluginsOfType.push_back( plg );
        }
    }
    qSort( pluginsOfType.begin(), pluginsOfType.end(), 
           SortHelper::sortPluginsByPath );
    return pluginsOfType;
}

PluginLoaderList PluginManager::pluginLoaders ( bool onlyOk ) const
{
    PluginLoaderList loadersRes;
    PluginLoadersMapConstIter it = loadersMap.begin();
    for ( ; it != loadersMap.end(); ++it ) {
        PluginLoaderList* loaders = it.value();
        PluginLoaderListConstIter loadersIt = loaders->begin();
        for ( ; loadersIt != loaders->end(); ++loadersIt ) {
            PluginLoader* loader = *loadersIt;
            // Check if onlyOk is set to true, status should be Ok
            if ( !onlyOk || 
                 loader->pluginLoaderStatusCode() == PluginLoader::OkStatus )
                loadersRes.push_back(loader);
        }
    }
    qSort( loadersRes.begin(), loadersRes.end(), 
           SortHelper::sortPluginLoadersByPath );
    return loadersRes;
}

/*****************************************************************************/
