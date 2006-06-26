package fermanext.trussunit;

import java.util.UUID;

public class TrussPivot implements fermanext.system.NativeObject
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
