
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

void ConfigTestThread::nodeRemoved ( Config::Node n )
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
        rootNode.createChildNode( name );
    }
}
*/
/*****************************************************************************/

const QString ConfigFileName( "config.xml" );

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
        attributesCreateRemove();
        valueSetReset();
        tagNameChanging();

        // Destroy everyrhing
        Config::destroyInstance( ConfigFileName, true );

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

        ConfigNode node = rootNode.createChildNode( "OtherNode" );

        rootNode.removeChildNodes( "Three" );
        node.remove();

        nodes = rootNode.childNodes();
        my_assert( nodes.size() == 0, "0 child nodes after remove" );

        int i = 0;
        node = rootNode;
        for ( i = 0; i < 50; ++i )
            node = node.createChildNode( QString("%1").arg(i) );

        nodes = rootNode.childNodes();
        my_assert( nodes.size() == 1, "1 child nodes after `tree` create" );

        nodes[0].remove();
        nodes = rootNode.childNodes();
        my_assert( nodes.size() == 0, "0 child nodes after remove" );

        testCaseEnd();
    }

    void attributesCreateRemove ()
    {
        testCaseBegin("attributesCreateRemove");

        Config& config = Config::instance( ConfigFileName );

        ConfigNode rootNode = config.rootNode();

        ConfigNode node = rootNode;
        int i = 0;
        for ( i = 0; i < 10; ++i ) {
            ConfigNode newNode1 = node.createChildNode( 
                                       QString("NewNode1_%1").arg(i) );
            ConfigNode newNode2 = node.createChildNode( 
                                       QString("NewNode2_%1").arg(i) );

            newNode1.addAttribute( NodeAttribute(
                                              QString("Attr1_%1").arg(i), 
                                              QString("Value1_%1").arg(i)) );
            newNode1.addAttribute( NodeAttribute(
                                              QString("Attr2_%1").arg(i),
                                              QString("Value2_%1").arg(i)) );

            newNode2.addAttribute( NodeAttribute(
                                              QString("Attr1_%1").arg(i),
                                              QString("Value1_%1").arg(i)) );

            newNode2.addAttribute( NodeAttribute(
                                              QString("Attr2_%1").arg(i),
                                              QString("Value2_%1").arg(i)) );

            node = newNode2;
        }


        node = rootNode;
        for ( i = 0; i < 10; ++i ) {
            ConfigNodeList nodes = node.childNodes();
            ConfigNode lastNode;
            int j = 0;
            foreach ( ConfigNode node, nodes ) {
                ++j;
                my_assert( node.getTagName() == 
                           QString("NewNode%1_%2").arg(j).arg(i), 
                           "Correct tag name" );
                NodeAttributeList attrs = node.getAttributes();
                int k = 0;
                foreach ( NodeAttribute attr, attrs ) {
                    ++k;
                    my_assert( attr.first == 
                               QString("Attr%1_%2").arg(k).arg(i),
                      QString("Correct attr for node #%1,%2").arg(j).arg(i) );
                    my_assert( attr.second ==
                               QString("Value%1_%2").arg(k).arg(i),
                      QString("Correct val for node #%1,%2").arg(j).arg(i) );

                    // Remove only first attr
                    if ( k == 0 ) {
                        bool remRes = node.removeAttribute( attr.first );
                        my_assert( remRes, "True result after attr remove" );
                    }
                }
                node.clearAttributes();
                attrs = node.getAttributes();
                my_assert( attrs.size() == 0, "0 attrs after clear" );
                
                node.remove();
                lastNode = node;
            }
            node = lastNode;
        }

        my_assert( rootNode.childNodes().size() == 0, 
                   "Root node should be empty" );

        testCaseEnd();
    }

    void valueSetReset ()
    {
        testCaseBegin("valueSetReset");

        Config& config = Config::instance( ConfigFileName );

        ConfigNode rootNode = config.rootNode();

        ConfigNode node = rootNode;
        ConfigNode lastNode;
        int i = 0;
        for ( i = 1; i <= 10; ++i ) {
            node = node.createChildNode( "NewNode" );

            my_assert( !node.hasValue(), "No value" );

            node.createChildNode( "TempNode" );

            my_assert( !node.hasValue() && 
                       node.childNodes().size() != 0, 
                       "No value and has a child" );

            node.setValue( "Value" );

            my_assert( node.hasValue() && 
                       node.childNodes().size() == 0, 
                       "Has value and children list is empty" );
            
            // Last node
            if ( i == 10 ) {
                node.setValue("Last Node");
                lastNode = node;
            }
        }

        my_assert( lastNode.getValue() == "Last Node",
                   "Correct last node value" );
        my_assert( lastNode.childNodes().size() == 0,
                   "No children for last node" );

        lastNode = lastNode.parentNode();

        while ( !lastNode.parentNode().isNull() ) {
            my_assert( lastNode.getValue() == QString() && 
                       lastNode.childNodes().size() != 0, 
                       "Value is empty and has children" );

            lastNode.setValue( "Value" );

            my_assert( lastNode.getValue() == "Value" && 
                       lastNode.childNodes().size() == 0, 
                       "Has value and children list is empty" );

            lastNode = lastNode.parentNode();
        }

        rootNode.removeAllChildNodes();

        my_assert( rootNode.childNodes().size() == 0,
                   "Root node should be empty" );

        testCaseEnd();
    }

    void tagNameChanging ()
    {
        testCaseBegin("tagNameChanging");

        Config& config = Config::instance( ConfigFileName );

        ConfigNode rootNode = config.rootNode();

        int i = 0;
        for ( i = 1; i <= 5; ++i ) {
            ConfigNode node = rootNode.createChildNode( "NewNode" );
            my_assert( node.getTagName() == "NewNode",
                       "Correct initial tag name" );
            node.setTagName( "OtherNewNode" );
            my_assert( node.getTagName() == "OtherNewNode",
                       "Correct tag name after change" );
            node.remove();
        }

        my_assert( rootNode.childNodes().size() == 0,
                   "Root node should be empty" );

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

int main ( int argc, char** argv )
{
    QCoreApplication app( argc, argv );

    ConfigTest test;

    test.start();
    
    app.exec();
    test.wait();    


    /*
      Test cases:
    
    #1. file creation (file removing)
    #2. file parse error
    #3. create nodes (+ threads, + signal catch)
    #4. remove nodes (+ threads, + signal catch)
    #4. remove all child nodes (+ threads, + signal catch)
    #5. find nodes 
    #6. changing attributes (+ threads, + signal catch)
    #7. remove attributes by name (+ threads, + signal catch)
    #7. clearing all attributes (+ threads, + signal catch)
    #8. setting value  (+ threads, + signal catch, +checking child removal)
    #8. resetting value  (+ threads, + signal catch)
    #9. tag name changing (+ threads, + signal catch)

    */

    test.lastReport();
   

    return test.status();
}
