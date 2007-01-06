
package fermanext.system;

import java.awt.Frame;
import java.util.*;
import java.util.jar.*;
import java.net.*;
import java.io.*;
import java.lang.reflect.*;
import fermanext.logging.log4cxx.*;

class PluginLoadException extends Exception {}

/**
 * Class for verifying and loading FermaNext Java plugins
 * from specified path. Every Java plugin should be a JAR 
 * file (even this plugin is only a single class), then 
 * JAR should contain Manifest file with "Main-Class:" 
 * signature (`main` method of this class can be skipped).
 * 
 * TODO: add further description.
 */
public final class JavaPluginLoader
{
    /** Loader logger */
    private static Logger logger = 
        Logger.getLogger("java.loader.JavaPluginLoader");
    
    /** Plugin manager */
    private PluginManager plgManager = null;
    /** Loader path */
    private String loaderPath = null;

    /** Class loaders and loaded Java plugins */
    private Map<String, URLClassLoader> classLoaders = 
        new HashMap<String, URLClassLoader>();
    private Map<String, JavaPlugin> javaPlugins = 
        new HashMap<String, JavaPlugin>();


    /** 
     * Natively creates java instance and associates it with native 
     * java plugin loader instance
     */        
    public static synchronized native JavaPluginLoader instance ( String id );

    /** Private constructor. Use #instance instead */
    private JavaPluginLoader ( PluginManager plgMng, String ldrPath )
    {
        plgManager = plgMng;
        loaderPath = ldrPath;
    }

    /**
     * Correctly disposes all frames and allows the
     * application to exit cleanly. Call this method 
     * before stopping JVM.
     */
    public void disposeAllFrames ()
    {
        logger.debug( "disposeAllFrames()" );
        Frame frames[] = Frame.getFrames();
        for ( int i = 0; i < frames.length; ++i )
            frames[i].dispose();
    }

    public String pluginLoaderPath ()
    { return loaderPath; }

    public PluginManager pluginManager ()
    { return plgManager; }

    public JavaPlugin loadPlugin ( String jarPath )
        throws PluginLoadException
    {
        logger.debug( "loadPlugin: " + jarPath );

        File file = new File( jarPath );

        // Change path to absolute
        // TODO: do we need this conversion? 
        // jarPath = file.getAbsolutePath();

        if ( javaPlugins.containsKey(jarPath) )
            return javaPlugins.get(jarPath);

        String javaPluginClassName = null;
        try { 
            JarFile jarFile = new JarFile( file );
            Manifest man = jarFile.getManifest();
            // Jar file should contain Manifest
            if ( man == null ) {
                logger.warn( "Can't find Manifest file in " + jarPath );
                throw new PluginLoadException();
            }
            // Manifest should contains 'Main-Class' attribute
            Attributes attrs = man.getMainAttributes();
            if ( !attrs.containsKey( Attributes.Name.MAIN_CLASS ) ) {
                logger.warn( "Can't find 'Main-Class' attribute in Manifest" );
                throw new PluginLoadException();
            }            
            javaPluginClassName = attrs.getValue( Attributes.Name.MAIN_CLASS );
        } catch ( IOException excp ) {
            logger.warn( "Can't open JAR file" + jarPath );
            throw new PluginLoadException();
        }

        try {
            URL url = null;
            // Create correct url
            if ( !jarPath.startsWith("file:///") )
                url = new URL("file:///" + jarPath);
            else
                url = new URL(jarPath);

            URLClassLoader loader = new URLClassLoader( new URL[]{ url } );
            Class<fermanext.system.JavaPlugin> javaPluginClass = 
                fermanext.system.JavaPlugin.class;
            Class loadedClass = loader.loadClass( javaPluginClassName );
            // Loaded class should extends basic Java plugin class
            if ( !javaPluginClass.isAssignableFrom(loadedClass) ) {
                logger.warn( "Class " + javaPluginClassName  + 
                             " is not a child of JavaPlugin: " + jarPath );
                throw new PluginLoadException();
            }
            // Find constructor of this Java plugin
            Constructor javaPluginConstr = 
                loadedClass.getConstructor( PluginManager.class,
                                            String.class );
            // Create an instance of this Java plugin
            JavaPlugin javaPluginInst = 
                (JavaPlugin)javaPluginConstr.newInstance( pluginManager(),
                                                          jarPath );

            logger.debug( "javaPluginInst.pluginPath " +
                          javaPluginInst.pluginPath() );

            // Correct save
            classLoaders.put( jarPath, loader );
            javaPlugins.put( jarPath, javaPluginInst );
            return javaPluginInst;
            
        } catch ( MalformedURLException excp ) {
            logger.warn( "Can't find Jar file, URL is wrong: " + jarPath );
            throw new PluginLoadException();
        }
        catch ( ClassNotFoundException excp ) {
            logger.warn( "Can't find Main-Class '" + javaPluginClassName +  
                         "' in " + jarPath );
            throw new PluginLoadException();
        }
        catch ( InstantiationException excp ) {
            logger.warn( "Can't find Main-Class '" + javaPluginClassName +  
                         "' in " + jarPath );
            throw new PluginLoadException();
        }
        catch ( IllegalAccessException excp ) {
            logger.warn( "Main-Class '" + javaPluginClassName +  
                         "' in " + jarPath + " is abstract" );
            throw new PluginLoadException();
        }
        catch ( NoSuchMethodException excp ) {
            logger.warn( "Can't find constructor of Main-Class '" + 
                         javaPluginClassName + "' in " + jarPath );
            throw new PluginLoadException();
        }
        catch ( InvocationTargetException excp ) {
            logger.warn( "Constructor of Main-Class '" + 
                         javaPluginClassName + "' in " + jarPath + 
                         " throws an exception" );
            throw new PluginLoadException();
        }
    }

    public void unloadPlugin ( String jarPath )
    {
        logger.debug( "unloadPlugin(String): " + jarPath );
        if ( !javaPlugins.containsKey(jarPath) ) {
            logger.warn( "Can't find '" + jarPath + "' to unload" );
            return;
        }
        classLoaders.remove(jarPath);
        javaPlugins.remove(jarPath);
    }

    public void unloadPlugin ( JavaPlugin plugin )
    {
        String jarPath = plugin.pluginPath();
        logger.debug( "unloadPlugin(JavaPlugin): " + jarPath );
        if ( jarPath.length() == 0 ) {
            logger.warn( "Can't find '" + jarPath + "' to unload" );
            return;
        }
        classLoaders.remove(jarPath);
        javaPlugins.remove(jarPath);
    }

    public void unloadPlugins ()
    {
        logger.debug( "unloadPlugins" );
        classLoaders.clear();
        javaPlugins.clear();
    }

    public Vector<JavaPlugin> loadedPlugins () 
    {
        Vector<JavaPlugin> plugins = new Vector<JavaPlugin>();
        Iterator<Map.Entry<String, JavaPlugin>> it = 
            javaPlugins.entrySet().iterator();
        while ( it.hasNext() ) {
            Map.Entry<String, JavaPlugin> entry = it.next();
            plugins.add( entry.getValue() );
        }

        return plugins;
    }

    public boolean pluginIsLoaded ( JavaPlugin plugin )
    {
        String jarPath = pathToPlugin( plugin );
        if ( jarPath.length() == 0 )
            return false;
        return true;
    }

    public int countLoadedPlugins ()
    {
        return loadedPlugins().size();
    }

    public String pathToPlugin ( JavaPlugin plugin )
    {
        Iterator<Map.Entry<String, JavaPlugin>> it = 
            javaPlugins.entrySet().iterator();
        while ( it.hasNext() ) {
            Map.Entry<String, JavaPlugin> entry = it.next();
            if ( entry.getValue() == plugin ) 
                return entry.getKey();            
        }        
        return new String();
    }
}
