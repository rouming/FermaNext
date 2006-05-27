
package fermanext.demo;

import java.util.Vector;
import fermanext.system.*;

public class DemoJavaPlugin extends JavaPlugin
{
    public DemoJavaPlugin ( String jarPath )
    {
        super( jarPath );
    }
    
    public void execute ( Vector<PluginExecutionArgument> args )
    {
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

