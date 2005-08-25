
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
}

bool DynaLoader::loadLibrary ( const QString& fileName )
{
    freeLibrary();
#if defined WINDOWS || defined WIN32  
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
#if defined WINDOWS || defined WIN32  
    ProcessAddress address = dl_sym(handle, funcName.ucs2());
#else
    ProcessAddress address = dl_sym(handle, QFile::encodeName(funcName));
#endif
    if ( address == 0 )
        throw AddressException();
    return address;
}

/*****************************************************************************/
