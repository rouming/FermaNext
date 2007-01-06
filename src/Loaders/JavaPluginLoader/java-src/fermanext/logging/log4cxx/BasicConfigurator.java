
package fermanext.logging.log4cxx;

public final class BasicConfigurator
{
    public native static void configure ();
    public native static void resetConfiguration ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
