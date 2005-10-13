
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <vector>
#include <qmap.h>
#include <qstringlist.h>
#include "Plugin.h"

typedef PluginChecksum PluginHandle;
typedef std::vector<PluginHandle> PluginHandleList ;

class PluginLoader;
class DynaLoader;

class PluginManager 
{
protected:
    void clean ();

public:
    // Exceptions
    class FindException {};

    virtual ~PluginManager ();

    virtual void loadPlugins ( const QString& path );
    virtual void unloadPlugins ();
    virtual void unloadPlugin ( const PluginHandle& );
    virtual Plugin& findPlugin ( const PluginHandle& ) throw (FindException);

    virtual PluginHandleList loadedPlugins () const;
    virtual QStringList loadedPluginsNames () const;
    virtual PluginHandleList loadedPluginsOfType ( const PluginType& ) const;

public:
    // Extension to load plugins from plugin dir.
    static const QString PluginExtension;

private:
    typedef QMap<PluginHandle, PluginLoader*> PluginsMap;
    typedef PluginsMap::iterator PluginsMapIter;
    typedef PluginsMap::const_iterator PluginsMapConstIter;

    typedef std::vector<DynaLoader*> PreloadedDynaLibs;
    typedef PreloadedDynaLibs::iterator PreloadedDynaLibsIter;

    PluginsMap plugins;
    PreloadedDynaLibs dynaLibs;
};

#endif //PLUGINMANAGER_H
