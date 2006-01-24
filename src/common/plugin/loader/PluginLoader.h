
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "Plugin.h"
#include "Global.h"

#include <qstring.h>
#include <qmap.h>

class PluginLoader;
class PluginManager;

/** Plugin loaders list */
typedef std::vector<PluginLoader*> PluginLoaderList;
typedef PluginLoaderList::iterator PluginLoaderListIter;
typedef PluginLoaderList::const_iterator PluginLoaderListConstIter;

/**
 * Pure virtual base class for plugin loading. 
 * If your own plugin can't be loaded by native way 
 * (#NativePluginLoader) new plugin loader should be coded.
 * Specific plugin loader must be a native library and have special 
 * hook (#PluginLoaderFrontEnd.h) which returns an instance of this 
 * loader and loading priority (#PluginManager::LoadingPriority), 
 * which determines the order of plugin loading. Loading priority should 
 * be choosed according to your needs.
 */
class PluginLoader
{
public:
    // Exceptions
    /** Occurs when plugin can't be loaded. */
    class PluginLoadException {};
    /** Occurs when plugin is already loaded. */
    class PluginIsAlreadyLoadedException {};

    /** Status codes */
    enum Status {
        OkStatus = 0,          /**< Evrth is ok. Successfully loaded. */
        InternalErrStatus,     /**< Internal error. */
    };

    PluginLoader ( PluginManager& );

    /**
     * Override in derived classes and often call #unloadPlugins.
     * BEWARE of calling virtual members from overriden destructors 
     * except #unloadPlugins.
     */
    virtual ~PluginLoader ();

    /**
     * Returns plugin extension in following string format: \\w\\w\\w
     * i.e. use three alphanumeric characters (an alphabetic character, 
     * or a decimal digit) to represent plugin format extension.
     */
    virtual const QString& pluginExtension () const = 0;

    /** 
     * Describes state by status code.
     * @return plugin loader status code
     * @see pluginLoaderStatusMsg for detailed information about status.
     */
    virtual Status pluginLoaderStatusCode () const = 0;

    /** 
     * Describes state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return status string msg. Returns empty string if status is Ok.
     * @see pluginLoaderStatusCode
     */
    virtual QString pluginLoaderStatusMsg () const = 0;

    /**
     * Just returns plugin manager from which this loader was instantiated.
     * @return plugin manager
     */
    PluginManager& pluginManager ();

    /**
     * Front-end for plugin loading, which is called by #PluginManager.
     * It stores every plugin successfully loaded by specific 
     * variant #specificLoadPlugin, which must be overriden in derived classes.
     * @see unloadPlugin
     * @param pathToPlugin path to plugin which should be loaded
     * @throw PluginLoadException plugin cannot be loaded
     * @throw PluginIsAlreadyLoadedException plugin is already loaded
     * @return plugin which is successfully loaded
     */
    Plugin& loadPlugin ( const QString& pathToPlugin )
        throw (PluginLoadException, PluginIsAlreadyLoadedException);

    /**
     * Front-end for plugin unloading, called by #PluginManager.
     * This method calls specific variant #specificUnloadPlugin,
     * which must be overriden in derived classes.
     * @see loadPlugin
     * @param pathToPlugin path to plugin which should be unloaded
     */
    void unloadPlugin ( const QString& pathToPlugin );

    /**
     * Front-end for plugin unloading, called by #PluginManager.
     * This method calls specific variant #specificUnloadPlugin,
     * which must be overriden in derived classes.
     * @see loadPlugin
     * @param plugin which should be unloaded
     */
    void unloadPlugin ( Plugin& plugin );

    /**
     * Front-end for plugins unloading. Unload all loaded plugins.
     * This method calls specific variant #specificUnloadPlugins,
     * which must be overriden in derived classes.
     */
    void unloadPlugins ();

    /**
     * Returns loaded plugins.
     * @return loaded plugins
     */
    PluginList loadedPlugins () const;

    /**
     * Checks if plugin is loaded.
     * @param plugin to check
     * @return true if it is loaded, false otherwise.
     */
    bool pluginIsLoaded ( const Plugin& plugin ) const;

    /**
     * Counts loaded plugins.
     * @return number of loaded plugins.
     */
    size_t countLoadedPlugins () const;

    /**
     * Returns path of loaded plugin.
     * @param plugin for which path should be returned.
     * @return valid string if plugin was loaded, empty string otherwise.
     */
    QString pathToPlugin ( const Plugin& plugin ) const;

protected:
    /**
     * Specific plugin loading. Override this method in derived class.
     * @see loadPlugin
     * @param pathToPlugin path to plugin which should be loaded
     * @throw PluginLoadException plugin cannot be loaded
     * @throw PluginIsAlreadyLoadedException plugin is already loaded
     * @return plugin which is successfully loaded
     */
    virtual Plugin& specificLoadPlugin ( const QString& pathToPlugin )
        throw (PluginLoadException) = 0;

    /**
     * Specific plugin unloading. Override this method in derived class.
     * @see unloadPlugin
     * @param plugin which should be unloaded
     */
    virtual void specificUnloadPlugin ( Plugin& plugin ) = 0;

    /**
     * Specific plugins unloading. Override this method in derived class.
     * @see unloadPlugins
     */
    virtual void specificUnloadPlugins () = 0;

    /** 
     * Sets plugin loader status.
     * @param s plugin loader status
     */
    void setStatus ( Status s );

private:
    /** Hidden copy constructor. */
    PluginLoader ( const PluginLoader& );
    /** Hidden copy operator. */
    PluginLoader& operator= ( const PluginLoader& );

private:
    typedef QMap<QString, Plugin*> PluginMap;
    typedef PluginMap::Iterator PluginMapIter;
    typedef PluginMap::ConstIterator PluginMapConstIter;

    PluginManager& pluginMng;
    Status status;
    PluginMap pluginMap;
};

#endif //PLUGINLOADER_H
