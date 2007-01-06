
package fermanext.system;

public final class PluginManager implements fermanext.system.NativeObject
{
    // Default uuid is invalid.
    private String uuid;

    // UUID getter.
    public final String getUUID ()
    { return uuid; }

    // UUID setter.
    public final void setUUID ( String uuid )
    { this.uuid = uuid; }

    // Checks if this plugin really wraps native class
    public final native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
