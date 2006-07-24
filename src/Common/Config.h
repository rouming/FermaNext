
#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QVariant>
#include <QPair>
#include <QList>
#include <QDomElement>
#include <QDomDocument>
#include <QHash>
#include <QFile>

class QMutex;

typedef QPair<QString, QString> NodeAttribute;
typedef QList<NodeAttribute> NodeAttributeList;

/** Subsidiary class, which registers Config::Node */
template < class Type >
class ConfigNodeTypeRegistrator
{ 
public:
    ConfigNodeTypeRegistrator () 
    {  qRegisterMetaType< Type >( "Config::Node" ); }
};

/** Private config node data */
template <class N> class ConfigNodePrivate;

/**
 * Configuration class, which manages xml configuration.
 */
class Config : public QObject
{
    Q_OBJECT
public:
    /**
     * Inner class, which represents xml node of configuration.
     */
    class Node
    {
    public:
        /** Constructs null node */
        Node ();
        /** Destructor */
        ~Node ();
        /** Parses root configuration xml node  */
        Node ( Config&, const QDomElement& rootNode );

        /** Just a copy constructor */
        Node ( const Node& );
        /** Just an assignment operator */
        Node& operator= ( const Node& );

        /** For search in lists and vectors */
        bool operator== ( const Node& );

        /** Returns config, from which this node was created */
        Config* config () const;

        /** 
         * Returns parent node. 
         * Node is null (#isNull) if it is a root node
         */
        Node parentNode () const;

        /** Removes this node from config */
        void remove ();

        /** 
         * Checks if this node was already removed 
         * @return true if it is removed, false otherwise
         */
        bool isRemoved () const;

        /** 
         * Node can be null, if it was 
         * constructed with default constructor.
         */
        bool isNull () const;

        /** Return tag name of this node */
        QString getTagName () const;
        /** Sets tag name */
        void setTagName ( const QString& );

        /** 
         * Config nodes can store child config nodes or text value.
         * So if this node has value, it doesn't have childs.
         * @see getValue
         * @see setValue
         */
        bool hasValue () const;

        /** 
         * Returns value of this node. If this node 
         * has other config nodes or value has not been
         * set before, returned string is empty 
         * @see setValue
         * @see hasValue
         */
        QString getValue () const;

        /**
         * Sets value to node. If node has child config nodes
         * they will be removed and value will be set.
         * @see getValue
         * @see hasValue
         */
        void setValue ( const QString& );

        /** Resets value. #hasValue should return false after call */
        void resetValue ();

        /** Returns attributes of this node */
        NodeAttributeList getAttributes () const;
        /** Adds new node attribute */
        bool addAttribute ( const NodeAttribute& );
        /** Tries to remove attribute from node */
        bool removeAttribute ( const QString& );
        /** Clears all attributes */
        void clearAttributes ();

        /** Creates child node with specified tag name */
        Node createChildNode ( const QString& tagName );
        /** Tries to remove child nodes with specified tag name */
        bool removeChildNodes ( const QString& tagName );
        /** Removes all child nodes */
        void removeAllChildNodes ();

        /** Finds child nodes with specified tag name */
        QList<Node> findChildNodes ( const QString& tagName ) const;
        /** Returns all child nodes */
        QList<Node> childNodes () const;

    private:
        /** Constructs with new data */
        Node ( ConfigNodePrivate<Node>* );

        /** Constructs configuration xml node with parent */
        Node ( Node& parent, const QString& tagName );
        /** Parses xml node  */
        Node ( Node& parent, const QDomElement& );

        /**
         * Parse childs for node
         * Sure, this method is not a const one, but it can be called
         * from some const, so const_cast is made inside.
         */
        void parse () const;

        /** 
         * Removes this node from config, but instead #remove, this method
         * is called from parent and it does nothing agains removing itself
         * from parent's child list, this should be done by parent.
         */
        void fromParentRemove ();
        
    private:
        ConfigNodePrivate<Node>* data;
    };

    //Exceptions
    /** Can't open file for read/write */
    class OpenConfigException {};


    /**
     * Create an instance of config.
     * @param fileName of configuration.
     * @throw OpenConfigException occurs when can't open file for read/write
     */
    static Config& instance ( const QString& fileName )
        /*throw (OpenConfigException)*/;

    /** 
     * Destroys config instance by specified file name.
     * @param fileName config file name
     * @param removeConfigFile if true, file will be removed
     * @return returns true if success, false otherwise
     */
    static bool destroyInstance ( const QString& fileName, 
                                  bool removeConfigFile = false );

    /** 
     * Destroys config instance. 
     * This is an overloaded member function, provided for convenience.
     */
    static bool destroyInstance ( Config& cfg,
                                  bool removeConfigFile = false );

    /** Return config file name */
    QString configFileName () const;

    /** Returns root node of this config */
    Config::Node rootNode () const;

private:
    /**
     * Is called from inner Node class before making some changes. 
     * After all changes have been made, node should unlock Config 
     * instance with #nodeHasBeenChanged, #nodeHasBeenCreated or 
     * #nodeBeforeRemove call.
     */
    void lockBeforeChange ();

    /**
     * Is called from inner Node class to notify about node changes.
     * Should be called often after #lockBeforeChange.
     */
    void nodeHasBeenChanged ( const Config::Node& );

    /**
     * Is called from inner Node class to notify about node creation.
     * Should be called often after #lockBeforeChange.
     */
    void nodeHasBeenCreated ( const Config::Node& );
    
    /**
     * Is called from inner Node class to notify about node removal. 
     * Should be called often after #lockBeforeChange.
     */
    void nodeHasBeenRemoved ( const Config::Node& );


    /** Flushes all changes to file */
    void flush ();

    /** 
     * Private constructor. Use #instance instead.
     * @throw OpenConfigException occurs when can't open file for read/write
     */
    Config ( const QString& fileName ) /*throw (OpenConfigException)*/;

    /** Destructor */
    ~Config ();
    /** Just a copy constructor */
    Config ( const Config& );
    /** Just an assignmen operator */
    Config& operator= ( const Config& );

signals:
    /** Emits, when some node has been changed */
    void onNodeChanged ( Config::Node );

    /** Emits, when some node has been created */
    void onNodeCreated ( Config::Node );

    /** Emits, when some node has been removed */
    void onNodeRemoved ( Config::Node );


private:
    typedef QHash<QString, Config*> HashInstances;
    typedef ConfigNodeTypeRegistrator<Node> NodeRegistrator;

    static HashInstances configInstances;
    static QMutex* instanceMutex;
    static QMutex* notificationMutex;
    static NodeRegistrator registrator;/**< Registrator should be static to 
                                            register #Config::Node only once */

    QDomDocument configDoc;
    QFile configFile;
    Config::Node rootConfigNode;
};

typedef Config::Node ConfigNode;
typedef QList<ConfigNode> ConfigNodeList;

#endif //CONFIG_H
