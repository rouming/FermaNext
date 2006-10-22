
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

    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const
    { /* nothing */ }

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

private:
    ExecutionResult specificExecute ( 
                              const PluginExecutionParams&,
                              const QList<UUIDObject*>&,
                              const QHash< QString, QList<Plugin*> >& )
    { return ExecutionResult( OkStatus, QString() ); }
};

// plugin export
FERMA_NEXT_PLUGIN(NativePluginTest)
