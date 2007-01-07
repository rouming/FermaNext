
#ifndef PLUGIN_H
#define PLUGIN_H

#include <QMap>
#include <QStringList>
#include <QObject>

#include "UUIDObject.h"
#include "PluginExecutionParams.h"

class Plugin;
class PluginManager;

/** Plugin list */
typedef QList<Plugin*> PluginList;
typedef PluginList::Iterator PluginListIter;
typedef PluginList::ConstIterator PluginListConstIter;

/** Map of required plugins according to plugin type */
typedef QMap<QString, PluginList>  RequiredPluginsMap;
typedef RequiredPluginsMap::Iterator RequiredPluginsMapIter;
typedef RequiredPluginsMap::ConstIterator RequiredPluginsMapIterConst;


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
class Plugin : public QObject,
               public UUIDObject
{
    Q_OBJECT
public:
    /** Status codes */
    enum Status {
        UnknownStatus = 0,      /**< Unknown status. */
        OkStatus = 1,           /**< Evrth is ok. Successfully loaded. */
        InternalErrStatus,      /**< Internal error. */
        UnresolvedDependStatus, /**< Unresolved dependencies. */
        ExecutionStopped        /**< Execution was stopped */
    };

    /** Dependence mode. */
    enum DependenceMode {
        Independent = 0,  /**< Plugin is independent */
        SingleDependent,  /**< Plugin depends on a single plugin
                               from #requiredPluginTypes list */
        MultiDependent    /**< Plugin depends on all plugins from
                               from #requiredPluginTypes list */
    };


    // Exceptions 

    /** Base class for all plugin exceptions */
    class PluginException
    {
    public:
        /** Constructor */
        PluginException ( const QString& uuid ) :
            pluginUuid(uuid)
        {}

        QString pluginUuid; /**< Uuid of plugin, which has thrown exception */
    };

    /** Occurs when context is not valid. */
    class ContextIsNotValidException : public PluginException 
    {
    public:
        /** Constructor */
        ContextIsNotValidException ( const QString& uuid ) :
            PluginException(uuid)
        {}
    };

    /** Occurs when dependencies of the plugin are not resolved. */
    class DependenciesAreNotResolvedException : public PluginException
    {
    public:
        /** Constructor */
        DependenciesAreNotResolvedException ( const QString& uuid ) :
            PluginException(uuid)
        {}

        QStringList unresolvedTypes; /**< Contains required 
                                           but unresolved types */
    };

    /** Occurs when plugin does not want to accept params. */
    class ParamsAreNotAcceptedException : public PluginException
    {
    public:
        /** Constructor */
        ParamsAreNotAcceptedException ( const QString& uuid ) :
            PluginException(uuid)
        {}

        /** 
         * Contains varible name (which describes what variable is wrong)
         * and reason (why plugin does not want to accept this variable)
         */
        typedef QPair<QString, QString> ParamsProblem;

        QList<ParamsProblem> problemList; /**< List of problems */
    };

    /** 
     * Occurs when #Plugin::execute called with wrong arguments.
     * Calling side should know correct order and type of arguments.
     */
    class WrongExecutionArgsException : public PluginException 
    {
    public:
        /** Constructor */
        WrongExecutionArgsException ( const QString& uuid ) :
            PluginException(uuid)
        {}
    };



    /**
     * Execution result.
     */
    class ExecutionResult
    {
    public:
        ExecutionResult () :
            status(UnknownStatus)
        {}
        ExecutionResult ( Status s, const QString& d ) :
            status(s), data(d)
        {}
        ExecutionResult ( Status s ) :
            status(s)
        {}

        Status status; /**< Status of the executed operation */
        QString data;  /**< Execution result data in XML */
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
     * This method emits some useful signals and then calls 
     * #specificExecute method of your plugin, so you should 
     * override #specificExecute in your successor class.
     * @param arguments to start execution
     * @return result of executed operation
     * @throw ContextIsNotValidException occurs when context is wrong.
     * @throw ParamsAreNotAcceptedException occurs when plugin does not 
     *        accept user's params.
     * @throw DependenciesAreNotResolvedException occurs when dependencies
     *        were not corretly resolved.
     * @throw WrongExecutionArgsException occurs when 
     *        contract (argument list) between calling side
     *        and plugin is wrong. 
     * @see specificExecute
     */
    Plugin::ExecutionResult execute ( const QList<UUIDObject*>& arguments ) 
        /*throw (ContextIsNotValidException,
                 ParamsAreNotAcceptedException,
                 DependenciesAreNotResolvedException,
                 WrongExecutionArgsException)*/;

    
    /** 
     * Stops execution. 
     * Should be implemented in child classes.
     * @see execute
     */
    virtual void stopExecution ();

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
    virtual Plugin::Status pluginStatusCode () const = 0;

    /** 
     * Describes plugin state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return plugin status str msg. Returns empty string if status is Ok
     * @see pluginStatusCode
     */
    virtual QString pluginStatusMsg () const = 0;

    /**
     * Every plugin may accept or reject suggested user's params.
     * This check should be done in inner execution mechanism,
     * before #specificExecute call.
     */
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const 
        /*throw (ParamsAreNotAcceptedException)*/ = 0;

    /**
     * Returns #Plugin::DependenceMode of this plugin.
     * @see requiredPluginTypes
     */
    virtual Plugin::DependenceMode dependenceMode () const;

    /**
     * Other plugins which are requried for properly use.
     * @see dependsOnPluginType
     * @see resolveDependence
     * @return list of plugin types. List is empty if nothing is required
     */
    virtual const QStringList& requiredPluginTypes () const;

    /**
     * Every plugin can be a user executable plugin (i.e. execution
     * can be started by user from GUI) or plugin can be only a part 
     * of execution tree (i.e. it makes sense to execute such plugin 
     * _only_ from another plugin, not from GUI). 
     * Note: it is absolutely correct if one plugin can be a user 
     *       executable plugin and can be a part of execution tree.
     *       Such plugin should return true.
     *       (by default #Plugin returns true)
     * @return true if plugin can be executed by user from GUI,
     *         false if plugin is a part of execution tree.
     */
    virtual bool isUserExecutable () const;

protected:
    /** 
     * Sets plugin status.
     * @param s plugin status
     */
    void setStatus ( Plugin::Status s );

private:
    /** 
     * Main entry point to execute plugin. This method is 
     * private because it can be called only from #execute method. 
     * You should override #specificExecute in child classes.
     * @param params of plugin.
     * @param args execution arguments.
     * @param deps plugin dependencies.
     * @return result of executed operation
     * @throw ContextIsNotValidException occurs when this plugin execute
     *        its dependence and there smth has gone wrong.
     * @throw ParamsAreNotAcceptedException occurs when this plugin execute
     *        its dependence and there smth has gone wrong.
     * @throw WrongExecutionArgsException, 
     *        occurs when contract (argument list) between calling side
     *        and plugin is wrong. 
     * @throw DependenciesAreNotResolvedException, 
     *        occurs when dependencies are not resolved.
     * @see execute
     */    
    virtual Plugin::ExecutionResult specificExecute ( 
                              const PluginExecutionParams& params,
                              const QList<UUIDObject*>& args,
                              const QHash< QString, QList<Plugin*> >& deps  )
        /*throw (ContextIsNotValidException,
                 ParamsAreNotAcceptedException,
                 DependenciesAreNotResolvedException,
                 WrongExecutionArgsException)*/ = 0;


signals:
    /** Is emitted before actual plugins execution */
    void beforeExecution ( Plugin& );
    /** Is emitted after actual plugins execution */
    void afterExecution ( Plugin&, Plugin::ExecutionResult );

private:
    Status status;
    PluginManager& plgMng;
    QString plgPath;
    RequiredPluginsMap requiredPluginsMap;
};

#endif //PLUGIN_H
