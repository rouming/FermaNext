
#ifndef PLUGINEXECUTIONCONTEXT_H
#define PLUGINEXECUTIONCONTEXT_H

#include "Plugin.h"
#include "PluginExecutionParams.h"
#include "PluginExecutionTree.h"

/** Execution context of plugin */
class PluginExecutionContext
{
public:
    // Exceptions

    /** 
     * Occurs when top node does not have any node, which is 
     * used in execution process, so context list becomes empty.
     * @see PluginExecutionTree::Node::isInUse
     */
    class ContextIsEmptyException {};

public:
    /** Default constructor. Constructed object is not valid (#isValid) */
    PluginExecutionContext ();

    /** 
     * Constructs execution context for the specified and properly 
     * prepared execution tree.
     * @throw ContextIsEmptyException if specified tree is not properly
     *        prepared and context will be empty.
     */
    PluginExecutionContext ( const PluginExecutionTree& ) 
        /*throw (ContextIsEmptyException)*/;

    /** 
     * Returns true if current context position points on context
     * and stays in bounds.
     */
    bool isValid () const;

    /** 
     * Steps to the next context. 
     * Often use #isValid to check validness of context pointer bounds.
     */
    void nextContext ();

    /** 
     * Steps to the previous context. 
     * Often use #isValid to check validness of context pointer bounds.
     */
    void previousContext ();

    /** 
     * Returns plugin dependencies in map, where type is key
     * and plugins list is value.
     */
    QHash< QString, QList<Plugin*> > 
        getCallerDependencies ( const Plugin* ) const;

    /** Returns plugin's params. */
    PluginExecutionParams getCallerParams ( const Plugin* ) const;

private:
    typedef QList< QList<PluginExecutionNode> > ContextList;

    PluginExecutionTree::Node topNode;
    ContextList contextList;
    int currentContext;
};

#endif //PLUGINEXECUTIONCONTEXT_H
