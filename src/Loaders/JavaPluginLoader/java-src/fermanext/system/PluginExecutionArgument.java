
package fermanext.system;

public interface PluginExecutionArgument
{
    /** 
     * This method should check if argument is valid and can be natively casted
     * to instance, from which it was created 
     */
    public boolean isValid ();    
}
