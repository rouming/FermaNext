
#include <QFile>
#include <QThread>
#include <QCoreApplication>

#include "../include/ConfigTestThread.h"
#include <iostream>

/*****************************************************************************/

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
        
        thread1.setConfig( &config );
        thread2.setConfig( &config );
        thread3.setConfig( &config );
        thread4.setConfig( &config );
        thread5.setConfig( &config );


        thread1.start();
        thread2.start();
        thread3.start();
        thread4.start();
        thread5.start();


        thread1.wait();
        thread2.wait();
        thread3.wait();
        thread4.wait();
        thread5.wait();

        testCaseEnd();
    }

protected:
    virtual bool event ( QEvent* e )
    { 
        if ( !isRunning() ) {
            start();
            return true;
        }
        return QObject::event(e);
    }


private:
    uint passed;
    uint failed;

    ConfigTestThread thread1;
    ConfigTestThread thread2;
    ConfigTestThread thread3;
    ConfigTestThread thread4;
    ConfigTestThread thread5;
};

int main ( int argc, char** argv )
{
    QCoreApplication app( argc, argv );

    ConfigTest test;
    test.start();
    
    app.exec();
    test.wait();    

    /*

    #1. file creation (file removing)
    #2. file parse error
    3. create nodes (+ threads, + signal catch)
    4. remove nodes (+ threads, + signal catch)
    4. remove all child nodes (+ threads, + signal catch)
    5. find nodes 
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
