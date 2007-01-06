
package fermanext.system;

import java.util.*;
import java.io.*;
import java.lang.reflect.*;
import fermanext.logging.log4cxx.*;

public abstract class JavaPlugin extends Plugin
{
    /** Plugin logger */
    private static Logger logger = Logger.getLogger("java.plugin.JavaPlugin");

    public JavaPlugin ( PluginManager pluginMng, String pluginPath )
    { super(pluginMng, pluginPath); }

    /**
     * This is a main execute method, which is called from native side.
     * This method searches for 'specificExecute' method, which should
     * be declated in derived class, then takes formal parameters of 
     * found method and invokes it with created args.
     */
    private final ExecutionResult nativeCall_specificExecute ( 
                                             PluginExecutionParams params,
                                             String[] uuids,
                                             Hashtable<String, Plugin[]> deps )
        throws ContextIsNotValidException,
               ParamsAreNotAcceptedException,
               DependenciesAreNotResolvedException,
               WrongExecutionArgsException
    {
        logger.debug( "nativeCall_execute(String[])" );

        /**************
         * Check uuids
         **************/
        if ( uuids == null ) {
            logger.warn( "Uuids array is null." );
            throw new WrongExecutionArgsException(getUUID());
        }

        /*********************************
         * Count execution argument types
         *********************************/
        Class<?>[] classes = executionArgumentTypes();

        if ( uuids.length != classes.length ) {
            logger.warn( "Execution argument types number " + classes.length + 
                         " doesn't equal to number of uuids " + uuids.length );
            throw new WrongExecutionArgsException(getUUID());
        }

        /*********************************************************************
         * Every execution argument type should be assignable from NativeObject
         *********************************************************************/
        Class<fermanext.system.NativeObject> nativeObjClass = 
                fermanext.system.NativeObject.class;

        for ( Class<?> paramClass : classes ) {
            if ( ! nativeObjClass.isAssignableFrom(paramClass) ) {
                logger.warn( "Execution argument type '" + 
                             paramClass.getName() + "' doesn't extend '" +  
                             nativeObjClass.getName() + "'" );
                throw new WrongExecutionArgsException(getUUID());
            }
        }

        /********************************
         * Create java wrappers by uuids
         ********************************/
        UUIDObject[] args = new UUIDObject[ classes.length ];
        for ( int i = 0; i < classes.length; ++i ) {
            try {
                NativeObject obj = 
                    nativeObjClass.cast( classes[i].newInstance() );
                if ( obj == null ) {
                    logger.warn( "Can't cast newly created instance of param "+
                                 "type '" +  classes[i].getName() + 
                                 "' to '" + nativeObjClass.getName() +
                                 "'" );
                    throw new WrongExecutionArgsException(getUUID());
                }

                // Set uuid
                obj.setUUID( uuids[i] );
                // Do native cast
                if ( !obj.isValid() ) {
                    logger.warn( "Can't create argument [" + i + "] of " +
                                 "class '"+  classes[i].getName() + 
                                 "', object "+ "is not valid: " + uuids[i] );
                    throw new WrongExecutionArgsException(getUUID());
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
                throw new WrongExecutionArgsException(getUUID());
            } 
            catch ( java.lang.IllegalAccessException e ) {
                logger.warn( "Can't create argument [" + i + "] of class '" +
                             classes[i].getName() + "', constructor is " +
                             "inaccessible: " + uuids[i] );
                throw new WrongExecutionArgsException(getUUID());
            }

        }

        // Invoking specific execute method
        return specificExecute( params, args, deps );
    }

    /** 
     * Main entry point to execute plugin. This method is 
     * private because it can be called only from #execute method. 
     * You should override #specificExecute in child classes.
     * @param params of plugin.
     * @param args execution arguments.
     * @param deps plugin dependencies.
     * @return result of executed operation
     * @throw ContextIsNotValidException occurs when this plugin execute
     *        its dependence and there smth has gone wrong.
     * @throw ParamsAreNotAcceptedException occurs when this plugin execute
     *        its dependence and there smth has gone wrong.
     * @throw WrongExecutionArgsException, 
     *        occurs when contract (argument list) between calling side
     *        and plugin is wrong. 
     * @throw DependenciesAreNotResolvedException, 
     *        occurs when dependencies are not resolved.
     * @see execute
     * @see executionArgumentTypes
     */    
    protected abstract ExecutionResult specificExecute ( 
                                           PluginExecutionParams params,
                                           UUIDObject[] args,
                                           Hashtable<String, Plugin[]> deps  )
        throws ContextIsNotValidException,
               ParamsAreNotAcceptedException,
               DependenciesAreNotResolvedException,
               WrongExecutionArgsException;


    /**
     * Returns classes of required execution arguments.
     * This method is used to dynamically create wrappers for required
     * types by their uuids.
     * @see nativeCall_specificExecute
     * @see specificExecute
     * @return classes of execution arguments
     */
    protected abstract Class<?>[] executionArgumentTypes ();
}
