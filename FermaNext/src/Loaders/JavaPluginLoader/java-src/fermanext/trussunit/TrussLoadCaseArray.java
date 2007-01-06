package fermanext.trussunit;

import java.util.UUID;

public final class TrussLoadCaseArray implements fermanext.system.NativeObject
{
    public native TrussLoadCase createLoadCase ();

    public native boolean removeLoadCase ( TrussLoadCase loadCase );
    public native boolean removeLoadCase ( int indx );

    public native boolean setCurrentLoadCase ( int indx );
    public native boolean setCurrentLoadCase ( TrussLoadCase loadCase );

    public native TrussLoadCase getCurrentLoadCase ();

    public native int getLoadCaseIndex ( TrussLoadCase loadCase );
    public native TrussLoadCase findLoadCase ( int indx );

    public native int countLoadCases ();

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
