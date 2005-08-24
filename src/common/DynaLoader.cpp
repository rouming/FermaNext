
#include "DynaLoader.h"

/*****************************************************************************
 * Dynamic Loader
 *****************************************************************************/

DynaLoader::DynaLoader () :
    handle(0)
{}

DynaLoader::DynaLoader ( const QString& fileName ) 
                                            throw (LibraryLoadException)
{
    loadLibrary( fileName );
}

DynaLoader::~DynaLoader ()
{
}

void DynaLoader::loadLibrary ( const QString& fileName )
                                            throw (LibraryLoadException)
{
    handle = dl_open((const unsigned short*)"fileName.ascii()");
    if ( handle == 0 )
        throw LibraryLoadException();
}

bool DynaLoader::freeLibrary ()
{
    return dl_close(handle);
}

ProcessAddress DynaLoader::getAddressHandler ( const QString& funcName )
                                            throw (AddressException)
{
	ProcessAddress adress = dl_sym(handle, (const unsigned short*)"fileName.ascii()");
    if ( address == 0 )
        throw AddressException();
}

/*****************************************************************************/
