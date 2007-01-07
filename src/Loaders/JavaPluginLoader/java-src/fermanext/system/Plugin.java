
package fermanext.system;

import java.util.Vector;
import java.util.TreeMap;
import java.util.Hashtable;

public abstract class Plugin implements NativeObject
{
    /** Status codes */
    public enum Status {
        UnknownStatus,          /**< Unknown status. */
        OkStatus,               /**< Evrth is ok. Successfully loaded. */
        InternalErrStatus,      /**< Internal error. */
        UnresolvedDependStatus, /**< Unresolved dependencies. */
        ExecutionStopped        /**< Execution was stopped */
    }

    /** Dependence mode. */
    public enum DependenceMode {
        Independent,          /**< Plugin is independent */
        SingleDependent,      /**< Plugin depends on a single plugin
                                   from #requiredPluginTypes list */
        MultiDependent        /**< Plugin depends on all plugins from
                                   from #requiredPluginTypes list */
    }

    // Exceptions 

    /** Base class for all plugin exceptions */
    public static abstract class PluginException extends Exception {

        /** Constructor */
        PluginException ( String uuid ) {
            pluginUuid = uuid;
        }

        /** Uuid of plugin, which has thrown exception */
        public String pluginUuid = new String();
    }

    /** Occurs when context is not valid. */
    public static class ContextIsNotValidException extends PluginException {
        
        /** Constrcutor */
        public ContextIsNotValidException ( String uuid )
        { super(uuid); }
    }

    /** Occurs when dependencies of the plugin are not resolved. */
    public static class DependenciesAreNotResolvedException 
        extends PluginException
    {
        /** Contains required but unresolved types */
        public Vector<String> unresolvedTypes = new Vector<String>();

        /** Constrcutor */
        public DependenciesAreNotResolvedException ( String uuid )
        { super(uuid); }
    }

    /** Occurs when plugin does not want to accept params. */
    public static class ParamsAreNotAcceptedException extends PluginException
    {
        /** 
         * Contains varible name (which describes what variable is wrong)
         * and reason (why plugin does not want to accept this variable)
         */
        public TreeMap<String, String> problemList =
            new TreeMap<String, String>();

        /** Constrcutor */
        public ParamsAreNotAcceptedException ( String uuid )
        { super(uuid); }
    }

    /** 
     * Occurs when #Plugin::execute called with wrong arguments.
     * Calling side should know correct order and type of arguments.
     */
    public static class WrongExecutionArgsException extends PluginException {

        /** Constrcutor */
        public WrongExecutionArgsException ( String uuid )
        { super(uuid); }
    }


    /** Execution result */
    public static final class ExecutionResult 
    {
        public ExecutionResult ()
        {}

        public ExecutionResult ( Status s )
        { status = s; }

        public ExecutionResult ( Status s, String d )
        { status = s; data = d; }

        public Status status = Status.UnknownStatus;
        public String data = new String();
    }

    /** 
     * Plugin costructor.
     * @param mng plugin manager. It is used e.g. for resolving dependencies.
     * @param path of plugin library, from which it was loaded.
     */
    public Plugin ( PluginManager mng, String path )
    { 
        m_pluginMng = mng;
        m_pluginPath = path; 
    }

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
    public final native ExecutionResult execute ( UUIDObject[] arguments )
        throws ContextIsNotValidException,
               ParamsAreNotAcceptedException,
               DependenciesAreNotResolvedException,
               WrongExecutionArgsException;

    /** 
     * Stops execution. 
     * Should be implemented in child classes.
     * @see execute
     */
    public void stopExecution ()
    { /* do nothing */ }


    /** 
     * Returns plugin manager.
     * @return plugin manager
     */
    public final PluginManager pluginManager ()
    { return m_pluginMng; }

    /**
     * Returns plugin path of a library, from which it was loaded.
     * @return plugin path as a string.
     */
    public final String pluginPath ()
    { return m_pluginPath; }

    /** 
     * Describes itself.
     * @return plugin info
     */
    public abstract PluginInfo pluginInfo ();

    /** 
     * Describes plugin state by status code.
     * @return plugin status code
     * @see pluginStatusMsg for detailed information about status.
     */
    public abstract Status pluginStatusCode ();

    /** Simply sets status. Should be called from #Plugin heirs. */
    protected final void setStatus ( Status status )
    { m_status = status; }

    /** 
     * Describes plugin state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return plugin status str msg. Returns empty string if status is Ok
     * @see pluginStatusCode
     */
    public abstract String pluginStatusMsg ();

    /**
     * Every plugin may accept or reject suggested user's params.
     * This check should be done in inner execution mechanism,
     * before #specificExecute call.
     */
    public abstract void tryToAcceptParams ( PluginExecutionParams p )
        throws ParamsAreNotAcceptedException;

    /**
     * Returns #Plugin::DependenceMode of this plugin.
     * @see requiredPluginTypes
     */
    public abstract DependenceMode dependenceMode ();

    /**
     * Other plugins which are requried for properly use.
     * @see dependsOnPluginType
     * @see resolveDependence
     * @return list of plugin types. List is empty if nothing is required
     */
    public abstract String[] requiredPluginTypes ();

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
    public abstract boolean isUserExecutable ();


    /** Returns UUID, which is set by native side. */
    public final String getUUID ()
    { return m_uuid; }

    /** Sets UUID. Must be called from native side. */
    public final void setUUID ( String uuid )
    { m_uuid = uuid; }

    /** Checks if this plugin really wraps native class */
    public final native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }

// Private members

    /** Singleton of plugin manager */
    private PluginManager m_pluginMng = new PluginManager();

    /** Path to JAR file */
    private String m_pluginPath = new String();

    /** Default plugin status */
    private Status m_status = Status.OkStatus;

    /** 
     * Plugin uuid. 
     * It is not valid by default. 
     * Should be set from native side. 
     * @see getUUID
     * @see setUUID
     */
    private String m_uuid = new String();
}
