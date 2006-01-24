
#include "DynaLoader.h"
#include <qfile.h>
#include <qregexp.h>
#include <qdir.h>
#include <qfileinfo.h>

/*****************************************************************************
 * Dynamic Loader
 *****************************************************************************/

const QString& DynaLoader::libExtension ()
{
#if defined _WIN32 || defined WIN32
    static QString extension = "dll";
#else
    static QString extension = "so";
#endif
    return extension;
}

const QString& DynaLoader::libPrefix ()
{
#if defined _WIN32 || defined WIN32
    static QString prefix = "";
#else
    static QString prefix = "lib";
#endif
    return prefix;
}

/*****************************************************************************/

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
    QFileInfo dlInfo( fileName );
    QString dlFileName = dlInfo.fileName();

    QString convertedFileName = fileName;
    QRegExp regExp( "^" + libPrefix() + ".+\\." + libExtension() + "$" );
    if ( -1 == regExp.search( dlFileName ) ) {
        dlFileName = libPrefix() + dlFileName + "." + libExtension();
        convertedFileName = dlInfo.dirPath() + QDir::separator() + dlFileName;
    }
    convertedFileName = QDir::convertSeparators( convertedFileName );
#if defined UNICODE && (defined _WIN32 || defined WIN32)
    handle = dl_open(convertedFileName.ucs2());
#else
    handle = dl_open(QFile::encodeName(convertedFileName));
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
