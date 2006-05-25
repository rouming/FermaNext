
package fermanext.logging.log4cxx;

public class NDC
{    
    public native static String get ();
    public native static String pop ();
    public native static void push ( String message );
    public native static void remove ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
