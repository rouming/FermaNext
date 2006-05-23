
#ifndef PLUGIN_H
#define PLUGIN_H

#include <vector>
#include <QMap>
#include <QStringList>
#include <QObject>

class Plugin;
class PluginManager;
class UUIDObject;

/** Plugin list */
typedef std::vector<Plugin*> PluginList;
typedef PluginList::iterator PluginListIter;
typedef PluginList::const_iterator PluginListConstIter;

/** Map of required plugins according to plugin type */
typedef QMap<QString, PluginList>  RequiredPluginsMap;
typedef RequiredPluginsMap::Iterator RequiredPluginsMapIter;
typedef RequiredPluginsMap::ConstIterator RequiredPluginsMapIterConst;

// Exceptions
/** Occurs when required plugin types can't be resolved */
struct RequiredPluginIsNotResolvedException { QStringList unresolvedTypes; };

/** 
 * Occurs when #Plugin::execute called with wrong arguments.
 * Calling side should know correct order and type of arguments.
 */
class WrongExecutionArgsException {};

/**
 * Plugin info, which provides basic knowledge about plugin: type and name.
 */
struct PluginInfo
{
    PluginInfo ()
    {}

    PluginInfo ( const QString& n, const QString& d, const QString& t ) :
        name(n), description(d), type(t)
    {}

    QString name;        /**< Plugin name. e.g. 'SimplePlugin'. */
    QString description; /**< Just plugin description. */
    QString type;        /**< String which represents plugin type. 
                              (e.g. 'calculation.simple').
                              So plugin type is a hierarchical structure 
                              of type names for easy search. (e.g. find all
                              plugins with types 'calculation.*', and the 
                              result can be: 'calculation.simple', 
                              'calculation.finiteelements', etc )
                              Another important moment is that nothing 
                              controls this type names. Plugin type is a 
                              contract between plugin and some part of the 
                              system, which wants to use this plugin. 
                              Types aren't hardcoded and aren't known by 
                              the whole system, i.e. logic bundle stays 
                              on your own. */
};

/**
 * Basic plugin class which should be
 * the parent of all real FN plugins.
 */
class Plugin : public QObject
{
    Q_OBJECT
public:
    /** Status codes */
    enum Status {
        OkStatus = 0,          /**< Evrth is ok. Successfully loaded. */
        InternalErrStatus,     /**< Internal error. */
        UnresolvedDependStatus /**< Unresolved dependencies. */
    };

    /** 
     * Resolving mode. Plugin manager #PluginManager should know in what mode 
     * it should resolve plugin dependencies. The thing is that for some 
     * plugins dependencies should be resolved immediately, i.e. after loading,
     * but for others this resolving should be made on demand.
     */
    enum ResolvingMode {
        Immediately = 0, /**< Dependence resolving is made by plugin manager 
                           *< immediately after all plugins have been loaded.*/
        OnDemand         /**< Dependence resolving is made by plugin itself 
                           *< on demand */
    };

    /** 
     * Plugin costructor.
     * @param mng plugin manager. It is used e.g. for resolving dependencies.
     * @param path of plugin library, from which it was loaded.
     */
    Plugin ( PluginManager& mng, const QString& path );

    /** Destructor. Must be ovveriden in child classes. */
    virtual ~Plugin ();

    /** 
     * Main entry point to execute plugin. 
     * @param arguments to start execution
     * @throw WrongExecutionArgsException occurs when 
     *        contract (argument list) between calling side
     *        and plugin is wrong. 
     */
    virtual void execute ( const QList<UUIDObject*>& arguments ) 
        /*throw (WrongExecutionArgsException)*/ = 0;

    /** 
     * Returns plugin manager.
     * @return plugin manager
     */
    PluginManager& pluginManager ();

    /**
     * Returns plugin path of a library, from which it was loaded.
     * @return plugin path as a string.
     */
    const QString& pluginPath () const;

    /** 
     * Describes itself.
     * @return plugin info
     */
    virtual const PluginInfo& pluginInfo () const = 0;

    /** 
     * Describes plugin state by status code.
     * @return plugin status code
     * @see pluginStatusMsg for detailed information about status.
     */
    virtual Status pluginStatusCode () const = 0;

    /** 
     * Describes plugin state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return plugin status str msg. Returns empty string if status is Ok
     * @see pluginStatusCode
     */
    virtual QString pluginStatusMsg () const = 0;

    /**
     * Other plugins which are requried for properly use.
     * @see dependsOnPluginType
     * @see resolveDependence
     * @return list of plugin types. List is empty if nothing is required
     */
    virtual const QStringList& requiredPluginTypes () const;

    /**
     * Returns dependence resolving mode. Default behaviour is to resolve
     * dependencies immediately, i.e. after loading.
     * @return dependence resolving mode.
     * @see requiredPluginTypes
     * @see resolveDependence
     */
    virtual ResolvingMode resolvingMode () const;

    /**
     * Checks plugin dependence
     * @see requiredPluginTypes
     * @see resolveDependence
     * @param type of the requried plugin
     * @return true if plugin depends on this plugin, false otherwise.
     */
    bool dependsOnPluginType ( const QString& type ) const;

    /**
     * Resolves plugin dependence.
     * @see requiredPluginTypes
     * @see dependsOnPluginType
     * @throw RequiredPluginIsNotResolvedException some plugin dependence 
     * can't be resolved
     */
    void resolveDependence () 
        /*throw (RequiredPluginIsNotResolvedException)*/;

protected:
    /** 
     * Plugin setup method to init plugin variables with dialog widget or 
     * smth like that.
     */
    virtual void startWizardSetup ();

    /** 
     * Sets plugin status.
     * @param s plugin status
     */
    void setStatus ( Status s );

private:
    Status status;
    PluginManager& plgMng;
    QString plgPath;
    RequiredPluginsMap requiredPluginsMap;
};

#endif //PLUGIN_H
