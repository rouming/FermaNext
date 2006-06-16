
package fermanext.system;

import java.util.UUID;
import java.util.Vector;
import fermanext.logging.log4cxx.*;

public abstract class JavaPlugin
{
    /** Plugin logger */
    private static Logger logger = Logger.getLogger("java.plugin.JavaPlugin");

    /** Status codes */
    public enum Status {
        OkStatus,
        InternalErrStatus,
        UnresolvedDependStatus
    }

    /** Resolving mode. */
    public enum ResolvingMode {
        Immediately,
        OnDeman
    }

    /** Path to JAR file */
    private String path = new String();

    public JavaPlugin ( String pluginPath )
    { path = pluginPath; }

    /**
     * Returns plugin path of a library, from which it was loaded.
     * @return plugin path as a string.
     */
    public final String pluginPath ()
    { return path; }

    /**
     * This is a main execute method, which is called from native side.
     * @see specificExecute
     */
    public final void execute ( String[] uuids )
    {
        logger.debug( "execute" );

        Vector<Class<? extends PluginArgument>> classes = executionTypes();

        if ( uuids.length != classes.size() )
            return;

        Vector<PluginArgument> args = new Vector<PluginArgument>();
        for ( int i = 0; i < classes.size(); ++i ) {
            try { 
                PluginArgument obj = classes.get(i).newInstance();
                obj.setUUID( uuids[i] );
                if ( !obj.isValid() ) {
                    logger.warn( "Can't create argument [" + i + "], object "+
                                 "is not valid: " + uuids[i] );
                    return;
                }
                // Append newcly created wrapper to args vector
                args.add( obj );
                logger.info( "Argument [" + i + "] is valid: " + uuids[i] );
            } catch ( java.lang.InstantiationException e ) {
                    logger.warn( "Can't create argument [" + i + "], argument "
                                 + "type is abstract: " + uuids[i] );
                return;
            } catch ( java.lang.IllegalAccessException e ) {
                    logger.warn( "Can't create argument [" + i + "], " +
                                 "constructor is protected or private: "
                                 + uuids[i] );
                return;
            }
        }
        specificExecute( args );
    }

    /**
     * Executes plugin with specified arguments.
     * Should be implemented in your own child class.
     * @see execute
     */
    public abstract void specificExecute ( Vector<PluginArgument> args );

    /**
     * Returns execution types for this plugin.
     * Should be implemented in child classes.
     * @see execute
     * @see specificExecute
     */
    public abstract Vector<Class<? extends PluginArgument>> executionTypes ();

    /** 
     * Describes itself.
     * @return plugin info
     */
    public abstract PluginInfo pluginInfo ();

    /** 
     * Describes plugin state by status code.
     * @return plugin status code
     * @see pluginStatusMsg for detailed information about status.
     */
    public abstract Status pluginStatusCode ();

    /** 
     * Describes plugin state by status msg. 
     * (e.g. describes the sort of internal error if it has been occured)
     * @return plugin status str msg. Returns empty string if status is Ok
     * @see pluginStatusCode
     */
    public abstract String pluginStatusMsg ();
}
