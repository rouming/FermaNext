
#include "DynaLoader.h"

#include <QFile>
#include <QRegExp>
#include <QDir>
#include <QFileInfo>

QString dl_error ()
{
#if defined _WIN32 || defined WIN32
    static const uint BUFFER_SIZE = 100;
    static char buffer[BUFFER_SIZE + 1];
    FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | 
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    0, //source
                    GetLastError(),
                    0, //language
                    buffer,
                    BUFFER_SIZE,
                    0 //args 
                  );
    return QString( buffer );
#else
    return QString( dlerror() );
#endif
}

/*****************************************************************************
 * Dynamic Loader
 *****************************************************************************/

const QString& DynaLoader::libExtension ()
{
#ifdef _WIN_
    static QString extension = "dll";
#elif _LIN_
    static QString extension = "so";
#elif _MAC_
    static QString extension = "dylib";
#else
    #error("Unknown compiler!");
#endif
    return extension;
}

const QString& DynaLoader::libPrefix ()
{
#ifdef _WIN_
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
    /*throw (DynaLoader::LibraryLoadException)*/ :
    handle(0)
{
    loadLibrary( fileName );
}

DynaLoader::~DynaLoader ()
{
    freeLibrary();
}

void DynaLoader::loadLibrary ( const QString& fileName ) 
    /*throw (DynaLoader::LibraryLoadException)*/
{
    freeLibrary();    
    QFileInfo dlInfo( fileName );
    QString dlFileName = dlInfo.fileName();

    QString convertedFileName = fileName;
//TODO: should load MAC frameworks with out .dyld postfix specified
#ifndef _MAC_
    QRegExp regExp( "^" + libPrefix() + ".+\\." + libExtension() + "$" );
    if ( -1 == regExp.indexIn( dlFileName ) ) {
        dlFileName = libPrefix() + dlFileName + "." + libExtension();
        convertedFileName = dlInfo.absolutePath() + QDir::separator() +
                            dlFileName;
    }
#endif
    convertedFileName = QDir::convertSeparators( convertedFileName );
#if defined UNICODE && (defined _WIN32 || defined WIN32)
    handle = dl_open(convertedFileName.toStdWString().data());
#else
    handle = dl_open(QFile::encodeName(convertedFileName));
#endif
    if ( handle == 0 )
        throw LibraryLoadException( dl_error() );
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
    /*throw (DynaLoader::AddressException)*/
{
    ProcAddress address = dl_sym(handle, funcName.toAscii().data());
    if ( address == 0 )
        throw AddressException( dl_error() );
    return address;
}

bool DynaLoader::isLoaded () const
{
    return handle != 0;
}

/*****************************************************************************/
