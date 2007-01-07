
package fermanext.demo;

import java.util.*;
import fermanext.system.*;
import fermanext.trussunit.*;
import fermanext.logging.log4cxx.*;

import javax.swing.JOptionPane;

public final class DemoJavaPlugin extends JavaPlugin
{
    /** Demo plugin logger */
    private static Logger logger = Logger.getLogger(
                               "java.plugin.DemoJavaPlugin");

    /** Execution argument types */
    private static Class[] m_execArgumentTypes = 
        new Class[]{ TrussUnit.class };

    public DemoJavaPlugin ( PluginManager pluginMng, String pluginPath )
    { super(pluginMng, pluginPath); }
    
    protected Class<?>[] executionArgumentTypes ()
    { return m_execArgumentTypes; }

    protected ExecutionResult specificExecute ( 
                                           PluginExecutionParams params,
                                           UUIDObject[] args,
                                           Hashtable<String, Plugin[]> deps  )
        throws ContextIsNotValidException,
               ParamsAreNotAcceptedException,
               DependenciesAreNotResolvedException,
               WrongExecutionArgsException
    {        
        logger.debug( "specificExecute" );

        if ( args == null || args.length != 1 ) {
            logger.error("args == null or args.length != 1");
            throw new WrongExecutionArgsException(getUUID());
        }

        
        TrussUnit truss = null;
        try { truss = (TrussUnit)args[0]; }
        catch ( Exception e ) {
            logger.error("Argument is not a truss unit instance");
            throw new WrongExecutionArgsException(getUUID());
        }

        if ( ! deps.containsKey("calculation.simple.native") ) {
            DependenciesAreNotResolvedException e = 
                new DependenciesAreNotResolvedException(getUUID());
            e.unresolvedTypes.add("calculation.simple.native" );
            throw e;
        }
        
        Plugin calcPlg = deps.get("calculation.simple.native")[0];
        
        if ( calcPlg == null ) {
            DependenciesAreNotResolvedException e = 
                new DependenciesAreNotResolvedException(getUUID());
            e.unresolvedTypes.add("calculation.simple.native (null)" );
            throw e;
        }
        
        System.out.println( "Calc plg uuid:" + calcPlg.getUUID() );
        System.out.println( "Plg mng uuid:" + calcPlg.pluginManager().getUUID() );
        System.out.println( "Plg path:" + calcPlg.pluginPath() );
        System.out.println( "Plg info name:" + calcPlg.pluginInfo().name );
        System.out.println( "Plg info type:" + calcPlg.pluginInfo().type );
        System.out.println( "Plg info type:" + calcPlg.pluginInfo().description );
        System.out.println( "Plg info type:" + calcPlg.pluginInfo().description );
        System.out.println( "Plg status code:" + calcPlg.pluginStatusCode() );
        System.out.println( "Plg status msg:" + calcPlg.pluginStatusMsg() );
        System.out.println( "Plg dependence mode:" + calcPlg.dependenceMode() );
        System.out.println( "Plg required types:" + calcPlg.requiredPluginTypes() );
        System.out.println( "Plg is executable:" + calcPlg.isUserExecutable() );

        ExecutionResult calcPlgRes = calcPlg.execute( args );


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

        return calcPlgRes;
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

    public void tryToAcceptParams ( PluginExecutionParams p )
        throws ParamsAreNotAcceptedException
    { }

    public DependenceMode dependenceMode ()
    { return DependenceMode.MultiDependent; }

    public String[] requiredPluginTypes ()
    { return new String[] { "calculation.*" }; }

    public boolean isUserExecutable ()
    { return true; }    
}

