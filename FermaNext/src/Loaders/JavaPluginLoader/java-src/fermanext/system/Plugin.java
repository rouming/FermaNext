
package fermanext.system;

public abstract class Plugin
{
    /** Status codes */
    public enum Status {
        OkStatus,
        InternalErrStatus,
        UnresolvedDependStatus
    }

    /** Resolving mode. */
    public enum ResolvingMode {
        Immediately,
        OnDemand
    }

    /** Path to JAR file */
    private String path = new String();

    public Plugin ( String pluginPath )
    { path = pluginPath; }

    /**
     * Returns plugin path of a library, from which it was loaded.
     * @return plugin path as a string.
     */
    public final String pluginPath ()
    { return path; }

    /**
     * This is a main execute method.
     */
    public abstract void execute ( UUIDObject[] args );

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

    /** 
     * Describes plugin state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return plugin status str msg. Returns empty string if status is Ok
     * @see pluginStatusCode
     */
    public abstract String pluginStatusMsg ();
}
