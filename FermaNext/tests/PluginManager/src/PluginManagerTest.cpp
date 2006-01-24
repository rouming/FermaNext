
#include <iostream>
#include <qstring.h>

#include "Common.h"

class PluginManagerTest
{
public:
    PluginManagerTest () :
        passed(0), failed(0)
    {}

    void assert ( bool val, const QString& str )
    {
        if ( val ) ++passed;
        else ++failed;    
        std::cout << (val ? "    OK: " : "NOT OK: ") << str.ascii() << "\n"; 
    }

    void lastReport ()
    {
        std::cout << "\n=================>>>\n" << 
                     "Passed: " << passed << "\n" << 
                     "Failed: " << failed << "\n";
    }

    void testCaseBegin ( const QString& testCaseName ) 
    {
        std::cout <<  "=========== " << testCaseName.ascii() << 
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

        PluginManager& plgMng = PluginManager::instance();
        PluginLoaderList loaders = plgMng.pluginLoaders();
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
             assert( dynCastRes, dynCastMsg );
             assert( excpCatchRes, excpCatchMsg );
             assert( singletonRes, singletonMsg );
        } 
        // we can't catch `std::bad_cast` for Windows. 
        // On Windows `bad_cast` is thrown which is not 
        // similar to `std::bad_cast`
        catch ( std::exception& ) { assert( 0, "Bad cast!" ); }
        
        testCaseEnd();
    }

    void registerLoadersTest ()
    {
        testCaseBegin("registerLoadersTest");

        PluginManager& plgMng = PluginManager::instance();
        plgMng.registerPluginLoaders( "loaders" );
        assert( plgMng.countPluginLoaders() == 1, 
                "One native plugin loader" );

        testCaseEnd();
    }

    void unregisterLoadersTest ()
    {
        testCaseBegin("unregisterLoadersTest");

        PluginManager& plgMng = PluginManager::instance();
        plgMng.unregisterPluginLoaders();
        assert( plgMng.countPluginLoaders() == 0, 
                "Zero plugin loaders after unregister" );
        assert( plgMng.loadedPlugins().size() == 0, 
                "Zero plugins after unload" );

        testCaseEnd();
    }

    void loadPluginsTest ()
    {
        testCaseBegin("loadPluginsTest");

        PluginManager& plgMng = PluginManager::instance();
        plgMng.loadPlugins( "plugins" );
        assert( plgMng.loadedPlugins().size() == 1, "One native plugin" );

        testCaseEnd();
    }

    void unloadPluginsTest ()
    {
        testCaseBegin("unloadPluginsTest");

        PluginManager& plgMng = PluginManager::instance();
        plgMng.unloadPlugins();
        assert( plgMng.countPluginLoaders() == 1, 
                "One native plugin loader" );
        assert( plgMng.loadedPlugins().size() == 0, 
               "Zero plugins after unload" );

        testCaseEnd();
    }


    void resolvingDependenciesTest ()
    {
        testCaseBegin("resolvingDependenciesTest");
        assert( false, "Not implemented!" );
        testCaseEnd();
    }

    void loadingUnloadingPluginsTest ()
    {
        testCaseBegin("loadingUnloadingPluginsTest");
        assert( false, "Not implemented!" );
        testCaseEnd();
    }
    
    void loadedPluginsByTypeTest ()
    {
        testCaseBegin("loadedPluginsByTypeTest");
        assert( false, "Not implemented!" );
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
