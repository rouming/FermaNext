
package fermanext.system;

class JavaPlugin {}

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
    public JavaPlugin loadPlugin ( String jarPath )
    { return new JavaPlugin(); }

    public void unloadPlugin ( JavaPlugin plugin )
    {}

    

}
