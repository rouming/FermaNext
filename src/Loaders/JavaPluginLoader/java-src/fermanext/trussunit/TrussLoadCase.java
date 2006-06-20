package fermanext.trussunit;

import java.util.Map;

public class TrussLoadCase extends fermanext.system.PluginArgument
{
    // Add new node load
    public native void addLoad ( TrussNode node, double x, double y );

    public native boolean removeLoad ( TrussNode node );

    // Returns load, null if fails
    public native TrussLoad findLoad ( TrussNode node );

    public native Map<TrussNode, TrussLoad> getTrussLoadMap ();

    public native int countLoads ();

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
