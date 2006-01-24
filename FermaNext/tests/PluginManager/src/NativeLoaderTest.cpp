
#include "Common.h"

class NativeLoaderTest : public CommonNativeLoaderTest
{
public:
    NativeLoaderTest ( PluginManager& mng ) : 
        CommonNativeLoaderTest(mng)
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
        PluginInstanceCall pluginInstanceCall = 0;
        try {
            dynaLoader->loadLibrary( pathToPlugin );
            pluginInstanceCall = (PluginInstanceCall)
                dynaLoader->getProcAddress( PLUGIN_INSTANCE_CALL );
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
            pluginInstanceCall( PluginManager::instance(), pathToPlugin );
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

private:
    NativeLoaderTest ( const NativeLoaderTest& );
    NativeLoaderTest& operator= ( const NativeLoaderTest& );
};

// plugin loader export
FERMA_NEXT_PLUGIN_LOADER(NativeLoaderTest, (PluginManager::LoadingPriority)0)
