
#include <QFile>

#include "FermaNextConfig.h"

/*****************************************************************************
 * Ferma Next Config
 *****************************************************************************/

const QDomNode FermaNextConfig::nullNode;

/*****************************************************************************/

FermaNextConfig::FermaNextConfig ( const QString& f )  
    throw (ConfigReadException, ConfigWriteException) :
    fileName(f),
    configDoc("FermaNextConfig")
{
    parseConfig();
}

void FermaNextConfig::parseConfig () 
    throw (ConfigReadException, ConfigParseException)
{
    QFile file( fileName );
    if ( !file.open( QIODevice::ReadOnly ) )
        throw ConfigReadException();
    if ( !configDoc.setContent( &file ) ) {
        file.close();
        throw ConfigParseException();
    }
    file.close();
}

QDomNode FermaNextConfig::findConfigNode ( const QString& name ) const
{
    QDomElement docElem = configDoc.documentElement();
    QDomNode n = docElem.firstChild();
    while( !n.isNull() ) {
        if ( n.nodeName() == name )
            return n;
        n = n.nextSibling();
    }
    return nullNode;
}

QDomNode FermaNextConfig::findConfigNode ( const QDomNode& node, 
                                           const QString& name ) const
{
    QDomNode n = node.firstChild();
    while( !n.isNull() ) {
        if ( n.nodeName() == name )
            return n;
        n = n.nextSibling();
    }
    return nullNode;
}

ConfigItems FermaNextConfig::getConfigItems ( const QDomNode& node ) const
{
    ConfigItems items;
    if ( node.isNull() )
        return items;
    QDomNode n = node.firstChild();
    while( !n.isNull() ) {
        if ( n.hasChildNodes() ) {
            QDomNode child = n.firstChild();
            if ( !child.hasChildNodes() && child.isText() ) {
                QDomText text = child.toText();
                items[n.nodeName()] = text.data();           
           }
        }
        else {
            if ( n.isText() ) {
                QDomText text = n.toText();
                items[node.nodeName()] = text.data();
                break;
            }            
        }
        n = n.nextSibling();
    }
    return items;
}

ConfigItems FermaNextConfig::getConfigItems ( const QString& name ) const
{
    return getConfigItems( findConfigNode(name) );
}

/****************************************************************************/
