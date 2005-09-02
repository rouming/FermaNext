
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <vector>
#include <qmap.h>
#include <qstringlist.h>
#include "FermaNextPlugin.h"
#include "DynaLoader.h"

typedef uint PluginHandle;
typedef std::vector<PluginHandle> PluginHandleList ;

// Safe wrapper of POD plugin structure
struct FermaNextPluginInfo
{
    QString name;
    QString description;
    PluginType type;

    FermaNextPluginInfo& operator = ( const PluginInfo& info_ )
    {
        name = info_.name;
        description = info_.description;
        type = info_.type;
        return *this;
    }
};

class PluginLoader 
{
protected:
    struct FermaNextPlugin
    {
        FermaNextPluginInfo info;
        DynaLoader loader;
    };

    void clean ();

public:
    // Exceptions
    class FindException {};

    virtual ~PluginLoader ();

    virtual void loadPlugins ( const QString& path );
    virtual void unloadPlugins ();
    virtual void unloadPlugin ( const PluginHandle& );
    virtual FermaNextPluginInfo& findPluginInfo ( const PluginHandle& ) 
        throw (FindException);

    virtual PluginHandleList loadedPlugins () const;
    virtual QStringList loadedPluginsNames () const;
    virtual PluginHandleList loadedPluginsOfType ( const PluginType& ) const;

private:
    typedef QMap<PluginHandle, FermaNextPlugin*> PluginsMap;
    typedef PluginsMap::iterator PluginsMapIter;
    typedef PluginsMap::const_iterator PluginsMapConstIter;

    PluginsMap plugins;
};

#endif //PLUGINLOADER_H
