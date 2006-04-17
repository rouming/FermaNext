
#include <QDomNamedNodeMap>
#include <QStringList>
#include <QFileInfo>

#include "Config.h"

/*****************************************************************************
 * Config::Node
 *****************************************************************************/

Config::Node::Node ( Config& config_, const QString& tagName ) :
    cfg(config_),
    parent(0),
    removedFlag(false)
{
    // There is no any need to lock data, this is a root node
    xmlData = cfg.configDoc.createElement( tagName );
}

Config::Node::Node ( Node& parent_, const QString& tagName ) :
    cfg(parent_.cfg),
    parent(&parent_),
    removedFlag(false)
{
    cfg.lockBeforeChange();
    xmlData = cfg.configDoc.createElement( tagName );
    cfg.nodeHasBeenCreated( *this );
}

Config::Node::Node ( const Node& node ) :
    cfg( node.cfg ),
    parent( node.parent ),
    xmlData( node.xmlData ),
    removedFlag( node.removedFlag )
{}

Config::Node& Config::Node::operator= ( const Config::Node& node )
{
    cfg = node.cfg;
    parent = node.parent;
    xmlData = node.xmlData;
    removedFlag = node.removedFlag;
    return *this;
}

const Config& Config::Node::config () const
{ return cfg; }

Config& Config::Node::config ()
{ return cfg; }

Config::Node* Config::Node::parentNode () const
{ return parent;  }

void Config::Node::remove ()
{ 
    if ( parent == 0 )
        return;    
    cfg.lockBeforeChange();
    parent->xmlData.removeChild( xmlData );
    // Notify about changes
    cfg.nodeHasBeenRemoved( *this );    
    removedFlag = true;
}

bool Config::Node::isRemoved () const
{ return removedFlag; }

QString Config::Node::getTagName () const
{ return xmlData.tagName(); }

void Config::Node::setTagName ( const QString& tagName )
{
    cfg.lockBeforeChange();
    xmlData.setTagName( tagName );
    // Notify about changes
    cfg.nodeHasBeenChanged( *this );
}

NodeAttributeList Config::Node::getAttributes () const
{
    NodeAttributeList resultList;
    QDomNamedNodeMap attributes = xmlData.attributes();
    int size = attributes.size();
    for ( int i = 0; i < size; ++i ) {
        QDomNode node = attributes.item(i);
        if ( ! node.isAttr() )
            continue;
        QDomAttr attr = node.toAttr();
        resultList.append( NodeAttribute(attr.name(), attr.value()) );
    }    
    return resultList;
}

bool Config::Node::addAttribute ( const NodeAttribute& attr )
{
    if ( xmlData.hasAttribute( attr.first ) )
        return false;
    cfg.lockBeforeChange();
    xmlData.setAttribute( attr.first, attr.second );
    // Notify about changes
    cfg.nodeHasBeenChanged( *this );
    return true;
}

bool Config::Node::removeAttribute ( const QString& attrName )
{
    if ( ! xmlData.hasAttribute( attrName ) )
        return false;
    cfg.lockBeforeChange();
    xmlData.removeAttribute( attrName );
    // Notify about changes
    cfg.nodeHasBeenChanged( *this );
    return true;
}

void Config::Node::clearAttributes ()
{
    QStringList attrNames;
    QDomNamedNodeMap attributes = xmlData.attributes();
    int size = attributes.size();
    int i = 0;
    for ( i = 0; i < size; ++i )
        attrNames.append( attributes.item(i).toAttr().name() );

    cfg.lockBeforeChange();

    for ( i = 0; i < size; ++i )        
        xmlData.removeAttribute( attrNames.at(i) );

    // Notify about changes
    cfg.nodeHasBeenChanged( *this );
}

Config::Node Config::Node::createChildNode ( const QString& tagName )
{
    return Node( *this, tagName );
}

/*
bool Config::Node::removeAllChildNodes ( const QString& tagName )
{
}


        QList<Config::Node> Config::Node::findChildNodes ( const QString& tagName ) const;

        QList<Config::Node> Config::Node::childNodes () const;

*/
/*****************************************************************************
 * Config
 *****************************************************************************/

Config::HashInstances Config::configInstances;
QMutex Config::instanceMutex;
QMutex Config::notificationMutex;

/*****************************************************************************/

Config& Config::instance ( const QString& fileName )
    /*throw (OpenFileException)*/
{
    instanceMutex.lock();
    QFileInfo fileInfo( fileName );
    QString absFilePath = fileInfo.absoluteFilePath();
    Config* cfgInstance = 0;
    if ( ! configInstances.contains( absFilePath ) ) {
        try { 
            cfgInstance = new Config( absFilePath );
            configInstances[ absFilePath ] = cfgInstance;
        }
        catch ( OpenFileException& ) {
            instanceMutex.unlock();
            throw;
        }

    } else {
        cfgInstance = configInstances[ absFilePath ];
    }
    instanceMutex.unlock();
    return *cfgInstance;
}

Config::Config ( const QString& fileName )
    /*throw (OpenFileException)*/ :
    configFile( fileName ),
    rootConfigNode(0)
{
    if ( !configFile.open( QIODevice::ReadWrite ) )
        throw OpenFileException();

    rootConfigNode = new Config::Node( *this, "Configuration" );
}

Config::~Config ()
{
    delete rootConfigNode;
}

Config::Node Config::rootNode () const
{ return *rootConfigNode; }

void Config::lockBeforeChange ()
{
    notificationMutex.lock();
}

void Config::nodeHasBeenChanged ( const Config::Node& node )
{

    ///
    /// Some code
    /// 

    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeChanged( *this, node );
    notificationMutex.unlock();
}


void Config::nodeHasBeenCreated ( const Config::Node& node )
{

    ///
    /// Some code
    /// 

    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeCreated( *this, node );
    notificationMutex.unlock();
}
    
void Config::nodeHasBeenRemoved ( const Config::Node& node )
{

    ///
    /// Some code
    /// 

    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeRemoved( *this, node );
    notificationMutex.unlock();
}

/*****************************************************************************/
