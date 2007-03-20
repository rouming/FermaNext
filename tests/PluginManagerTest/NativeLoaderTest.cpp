
#include "Common.h"
#include "PrivateCommon.h"

class NativeLoaderTest : public CommonNativeLoaderTest
{
public:
    NativeLoaderTest ( PluginManager& mng, const QString& path ) : 
        CommonNativeLoaderTest(mng, path)
    {}
    virtual ~NativeLoaderTest () {}
    virtual const QString& pluginExtension () const 
    { return DynaLoader::libExtension(); }
    virtual Status pluginLoaderStatusCode () const { return OkStatus; }
    virtual QString pluginLoaderStatusMsg () const { return ""; }

protected:
    virtual Plugin& specificLoadPlugin ( const QString& pathToPlugin )
        throw (PluginLoadException)
    {
        DynaLoader* dynaLoader = new DynaLoader;
        PluginInstanceInitCall pluginInstanceInitCall = 0;
        try {
            dynaLoader->loadLibrary( pathToPlugin );
            pluginInstanceInitCall = (PluginInstanceInitCall)
                dynaLoader->getProcAddress( PLUGIN_INSTANCE_INIT_CALL );
        }
        catch ( DynaLoader::LibraryLoadException& ) {
            delete dynaLoader;
            std::cout << "DynaLoader::LibraryLoadException\n";
            throw PluginLoadException();
        }
        catch ( DynaLoader::AddressException& ) {
            delete dynaLoader;
            std::cout << "DynaLoader::AddressException\n";
            throw PluginLoadException();
        }

        NativePlugin* plugin = 
            pluginInstanceInitCall( pluginManager(), pathToPlugin );
        if ( plugin == 0 ) {
            delete dynaLoader;
            std::cout << "is 0\n";
            throw PluginLoadException();
        }
        std::cout << "NativeLoader: plugin loaded " << plugin  << "\n";

        return *plugin;
    }

    virtual void specificUnloadPlugin ( Plugin& plugin )
    {
        std::cout << "NativeLoader: plugin unloaded " << &plugin  << "\n";
    }

    virtual void specificUnloadPlugins () {}


///////////////////////////////////////////////////////////////////////////////
//  inherited from CommonNativeLoaderTest
///////////////////////////////////////////////////////////////////////////////

    bool dynamicCastTest ( Plugin& plg ) const
    {
        bool cast_is_ok = true;
        try { dynamic_cast<NativePlugin&>(plg); }
        // we can't catch `std::bad_cast` for Windows. 
        // On Windows `bad_cast` is thrown which is not similar to `std::bad_cast`
        catch ( std::exception& ) { cast_is_ok = false; }
        return cast_is_ok;
    }

    bool exceptionCatchTest ( Plugin& plg ) const
    {
        bool expected_exception = false;
        try { 
            CommonNativePluginTest& nativePlg = 
                dynamic_cast<CommonNativePluginTest&>(plg);
            // This method should throw exception
            nativePlg.throwStatusAsException();
        }
        catch ( Plugin::Status& ) 
            { expected_exception = true; }
        catch ( ... ) { /* unexpected exception */ } 
        return expected_exception;
    }

    bool singletonInstanceTest ()
    {
        PluginList plugins = loadedPlugins();
        if ( plugins.size() == 0 ) {
            qWarning( "plugins.size() == 0" );
            return false;
        }

        // Get first plugin
        Plugin& plg = *plugins.at(0);

        try { 
            CommonNativePluginTest& nativePlg = 
                dynamic_cast<CommonNativePluginTest&>(plg);
        
            return &PrivateSingleInstance::instance() == 
                   &nativePlg.getPrivateSingleInstance();
        }
        catch ( ... ) {
            qWarning( "Bad cast from 'Plugin' to 'CommonNativePluginTest'" );
            return false;            
        } 

        qWarning( "Unreachable line" );
        return false;
    }

private:
    NativeLoaderTest ( const NativeLoaderTest& );
    NativeLoaderTest& operator= ( const NativeLoaderTest& );
};

// plugin loader export
FERMA_NEXT_PLUGIN_LOADER(NativeLoaderTest, (PluginManager::LoadingPriority)0)
