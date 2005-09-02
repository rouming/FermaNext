
#include "DynaLoader.h"

/*****************************************************************************
 * Dynamic Loader
 *****************************************************************************/

#include <qfile.h>

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

const QString& DynaLoader::libExtension ()
{
#if defined WINDOWS || defined WIN32
    static QString extension = "dll";
#else
    static QString extension = "so";
#endif
    return extension;
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

/*****************************************************************************/
