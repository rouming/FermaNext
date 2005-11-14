
#include "DynaLoader.h"
#include <qfile.h>
#include <qregexp.h>
#include <qdir.h>
#include <qfileinfo.h>

/*****************************************************************************
 * Dynamic Loader
 *****************************************************************************/

#if defined WINDOWS || defined WIN32
    const QString DynaLoader::LibExtension = "dll";
    const QString DynaLoader::LibPrefix    = "";
#else
    const QString DynaLoader::LibExtension = "so";
    const QString DynaLoader::LibPrefix    = "lib";
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

void DynaLoader::loadLibrary ( QString fileName ) 
    throw (LibraryLoadException)
{
    static QRegExp regExp( "^" + LibPrefix + ".+\\." + LibExtension + "$" );
    freeLibrary();
    QFileInfo dlInfo( fileName );
    QString dlFileName = dlInfo.fileName();
    if ( -1 == regExp.search( dlFileName ) ) {
        dlFileName = LibPrefix + dlFileName + "." + LibExtension;
        fileName = dlInfo.dirPath() + QDir::separator() + dlFileName;
    }
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
