
#include <iostream>

#include "Config.h"
#include "Plugin.h"
#include "PluginManager.h"
#include "PluginExecutionTree.h"

/*****************************************************************************
 * Local Plugin manager for testing
 *****************************************************************************/

class LocalPluginManager : public PluginManager
{
public:
    LocalPluginManager () :
        PluginManager( Config::instance("temp") )
    {}

    void setPluginList ( const QList<Plugin*>& plgs )
    { plugins = plgs; }

    virtual PluginList loadedPlugins ( bool ) const
    { return plugins; }

private:
    QList<Plugin*> plugins;
};

LocalPluginManager& plgManagerInstance ()
{
    static LocalPluginManager* instance = 0;
    if ( instance == 0 )
        instance = new LocalPluginManager;
    return *instance;
}

/*****************************************************************************
 * Common Plugin
 *****************************************************************************/

class CommonPlugin : public Plugin
{
public:
    CommonPlugin () : Plugin( plgManagerInstance(), "nothing" ) {}

    virtual ExecutionResult specificExecute ( 
                              const PluginExecutionParams&,
                              const QList<UUIDObject*>&,
                              const QHash< QString, QList<Plugin*> >& )
    { return ExecutionResult(OkStatus, QString()); }
    virtual Status pluginStatusCode () const
    { return OkStatus; }
    virtual QString pluginStatusMsg () const
    { return QString(); }
    virtual void tryToAcceptParams ( const PluginExecutionParams& ) const
    { /* nothing */ }
    virtual Plugin::DependenceMode dependenceMode () const
    { return MultiDependent; }
};

/*****************************************************************************
 * Different plugins
 *****************************************************************************/

//////////////////////////////////////////////////////////////////////////////
//// SIMPLE
//////////////////////////////////////////////////////////////////////////////

class SP1 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("SP1", "SP1", "simple"); return pi; }
    virtual Plugin::DependenceMode dependenceMode () const
    { return Independent; }

};

class SP2 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("SP2", "SP2", "simple"); return pi; }
    virtual Plugin::DependenceMode dependenceMode () const
    { return Independent; }
};

class SP3 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("SP3", "SP3", "simple"); return pi; }
    virtual Plugin::DependenceMode dependenceMode () const
    { return Independent; }
};


//////////////////////////////////////////////////////////////////////////////
//// CALCulation
//////////////////////////////////////////////////////////////////////////////

class CP1 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("CP1", "CP1", "calc"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "simple" ); return l; }
};

class CP2 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("CP2", "CP2", "calc"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "calc" << "simple" ); return l; }
};

class CP3 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("CP3", "CP3", "calc"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "calc" << "simple" ); return l; }
};

//////////////////////////////////////////////////////////////////////////////
//// OPTimization
//////////////////////////////////////////////////////////////////////////////

class OP1 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("OP1", "OP1", "opt"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "NMOT" ); return l; }
};

class OP2 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("OP2", "OP2", "opt"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "opt" << "calc" ); return l; }
};

class OP3 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("OP3", "OP3", "opt"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "opt" << "calc" ); return l; }
};

class OP4 : public CommonPlugin
{
public:
    virtual const PluginInfo& pluginInfo () const
    { static PluginInfo pi("OP4", "OP4", "opt"); return pi; }

    virtual const QStringList& requiredPluginTypes () const
    { static QStringList l( QStringList() << "opt" << "calc" << "simple" ); 
      return l; }
};


QList<Plugin*>& pluginList () 
{
    static QList<Plugin*>* instance = 0;
    if ( instance == 0 ) {
        instance = new QList<Plugin*>;
       
        (*instance) << 
            new SP1 << new SP2 << new SP3 <<
            new CP1 << new CP2 << new CP3 <<
            new OP1 << new OP2 << new OP3 << new OP4;
    }
    return *instance;
}

/*****************************************************************************/

void printNode ( PluginExecutionTree::Node& node, int tabs = 0 )
{
    std::cout  << "Node: ";
     for ( int i = 0; i < tabs; ++i )
         std::cout << "\t";

    qWarning( "%s, can be resolved: %s", 
              qPrintable(node.getPlugin()->pluginInfo().name),
                (node.canBeResolved() ? "yes" : "no"));

    QList<PluginExecutionTree::Node> childs = node.childNodes();
    foreach ( PluginExecutionTree::Node n, childs )
        printNode( n, tabs + 1 );
}

int main ()
{
    LocalPluginManager& plgMng = plgManagerInstance();
    QList<Plugin*>& plugins = pluginList();
    plgMng.setPluginList( plugins );
    
    PluginExecutionTree tree( plgMng );

    foreach ( Plugin* plg, plugins ) {
        std::cout << ">>>>>>>>>>>>>>>>>>>>>> " <<
                     "building tree for " << 
                     qPrintable(plg->pluginInfo().name) <<
                     " <<<<<<<<<<<<<<<<<< \n";
        PluginExecutionTree::Node node = tree.buildExecutionTree( plg );
        printNode( node );
    }
    return 0;
}

/*****************************************************************************/
