package fermanext.trussunit;

import java.util.UUID;

public final class TrussNode implements fermanext.system.NativeObject
{
    public enum Fixation {
        Unfixed,
        FixationByX,
        FixationByY, 
        FixationByXY;
    }

    native public void setFixation ( Fixation f );
    native public Fixation getFixation ();
    
    native public void setPoint ( DoublePoint p );
    native public void setPoint ( double x, double y );
        
    native public DoublePoint getPoint ();
    native public double getX ();
    native public double getY ();

    native public int getNumber ();

    // Random UUID
    private String uuid = UUID.randomUUID().toString();

    // UUID getter.
    public final String getUUID ()
    { return uuid; }

    // UUID setter.
    public final void setUUID ( String uuid )
    { this.uuid = uuid; }

    // Checks if this plugin really wraps native class
    public final native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
