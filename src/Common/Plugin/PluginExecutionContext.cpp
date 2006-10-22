
#include "PluginExecutionContext.h"

/*****************************************************************************/

PluginExecutionContext::PluginExecutionContext () :
    currentContext(-1)
{}

PluginExecutionContext::PluginExecutionContext ( 
    const PluginExecutionTree& tree ) 
    /*throw (ContextIsEmptyException)*/ :
    topNode(tree.getTreeTop()),
    currentContext(-1)
{
    if ( topNode.isNull() || ! topNode.isInUse() )
        throw ContextIsEmptyException();

    // Zero context always contains top node
    QList<PluginExecutionTree::Node> zeroContext;
    zeroContext.append(topNode);
    contextList.append(zeroContext);

    QList<PluginExecutionTree::Node> childs = topNode.childNodes();

    while ( ! childs.isEmpty() ) {
        // Pick up only is in use children for next context
        QList<PluginExecutionTree::Node> isInUseChilds;
        foreach ( PluginExecutionTree::Node child, childs ) {
            if ( !child.isNull() && child.isInUse() )
                isInUseChilds.append(child);
        }

        if ( isInUseChilds.isEmpty() )
            // Nothing to do
            break;

        // Save next context
        contextList.append(isInUseChilds);

        // Take all children for next context
        QList<PluginExecutionTree::Node> nextChilds;
        foreach ( PluginExecutionTree::Node isInUseChild, isInUseChilds ) {
            nextChilds << isInUseChild.childNodes();
        }
        // Save children for next iteration
        childs = nextChilds;
    }

    // Current context at -1 pos, and is not valid.
    // First 'next()' call should validate context
    currentContext = -1;
}

bool PluginExecutionContext::isValid () const
{
    if ( topNode.isNull() )
        return false;
    if ( currentContext < 0 || currentContext >= contextList.size() )
        return false;
    return true;
}

void PluginExecutionContext::nextContext ()
{
    if ( topNode.isNull() )
        return;

    if ( currentContext < 0 )
        // ok, first call
        currentContext = 0;
    else if ( currentContext >= contextList.size() )
        // bad, end pos
        currentContext = contextList.size();
    else
        ++currentContext;
}

void PluginExecutionContext::previousContext ()
{
    if ( topNode.isNull() )
        return;

    if ( currentContext < 0 )
        // bad, should be -1
        currentContext = -1;
    else if ( currentContext >= contextList.size() )
        // ok, last pos
        currentContext = contextList.size() - 1;
    else
        --currentContext;
}

QHash< QString, QList<Plugin*> > 
PluginExecutionContext::getCallerDependencies ( const Plugin* plg ) const
{
    typedef QHash< QString, QList<Plugin*> > DependenciesMap ;

    Q_ASSERT(plg);

    if ( ! isValid() )
        return DependenciesMap();

    QList<PluginExecutionTree::Node> context = contextList[currentContext];
    foreach ( PluginExecutionTree::Node node, context ) {
        if ( node.getPlugin() == plg ) {
            QList<PluginExecutionTree::Node> childs = node.childNodes();
            DependenciesMap isInUsePlugins;
            foreach ( PluginExecutionTree::Node child, childs ) {
                if ( !child.isNull() && child.getPlugin() != 0 &&
                     child.isInUse() ) {

                    Plugin* plg = child.getPlugin();
                    QString type = plg->pluginInfo().type;
                    QList<Plugin*>& plugins = isInUsePlugins[type];
                    plugins << plg;
                }
            }
            return isInUsePlugins;
        }
    }

    // Nothing was found. Wrong plg? 
    return DependenciesMap();
}

PluginExecutionParams PluginExecutionContext::getCallerParams ( 
    const Plugin* plg ) const
{
    Q_ASSERT(plg);

    if ( ! isValid() )
        return PluginExecutionParams();

    QList<PluginExecutionTree::Node> context = contextList[currentContext];
    foreach ( PluginExecutionTree::Node node, context ) {
        if ( !node.isNull() && node.getPlugin() == plg ) {
            return node.getPluginParams();
        }
    }

    // Nothing was found. Wrong plg? 
    return PluginExecutionParams();
}

/*****************************************************************************/
