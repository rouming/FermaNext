
#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QVariant>
#include <QPair>
#include <QList>
#include <QDomElement>
#include <QDomDocument>
#include <QMutex>
#include <QHash>
#include <QFile>

typedef QPair<QString, QString> NodeAttribute;
typedef QList<NodeAttribute> NodeAttributeList;

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
        /** Prases root configuration xml node  */
        Node ( Config&, const QDomElement& rootNode );
        /** Constructs configuration xml node with parent */
        Node ( Node& parent, const QString& tagName );
        /** Prases xml node  */
        Node ( Node& parent, const QDomElement& );
        
        /** Just a copy constructor */
        Node ( const Node& );
        /** Just an assignmen operator */
        Node& operator= ( const Node& );        

        /** Returns config, from which this node was created */
        Config* config () const;

        /** Returns parent node */
        Node* parentNode () const;

        /** Removes this node from config */
        void remove ();

        /** 
         * Checks if this node was already removed 
         * @return true if it is removed, false otherwise
         */
        bool isRemoved () const;

        /** 
         * Node can be null, if it was constructed only 
         * with config parameter
         */
        bool isNull () const;

        /** Return tag name of this node */
        QString getTagName () const;
        /** Sets tag name */
        void setTagName ( const QString& );

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
        /** Tries to remove child node with specified tag name */
        bool removeChildNode ( const QString& tagName );
        /** Removes all child nodes */
        void removeAllChildNodes ( const QString& tagName );

        /** Finds child nodes with specified tag name */
        QList<Node> findChildNodes ( const QString& tagName ) const;
        /** Returns all child nodes */
        QList<Node> childNodes () const;

    private:
        /**
         * Parse childs for node
         * Sure, this method is not a const one, but it can be called
         * from some const, so const_cast is made inside.
         */
        void parse () const;
        
    private:
        Config* cfg;
        Node* parent;
        QDomElement xmlData;
        mutable QList<Node> childs;
        bool removedFlag;
        mutable bool fullyParsed;
    };

    // Inner Node class should be a friend of Config to have possibility 
    // to notify directly about data changes
    friend class Node;

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
    ~Config ();
    Config ( const Config& );
    Config& operator= ( const Config& );



signals:
    /** Emits, when some node has been changed */
    void onNodeChanged ( const Config&, const Config::Node& );

    /** Emits, when some node has been created */
    void onNodeCreated ( const Config&, const Config::Node& );

    /** Emits, when some node has been removed */
    void onNodeRemoved ( const Config&, const Config::Node& );


private:
    typedef QHash<QString, Config*> HashInstances;

    static HashInstances configInstances;
    static QMutex instanceMutex;
    static QMutex notificationMutex;

    QDomDocument configDoc;
    QFile configFile;
    Config::Node rootConfigNode;
};

typedef Config::Node ConfigNode;
typedef QList<ConfigNode> ConfigNodeList;


#endif //CONFIG_H
