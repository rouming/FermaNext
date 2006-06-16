
package fermanext.system;

import java.util.UUID;

public abstract class PluginArgument
{
    /** Random UUID */
    private String uuid = UUID.randomUUID().toString();

    /** 
     * This method should check if argument is valid and can be natively casted
     * to instance, from which it was created 
     */
    public abstract boolean isValid ();

    /**
     * UUID getter.
     * @return uuid
     */
    public final String getUUID ()
    { return uuid; }

    /**
     * UUID setter.
     * @param uuid of the argument
     */
    public final void setUUID ( String uuid )
    { this.uuid = uuid; }
}
