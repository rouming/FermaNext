
#include "DynaLoader.h"

/*****************************************************************************
 * Dynamic Loader
 *****************************************************************************/

#if defined WINDOWS || defined WIN32
    const QString DynaLoader::LibExtension = "dll";
#else
    const QString DynaLoader::LibExtension = "so";
#endif

DynaLoader::DynaLoader () :
    handle(0)
{}

DynaLoader::DynaLoader ( const QString& fileName )
    throw (LibraryLoadException) :
    handle(0)
{
    loadLibrary( fileName );
}

DynaLoader::~DynaLoader ()
{
    freeLibrary();
}

void DynaLoader::loadLibrary ( const QString& fileName ) 
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

bool DynaLoader::freeLibrary ()
{
    if ( handle ) {
        bool res = dl_close(handle);
        handle = 0;
        return res;
    }
    return false;
}

ProcAddress DynaLoader::getProcAddress ( const QString& funcName ) const
    throw (AddressException)
{
    ProcAddress address = dl_sym(handle, funcName.ascii());
    if ( address == 0 )
        throw AddressException();
    return address;
}

bool DynaLoader::isLoaded () const
{
    return handle != 0;
}

/*****************************************************************************/
