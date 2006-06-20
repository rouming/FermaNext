package fermanext.trussunit;

public class TrussLoad extends fermanext.system.PluginArgument
{
    public native double getXForce ();
    public native double getYForce ();

    public native void setXForce ( double xF );
    public native void setYForce ( double yF );
    public native void setForces ( double xF, double yF );

    public native boolean isEnabled ();
    public native boolean isRemoved ();

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
