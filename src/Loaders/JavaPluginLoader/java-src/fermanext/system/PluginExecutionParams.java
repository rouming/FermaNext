
package fermanext.system;

import java.util.HashMap;
import java.util.Map;

import fermanext.system.Variant;

public final class PluginExecutionParams
{
    /** Adds param. */
    public void addParam ( String name, Variant value )
    { params.put( name, value );  }

    /** Removes params. */
    public void removeParam ( String name )
    { params.remove( name ); }

    /** Returns whole map of params */
    public Map<String, Variant> getParams ()
    { return params; }

//Private members:
    private HashMap<String, Variant> params = new HashMap<String, Variant>();
}
