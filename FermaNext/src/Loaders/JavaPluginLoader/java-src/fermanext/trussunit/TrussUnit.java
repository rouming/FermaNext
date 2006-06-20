package fermanext.trussunit;

public class TrussUnit extends fermanext.system.PluginArgument
{
    public native TrussNode findNodeByCoord ( DoublePoint p );
    public native TrussNode findNodeByNumber ( int num );
    public native TrussPivot findPivotByNumber ( int num );
    public native TrussPivot findPivotByNodes ( TrussNode n1, TrussNode n2 );
    public native TrussPivot[] findAdjoiningPivots ( TrussNode node );
    public native TrussNode createNode ( DoublePoint p );
    public native TrussNode createNode ( double x, double y );

    public native TrussPivot createPivot ( TrussNode first, TrussNode last );
    public native int countNodes ();
    public native int countPivots ();
    public native TrussNode[] getNodeList ();
    public native TrussPivot[] getPivotList ();
    public native DoubleSize getTrussAreaSize ();
    public native TrussLoadCaseArray getLoadCases ();
    public native TrussMaterial getMaterial ();
    public native void setMaterial ( TrussMaterial mat );
    public native int getNumber ();

    // Checks if this plugin really wraps native class
    public native boolean isValid ();

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
