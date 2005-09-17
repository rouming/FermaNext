
#include "PluginLoader.h"

/*****************************************************************************
 * Dynamic Plugin Loader
 *****************************************************************************/

#include <qfile.h>

PluginLoader::PluginLoader () :
    handle(0)
{}

PluginLoader::PluginLoader ( const QString& fileName )
    throw (LibraryLoadException) :
    handle(0)
{
    loadLibrary( fileName );
}

PluginLoader::~PluginLoader ()
{
    freeLibrary();
}

const QString& PluginLoader::libExtension ()
{
#if defined WINDOWS || defined WIN32
    static QString extension = "dll";
#else
    static QString extension = "so";
#endif
    return extension;
}

void PluginLoader::loadLibrary ( const QString& fileName ) 
    throw (LibraryLoadException)
{
    freeLibrary();
#if defined UNICODE && (defined WINDOWS || defined WIN32)
    handle = dl_open(fileName.ucs2());
#else
    handle = dl_open(QFile::encodeName(fileName));
#endif
    if ( handle == 0 )
        throw LibraryLoadException();
}

bool PluginLoader::freeLibrary ()
{
    if ( handle ) {
        bool res = dl_close(handle);
        handle = 0;
        return res;
    }
    return false;
}

ProcAddress PluginLoader::getProcAddress ( const QString& funcName ) const
    throw (AddressException)
{
    ProcAddress address = dl_sym(handle, funcName.ascii());
    if ( address == 0 )
        throw AddressException();
    return address;
}

bool PluginLoader::isLoaded () const
{
    return handle != 0;
}

/*****************************************************************************/
