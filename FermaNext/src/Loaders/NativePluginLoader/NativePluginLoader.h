
#ifndef NATIVEPLUGINLOADER_H
#define NATIVEPLUGINLOADER_H

#include "PluginLoader.h"

class DynaLoader;
class NativePlugin;

class NativePluginLoader : public PluginLoader
{
public:
    NativePluginLoader ( PluginManager& );

    /** 
     * Destroys all laoded native plugins.
     */
    virtual ~NativePluginLoader ();

    /** 
     * Returns native plugin extension.
     * @see PluginLoader::pluginExtension
     */
    virtual const QString& pluginExtension () const;

    /** 
     * Describes state by status code.
     * @return plugin loader status code
     * @see PluginLoader::pluginLoaderStatusCode
     * @see pluginLoaderStatusMsg for detailed information about status.
     */
    virtual Status pluginLoaderStatusCode () const;

    /** 
     * Describes state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return status string msg. Returns empty string if status is Ok.
     * @see PluginLoader::pluginLoaderStatusMsg
     * @see pluginLoaderStatusCode
     */
    virtual QString pluginLoaderStatusMsg () const;

protected:
    /** 
     * Loads native plugins with support of dynamic libs.
     * @see PluginLoader::specificLoadPlugin
     */
    virtual Plugin& specificLoadPlugin ( const QString& pathToPlugin )
        throw (PluginLoadException);

    /** 
     * Unloads native plugins.
     * @see PluginLoader::specificUnloadPlugin
     */
    virtual void specificUnloadPlugin ( Plugin& plugin );

    /**
     * Unloads all native plugins.
     * @see PluginLoader::specificUnloadPlugins
     */
    virtual void specificUnloadPlugins ();

private:
    /** Hidden copy constructor. */
    NativePluginLoader ( const NativePluginLoader& );
    /** Hidden copy operator. */
    NativePluginLoader& operator= ( const NativePluginLoader& );

private:
    typedef QMap< NativePlugin*, DynaLoader* > NativePluginMap;
    typedef NativePluginMap::Iterator NativePluginMapIter;

    NativePluginMap pluginMap;
};

#endif //NATIVEPLUGINLOADER_H
