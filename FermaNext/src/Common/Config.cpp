
#include <QDomNamedNodeMap>
#include <QStringList>
#include <QFileInfo>
#include <QTextStream>

#include "Config.h"

/*****************************************************************************
 * Config::Node
 *****************************************************************************/

Config::Node::Node () :
    cfg(0),
    parent(0),
    removedFlag(false),
    fullyParsed(false)
{}

Config::Node::Node ( Config& config_, const QDomElement& xmlData_ ) :
    cfg(&config_),
    parent(0),
    xmlData(xmlData_),
    removedFlag(false),
    fullyParsed(false)
{}

Config::Node::Node ( Node& parent_, const QString& tagName ) :
    cfg(parent_.cfg),
    parent(&parent_),
    removedFlag(false),
    fullyParsed(false)
{
    cfg->lockBeforeChange();
    xmlData = cfg->configDoc.createElement( tagName );
    parent->xmlData.appendChild( xmlData );
    cfg->nodeHasBeenCreated( *this );
}

Config::Node::Node ( Node& parent_, const QDomElement& xmlData_ ) :
    cfg(parent_.cfg),
    parent(&parent_),
    xmlData(xmlData_),
    removedFlag(false),
    fullyParsed(false)
{}

Config::Node::Node ( const Node& node ) :
    cfg( node.cfg ),
    parent( node.parent ),
    xmlData( node.xmlData ),
    childs( node.childs ),
    removedFlag( node.removedFlag ),
    fullyParsed( node.fullyParsed )
{}

Config::Node& Config::Node::operator= ( const Config::Node& node )
{
    cfg = node.cfg;
    parent = node.parent;
    xmlData = node.xmlData;
    childs = node.childs;
    removedFlag = node.removedFlag;
    fullyParsed = node.fullyParsed;
    return *this;
}

Config* Config::Node::config () const
{ return cfg; }

Config::Node* Config::Node::parentNode () const
{ return parent;  }

void Config::Node::remove ()
{ 
    if ( parent == 0 )
        return;    
    cfg->lockBeforeChange();
    parent->xmlData.removeChild( xmlData );
    // Notify about changes
    cfg->nodeHasBeenRemoved( *this );    
    removedFlag = true;
}

bool Config::Node::isRemoved () const
{ return removedFlag; }

bool Config::Node::isNull () const
{ return cfg == 0; }

QString Config::Node::getTagName () const
{ return xmlData.tagName(); }

void Config::Node::setTagName ( const QString& tagName )
{
    cfg->lockBeforeChange();
    xmlData.setTagName( tagName );
    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
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
    cfg->lockBeforeChange();
    xmlData.setAttribute( attr.first, attr.second );
    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
    return true;
}

bool Config::Node::removeAttribute ( const QString& attrName )
{
    if ( ! xmlData.hasAttribute( attrName ) )
        return false;
    cfg->lockBeforeChange();
    xmlData.removeAttribute( attrName );
    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
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

    cfg->lockBeforeChange();

    for ( i = 0; i < size; ++i )        
        xmlData.removeAttribute( attrNames.at(i) );

    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
}

Config::Node Config::Node::createChildNode ( const QString& tagName )
{
    // Suspended parse
    if ( !fullyParsed )
        parse();

    Config::Node node( *this, tagName );
    childs.append( node );
    return node;
}

bool Config::Node::removeChildNode ( const QString& tagName )
{
    // Suspended parse
    if ( !fullyParsed )
        parse();
    QList<Config::Node>::Iterator iter = childs.begin();
    for ( ; iter != childs.end(); ++iter ) {
        Config::Node& node = *iter;
        if ( node.getTagName() == tagName ) {
            node.remove();
            iter = childs.erase(iter);
            return true;
        }
    }
    return false;
}

void Config::Node::removeAllChildNodes ( const QString& tagName )
{
    // Suspended parse
    if ( !fullyParsed )
        parse();
    QList<Config::Node>::Iterator iter = childs.begin();
    while (  iter != childs.end() ) {
        Config::Node& node = *iter;
        if ( node.getTagName() == tagName ) {
            node.remove();
            iter = childs.erase(iter);
        } else
            ++iter;
    }            
}

QList<Config::Node> Config::Node::findChildNodes ( 
    const QString& tagName ) const
{
    // Suspended parse
    if ( !fullyParsed )
        parse();
    QList<Config::Node> result;
    QList<Config::Node>::ConstIterator iter = childs.begin();
    for ( ; iter < childs.end(); ++iter ) {
        const Config::Node& node = *iter;
        if ( node.getTagName() == tagName )
            result.append( node );            
    }    
    return result;
}

QList<Config::Node> Config::Node::childNodes () const
{ 
    // Suspended parse
    if ( !fullyParsed )
        parse();
    return childs; 
}

void Config::Node::parse () const
{
    Config::Node* self = const_cast<Config::Node*>(this);
    childs.clear();
    QDomNode n = xmlData.firstChild();
    while( !n.isNull() ) {
        if ( n.isElement() ) {
            QDomElement element = n.toElement();
            childs.append( Config::Node( *self, element ) );
        }
        n = n.nextSibling();
    }
    fullyParsed = true;
}

/*****************************************************************************
 * Config
 *****************************************************************************/

Config::HashInstances Config::configInstances;
QMutex Config::instanceMutex;
QMutex Config::notificationMutex;

/*****************************************************************************/

Config& Config::instance ( const QString& fileName )
    /*throw (OpenConfigException)*/
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
        catch ( ... ) {
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
    /*throw (OpenConfigException)*/ :
    configFile( fileName )
{
    if ( !configFile.open( QIODevice::ReadWrite ) )
        throw OpenConfigException();

    QDomElement rootElement;

    if ( !configDoc.setContent( &configFile ) ) {
        // Clear all doc
        configDoc.clear();
        // Create default header
        QDomNode xmlInstr = configDoc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF8\"") );
        // First line as XML header
        configDoc.insertBefore( xmlInstr, QDomNode() );
        // Create root element
        rootElement = configDoc.createElement( "Configuration" );
        configDoc.appendChild(rootElement);
    } else {
        // Parsed successfully. Take root element.
        rootElement = configDoc.documentElement();
    }    
    rootConfigNode = Config::Node( *this, rootElement );
}

Config::~Config ()
{}

Config::Node Config::rootNode () const
{ return rootConfigNode; }

void Config::lockBeforeChange ()
{ notificationMutex.lock(); }

void Config::flush ()
{
    configFile.resize(0);
    QTextStream stream( &configFile );
    configDoc.save( stream, 4 );
}

void Config::nodeHasBeenChanged ( const Config::Node& node )
{
    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeChanged( *this, node );
    // We should unlock previously locked mutex
    notificationMutex.unlock();
}


void Config::nodeHasBeenCreated ( const Config::Node& node )
{
    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeCreated( *this, node );
    // We should unlock previously locked mutex
    notificationMutex.unlock();
}
    
void Config::nodeHasBeenRemoved ( const Config::Node& node )
{
    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeRemoved( *this, node );
    // We should unlock previously locked mutex
    notificationMutex.unlock();
}

/*****************************************************************************/
