
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
    
    public ExecutionResult specificExecute ( TrussUnit truss )
    {        
        logger.debug( "specificExecute" );

        logger.debug( "count nodes: " + truss.countNodes() );

        TrussNode newNode = truss.createNode( 12.3, 34.5 );
        newNode.setPoint( 34.5, 12.3);
        newNode.setFixation( TrussNode.Fixation.FixationByX );

        TrussPivot[] pivots = truss.getPivotList();
        System.out.println( "Pivots number: " + pivots.length );
        for ( TrussPivot pivot : pivots ) {
            TrussNode fNode = pivot.getFirstNode();
            TrussNode lNode = pivot.getLastNode();
            System.out.println( "   pivot " + pivot.getNumber() + 
                            " (" + fNode.getX() + ", " + fNode.getY() + "),"+
                            " (" + lNode.getX() + ", " + lNode.getY() + ")" );
        }

        TrussNode[] nodes = truss.getNodeList();
        System.out.println( "\nNodes number: " + nodes.length );
        for ( TrussNode node : nodes ) {            
            System.out.println( "   node " + node.getNumber() + 
                           " (" + node.getX() + ", " + node.getY() + ")" );
        }

        TrussLoadCaseArray loadCases = truss.getLoadCases();
           
        int loadCasesNum = loadCases.countLoadCases();
        System.out.println( "\nLoad cases number: " + loadCasesNum );
        for ( int i = 1; i <= loadCasesNum; ++i ) {
            TrussLoadCase loadCase = loadCases.findLoadCase( i );

            int loadsNum = loadCase.countLoads();
            System.out.println( "  load case " + i + " has " + loadsNum + 
                                " loads");
            for ( TrussNode node : nodes ) {
                TrussLoad load = loadCase.findLoad( node );
                if ( load == null ) 
                    continue;
                System.out.println( "    load for node " + node.getNumber() +
                                    " has X force=" +  load.getXForce() + 
                                    " and Y Force=" +   load.getYForce() );
            }
        }

        JOptionPane.showMessageDialog( null,
                                       "This is a pure demo Java plugin!\n" + 
                                       "Truss unit has " + truss.countNodes() +
                                       " nodes!",
                                       "Java plugin works great!",
                                       JOptionPane.INFORMATION_MESSAGE );

        return new ExecutionResult( Status.OkStatus, "Nothing" );
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

