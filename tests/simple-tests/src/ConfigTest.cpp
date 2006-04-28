
#include <QFile>
#include <QThread>
#include <QCoreApplication>

#include "../include/ConfigTestThread.h"
#include <iostream>

/*****************************************************************************/

ConfigTestThread::ConfigTestThread ( Config& cfg ) :
    config(cfg)
{
    QObject::connect( &config, 
                      SIGNAL(onNodeCreated(Config::Node)), 
                      SLOT(nodeCreated(Config::Node)),
                      Qt::QueuedConnection );
}

void ConfigTestThread::nodeChanged ( Config::Node )
{
    qWarning("Created");
}

void ConfigTestThread::nodeCreated ( Config::Node )
{
    qWarning("Created");
}

void ConfigTestThread::nodeRemoved ( Config::Node )
{
    qWarning("Created");
}

void ConfigTestThread::run ()
{
    ConfigNode rootNode = config.rootNode();    
    for ( uint i = 0; i < 10; ++i ) {
        rootNode.createChildNode( "1" );
        QCoreApplication::processEvents();
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

        //        QCoreApplication::quit();
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

        Config& config3 = Config::instance( ConfigFileName );
        my_assert( &config == &config3, "Instance should be new" );

        Config::destroyInstance( config3 );
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

        ConfigTestThread thread1( config );
        ConfigTestThread thread2( config );
        ConfigTestThread thread3( config );
        ConfigTestThread thread4( config );
        ConfigTestThread thread5( config );

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
