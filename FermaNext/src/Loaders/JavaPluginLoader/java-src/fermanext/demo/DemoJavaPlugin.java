
package fermanext.demo;

import java.util.Vector;
import fermanext.system.*;
import fermanext.trussunit.*;
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
    
    public void specificExecute ( TrussUnit truss )
    {        
        logger.debug( "specificExecute" );

        logger.debug( "count nodes: " + truss.countNodes() );

        TrussNode node = truss.createNode( 12.3, 34.5 );
        node.setPoint( 34.5, 12.3);
        node.setFixation( TrussNode.Fixation.FixationByX );        

        JOptionPane.showMessageDialog( null,
                                       "This is a pure demo Java plugin!\n" + 
                                       "Truss unit has " + truss.countNodes() +
                                       " nodes!",
                                       "Java plugin works great!",
                                       JOptionPane.INFORMATION_MESSAGE );

    }

    public PluginInfo pluginInfo ()
    { 
        return new PluginInfo( "DemoJavaPlugin", 
                               "Just a demo java plugin",
                               "calculation.demo.java" );
    }
                             
    public Status pluginStatusCode ()
    { return Status.OkStatus; }

    public String pluginStatusMsg ()
    { return new String(); }
    
}

