
#ifndef JAVAPLUGINLOADER_H
#define JAVAPLUGINLOADER_H

#include "PluginLoader.h"

#include "JavaVM/JavaVirtualMachine.h"

class JavaPluginLoader : public PluginLoader
{
public:
    /** Constructor */
    JavaPluginLoader ( PluginManager&, const QString& path );

    /** Destroys all laoded java plugins. */
    virtual ~JavaPluginLoader ();

    /** 
     * Returns java plugin extension.
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
     * Loads java plugins.
     * @see PluginLoader::specificLoadPlugin
     */
    virtual Plugin& specificLoadPlugin ( const QString& pathToPlugin )
        /*throw (PluginLoadException)*/;

    /** 
     * Unloads java plugins.
     * @see PluginLoader::specificUnloadPlugin
     */
    virtual void specificUnloadPlugin ( Plugin& plugin );

    /**
     * Unloads all java plugins.
     * @see PluginLoader::specificUnloadPlugins
     */
    virtual void specificUnloadPlugins ();

private:
    /** Hidden copy constructor. */
    JavaPluginLoader ( const JavaPluginLoader& );
    /** Hidden copy operator. */
    JavaPluginLoader& operator= ( const JavaPluginLoader& );

private:
    /** JVM */
    JavaVirtualMachine* javaVM;
    /** Loaded java 'FermaNext' class */
    JClass fnClass;
    /** Instance of java 'FermaNext' class */
    JObject fnObject;
    /** Instance of java 'JavaPluginLoader' class */
    JObject plgLoaderObject;
};

#endif //JAVAPLUGINLOADER_H
