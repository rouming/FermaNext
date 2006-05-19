
package fermanext.system;

import java.util.*;
import java.util.jar.*;
import java.net.*;
import java.io.*;
import java.lang.reflect.*;

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
public class JavaPluginLoader
{
    /** Class loaders and loaded Java plugins */
    private Map<String, URLClassLoader> classLoaders = new HashMap();
    private Map<String, JavaPlugin> javaPlugins = new HashMap();


    public JavaPlugin loadPlugin ( String jarPath )
        throws PluginLoadException
    {
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
                System.out.println("man == null");
                throw new PluginLoadException();
            }
            // Manifest should contains 'Main-Class' attribute
            Attributes attrs = man.getMainAttributes();
            if ( !attrs.containsKey( Attributes.Name.MAIN_CLASS ) ) {
                System.out.println("!Main-Class");
                throw new PluginLoadException();
            }            
            javaPluginClassName = attrs.getValue( Attributes.Name.MAIN_CLASS );
        } catch ( IOException excp ) {
            System.out.println("can't open jarile");
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
            Class javaPluginClass = fermanext.system.JavaPlugin.class;
            Class loadedClass = loader.loadClass( javaPluginClassName );
            // Loaded class should extends basic Java plugin class
            if ( !javaPluginClass.isAssignableFrom(loadedClass) ) {
                System.out.println("is not a javaplugin");
                throw new PluginLoadException();
            }
            // Find constructor of this Java plugin
            Constructor javaPluginConstr = 
                loadedClass.getConstructor( String.class );
            // Create an instance of this Java plugin
            JavaPlugin javaPluginInst = 
                (JavaPlugin)javaPluginConstr.newInstance( jarPath );

            // Correct save
            classLoaders.put( jarPath, loader );
            javaPlugins.put( jarPath, javaPluginInst );
            return javaPluginInst;
            
        } catch ( MalformedURLException excp ) {
            System.out.println("MalformedURLException");
            throw new PluginLoadException();
        }
        catch ( ClassNotFoundException excp ) {
            System.out.println("ClassNotFoundException");
            throw new PluginLoadException();
        }
        catch ( InstantiationException excp ) {
            System.out.println("InstantiationException");
            throw new PluginLoadException();
        }
        catch ( IllegalAccessException excp ) {
            System.out.println("IllegalAccessException");
            throw new PluginLoadException();
        }
        catch ( NoSuchMethodException excp ) {
            System.out.println("NoSuchMethodException");
            throw new PluginLoadException();
        }
        catch ( SecurityException excp ) {
            System.out.println("SecurityException");
            throw new PluginLoadException();
        }
        catch ( InvocationTargetException excp ) {
            System.out.println("InvocationTargetException");
            throw new PluginLoadException();
        }
    }

    public void unloadPlugin ( String jarPath )
    {
        if ( !javaPlugins.containsKey(jarPath) )
            return;
        classLoaders.remove(jarPath);
        javaPlugins.remove(jarPath);
    }

    public void unloadPlugin ( JavaPlugin plugin )
    {
        String jarPath = pathToPlugin( plugin );
        if ( jarPath.length() == 0 )
            return;
        classLoaders.remove(jarPath);
        javaPlugins.remove(jarPath);
    }

    public void unloadPlugins ()
    {
        classLoaders.clear();
        javaPlugins.clear();
    }

    public Vector<JavaPlugin> loadedPlugins () 
    {
        Vector<JavaPlugin> plugins = new Vector();
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


    public static void main ( String[] args )
    {
        JavaPluginLoader loader = new JavaPluginLoader();
        try { 
            JavaPlugin javaPlugin = loader.loadPlugin( args[0] );
            System.out.println( "Plugin loaded >>>>" );
            System.out.println( "Path: " + javaPlugin.pluginPath() );
            System.out.println( "Name: " + javaPlugin.pluginInfo().name );
            System.out.println( "Desc: " + javaPlugin.pluginInfo().description);
            System.out.println( "Type: " + javaPlugin.pluginInfo().type );
        } catch ( PluginLoadException excp ) {
            System.out.println("Can't load plugin");
        }
    }
}
