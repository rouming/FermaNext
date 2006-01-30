
#include "PluginLoaderFrontEnd.h"
#include "PluginManager.h"
#include "DynaLoader.h"
#include <qmutex.h>
#include <algorithm>
#include <qdir.h>
#include <qregexp.h>

/*****************************************************************************
 * Plugin Manager
 *****************************************************************************/

static QMutex mutex;
PluginManager* PluginManager::plgMngInstance = 0;

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

PluginManager& PluginManager::instance ()
{
    if ( plgMngInstance == 0 ) {
        mutex.lock();
        if ( plgMngInstance == 0 ) 
            plgMngInstance = new PluginManager;
        mutex.unlock();
    }
    return *plgMngInstance;
}

/*****************************************************************************/

PluginManager::PluginManager ()
{}

PluginManager::~PluginManager ()
{
    unregisterPluginLoaders();
}

void PluginManager::registerPluginLoaders ( const QString& path )
{
    QDir ldrDir( path, "*." + systemPluginLoaderExtension() + "." +
                       DynaLoader::libExtension(),
                 QDir::Name | QDir::IgnoreCase, 
                 QDir::Files | QDir::Readable );

    // Notify everyone about amount of loaders
    emit onPluginLoadersRegistration( ldrDir.count() );

    // Try to register loaders from path
    for ( uint i = 0; i < ldrDir.count(); ++i ) {
        try {  registerPluginLoader( path + QDir::separator() + ldrDir[i] ); }
        catch ( RegisterPluginLoaderException& ) {
            //TODO: log this exception
        }
    }
}

void PluginManager::unregisterPluginLoaders ()
{
    // Firstly unload all plugins.
    unloadPlugins();

    PluginLoaderList loaders = pluginLoaders();

    // Notify everyone about amount of loaders
    emit onPluginLoadersUnregistration( loaders.size() );

    PluginLoaderListConstIter it = loaders.begin();
    for ( ; it != loaders.end(); ++it ) {
        PluginLoader* loader = *it;
        unregisterPluginLoader( *loader );
    }
}

void PluginManager::registerPluginLoader ( const QString& pathToLoaderLib )
    throw (RegisterPluginLoaderException)
{
    DynaLoader* nativeLib = new DynaLoader;
    PluginLoaderInstanceCall loaderInstanceCall = 0;
    try {
        emit onBeforePluginLoaderRegistration( pathToLoaderLib );
        nativeLib->loadLibrary( pathToLoaderLib );
        loaderInstanceCall = (PluginLoaderInstanceCall)
                      nativeLib->getProcAddress( PLUGIN_LOADER_INSTANCE_CALL );
    }
    catch ( DynaLoader::LibraryLoadException& ) {
        delete nativeLib;
        // TODO: log this exception
        throw RegisterPluginLoaderException();
    }
    catch ( DynaLoader::AddressException& ) {
        delete nativeLib;
        // TODO: log this exception
        throw RegisterPluginLoaderException();
    }

    LoadingPriority priority;
    PluginLoader* loader = loaderInstanceCall( *this, &priority );
    if ( loader == 0 ) {
        delete nativeLib;
        throw RegisterPluginLoaderException();
    }
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
    PluginLoadersMapIter loaderMapIt = loadersMap.begin();
    for ( ; loaderMapIt != loadersMap.end(); ++loaderMapIt ) {
        PluginLoaderList* loaders = loaderMapIt.data();
        PluginLoaderListIter it = std::find( loaders->begin(), loaders->end(),
                                             &loader );
        // Continue loop if nothing has benn found
        if ( it == loaders->end() )
            continue;

        // Notify everyone
        emit onPluginLoaderUnregistration( loader );

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
            loadersMap.remove( loaderMapIt );
        }

        // Free native lib
        DynaLoader* nativeLib = loadersLibMap[&loader];
        loadersLibMap.remove( &loader );
        delete nativeLib;

        return;
    }    
}

RequiredPluginsMap PluginManager::resolveDependence ( Plugin& plugin, 
                                                      const QStringList& types )
    throw (RequiredPluginIsNotResolvedException)
{
    QStringList unresolvedTypes;
    RequiredPluginsMap requiredPlugins;
    QStringList::ConstIterator it = types.begin();
    for ( ; it != types.end(); ++it ) {
        QString requiredType = *it;
        PluginList plugins = loadedPluginsOfType( requiredType );
        if ( plugins.size() == 0 )
            unresolvedTypes.push_back( requiredType );
        else if ( plugins.size() == 1 )
            requiredPlugins[requiredType].push_back( plugins[0] );
        else {
            try {
                Plugin& plg = 
                    chooseRequiredPlugin( plugin, requiredType, plugins );
                requiredPlugins[requiredType].push_back( &plg );
            } catch ( PluginListIsEmptyException& ) {}
        }
    }
    if ( unresolvedTypes.size() > 0 ) {
        RequiredPluginIsNotResolvedException excp;
        excp.unresolvedTypes = unresolvedTypes;
        throw excp;
    }
    
    return requiredPlugins;
}

Plugin& PluginManager::chooseRequiredPlugin ( Plugin& /*plugin*/, 
                                              const QString& /*type*/,
                                              const PluginList& plugins ) 
    throw (PluginListIsEmptyException)
{
    if ( plugins.size() == 0 )
        throw PluginListIsEmptyException();    

    // TODO: create GUI dialog for extended shoice.
    return *plugins[0];
}

void PluginManager::loadPlugins ( const QString& path )
{
    // Firstly unload all plugins
    unloadPlugins();

    // Preload all dynamic system libs before plugin loading. 
    // This should be done because some plugins can require 
    // other system dynamic libs, which must be loaded first.
    //
    QDir preloadedLibsDir( path, "*." + DynaLoader::libExtension(),
                           QDir::Name | QDir::IgnoreCase, 
                           QDir::Files | QDir::Readable );

    for ( uint i = 0; i < preloadedLibsDir.count(); ++i ) {
        QString dir = preloadedLibsDir[i];
        // Preload only simple dynamic system libs. Ignore FN plugins.
        if ( dir.contains( QRegExp( ".*\\." +  systemPluginExtension() + 
                                    "\\.\\w+$" ) ) )
            continue;
        try {
            DynaLoader* systemLib = 
                new DynaLoader( path + QDir::separator() + dir );
            systemLibs.push_back( systemLib );
        } catch ( ... ) {}
    }

    typedef std::vector<QString> PluginPathList;
    typedef PluginPathList::const_iterator PluginPathListConstIter;
    typedef QMap<QString, PluginLoader*> PluginPathMap;

    // Save all paths of plugins
    PluginPathList pluginPathList;
    PluginPathMap pluginPathMap;

    // Get all FermaNext plugins paths in priority order..
    //
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
    emit onPluginsLoad( pluginPathList.size() );

    // Load all plugins by sored plugins paths in priority order..
    PluginPathListConstIter pathIt = pluginPathList.begin();
    for ( ; pathIt != pluginPathList.end(); ++pathIt ) {
        const QString& path = *pathIt;
        try {
            emit onBeforePluginLoad( path );
            PluginLoader* loader = pluginPathMap[ path ];
            Plugin& plg = loader->loadPlugin( path );
            plugins[&plg] = loader;
            emit onAfterPluginLoad( plg );
        }
        catch ( PluginLoader::PluginIsAlreadyLoadedException& ) {
            //TODO: this exception should be written to log
        }
        catch ( PluginLoader::PluginLoadException& ) {
            //TODO: this exception should be written to log
        }
    }

    //TODO: DO NOT FORGET TO RESOLVE DEPENDENCIES
}

void PluginManager::unloadPlugins ()
{
    PluginList loadedPlgs = loadedPlugins();
    emit onPluginsUnload( loadedPlgs.size() );

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
}

void PluginManager::unloadPlugin ( Plugin& plg )
{
    if ( ! plugins.contains( &plg ) )
        return;
    PluginLoader* loader = plugins[&plg];
    emit onPluginUnload( plg );
    loader->unloadPlugin( plg );
    plugins.remove( &plg );

    //TODO: DO NOT FORGET DEPENDENCIES
}

PluginList PluginManager::loadedPlugins () const
{
    PluginList pluginList;
    PluginsMapConstIter it = plugins.begin();    
    for ( ; it != plugins.end(); ++it )
        pluginList.push_back( it.key() );
    return pluginList;
}

PluginList PluginManager::loadedPluginsOfType ( const QString& loadType ) const
{
    QString type = loadType;
    // Escape all dots
    type.replace( ".", "\\." );
    // Add dots to asterics
    type.replace( "*", ".*" );
    QRegExp regExp( "^" + type + "$" ); 
    regExp.setCaseSensitive( false );

    PluginList pluginsOfType;
    PluginList plugins = loadedPlugins();
    PluginListConstIter it = plugins.begin();
    for ( ; it != plugins.end(); ++it ) {
        Plugin* plg = *it;
        const PluginInfo& plgInfo = plg->pluginInfo();
        if ( regExp.search( plgInfo.type ) != -1 )
            pluginsOfType.push_back( plg );
    }
    return pluginsOfType;
}

PluginLoaderList PluginManager::pluginLoaders () const
{
    PluginLoaderList loadersRes;
    PluginLoadersMapConstIter it = loadersMap.begin();
    for ( ; it != loadersMap.end(); ++it ) {
        PluginLoaderList* loaders = it.data();
        size_t size = loadersRes.size();
        loadersRes.resize( size + loaders->size() );
        std::copy( loaders->begin(), loaders->end(), loadersRes.begin() +size );
    }    
    return loadersRes;
}

size_t PluginManager::countPluginLoaders () const
{
    return pluginLoaders().size();
}

/*****************************************************************************/
