package fermanext.trussunit;

public class TrussNode extends fermanext.system.PluginArgument
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

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
