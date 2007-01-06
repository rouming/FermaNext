package fermanext.trussunit;

import java.util.Map;
import java.util.UUID;

public final class TrussLoadCase implements fermanext.system.NativeObject
{
    // Add new node load
    public native void addLoad ( TrussNode node, double x, double y );

    public native boolean removeLoad ( TrussNode node );

    // Returns load, null if fails
    public native TrussLoad findLoad ( TrussNode node );

    public native int countLoads ();

    // Random UUID
    private String uuid = UUID.randomUUID().toString();

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
