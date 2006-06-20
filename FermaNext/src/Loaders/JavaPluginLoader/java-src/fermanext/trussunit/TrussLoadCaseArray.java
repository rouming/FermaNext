package fermanext.trussunit;

public class TrussLoadCaseArray extends fermanext.system.PluginArgument
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

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
