
#include "Common.h"

//////////////////////////////////////////////////////
// Common Native loader
//////////////////////////////////////////////////////

CommonNativeLoaderTest::CommonNativeLoaderTest ( PluginManager& mng ) : 
    PluginLoader(mng)
{}

CommonNativeLoaderTest::~CommonNativeLoaderTest () 
{}

bool CommonNativeLoaderTest::dynamicCastTest ( Plugin& plg ) const
{
    bool cast_is_ok = true;
    try { dynamic_cast<NativePlugin&>(plg); }
    // we can't catch `std::bad_cast` for Windows. 
    // On Windows `bad_cast` is thrown which is not similar to `std::bad_cast`
    catch ( std::exception& ) { cast_is_ok = false; }
    return cast_is_ok;
}

bool CommonNativeLoaderTest::exceptionCatchTest ( Plugin& plg ) const
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

bool CommonNativeLoaderTest::singletonInstanceTest ()
{
    return &PluginManager::instance() == &pluginManager();
}

//////////////////////////////////////////////////////
// Common Native plugin
//////////////////////////////////////////////////////

CommonNativePluginTest::CommonNativePluginTest ( 
    PluginManager& mng, 
    const QString& path ) :
    NativePlugin( mng, path )
{}

CommonNativePluginTest::~CommonNativePluginTest () 
{}

void CommonNativePluginTest::throwStatusAsException () 
    //throw (Plugin::Status)
{ throw OkStatus; }
