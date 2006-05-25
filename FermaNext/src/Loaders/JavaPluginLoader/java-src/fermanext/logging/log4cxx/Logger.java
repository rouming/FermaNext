
package fermanext.logging.log4cxx;

import java.util.UUID;
import java.util.Map;
import java.util.HashMap;

public class Logger 
{
    // Static root logger instance
    private static Logger rootLogger = null;
    // Static hash already created loggers
    private static Map<String, Logger> loggers = new HashMap();

    private String name = new String();
    private boolean isRootLogger = true;

    private Logger () {}

    private Logger ( String name_ ) 
    { isRootLogger = false; name = name_; }

    private synchronized native static Logger getLog4CXXLogger ( String name );

    public synchronized static Logger getRootLogger ()
    { 
        if ( rootLogger == null )
            rootLogger = new Logger();
        return rootLogger;
    }

    public synchronized static Logger getLogger ( String name )
    {
        if ( loggers.containsKey(name) )
            return loggers.get(name);
        Logger logger = getLog4CXXLogger(name);
        loggers.put( name, logger );
        return logger;
    }

    // printing methods:
    public native void debug ( Object message );
    public native void info ( Object message );
    public native void warn ( Object message );
    public native void error ( Object message );
    public native void fatal ( Object message );

    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
