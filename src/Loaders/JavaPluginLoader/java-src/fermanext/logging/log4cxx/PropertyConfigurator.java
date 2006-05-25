
package fermanext.logging.log4cxx;

public class PropertyConfigurator
{
    public native static void configureAndWatch ( String fileName, long delay );

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
