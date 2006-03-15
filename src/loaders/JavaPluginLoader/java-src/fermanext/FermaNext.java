
package fermanext;

import java.awt.Frame;

public class FermaNext
{
    /**
       Correctly disposes all frames and allows the
       application to exit cleanly. Call this method 
       before stopping jvm.
     */
    public static void disposeAllFrames ()
    {
        Frame frames[] = Frame.getFrames();
        for ( int i = 0; i < frames.length; ++i )
            frames[i].dispose();
    }
};
