
package fermanext.system;

import java.util.*;
import java.io.*;
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
    public final ExecutionResult execute ( String[] uuids )
    {
        logger.debug( "execute" );

        if ( uuids == null || uuids.length == 0 ) {
            // FIXME: throw exception instead returning smth
            logger.warn( "Uuids array is null or empty." );
            
            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }

        Method[] methods = getClass().getDeclaredMethods();
        
        Method specificExecuteMethod = null;
        for ( Method method : methods ) {
            if ( method.getName() == "specificExecute" ) {
                if ( specificExecuteMethod == null )
                    specificExecuteMethod = method;
                else {
                    // FIXME: throw exception instead returning smth
                    logger.warn( "'specificExecute' method is declared " + 
                                 "several times, should be declared once." );
                    
                    return new ExecutionResult( 
                                         Plugin.Status.InternalErrStatus, "" );
                }
            }
        }

        if ( specificExecuteMethod == null ) {
            // FIXME: throw exception instead returning smth
            logger.warn( "'specificExecute' is not declared." );

            
            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }

        Class<?> retType = specificExecuteMethod.getReturnType();
        if ( Plugin.ExecutionResult.class != retType ) {
            logger.warn( "Return type '" + 
                         retType.getName() +
                         "' of 'specificExecute' is wrong. Must be '" +
                         Plugin.ExecutionResult.class.getName() + "'" );
            // FIXME: throw exception instead returning smth

            
            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }

        Class<?>[] classes = specificExecuteMethod.getParameterTypes();
        if ( uuids.length != classes.length ) {
            logger.warn( "Parameters number " + classes.length + 
                         " doesn't equal to number of uuids " + uuids.length );
            // FIXME: throw exception instead returning smth

            
            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }

        Class<fermanext.system.NativeObject> nativeObjClass = 
                fermanext.system.NativeObject.class;
        
        for ( Class<?> paramClass : classes ) {
            if ( ! nativeObjClass.isAssignableFrom(paramClass) ) {
                logger.warn( "Parameter class '" + paramClass.getName() + 
                             "' doesn't extend '" +  
                             nativeObjClass.getName() + "'" );
                // FIXME: throw exception instead returning smth

                
                return new ExecutionResult( Plugin.Status.InternalErrStatus, 
                                            "" );
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

                    // FIXME: throw exception instead returning smth
                    return new ExecutionResult( 
                                         Plugin.Status.InternalErrStatus, "" );
                }

                obj.setUUID( uuids[i] );
                if ( !obj.isValid() ) {
                    logger.warn( "Can't create argument [" + i + "] of " +
                                 "class '"+  classes[i].getName() + 
                                 "', object "+ "is not valid: " + uuids[i] );
                    // FIXME: throw exception instead returning smth
                    return new ExecutionResult( 
                                         Plugin.Status.InternalErrStatus, "" );

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
                // FIXME: throw exception instead returning smth

                
                return new ExecutionResult( 
                                         Plugin.Status.InternalErrStatus, "" );

            } 
            catch ( java.lang.IllegalAccessException e ) {
                logger.warn( "Can't create argument [" + i + "] of class '" +
                             classes[i].getName() + "', constructor is " +
                             "inaccessible: " + uuids[i] );
                // FIXME: throw exception instead returning smth

                
                return new ExecutionResult( 
                                         Plugin.Status.InternalErrStatus, "" );
            }

        }

        Plugin.ExecutionResult execResults = null;
        
        // Invoke execution method
        try { 
            Object objRes = specificExecuteMethod.invoke(this, args);
            execResults = Plugin.ExecutionResult.class.cast( objRes );  
        }
        catch (  IllegalAccessException e  ) {
            logger.warn( "Can't invoke method '" + 
                         specificExecuteMethod.getName() + "', method is " +
                         "inaccessible." );
            // FIXME: throw exception instead returning smth

            
            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }
        catch ( IllegalArgumentException e ) {
            logger.warn( "Can't invoke method '" + 
                         specificExecuteMethod.getName() + "', parameters " +
                         "differ." );
            // FIXME: throw exception instead returning smth

            
            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }
        catch ( InvocationTargetException e ) {
            StringWriter sw = new StringWriter();
            e.printStackTrace( new PrintWriter(sw) );
            String stackTrace = sw.toString();

            // TODO: in future execution method can throw an exception.

            logger.warn( "Can't invoke method '" + 
                         specificExecuteMethod.getName() + "', method throws "+
                         "an exception:\n" + stackTrace );
            // FIXME: throw exception instead returning smth

            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }
        catch ( ClassCastException e ) {
            logger.warn( "Can't cast return object to execution result" );
            // FIXME: throw exception instead returning smth

            return new ExecutionResult( Plugin.Status.InternalErrStatus, "" );
        }

        return execResults;
    }

    /**
     * This is a main execute method.
     * @see execute
     */
    public final ExecutionResult execute ( UUIDObject[] args )
    {
        String[] uuids = new String[args.length];
        for ( int i = 0; i < uuids.length; ++i )
            uuids[i] = args[i].getUUID();
        return execute( uuids );
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
