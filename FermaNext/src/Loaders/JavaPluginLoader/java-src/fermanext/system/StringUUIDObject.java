
package fermanext.system;

import java.util.UUID;

public class StringUUIDObject implements UUIDObject
{
    /** Constructor */
    public StringUUIDObject ( String data )
    { 
        setData(data);
        nativeRegistration();
    }

    /** Native registration */
    private final native void nativeRegistration ();    

    /** Returns data */
    public String getData ()
    { return m_dataStr; }

    /** Sets data */
    public void setData ( String data )
    { m_dataStr = data; }

    /** Returns UUID, which is set by native side. */
    public final String getUUID ()
    { return m_uuid; }

    /** Sets UUID. Must be called from native side. */
    public final void setUUID ( String uuid )
    { m_uuid = uuid; }

// Private Members

    /** 
     * Plugin random uuid. 
     * It is not valid by default. 
     * Should be set from native side. 
     * @see getUUID
     * @see setUUID
     */
    private String m_uuid = UUID.randomUUID().toString();

    /** Data string */
    private String m_dataStr = new String();


    static { 
        System.loadLibrary("JavaPluginLoader.ldr");
    }
}
