
#include <QDomNamedNodeMap>
#include <QStringList>
#include <QFileInfo>
#include <QTextStream>
#include <QMetaType>

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
    QMutexLocker locker( cfg->notificationMutex );
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
    QMutexLocker locker( cfg->notificationMutex );
    parent->xmlData.removeChild( xmlData );
    removedFlag = true;
    // Notify about changes
    cfg->nodeHasBeenRemoved( *this );
}

bool Config::Node::isRemoved () const
{ return removedFlag; }

bool Config::Node::isNull () const
{ return cfg == 0; }

QString Config::Node::getTagName () const
{ return xmlData.tagName(); }

void Config::Node::setTagName ( const QString& tagName )
{
    QMutexLocker locker( cfg->notificationMutex );
    xmlData.setTagName( tagName );
    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
}

bool Config::Node::hasValue () const
{ return !textData.isNull(); }

QString Config::Node::getValue () const
{
    if ( !xmlData.hasChildNodes() )
        return QString();
    QDomNode n = xmlData.firstChild();
    if ( n.isText() )
        return n.toText().data();
    return QString();
}

void Config::Node::setValue ( const QString& value )
{
    QMutexLocker locker( cfg->notificationMutex );

    if ( hasValue() ) {
        QDomText newTextData = cfg->configDoc.createTextNode( value );
        xmlData.replaceChild( newTextData, textData );
    } else {
        // This is not a text node, so clear all childs
        removeAllChildNodes();
        textData = cfg->configDoc.createTextNode( value );
        xmlData.appendChild( textData );
    }

    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
}

void Config::Node::resetValue ()
{
    if ( !hasValue() )
        return;

    QMutexLocker locker( cfg->notificationMutex );

    xmlData.removeChild( textData );
    textData.clear();

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
    QMutexLocker locker( cfg->notificationMutex );
    xmlData.setAttribute( attr.first, attr.second );
    // Notify about changes
    cfg->nodeHasBeenChanged( *this );
    return true;
}

bool Config::Node::removeAttribute ( const QString& attrName )
{
    if ( ! xmlData.hasAttribute( attrName ) )
        return false;
    QMutexLocker locker( cfg->notificationMutex );
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

    QMutexLocker locker( cfg->notificationMutex );

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
    else
        // We should reset value if this node has it before
        if ( hasValue() )
            resetValue();

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

void Config::Node::removeAllChildNodes ()
{
    // Suspended parse
    if ( !fullyParsed )
        parse();
    QList<Config::Node>::Iterator iter = childs.begin();
    while (  iter != childs.end() ) {
        Config::Node& node = *iter;
        node.remove();
        iter = childs.erase(iter);
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
    // We are using const cast to make this method constant,
    // as it is called from other constant methods
    Config::Node* self = const_cast<Config::Node*>(this);

    self->fullyParsed = true;
    self->childs.clear();

    bool hasValueParsed = false;
    QDomNode n = xmlData.firstChild();
    while( !n.isNull() ) {
        if ( n.isText() ) { 
            hasValueParsed = true;
            self->setValue( n.toText().data() );
        }
        n = n.nextSibling();
    }

    // There is no any child if this node has value
    if ( hasValueParsed )
        return;

    n = xmlData.firstChild();
    while( !n.isNull() ) {
        if ( n.isElement() ) {
            QDomElement element = n.toElement();
            self->childs.append( Config::Node( *self, element ) );
        }
        n = n.nextSibling();
    }
}

/*****************************************************************************
 * Config
 *****************************************************************************/

Config::HashInstances Config::configInstances;
QMutex* Config::instanceMutex = new QMutex;
QMutex* Config::notificationMutex = new QMutex( QMutex::Recursive );
ConfigNodeTypeRegistrator<Config::Node> Config::registrator;

/*****************************************************************************/

Config& Config::instance ( const QString& fileName )
    /*throw (OpenConfigException)*/
{
    QMutexLocker locker( instanceMutex );
    QFileInfo fileInfo( fileName );
    QString absFilePath = fileInfo.absoluteFilePath();
    Config* cfgInstance = 0;
    if ( ! configInstances.contains( absFilePath ) ) {
        cfgInstance = new Config( absFilePath );
        configInstances[ absFilePath ] = cfgInstance;
    } else {
        cfgInstance = configInstances[ absFilePath ];
    }
    return *cfgInstance;
}

bool Config::destroyInstance ( const QString& fileName, bool removeConfigFile )
{
    QMutexLocker locker( instanceMutex );
    QFileInfo fileInfo( fileName );
    QString absFilePath = fileInfo.absoluteFilePath();

    if ( ! configInstances.contains(absFilePath) )
        return false;

    Config* cfgInstance = configInstances[ absFilePath ];
    configInstances.remove( absFilePath );
    delete cfgInstance;

    // Remove config file on request
    if ( removeConfigFile )
        QFile::remove( absFilePath );

    return true;
}

bool Config::destroyInstance ( Config& cfg,  bool removeConfigFile )
{ return destroyInstance( cfg.configFileName(), removeConfigFile ); }

Config::Config ( const QString& fileName )
    /*throw (OpenConfigException)*/ :
    configFile( fileName )
{
    if ( !configFile.open( QIODevice::ReadWrite ) )
        throw OpenConfigException();

    qint64 fileSize = configFile.size();

    QDomElement rootElement;

    if ( fileSize == 0 ) {
        // Create default header
        QDomNode xmlInstr = configDoc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF-8\"") );
        // First line as XML header
        configDoc.insertBefore( xmlInstr, QDomNode() );
        // Create root element
        rootElement = configDoc.createElement( "Configuration" );
        configDoc.appendChild(rootElement);
        // Flushes newly created header
        flush();
    } else if ( configDoc.setContent( &configFile ) ) {
        // Parsed successfully. Take root element.
        rootElement = configDoc.documentElement();
    }
    else
        // Can't parse
        throw OpenConfigException();

    rootConfigNode = Config::Node( *this, rootElement );
}

Config::~Config ()
{ configFile.close(); }


QString Config::configFileName () const
{ return configFile.fileName(); }

Config::Node Config::rootNode () const
{ return rootConfigNode; }

void Config::lockBeforeChange ()
{ notificationMutex->lock(); }

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
    emit onNodeChanged( node );
}


void Config::nodeHasBeenCreated ( const Config::Node& node )
{
    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeCreated( node );
}
    
void Config::nodeHasBeenRemoved ( const Config::Node& node )
{
    // Everything should be flushed
    flush();
    // Notify everyone
    emit onNodeRemoved( node );
}

/*****************************************************************************/
