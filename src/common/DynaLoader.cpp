
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
    if ( ! loadLibrary( fileName ) )
        throw LibraryLoadException();
}

DynaLoader::~DynaLoader ()
{
    freeLibrary();
}

bool DynaLoader::loadLibrary ( const QString& fileName )
{
    freeLibrary();
#if defined UNICODE && (defined WINDOWS || defined WIN32)
    handle = dl_open(fileName.ucs2());
#else
    handle = dl_open(QFile::encodeName(fileName));
#endif
    return handle == 0 ? false : true;
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

ProcessAddress DynaLoader::getAddressHandler ( const QString& funcName )
    throw (AddressException)
{
    ProcessAddress address = dl_sym(handle, funcName.ascii());
    if ( address == 0 )
        throw AddressException();
    return address;
}

/*****************************************************************************/
