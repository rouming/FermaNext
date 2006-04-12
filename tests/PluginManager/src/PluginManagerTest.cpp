
#include <QString>

#include "Common.h"

class PluginManagerTest : public PluginManager
{
public:
    PluginManagerTest () :
        passed(0), failed(0)
    {}

    void my_assert ( bool val, const QString& str )
    {
        if ( val ) ++passed;
        else ++failed;    
        std::cout << (val ? "    OK: " : "NOT OK: ") << qPrintable(str) << "\n"; 
    }

    void lastReport ()
    {
        std::cout << "\n=================>>>\n" << 
                     "Passed: " << passed << "\n" << 
                     "Failed: " << failed << "\n";
    }

    void testCaseBegin ( const QString& testCaseName ) 
    {
        std::cout <<  "=========== " << qPrintable(testCaseName) << 
                      " ==========\n";
    }

    void testCaseEnd () 
    {
        std::cout << "=================================\n\n";
    }

    uint status ()
    { return failed; }


    //////////////////////////////////////////////////////////////////////////
    // TEST CASES
    //////////////////////////////////////////////////////////////////////////

    void rttiBehaviourAcrossDynamicLibraries ()
    {
        testCaseBegin("rttiBehaviourAcrossDynamicLibraries");

        PluginLoaderList loaders = pluginLoaders();
        PluginLoader& loader = *loaders.at(0);
        PluginList plugins = loader.loadedPlugins();
        Plugin& plugin = *plugins.at(0);

        try {
            CommonNativeLoaderTest& nativeLoader = 
                dynamic_cast<CommonNativeLoaderTest&>(loader);

             bool dynCastRes   = nativeLoader.dynamicCastTest( plugin );
             bool excpCatchRes = nativeLoader.exceptionCatchTest( plugin );
             bool singletonRes = nativeLoader.singletonInstanceTest();

#if defined _WIN32 || defined WIN32
             QString dynCastMsg( "Dynamic cast" );
             QString excpCatchMsg( "Exception catch" );
             QString singletonMsg( "Single instance " );
             if ( !singletonRes ) {
                 singletonMsg += "WARNING!!!!\nSingleton doesn't work "
                                 "properly!!!\nYour should build all "
                                 "plugins and loaders classes in dll\n"
                                 "and export them with __declspec(dllexport) "
                                 "declaration";
                 singletonRes = true;
             }
#else
             QString dynCastMsg( "Dynamic cast (executable should be "
                                 "built with '-export-dynamic' ld param)" );
             QString excpCatchMsg( "Exception catch (executable should be "
                                   "built with '-export-dynamic' ld param)" );
             QString singletonMsg( "Single instance (every shared object "
                                   "should be loaded with "
                                   "'RTLD_GLOBAL' param)" );
#endif
             my_assert( dynCastRes, dynCastMsg );
             my_assert( excpCatchRes, excpCatchMsg );
             my_assert( singletonRes, singletonMsg );
        } 
        // we can't catch `std::bad_cast` for Windows. 
        // On Windows `bad_cast` is thrown which is not 
        // similar to `std::bad_cast`
        catch ( std::exception& ) { my_assert( 0, "Bad cast!" ); }
        
        testCaseEnd();
    }

    void registerLoadersTest ()
    {
        testCaseBegin("registerLoadersTest");

        registerPluginLoaders( "loaders" );
        my_assert( pluginLoaders().size() == 1, 
                "One native plugin loader" );

        testCaseEnd();
    }

    void unregisterLoadersTest ()
    {
        testCaseBegin("unregisterLoadersTest");

        unregisterPluginLoaders();
        my_assert( pluginLoaders().size() == 0, 
                "Zero plugin loaders after unregister" );
        my_assert( loadedPlugins().size() == 0, 
                "Zero plugins after unload" );

        testCaseEnd();
    }

    void loadPluginsTest ()
    {
        testCaseBegin("loadPluginsTest");

        loadPlugins( "plugins" );
        my_assert( loadedPlugins().size() == 1, "One native plugin" );

        testCaseEnd();
    }

    void unloadPluginsTest ()
    {
        testCaseBegin("unloadPluginsTest");

        unloadPlugins();
        my_assert( pluginLoaders().size() == 1, 
                "One native plugin loader" );
        my_assert( loadedPlugins().size() == 0, 
               "Zero plugins after unload" );

        testCaseEnd();
    }


    void resolvingDependenciesTest ()
    {
        testCaseBegin("resolvingDependenciesTest");
        my_assert( false, "Not implemented!" );
        testCaseEnd();
    }

    void loadingUnloadingPluginsTest ()
    {
        testCaseBegin("loadingUnloadingPluginsTest");
        my_assert( false, "Not implemented!" );
        testCaseEnd();
    }
    
    void loadedPluginsByTypeTest ()
    {
        testCaseBegin("loadedPluginsByTypeTest");
        my_assert( false, "Not implemented!" );
        testCaseEnd();
    }

private:
    uint passed;
    uint failed;
};

int main ()
{
    PluginManagerTest test;
    test.registerLoadersTest();
    test.loadPluginsTest();

    test.rttiBehaviourAcrossDynamicLibraries();

    test.unloadPluginsTest();
    test.unregisterLoadersTest();

    /*
    test.resolvingDependenciesTest();
    test.loadingUnloadingPluginsTest();
    test.loadedPluginsByTypeTest();
    */

    test.lastReport();
   

    return test.status();
}
