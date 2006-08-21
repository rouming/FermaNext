
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <vector>
#include <QMap>
#include <QObject>

#include "PluginLoader.h"
#include "PluginExecutionTree.h"

class DynaLoader;
class Config;

/** 
 * Class for managing all plugins of FN system. All specific 
 * plugins (not #NativePlugin) should have their loaders and be registered 
 * by #registerPluginLoader.
 */
class PluginManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Extension which distinguishes FN system plugin from other libs.
     * @return plugin extension
     */
    static const QString& systemPluginExtension ();

    /**
     * Extension which distinguishes FN system plugin loader from other libs.
     * @return plugin extension
     */
    static const QString& systemPluginLoaderExtension ();

public:
    /** 
     * #PluginManager loads plugins in some order. 
     * #NativePluginLoader should be the first, so it has max system priority.
     * If loaders have the same priority, #PluginManager will randomly order 
     * them.
     */
    enum LoadingPriority {
        HighPriority    = 1,
        NormalPriority  = 2,
        LowPriority     = 3
    };

    /**
     * Plugin manager contructor.
     * @param config, manager will share it 
     * with plugins and loaders.
     */
    PluginManager ( Config& config );

    /**
     * Correctly unload all plugins.
     */
    ~PluginManager ();

    /** Returns config */
    Config& config ();

    /** 
     * Builds execution tree for specified plugin.  
     * @see PluginExecutionTree
     */
    const PluginExecutionTree& buildExecutionTree ( Plugin& plugin );

    /**
     * Resolves dependencies.
     * @param plugin which dependencies should be resolved.
     * @param types of required plugins.
     * @return map of required plugins.
     * @throw RequiredPluginIsNotResolvedException occurs when plugin 
     * dependencies can't be resolved.
     */
    RequiredPluginsMap resolveDependence ( Plugin& plugin, 
                                           const QStringList& types )
        /*throw (RequiredPluginIsNotResolvedException)*/;

    /**
     * Register all plugin loaders from specified path.
     * Reload them if loaders have been already registered.
     * @param path to directory with plugin loaders
     * @see unregisterPluginLoaders
     */
    void registerPluginLoaders ( const QString& path );

    /**
     * Unregister all plugin loaders. It is evident, that all plugins will be 
     * unloaded.
     * @see registerPluginLoaders
     * @see unloadPlugins
     */
    void unregisterPluginLoaders ();

    /**
     * Load all plugins from specified path by registered plugin loaders.
     * Plugins will be reloaded if they have been already loaded.
     * NOTE: if plugins have been already loaded and now we call
     *       #loadPlugins with the same dir, plugins UUIDs will be kept.
     *       This is used for correct getting plugin by UUID.
     * @see getPluginByUUID
     */
    void loadPlugins ( const QString& path );

    /**
     * Unload all plugins.
     */
    void unloadPlugins ();

    /**
     * Reload all plugins. 
     * Just a wrapper on #loadPlugins call with the same directory.
     * @see loadPlugins
     */
    void reloadPlugins ();

    /**
     * Unload plugin.
     * @param plugin to be unloaded
     */
    void unloadPlugin ( Plugin& plugin );

    /**
     * Finds plugin by UUID.
     * @param uuid of the plugin.
     * @param onlyOk, if true returns plugin only with #Plugin::OkStatus.
     */
    Plugin* findPluginByUUID ( const QString& uuid, bool onlyOk = true ) const;

    /**
     * Returns loaded plugins.
     * @param onlyOk, if true returns plugins with 
     * #Plugin::OkStatus, or returns all otherwise.
     * @return loaded plugins
     */
    PluginList loadedPlugins ( bool onlyOk = true ) const;

    /**
     * Returns loaded plugins by specified plugin type.
     * @param type of plugins to be returned
     * @param onlyOk, if true returns plugins with 
     * #Plugin::OkStatus, or returns all otherwise.
     * @return loaded plugins of specified type.
     */
    PluginList loadedPluginsOfType ( const QString& type,
                                     bool onlyOk = true ) const;

protected:
    /**
     * Returns plugin loaders in priority order.
     * @param onlyOk, if true returns loaders with 
     * #PluginLoader::OkStatus, or returns all otherwise.
     * @return plugin loaders
     * @see PluginLoader::pluginLoaderStatusCode
     */
    PluginLoaderList pluginLoaders ( bool onlyOk = true ) const;

signals:    
    ///////////////////////////////////////////////////////////////////////
    // Plugin Signals
    ///////////////////////////////////////////////////////////////////////

    /** 
     * Is emitted, when #loadPlugins has been called.
     * @param plgNum describes how many plugins are going to be loaded.
     */
    void onBeforePluginsLoad ( uint plgNum );

    /** 
     * Is emitted, when #loadPlugins has been called.
     * @param plgNum describes how many plugins are already loaded.
     */
    void onAfterPluginsLoad ( uint plgNum );

    /** 
     * Is emitted before plugin load. If loading fails, #onAfterPluginLoad
     * will not be emitted.
     * @param path to plugin, which is going to be loaded.
     */
    void onBeforePluginLoad ( const QString& path );

    /** 
     * Is emitted after successful plugin load.
     * @param plg plugin.
     */
    void onAfterPluginLoad ( Plugin& plg );

    /** 
     * Is emitted, when #unloadPlugins has been called.
     * @param plgNum describes how many plugins are going to be unloaded.
     */
    void onBeforePluginsUnload ( uint plgNum );

    /** 
     * Is emitted, when #unloadPlugins has been called.
     * @param plgNum describes how many plugins are already unloaded.
     */
    void onAfterPluginsUnload ( uint plgNum );

    /** 
     * Is emitted before plugin unload.
     * @param plg plugin.
     */
    void onBeforePluginUnload ( Plugin& plg );

    /** 
     * Is emitted after plugin unload.
     * @param path to unloaded plugin.
     */
    void onAfterPluginUnload ( const QString& plgPath );

    /** 
     * Is emitted before some plugin execution.
     */
    void onBeforePluginExecution ( Plugin& );

    /** 
     * Is emitted after some plugin execution.
     */
    void onAfterPluginExecution ( Plugin&, Plugin::ExecutionResult );


    ///////////////////////////////////////////////////////////////////////
    // Plugin Loader Signals
    ///////////////////////////////////////////////////////////////////////

    /** 
     * Is emitted, when #loadPlugins has been called.
     * @param num describes how many plugin loaders are going to be registered.
     */
    void onPluginLoadersRegistration ( uint num );

    /** 
     * Is emitted before loader registration. If registration fails, 
     * #onAfterPluginLoaderRegistration will not be emitted.
     * @param path to loader, which is going to be registered.
     */
    void onBeforePluginLoaderRegistration ( const QString& path );

    /** 
     * Is emitted after successful plugin loader registration.
     * @param plgLoader plugin loader.
     * @param priority of loading.
     */
    void onAfterPluginLoaderRegistration ( 
                                     PluginLoader& plgLoader,
                                     PluginManager::LoadingPriority priority );

    /** 
     * Is emitted, when #unloadPlugins or #unregisterPluginLoaders has been 
     * called.
     * @param num describes how many plugin loaders are going to be 
     * unregistered.
     */
    void onPluginLoadersUnregistration ( uint num );

    /** 
     * Is emitted before plugin loader unregistration.
     * @param plgLoader plugin loader..
     */
    void onBeforePluginLoaderUnregistration ( PluginLoader& plgLoader );

    /** 
     * Is emitted after plugin loader unregistration.
     * @param plgLoaderPath plugin loader path.
     */
    void onAfterPluginLoaderUnregistration ( const QString& plgLoaderPath );

private slots:
    /** This slot emits #onBeforePluginExecution signal */
    void beforePluginExecution ( Plugin& );

    /** This slot emits #onAfterPluginExecution signal */
    void afterPluginExecution ( Plugin&, Plugin::ExecutionResult );

private:
    // Exceptions
    class PluginListIsEmptyException {};
    class RegisterPluginLoaderException {};

    /**
     * Registers new plugin loader.
     * @param pathToLoaderLib path to native plugin loader lib
     * @throw RegisterPluginLoaderException occurs when plugin loader 
     * has not been registered
     */
    void registerPluginLoader ( const QString& pathToLoaderLib )
        /*throw (RegisterPluginLoaderException)*/;

    /**
     * Simply unregisters plugin loader.
     * @param loader to be unregistered.
     * @see registerPluginLoader
     */
    void unregisterPluginLoader ( PluginLoader& loader );

    /**
     * Delegates the choice of required plugin to the user (e.g. creates 
     * GUI dialog window with special combobox).
     * @param plugin for which user should choose required plugin.
     * @param type of required plugin.
     * @param plugins list from which user should decide what required plugin 
     * is better.
     */
    Plugin& chooseRequiredPlugin ( Plugin& plugin, 
                                   const QString& type,
                                   const PluginList& plugins )
        /*throw (PluginListIsEmptyException)*/;

    /** Hidden copy constructor. */
    PluginManager ( const PluginManager& );
    /** Hidden copy operator. */
    PluginManager& operator= ( const PluginManager& );

private:
    typedef QMap<LoadingPriority, PluginLoaderList*> PluginLoadersMap;
    typedef PluginLoadersMap::Iterator PluginLoadersMapIter;
    typedef PluginLoadersMap::ConstIterator PluginLoadersMapConstIter;

    typedef QMap<PluginLoader*, DynaLoader*> PluginLoadersLibMap;
    typedef PluginLoadersLibMap::Iterator PluginLoadersLibMapIter;
    typedef PluginLoadersLibMap::ConstIterator PluginLoadersLibMapConstIter;

    typedef QMap<Plugin*, PluginLoader*> PluginsMap;
    typedef PluginsMap::Iterator PluginsMapIter;
    typedef PluginsMap::ConstIterator PluginsMapConstIter;

    typedef std::vector<DynaLoader*> PreloadedSystemLibs;
    typedef PreloadedSystemLibs::iterator PreloadedSystemLibsIter;

    PreloadedSystemLibs systemLibs;    /**< preloaded system libs */
    PluginLoadersMap loadersMap;       /**< loaders in loading order */
    PluginLoadersLibMap loadersLibMap; /**< loaders and their libs */
    PluginsMap plugins;                /**< plugins and their loaders */
    QString pluginsPath;               /**< plugins path */
    Config& cfg;                       /**< global app config */
    PluginExecutionTree execTree;
};

#endif //PLUGINMANAGER_H