package fermanext.trussunit;

import java.util.UUID;

public class TrussLoad implements fermanext.system.NativeObject
{
    public native double getXForce ();
    public native double getYForce ();

    public native void setXForce ( double xF );
    public native void setYForce ( double yF );
    public native void setForces ( double xF, double yF );

    public native boolean isEnabled ();
    public native boolean isRemoved ();

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
