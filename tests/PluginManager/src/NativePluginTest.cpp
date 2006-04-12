
#include "Common.h"
#include "PrivateCommon.h"

class NativePluginTest : public CommonNativePluginTest
{
public:
    NativePluginTest ( PluginManager& mng, 
                 const QString& path ) :
       CommonNativePluginTest( mng, path )
    {}

    virtual ~NativePluginTest () {}

    virtual const PluginInfo& pluginInfo () const
    {
        static PluginInfo inf( "NativeTest",
                               "NativeTest",
                               "native.test.test" );
        return inf; 
    }

    virtual Status pluginStatusCode () const 
    { return OkStatus;  }

    virtual QString pluginStatusMsg () const 
    { return "";  }

///////////////////////////////////////////////////////////////////////////////
//  inherited from CommonNativePluginTest
///////////////////////////////////////////////////////////////////////////////

    PrivateSingleInstance& getPrivateSingleInstance () const
    {
        return PrivateSingleInstance::instance();
    }

    void throwStatusAsException () 
        //throw (Plugin::Status)
    { throw OkStatus; }
};

// plugin export
FERMA_NEXT_PLUGIN(NativePluginTest)
