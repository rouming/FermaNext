package fermanext.trussunit;

public class DoubleSize extends java.awt.geom.Dimension2D
{
    private double width = 0;
    private double height = 0;

    DoubleSize ()
    {}

    DoubleSize ( double w, double h )
    {
        width = w;
        height = h;
    }

    public double getHeight ()
    {
        return height;
    }

    public double getWidth ()
    {
        return width;
    }

    public void setSize ( double w, double h )
    {
        width = w;
        height = h;
    }
}


