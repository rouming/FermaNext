
package fermanext.system;

import java.awt.Frame;

public class FermaNext
{
    private static FermaNext fnInstance = null;

    private JavaPluginLoader pluginLoader = new JavaPluginLoader();

    /**
     * Returns FermaNext instance according 
     * to Singleton template.
     * Note: method is not thread-safe.
     */
    public static FermaNext instance () 
    {
        if ( fnInstance == null ) {
            fnInstance = new FermaNext();
        }
        return fnInstance;
    }
    
    /** Use #instance instead */
    private FermaNext () {}           
    
    /**
     * Correctly disposes all frames and allows the
     * application to exit cleanly. Call this method 
     * before stopping JVM.
     */
    public void disposeAllFrames ()
    {
        Frame frames[] = Frame.getFrames();
        for ( int i = 0; i < frames.length; ++i )
            frames[i].dispose();
    }

    /** Return plugin loader */
    public JavaPluginLoader pluginLoader ()
    { return pluginLoader; }

    
}
