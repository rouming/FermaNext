
#include <QFile>

#include "Config.h"
#include <iostream>

const QString ConfigFileName( "../build/config.xml" );

class ConfigTest
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


    //////////////////////////////////////////////////////////////////////////
    // TEST CASES
    //////////////////////////////////////////////////////////////////////////

    void configCreation ()
    {
        testCaseBegin("configCreation");

        QFile::remove( ConfigFileName );
        Config& config = Config::instance( ConfigFileName );
        my_assert( QFile::exists(ConfigFileName), 
                   "Config file should be created" );

        Config& config2 = Config::instance( ConfigFileName );
        my_assert( &config == &config2, "Instances should be unique" );
                
        testCaseEnd();
    }


private:
    uint passed;
    uint failed;
};

int main ()
{
    ConfigTest test;
    test.configCreation();

    /*

    1. file creation
    2. file parse (+ error)
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
