
#include "PluginLoader.h"
#include "PluginLoaderFrontEnd.h"
#include "NativePluginFrontEnd.h"
#include "NativePlugin.h"
#include "PluginManager.h"
#include "DynaLoader.h"

#include <iostream>

//////////////////////////////////////////////////////
// Common Native plugin 
//////////////////////////////////////////////////////
class CommonNativePluginTest : public NativePlugin
{
public:
    CommonNativePluginTest ( PluginManager& mng, const QString& path );

    virtual ~CommonNativePluginTest ();

    virtual void throwStatusAsException ();
        //throw(Plugin::Status)
};

//////////////////////////////////////////////////////
// Common Native loader
//////////////////////////////////////////////////////
class CommonNativeLoaderTest : public PluginLoader
{
public:
    CommonNativeLoaderTest ( PluginManager& mng );
    virtual ~CommonNativeLoaderTest ();

    virtual bool dynamicCastTest ( Plugin& plg ) const;
    virtual bool exceptionCatchTest ( Plugin& plg ) const;
    virtual bool singletonInstanceTest ();

private:
    CommonNativeLoaderTest ( const CommonNativeLoaderTest& );
    CommonNativeLoaderTest& operator= ( const CommonNativeLoaderTest& );
};

