
#include "PluginExecutionTree.h"
#include "PluginManager.h"

/*****************************************************************************
 * Node Private
 *****************************************************************************/

template <class N>
class NodePrivate
{
public:
    NodePrivate () :
        ref(1), parent(0), 
        plugin(0), inUse(false),
        canBeResolved(false)
    {}

    ~NodePrivate ()
    {
        parent = 0;
        plugin = 0;
        inUse = false;
    }

    QAtomic ref;
    NodePrivate<N>* parent;
    QList<N> childs;
    Plugin* plugin;
    bool inUse;
    bool canBeResolved;
    PluginParams pluginParams;
};

/*****************************************************************************
 * PluginExecutionTree::Node
 *****************************************************************************/

PluginExecutionTree::Node::Node () :
    data(0)
{}

PluginExecutionTree::Node::Node ( Plugin* plugin ) :
    data( new NodePrivate<PluginExecutionTree::Node> )
{
    data->plugin = plugin;
}

PluginExecutionTree::Node::~Node ()
{
    if ( data && !data->ref.deref() ) {
        // Zeroes parents
        PluginExecutionNodeList::Iterator it = data->childs.begin();
        for ( ; it != data->childs.end(); ++it )
            it->data->parent = 0;
        delete data;
    }
}

PluginExecutionTree::Node::Node ( Node& parent_, Plugin* plugin ) :
    data( new NodePrivate<PluginExecutionTree::Node> )
{
    data->plugin = plugin;
    data->parent = parent_.data;
}

PluginExecutionTree::Node::Node ( 
    NodePrivate<PluginExecutionTree::Node>* nodeData ) :
    data( nodeData )
{
    if ( data )
        data->ref.ref();
}

PluginExecutionTree::Node::Node ( const Node& node ) :
    data( node.data )
{
    if ( data )
        data->ref.ref();
}

PluginExecutionTree::Node& PluginExecutionTree::Node::operator= ( 
    const PluginExecutionTree::Node& node )
{
    NodePrivate<PluginExecutionTree::Node>* p = node.data;
    if ( p )
        p->ref.ref();
    p = qAtomicSetPtr( &data, p );
    if ( p && !p->ref.deref() )
        delete p;
    return *this;
}

bool PluginExecutionTree::Node::operator== (
    const PluginExecutionTree::Node& node )
{ return (node.data == data); }

PluginExecutionTree::Node PluginExecutionTree::Node::parentNode () const
{ return PluginExecutionTree::Node( data->parent );  }

bool PluginExecutionTree::Node::isNull () const
{ return data == 0; }

PluginExecutionTree::Node PluginExecutionTree::Node::createChildNode (
    Plugin* plugin )
{
    if ( isNull() )
        return PluginExecutionTree::Node();

    PluginExecutionTree::Node node( *this, plugin );
    data->childs.append( node );
    return node;
}

void PluginExecutionTree::Node::removeChildNode ( 
    PluginExecutionTree::Node& node )
{
    if ( isNull() )
        return;

    PluginExecutionNodeList::Iterator foundIt = 
        qFind( data->childs.begin(), data->childs.end(), node );

    if ( foundIt == data->childs.end() )
        // Nothing found
        return;

    // Erase from children
    data->childs.erase( foundIt );

    // Zeroes parent
    node.data->parent = 0;
    node.data->inUse = false;
}

QList<PluginExecutionTree::Node> PluginExecutionTree::Node::childNodes () const
{ 
    if ( isNull() )
        return QList<PluginExecutionTree::Node>();
    return data->childs;
}


Plugin* PluginExecutionTree::Node::getPlugin () const
{
    if ( isNull() )
        return 0;
    return data->plugin;
}

bool PluginExecutionTree::Node::isDependent () const
{
    if ( isNull() || data->plugin == 0 )
        return false;
    if ( data->plugin->dependenceMode() == Plugin::Independent )
        return false;
    return true;
}

bool PluginExecutionTree::Node::canBeResolved () const
{
    if ( isNull() || data->plugin == 0 )
        return false;
    return data->canBeResolved;
}

void PluginExecutionTree::Node::canBeResolved ( bool canBeResolved_ )
{
    if ( isNull() || data->plugin == 0 )
        return;
    data->canBeResolved = canBeResolved_;
}

bool PluginExecutionTree::Node::arePluginParamsValid () const
{
    // TODO:
    return true;
}

void PluginExecutionTree::Node::use ( bool useFlag )
{
    if ( isNull() )
        return;
    data->inUse = useFlag;
}

bool PluginExecutionTree::Node::isInUse () const
{
    if ( isNull() )
        return false;
    return data->inUse;
}

/*****************************************************************************
 * PluginExecutionTree::Node
 *****************************************************************************/

PluginExecutionTree::PluginExecutionTree ( PluginManager& mng ) :
    plgMng(mng)
{}

PluginExecutionTree::Node PluginExecutionTree::buildExecutionTree ( 
    Plugin* plugin )
{
    Q_ASSERT(plugin);
    treeTop = PluginExecutionTree::Node( plugin );
    buildExecutionTree( treeTop );
    return treeTop;
}

PluginExecutionTree::Node PluginExecutionTree::getTreeTop () const
{ return treeTop; }

void PluginExecutionTree::getParents ( 
    PluginExecutionTree::Node& child, PluginList& parents ) 
{
    if ( child.parentNode().isNull() ) {
        // Top
        parents.append( child.getPlugin() );
        return;
    }
    PluginExecutionTree::Node parent = child.parentNode();
    getParents( parent, parents );    
    parents.append( child.getPlugin() );
}

PluginExecutionNodeList PluginExecutionTree::getDependentPlugins ( 
    PluginExecutionNode& parent )
{
    RequiredPluginsMap pluginsMap = 
        plgMng.getDependencies( parent.getPlugin() );

    PluginList parents;
    getParents( parent, parents );

    PluginExecutionNodeList dependencies;

    foreach ( PluginList plugins, pluginsMap.values() ) {    
        foreach ( Plugin* plugin, plugins ) {
            // Pick up plugins which were not mentioned before
            // and which are not equal to the parent plugin.
            if ( ! parents.contains(plugin) && plugin != parent.getPlugin() ) {
                PluginExecutionNode n = parent.createChildNode( plugin );
                dependencies.append(n);
            }
        }
    }
    return dependencies;
}

bool PluginExecutionTree::buildExecutionTree ( 
    PluginExecutionTree::Node& parent )
{
    if ( ! parent.isDependent() ) {
        // Nothing to resolve, so of cource true
        parent.canBeResolved(true);
        return true;
    }

    PluginExecutionNodeList dependencies = getDependentPlugins( parent );
    if ( dependencies.size() == 0 ) {
        // Can't get dependencies, so can't be resolved
        parent.canBeResolved(false);
        return false;
    }
    
    bool validDependence = false;
    foreach ( PluginExecutionNode depNode, dependencies ) {
        bool ok = buildExecutionTree( depNode );
        if ( ! ok ) {
            // Nothing to do
        } else
            validDependence = true;
    }

    // We can resolve this node if at least we can resolve one dependence branch :)
    parent.canBeResolved( validDependence );

    return validDependence;
}

/*****************************************************************************/
