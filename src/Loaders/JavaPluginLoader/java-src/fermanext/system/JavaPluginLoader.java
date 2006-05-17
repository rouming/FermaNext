
package fermanext.system;

import java.util.*;
import java.util.jar.*;
import java.net.*;
import java.io.*;

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
        if ( javaPlugins.containsKey(jarPath) )
            return javaPlugins.get(jarPath);

        String javaPluginClassName = null;
        try { 
            JarFile jarFile = new JarFile( new File(jarPath) );
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
            // Create correct url
            if ( !jarPath.startsWith("file:///") )
                jarPath = "file:///" + jarPath;
            URL url = new URL(jarPath);                
            URLClassLoader loader = new URLClassLoader( new URL[]{ url } );
            Class javaPluginClass = fermanext.system.JavaPlugin.class;
            Class loadedClass = loader.loadClass( javaPluginClassName );
            // Loaded class should extends basic Java plugin class
            if ( !javaPluginClass.isAssignableFrom(loadedClass) ) {
                System.out.println("is not a javaplugin");
                throw new PluginLoadException();
            }
            JavaPlugin javaPluginInst = (JavaPlugin)loadedClass.newInstance();
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
    }

    public void unloadPlugin ( String jarPath )
    {
        if ( !javaPlugins.containsKey(jarPath) )
            return;
        classLoaders.remove(jarPath);
        javaPlugins.remove(jarPath);
    }


    public static void main ( String[] args )
    {
        JavaPluginLoader loader = new JavaPluginLoader();
        try { 
            loader.loadPlugin( args[0] );
        } catch ( PluginLoadException excp )
            {}
    }
}
