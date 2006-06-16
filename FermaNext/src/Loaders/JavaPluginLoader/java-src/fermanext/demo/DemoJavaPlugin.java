
package fermanext.demo;

import java.util.Vector;
import fermanext.system.*;
import fermanext.logging.log4cxx.*;

import javax.swing.JOptionPane;

public class DemoJavaPlugin extends JavaPlugin
{
    /** Demo plugin logger */
    private static Logger logger = Logger.getLogger(
                               "java.plugin.DemoJavaPlugin");

    public DemoJavaPlugin ( String jarPath )
    {
        super( jarPath );
    }
    
    public void specificExecute ( Vector<PluginArgument> args )
    {
        logger.debug( "specificExecute: " + args.size() );
        fermanext.trussunit.TrussUnit truss = 
            (fermanext.trussunit.TrussUnit)args.get(0);
        logger.debug( "count nodes: " + truss.countNodes() );

        JOptionPane.showMessageDialog(null,
                                      "This is a pure demo Java plugin!\n" + 
                                      "Truss unit has " + truss.countNodes() +
                                      " nodes!" );
    }

    public Vector<Class<? extends PluginArgument>> executionTypes ()
    {
        Vector<Class<? extends PluginArgument>> vec =
            new Vector<Class<? extends PluginArgument>>();

        // First argument is a TrussUnit class
        vec.add( fermanext.trussunit.TrussUnit.class );

        return vec;
    }    
    
    public PluginInfo pluginInfo ()
    { 
        return new PluginInfo( "DemoJavaPlugin", 
                               "Just a demo java plugin",
                               "demo.java" );
    }
                             
    public Status pluginStatusCode ()
    { return Status.OkStatus; }

    public String pluginStatusMsg ()
    { return new String(); }
    
}

