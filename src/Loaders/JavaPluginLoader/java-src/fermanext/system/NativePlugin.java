
package fermanext.system;

import java.util.UUID;

public final class NativePlugin extends Plugin
{
    public NativePlugin ( PluginManager pluginManager, String pluginPath )
    { super(pluginManager, pluginPath); }


    /** 
     * Describes itself.
     * @return plugin info
     */
    public native PluginInfo pluginInfo ();

    /** 
     * Describes plugin state by status code.
     * @return plugin status code
     * @see pluginStatusMsg for detailed information about status.
     */
    public native Status pluginStatusCode ();


    /** 
     * Describes plugin state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return plugin status str msg. Returns empty string if status is Ok
     * @see pluginStatusCode
     */
    public native String pluginStatusMsg ();

    /**
     * Every plugin may accept or reject suggested user's params.
     * This check should be done in inner execution mechanism,
     * before #specificExecute call.
     */
    public native void tryToAcceptParams ( PluginExecutionParams p )
        throws ParamsAreNotAcceptedException;

    /**
     * Returns #Plugin::DependenceMode of this plugin.
     * @see requiredPluginTypes
     */
    public native DependenceMode dependenceMode ();

    /**
     * Other plugins which are requried for properly use.
     * @see dependsOnPluginType
     * @see resolveDependence
     * @return list of plugin types. List is empty if nothing is required
     */
    public native String[] requiredPluginTypes ();

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
    public native boolean isUserExecutable ();
}
