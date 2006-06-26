package fermanext.trussunit;

import java.util.Map;
import java.util.UUID;

public class TrussLoadCase implements fermanext.system.NativeObject
{
    // Add new node load
    public native void addLoad ( TrussNode node, double x, double y );

    public native boolean removeLoad ( TrussNode node );

    // Returns load, null if fails
    public native TrussLoad findLoad ( TrussNode node );

    public native Map<TrussNode, TrussLoad> getTrussLoadMap ();

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
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
