
package fermanext.system;

import java.util.UUID;
import java.util.Vector;
import java.lang.reflect.*;
import fermanext.logging.log4cxx.*;

public abstract class JavaPlugin extends Plugin
{
    /** Plugin logger */
    private static Logger logger = Logger.getLogger("java.plugin.JavaPlugin");

    public JavaPlugin ( String pluginPath )
    { super(pluginPath); }


    /**
     * This is a main execute method, which is called from native side.
     * This method searches for 'specificExecute' method, which should
     * be declated in derived class, then takes formal parameters of 
     * found method and invokes it with created args.
     */
    public final void execute ( String[] uuids )
    {
        logger.debug( "execute" );

        if ( uuids == null || uuids.length == 0 ) {
            // TODO: throw exception
            logger.warn( "Uuids array is null or empty." );
            return;
        }

        Method[] methods = getClass().getDeclaredMethods();
        
        Method specificExecuteMethod = null;
        for ( Method method : methods ) {
            if ( method.getName() == "specificExecute" ) {
                if ( specificExecuteMethod == null )
                    specificExecuteMethod = method;
                else {
                    // TODO: throw exception
                    logger.warn( "'specificExecute' method is declared " + 
                                 "several times, should be declared once." );
                    return;
                }
            }
        }

        if ( specificExecuteMethod == null ) {
            // TODO: throw exception
            logger.warn( "'specificExecute' is not declared." );
            return;        
        }

        Class<?>[] classes = specificExecuteMethod.getParameterTypes();
        if ( uuids.length != classes.length ) {
            logger.warn( "Parameters number " + classes.length + 
                         " doesn't equal to number of uuids " + uuids.length );
            // TODO: throw exception
            return;
        }

        Class<fermanext.system.NativeObject> nativeObjClass = 
                fermanext.system.NativeObject.class;
        
        for ( Class<?> paramClass : classes ) {
            if ( ! nativeObjClass.isAssignableFrom(paramClass) ) {
                logger.warn( "Parameter class '" + paramClass.getName() + 
                             "' doesn't extend '" +  
                             nativeObjClass.getName() + "'" );
                // TODO: throw exception
                return;
            }
        }
            

        Object[] args = new Object[ classes.length ];
        for ( int i = 0; i < classes.length; ++i ) {
            try {
                NativeObject obj = 
                    nativeObjClass.cast( classes[i].newInstance() );
                if ( obj == null ) {
                    logger.warn( "Can't cast newly created instance of param "+
                                 "type '" +  classes[i].getName() + 
                                 "' to '" + nativeObjClass.getName() +
                                 "'" );
                    // TODO: throw exception
                    return;                    
                }

                obj.setUUID( uuids[i] );
                if ( !obj.isValid() ) {
                    logger.warn( "Can't create argument [" + i + "] of " +
                                 "class '"+  classes[i].getName() + 
                                 "', object "+ "is not valid: " + uuids[i] );
                    // TODO: throw exception
                    return;
                }
                // Save newly created wrapper to args array
                args[i] = obj;
                logger.info( "Argument [" + i + "] of class '" + 
                             classes[i].getName() +"' is valid: " + uuids[i] );
            }
            catch ( java.lang.InstantiationException e ) {
                logger.warn( "Can't create argument [" + i + "] of class '" +
                             classes[i].getName() + "', argument type " +
                             "is abstract: " + uuids[i] );
                // TODO: throw exception
                return;
            } 
            catch ( java.lang.IllegalAccessException e ) {
                logger.warn( "Can't create argument [" + i + "] of class '" +
                             classes[i].getName() + "', constructor is " +
                             "inaccessible: " + uuids[i] );
                // TODO: throw exception
                return;
            }

        }
        
        // Invoke execution method
        try { specificExecuteMethod.invoke(this, args); }
        catch (  IllegalAccessException e  ) {
            logger.warn( "Can't invoke method '" + 
                         specificExecuteMethod.getName() + "', method is " +
                         "inaccessible." );
            // TODO: throw exception
            return;
        }
        catch ( IllegalArgumentException e ) {
            logger.warn( "Can't invoke method '" + 
                         specificExecuteMethod.getName() + "', parameters " +
                         "differ." );
            // TODO: throw exception
            return;
        }
        catch ( InvocationTargetException e ) {

            // TODO: in future execution method can throw an exception.

            logger.warn( "Can't invoke method '" + 
                         specificExecuteMethod.getName() + "', method throws "+
                         "exception." );
            // TODO: throw exception
            return;
        }
    }

    /**
     * This is a main execute method.
     * @see execute
     */
    public final void execute ( UUIDObject[] args )
    {
        String[] uuids = new String[args.length];
        for ( int i = 0; i < uuids.length; ++i )
            uuids[i] = args[i].getUUID();
        execute( uuids );
    }

    /**
     * Executes plugin with specified arguments.
     * Should be implemented in your derived class.
     * You should declare your own #specificExecute method 
     * with assumed arguments, #execute method will dynamically 
     * find this method, dynamically create objects of parameters 
     * types and then will invoke it. 
     * @see execute
     */
    /* public abstract void specificExecute ( ... );*/


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
