
#ifndef PLUGINEXECUTIONTREE_H
#define PLUGINEXECUTIONTREE_H

#include <QtGlobal>
#include "Plugin.h"
#include "PluginExecutionParams.h"

#if QT_VERSION >= 0x040400
  #include <QAtomicPointer>
#endif

class PluginManager;

/** Private node data */
template <class N> class NodePrivate;

/**
 * Execution tree of a plugin which is going to be executed.
 * Every plugin may have single or several dependencies, in one's
 * part this dependent plugin may have dependencies too. So this
 * class is an execution tree of every possible variants of execution.
 */
class PluginExecutionTree
{
public:
    /** Node of the execution tree */
    class Node
    {
        // Should access child create/remove methods
        friend class PluginExecutionTree;
    public:
        /** Constructs null node */
        Node ();
        /** Constructs node with plugin */
        Node ( Plugin* );
        /** Destructor */
        ~Node ();

        /** Just a copy constructor */
        Node ( const Node& );
        /** Just an assignment operator */
        Node& operator= ( const Node& );

        /** For search in lists and vectors */
        bool operator== ( const Node& );

        /**
         * Returns parent node.
         * Node is null (#isNull) if it is a root node
         */
        Node parentNode () const;

        /**
         * Node can be null, if it was
         * constructed with default constructor.
         */
        bool isNull () const;

        /** Returns all child nodes */
        QList<Node> childNodes () const;

        /** Returns plugin of this node */
        Plugin* getPlugin () const;

        /** Returns true if this plugin depends on other */
        bool isDependent () const;

        /**
         * Returns true if this plugin (whith all its dependencies)
         * can be resolved.
         */
        bool canBeResolved () const;

        /** Sets plugin params. */
        void setPluginParams ( const PluginExecutionParams& );

        /**
         * Returns current plugin params.
         * Params can be not accepted (#areParamsAccepted),
         * so check them first.
         */
        PluginExecutionParams getPluginParams () const;

        /** Returns true if params are accepted by plugin */
        bool areParamsAccepted () const;

        /** Should we use plugin in execution or not */
        void use ( bool );

        /**
         * Checks if this plugin is in use.
         * I.e. is a node of execution process.
         */
        bool isInUse () const;

        /** Returns node uuid */
        QString uuid () const;

    private:
        /** Constructs with new data */
        Node ( NodePrivate<Node>* );

        /** Constructs plugin node with parent */
        Node ( Node& parent, Plugin* );

        /**
         * Creates child node.
         * Should be called only by outer #PluginExecutionTree class.
         */
        Node createChildNode ( Plugin* );

        /**
         * Removes child node if can find it.
         * Should be called only by outer #PluginExecutionTree class.
         */
        void removeChildNode ( Node& );

        /** Set if this plugin can be resolved or not */
        void canBeResolved ( bool );

    private:
#if QT_VERSION >= 0x040400
        QAtomicPointer< NodePrivate<Node> > data;
#else
        NodePrivate<Node>* data;
#endif
    };

public:
    /** Constructor */
    PluginExecutionTree ( PluginManager& );

    /**
     * Builds execution tree for specified plugin
     * @return top node of the execution tree, actually,
     *         top node always contains argument plugin.
     */
     PluginExecutionTree::Node buildExecutionTree ( Plugin* plg );

     /** Return last top of the execution tree */
     PluginExecutionTree::Node getTreeTop () const;

    /**
     * Tries to find node by uuid.
     * If node is not found, returns null node.
     * @see PluginExecutionTree::Node::isNull
     * @see PluginExecutionTree::Node::uuid
     */
    PluginExecutionTree::Node findNodeByUUID ( const QString& ) const;

private:
    /** Recursively finds node by specified uuid */
    PluginExecutionTree::Node findNodeByUUID (
                                     const PluginExecutionTree::Node&,
                                     const QString& uuid ) const;

    /** Recursively gather parents from bottom to top */
    void getParents ( Node& child, PluginList& parents );
    /** Returns plugin dependencies for the specified plugin node */
    QList<Node> getDependentPlugins( Node& );
    /** Recursively builds execution tree */
    bool buildExecutionTree ( Node& );

private:
    PluginManager& plgMng;
    Node treeTop;
};

typedef PluginExecutionTree::Node PluginExecutionNode;
typedef QList<PluginExecutionNode> PluginExecutionNodeList;

#endif //PLUGINEXECUTIONTREE_H
