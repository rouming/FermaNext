
#include "PluginLoader.h"
#include "PluginLoaderFrontEnd.h"
#include "NativePluginFrontEnd.h"
#include "NativePlugin.h"
#include "PluginManager.h"
#include "DynaLoader.h"

#include <iostream>

class PrivateSingleInstance;

//////////////////////////////////////////////////////
// Common Native plugin 
//////////////////////////////////////////////////////
class CommonNativePluginTest : public NativePlugin
{
public:
    CommonNativePluginTest ( PluginManager& mng, const QString& path ) :
        NativePlugin( mng, path )
    {}

    virtual ~CommonNativePluginTest () {}

    virtual PrivateSingleInstance& getPrivateSingleInstance () const = 0;
    virtual void throwStatusAsException () = 0;
        //throw(Plugin::Status)
};

//////////////////////////////////////////////////////
// Common Native loader
//////////////////////////////////////////////////////
class CommonNativeLoaderTest : public PluginLoader
{
public:
    CommonNativeLoaderTest ( PluginManager& mng, const QString& path ) :
        PluginLoader(mng, path) 
    {}
    virtual ~CommonNativeLoaderTest () {}

    virtual bool dynamicCastTest ( Plugin& plg ) const = 0;
    virtual bool exceptionCatchTest ( Plugin& plg ) const = 0;
    virtual bool singletonInstanceTest () = 0;

private:
    CommonNativeLoaderTest ( const CommonNativeLoaderTest& );
    CommonNativeLoaderTest& operator= ( const CommonNativeLoaderTest& );
};

