package fermanext.trussunit;

public class TrussPivot extends fermanext.system.PluginArgument
{
    public native TrussNode getFirstNode ();
    public native TrussNode getLastNode ();
    
    public native void setFirstNode ( TrussNode first );
    public native void setLastNode ( TrussNode last );
    
    public native TrussMaterial getMaterial ();
    public native void setMaterial ( TrussMaterial mat );

    public native double getThickness ();
    public native void setThickness ( double th );

    public native int getNumber ();

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
