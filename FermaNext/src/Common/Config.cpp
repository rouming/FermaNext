
#include <QDomNamedNodeMap>
#include <QStringList>
#include <QFileInfo>
#include <QTextStream>
#include <QMetaType>
#include <QAtomic>
#include <QMutex>

#include "Config.h"

/*****************************************************************************
 * Private config node data
 *****************************************************************************/

template <class N>
class ConfigNodePrivate
{
public:
    ConfigNodePrivate () :
        ref(1), parent(0),  cfg(0), 
        removedFlag(false),
        fullyParsed(false)
    {}
        
    QAtomic ref;
    ConfigNodePrivate<N>* parent;
    Config* cfg;
    QDomElement xmlData;
    QDomText textData;
    QList<N> childs;
    bool removedFlag;
    bool fullyParsed;
};

/*****************************************************************************
 * Config::Node
 *****************************************************************************/

Config::Node::Node () :
    data(0)
{}

Config::Node::~Node ()
{
    if ( data && !data->ref.deref() ) {
        // Zeroes parents
        ConfigNodeList::Iterator it = data->childs.begin();
        for ( ; it != data->childs.end(); ++it )
            it->data->parent = 0;            
        delete data;
    }
}

Config::Node::Node ( Config& config_, const QDomElement& xmlData_ ) :
    data( new ConfigNodePrivate<Config::Node> )
{
    data->cfg = &config_;    
    data->xmlData = xmlData_;
}

Config::Node::Node ( Node& parent_, const QString& tagName ) :
    data( new ConfigNodePrivate<Config::Node> )
{
    data->cfg = parent_.data->cfg;
    data->parent = parent_.data;

    QMutexLocker locker( data->cfg->notificationMutex );
    data->xmlData = data->cfg->configDoc.createElement( tagName );
    data->parent->xmlData.appendChild( data->xmlData );
    data->cfg->nodeHasBeenCreated( *this );
}

Config::Node::Node ( Node& parent_, const QDomElement& xmlData_ ) :
    data( new ConfigNodePrivate<Config::Node> )
{
    data->cfg = parent_.data->cfg;
    data->parent = parent_.data;
    data->xmlData = xmlData_;
}

Config::Node::Node ( ConfigNodePrivate<Config::Node>* nodeData ) :
    data( nodeData )
{
    if ( data )
        data->ref.ref();
}

Config::Node::Node ( const Node& node ) :
    data( node.data )
{
    if ( data )
        data->ref.ref();
}

Config::Node& Config::Node::operator= ( const Config::Node& node )
{
    ConfigNodePrivate<Config::Node>* p = node.data;
    if ( p )
        p->ref.ref();
    p = qAtomicSetPtr( &data, p );
    if ( p && !p->ref.deref() )
        delete p;
    return *this;
}

bool Config::Node::operator== ( const Config::Node& node )
{ return (node.data == data); }

Config* Config::Node::config () const
{ return data->cfg; }

Config::Node Config::Node::parentNode () const
{ return Config::Node( data->parent );  }

void Config::Node::remove ()
{ 
    if ( isNull() || data->parent == 0 )
        return;
    QMutexLocker locker( data->cfg->notificationMutex );
    // Remove from child
    ConfigNodeList& parentChilds = data->parent->childs;
    ConfigNodeList::Iterator it = qFind( parentChilds.begin(), 
                                         parentChilds.end(), *this );

    if ( it != parentChilds.end() )
        parentChilds.erase( it );

    // Remove xml
    data->parent->xmlData.removeChild( data->xmlData );
    // Disconnect from parent node
    data->parent = 0;
    data->removedFlag = true;
    // Notify about changes
    data->cfg->nodeHasBeenRemoved( *this );
}

void Config::Node::fromParentRemove ()
{ 
    if ( isNull() || data->parent == 0 )
        return;
    QMutexLocker locker( data->cfg->notificationMutex );
    // Remove xml
    data->parent->xmlData.removeChild( data->xmlData );
    // Disconnect from parent node
    data->parent = 0;
    data->removedFlag = true;
    // Notify about changes
    data->cfg->nodeHasBeenRemoved( *this );
}

bool Config::Node::isRemoved () const
{ 
    if ( isNull() )
        return true;
    return data->removedFlag;
}

bool Config::Node::isNull () const
{ return data == 0; }

QString Config::Node::getTagName () const
{ 
    if ( isNull() )
        return QString();
    return data->xmlData.tagName(); 
}

void Config::Node::setTagName ( const QString& tagName )
{
    if ( isNull() )
        return;
    QMutexLocker locker( data->cfg->notificationMutex );
    data->xmlData.setTagName( tagName );
    // Notify about changes
    data->cfg->nodeHasBeenChanged( *this );
}

bool Config::Node::hasValue () const
{ 
    if ( isNull() )
        return false;
    return !data->textData.isNull(); 
}

QString Config::Node::getValue () const
{
    if ( isNull() || !data->xmlData.hasChildNodes() )
        return QString();
    QDomNode n = data->xmlData.firstChild();
    if ( n.isText() )
        return n.toText().data();
    return QString();
}

void Config::Node::setValue ( const QString& value )
{
    if ( isNull() )
        return;
    QMutexLocker locker( data->cfg->notificationMutex );

    if ( hasValue() ) {
        QDomText newTextData = data->cfg->configDoc.createTextNode( value );
        data->xmlData.replaceChild( newTextData, data->textData );
    } else {
        // This is not a text node, so clear all childs
        removeAllChildNodes();
        data->textData = data->cfg->configDoc.createTextNode( value );
        data->xmlData.appendChild( data->textData );
    }

    // Notify about changes
    data->cfg->nodeHasBeenChanged( *this );
}

void Config::Node::resetValue ()
{
    if ( isNull() || !hasValue() )
        return;

    QMutexLocker locker( data->cfg->notificationMutex );

    data->xmlData.removeChild( data->textData );
    data->textData.clear();

    // Notify about changes
    data->cfg->nodeHasBeenChanged( *this );
}

NodeAttributeList Config::Node::getAttributes () const
{    
    NodeAttributeList resultList;
    if ( isNull() )
        return resultList;
    QDomNamedNodeMap attributes = data->xmlData.attributes();
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
    if ( isNull() || data->xmlData.hasAttribute(attr.first) )
        return false;
    QMutexLocker locker( data->cfg->notificationMutex );
    data->xmlData.setAttribute( attr.first, attr.second );
    // Notify about changes
    data->cfg->nodeHasBeenChanged( *this );
    return true;
}

bool Config::Node::removeAttribute ( const QString& attrName )
{
    if ( isNull() || !data->xmlData.hasAttribute(attrName) )
        return false;
    QMutexLocker locker( data->cfg->notificationMutex );
    data->xmlData.removeAttribute( attrName );
    // Notify about changes
    data->cfg->nodeHasBeenChanged( *this );
    return true;
}

void Config::Node::clearAttributes ()
{
    if ( isNull() )
        return;
    QStringList attrNames;
    QDomNamedNodeMap attributes = data->xmlData.attributes();
    int size = attributes.size();
    int i = 0;
    for ( i = 0; i < size; ++i )
        attrNames.append( attributes.item(i).toAttr().name() );

    QMutexLocker locker( data->cfg->notificationMutex );

    for ( i = 0; i < size; ++i )        
        data->xmlData.removeAttribute( attrNames.at(i) );

    // Notify about changes
    data->cfg->nodeHasBeenChanged( *this );
}

Config::Node Config::Node::createChildNode ( const QString& tagName )
{
    if ( isNull() )
        return Config::Node();
    // Suspended parse
    if ( !data->fullyParsed )
        parse();

    // We should reset value if this node has it before
    if ( hasValue() )
        resetValue();

    Config::Node node( *this, tagName );
    data->childs.append( node );
    return node;
}

bool Config::Node::removeChildNodes ( const QString& tagName )
{
    if ( isNull() )
        return false;
    // Suspended parse
    if ( !data->fullyParsed )
        parse();
    bool smthRemoved = false;
    QList<Config::Node>::Iterator it = data->childs.begin();
    for ( ; it != data->childs.end(); ) {
        Config::Node& node = *it;
        if ( node.getTagName() == tagName ) {
            node.fromParentRemove();
            it = data->childs.erase( it );
            smthRemoved = true;
        }
        else 
            ++it;
        
    }
    return (smthRemoved ? true : false);
}

void Config::Node::removeAllChildNodes ()
{
    if ( isNull() )
        return;
    // Suspended parse
    if ( !data->fullyParsed )
        parse();
    QList<Config::Node>::Iterator it = data->childs.begin();
    while ( it != data->childs.end() ) {
        Config::Node& node = *it;
        node.fromParentRemove();
        it = data->childs.erase( it );
    }
}

QList<Config::Node> Config::Node::findChildNodes ( 
    const QString& tagName ) const
{
    // Suspended parse
    if ( !data->fullyParsed )
        parse();
    QList<Config::Node> result;
    QList<Config::Node>::ConstIterator iter = data->childs.begin();
    for ( ; iter < data->childs.end(); ++iter ) {
        const Config::Node& node = *iter;
        if ( node.getTagName() == tagName )
            result.append( node );            
    }    
    return result;
}

QList<Config::Node> Config::Node::childNodes () const
{ 
    if ( isNull() )
        return QList<Config::Node>();
    // Suspended parse
    if ( !data->fullyParsed )
        parse();
    return data->childs; 
}

void Config::Node::parse () const
{
    if ( isNull() )
        return;

    // We are using const cast to make this method constant,
    // as it is called from other constant methods
    Config::Node* self = const_cast<Config::Node*>(this);

    self->data->fullyParsed = true;
    self->data->childs.clear();

    bool hasValueParsed = false;
    QDomNode n = self->data->xmlData.firstChild();
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

    n = self->data->xmlData.firstChild();
    while( !n.isNull() ) {
        if ( n.isElement() ) {
            QDomElement element = n.toElement();
            self->data->childs.append( Config::Node( *self, element ) );
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
