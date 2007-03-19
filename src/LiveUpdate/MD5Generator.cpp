
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDomElement>

#include "md5.h"
#include "MD5Generator.h"
#include "Global.h"

/*****************************************************************************
 * Helpers
 *****************************************************************************/

static void md5ForFiles ( QDomDocument& doc, QDomElement& parent,
                          QFileInfo& fileInfo, const QStringList& except,
                          bool recursion, bool rootCall )
{
    if ( ! fileInfo.exists() )
        return;

    // Skip if found
    if ( -1 != except.indexOf(fileInfo.absoluteFilePath()) )
        return;

    // Directory
    if ( fileInfo.isDir() ) {
        QDomElement elem;
        // If root call do not create root directory element
        if ( ! rootCall ) {
            elem = doc.createElement( "Directory" );
            elem.setAttribute( "name", fileInfo.fileName() );
            parent.appendChild( elem );
        }
        else
            elem = parent;

        QDir::Filters filters =  QDir::NoDotAndDotDot | QDir::Readable;
        QDir dir( fileInfo.filePath() );

        if ( ! recursion ) {
            QFileInfoList dirList = dir.entryInfoList( filters | QDir::Dirs, 
                                                QDir::DirsFirst | QDir::Name );
            foreach ( QFileInfo dirInfo, dirList ) {
                QDomElement dirElem = doc.createElement( "Directory" );
                dirElem.setAttribute( "name", dirInfo.fileName() );
                elem.appendChild( dirElem );                
            }            
            filters |= QDir::Files;
        }
        else
            filters |= QDir::AllEntries;

        QFileInfoList infoList = dir.entryInfoList( filters, QDir::DirsFirst | 
                                                             QDir::Name );

        foreach ( QFileInfo fInfo, infoList )
            ::md5ForFiles( doc, elem, fInfo, except, recursion, false );
    }
    // File
    else if ( fileInfo.isFile() ) {
        QDomElement elem = doc.createElement( "File" );
        elem.setAttribute( "name", fileInfo.fileName() );
        
        QFile file( fileInfo.absoluteFilePath() );
        if ( ! file.open(QIODevice::ReadOnly) )
            return;

        QString md5Sum = ::qtMD5( file.readAll() );
        elem.setAttribute( "md5", md5Sum );

        QFile::Permissions perms = file.permissions();
        if ( perms.testFlag(QFile::ExeOwner) )
            elem.setAttribute( "permissions", "executable" );

        parent.appendChild( elem );
    }
}

/*****************************************************************************
 * md5 Generator
 *****************************************************************************/

QDomDocument MD5Generator::md5ForFiles ( const QString& path, 
                                         const QStringList& except,
                                         bool recursion )
{
    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF-8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );

    QDomElement md5Header = doc.createElement( "MD5" );
    md5Header.setAttribute( "recursion", (recursion ? "on" : "off") );
    md5Header.setAttribute( "appVersion", Global::applicationVersionNumber() );
    doc.appendChild( md5Header );

    QFileInfo fInfo( path );

    QStringList exceptList = except;
    for ( int i = 0; i < exceptList.size(); ++i ) {
        QString& exceptPath = exceptList[i];
        QFileInfo fi(exceptPath);
        exceptPath = fi.absoluteFilePath();
    }

    ::md5ForFiles( doc, md5Header, fInfo, exceptList, recursion, true );

    return doc;
}

/*****************************************************************************/
