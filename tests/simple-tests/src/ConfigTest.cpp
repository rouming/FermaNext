
#include <iostream>

#include <QFile>
#include <QThread>
#include <QCoreApplication>

//#include "../include/ConfigTestThread.h"

#include "Config.h"


/*****************************************************************************/
/*
int ConfigTestThread::threadCounter = 0;

ConfigTestThread::ConfigTestThread () :
    config(0),
    name( QString("Thread #%1").arg(++threadCounter) )
{}

void ConfigTestThread::nodeChanged ( Config::Node )
{
    qWarning( qPrintable(name + ": node changed") );
}

void ConfigTestThread::nodeCreated ( Config::Node )
{
    qWarning( qPrintable(name + ": node created") );
}

void ConfigTestThread::nodeRemoved ( Config::Node )
{
    qWarning( qPrintable(name + ": node removed") );
}

void ConfigTestThread::setConfig ( Config* cfg )
{
    if ( config )
        config->disconnect( this );
    config = cfg;
    if ( config ) {
        QObject::connect( config, 
                          SIGNAL(onNodeCreated(Config::Node)), 
                          SLOT(nodeCreated(Config::Node)),
                          Qt::QueuedConnection );

        QObject::connect( config, 
                          SIGNAL(onNodeChanged(Config::Node)), 
                          SLOT(nodeChanged(Config::Node)),
                          Qt::QueuedConnection );

        QObject::connect( config, 
                          SIGNAL(onNodeRemoved(Config::Node)), 
                          SLOT(nodeRemoved(Config::Node)),
                          Qt::QueuedConnection );

    }

}

void ConfigTestThread::run ()
{
    ConfigNode rootNode = config->rootNode();    
    for ( uint i = 0; i < 10; ++i ) {
        //qWarning( qPrintable(QString("changing ") + name) );
        rootNode.createChildNode( name );
    }
}
*/
/*****************************************************************************/

const QString ConfigFileName( "../build/config.xml" );

class ConfigTest : public QThread
{
public:
    ConfigTest () :
        passed(0), failed(0)
    {}

    void my_assert ( bool val, const QString& str )
    {
        if ( val ) ++passed;
        else ++failed;    
        std::cout << (val ? "    OK: " : "NOT OK: ") << 
                     qPrintable(str) << "\n"; 
    }

    void lastReport ()
    {
        std::cout << "\n=================>>>\n" << 
                     "Passed: " << passed << "\n" << 
                     "Failed: " << failed << "\n";
    }

    void testCaseBegin ( const QString& testCaseName ) 
    {
        std::cout <<  "=========== " << qPrintable(testCaseName) << 
                      " ==========\n";
    }

    void testCaseEnd () 
    {
        std::cout << "=================================\n\n";
    }

    uint status ()
    { return failed; }

    void run () 
    {
        configCreationRemoving();
        childNodesCreateRemove();

        QCoreApplication::quit();
    }


    //////////////////////////////////////////////////////////////////////////
    // TEST CASES
    //////////////////////////////////////////////////////////////////////////

    void configCreationRemoving ()
    {
        testCaseBegin("configCreationRemoving");

        QFile::remove( ConfigFileName );
        Config& config = Config::instance( ConfigFileName );
        my_assert( QFile::exists(ConfigFileName), 
                   "Config file should be created" );

        Config& config2 = Config::instance( ConfigFileName );
        my_assert( &config == &config2, "Instances should be unique" );

        Config::destroyInstance( config2, true );
        my_assert( !QFile::exists(ConfigFileName), 
                   "Config file should be destroyed " );

        QFile configFile( ConfigFileName );
        bool openRes = configFile.open( QIODevice::WriteOnly | 
                                        QIODevice::Append );
        my_assert( openRes, "Config file is opened with QFile" );
        
        configFile.write( "some stuff to occur parse exception" );
        configFile.close();

        bool excep = false;
        try { Config::instance( ConfigFileName ); }
        catch ( ... ) { excep = true; }
        my_assert( excep, "Can't parse corrupted config" );

        // Remove corrupted file
        QFile::remove( ConfigFileName );        
                
        testCaseEnd();
    }

    void childNodesCreateRemove ()
    {
        testCaseBegin("childNodesCreateRemove");

        Config& config = Config::instance( ConfigFileName );

        ConfigNode rootNode = config.rootNode();

        rootNode.createChildNode( "One" );
        rootNode.createChildNode( "Two" );
        rootNode.createChildNode( "Three" );

        ConfigNodeList nodes = rootNode.childNodes();
        my_assert( nodes.size() == 3, "3 child nodes after create" );

        my_assert( nodes[0].getTagName() == "One", "First is 'One'" );
        my_assert( nodes[1].getTagName() == "Two", "Second is 'Two'" );
        my_assert( nodes[2].getTagName() == "Three", "Third is 'Three'" );

        nodes[0].remove();
        rootNode.removeChildNodes( "Two" );

        nodes = rootNode.childNodes();

        my_assert( nodes.size() == 1, "1 child node after remove" );
        my_assert( nodes[0].getTagName() == "Three", "First is 'Three'" );

        rootNode.createChildNode( "Three" );
        rootNode.createChildNode( "Three" );

        nodes = rootNode.findChildNodes( "Three" );
        my_assert( nodes.size() == 3, "3 child nodes after create" );

        my_assert( nodes[0].getTagName() == "Three", "First is 'Three'" );
        my_assert( nodes[1].getTagName() == "Three", "Second is 'Three'" );
        my_assert( nodes[2].getTagName() == "Three", "Third is 'Three'" );

        rootNode.createChildNode( "OtherNode" );

        rootNode.removeChildNodes( "Three" );

        nodes = rootNode.childNodes();
        my_assert( nodes.size() == 1, "1 child nodes after remove" );

        int i = 0;
        ConfigNode node = rootNode;
        for ( i = 0; i < 1; ++i )
            node = node.createChildNode( QString("%1").arg(i) );

        nodes = rootNode.childNodes();
        my_assert( nodes.size() == 2, "2 child nodes after create" );

        foreach ( ConfigNode node, nodes ) {
            //            if ( i % 2 )
            node.remove();
                //            else
            //rootNode.removeChildNodes( node.getTagName() );
        }

        std::cout << "SZ " << rootNode.childNodes().size() << "\n";
        my_assert( rootNode.childNodes().size() == 0, "0 child nodes after remove" );
        

        testCaseEnd();
    }

private:
    uint passed;
    uint failed;
    /*
    ConfigTestThread thread1;
    ConfigTestThread thread2;
    ConfigTestThread thread3;
    ConfigTestThread thread4;
    ConfigTestThread thread5;
    */
};

struct Data 
{
    int val1;
    int val2;
    QList<int> list;
};

#include <QAtomic>



int main ( int argc, char** argv )
{

    ConfigSharedData< NodeData<int> > sd;
    ConfigSharedData< NodeData<int> > sd2 = sd;
    ConfigSharedData< NodeData<int> > sd3; sd3 = sd2;



    //    exit(1);

    QCoreApplication app( argc, argv );

    ConfigTest test;

    test.start();
    
    app.exec();
    test.wait();    


    /*

    #1. file creation (file removing)
    #2. file parse error
    #3. create nodes (+ threads, + signal catch)
    #4. remove nodes (+ threads, + signal catch)
    #4. remove all child nodes (+ threads, + signal catch)
    #5. find nodes 
    6. changing attributes (+ threads, + signal catch)
    7. remove attributes by name (+ threads, + signal catch)
    7. clearing all attributes (+ threads, + signal catch)
    8. setting value  (+ threads, + signal catch, +checking child removal)
    8. resetting value  (+ threads, + signal catch)
    9. tag name changing (+ threads, + signal catch)

    */

    test.lastReport();
   

    return test.status();
}
