package fermanext.trussunit;

public class TrussMaterial extends fermanext.system.PluginArgument
{
    public native void setMaterialName ( String name );
    public native void setWorkingStress ( double stress );
    public native void setElasticityModule ( double module );
    public native void setDensity ( double density );

    public native String getMaterialName ();
    public native double getWorkingStress ();
    public native double getElasticityModule ();
    public native double getDensity ();

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}


