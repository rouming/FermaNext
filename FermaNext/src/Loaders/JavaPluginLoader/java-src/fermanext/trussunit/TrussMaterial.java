package fermanext.trussunit;

import java.util.UUID;

public class TrussMaterial implements fermanext.system.NativeObject
{
    public native void setMaterialName ( String name );
    public native void setWorkingStress ( double stress );
    public native void setElasticityModule ( double module );
    public native void setDensity ( double density );

    public native String getMaterialName ();
    public native double getWorkingStress ();
    public native double getElasticityModule ();
    public native double getDensity ();

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


