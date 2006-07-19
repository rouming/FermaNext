
package fermanext.system;

import java.util.UUID;

public class NativePlugin extends Plugin 
                          implements NativeObject
{
    public NativePlugin ( String pluginPath )
    { super(pluginPath); }

    /**
     * This is a main execute method.
     */
    public native ExecutionResult execute ( UUIDObject[] uuids );

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


    // Random UUID
    private String uuid = UUID.randomUUID().toString();

    // UUID getter.
    public final String getUUID ()
    { return uuid; }

    // UUID setter.
    public final void setUUID ( String uuid )
    { this.uuid = uuid; }

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
