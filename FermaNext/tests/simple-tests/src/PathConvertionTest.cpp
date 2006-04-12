
#include <iostream>
#include <QStringList>

#include "Global.h"


int PASSED = 0, FAILED = 0;

void my_assert( bool val, const QString& str ) 
{
    if ( val ) ++PASSED;
    else ++FAILED;
    
    std::cout << (val ? "    OK: " : "NOT OK: ") << qPrintable( str ) << "\n";
}

int main ()
{
    //FORMAT: file, basedir, result
    QString paths[][3] = 
    { 
        // original tests
        { "/some/path/dir/file", "/some/path/dir", "file" },
        { "/some/path/dir/file", "/some/path/dir/", "file" },

        { "/some/path/dir/file", "/some/path",  "dir/file" },
        { "/some/path/dir/file", "/some/",  "path/dir/file" },
        { "/some/path/dir/file", "/",  "some/path/dir/file" },
        { "/some/path/dir/file", "",  "some/path/dir/file" },

        { "/some/path/dir/file", "/some/path/dir/another/dir", "../../file" },
        { "/some/path/dir/file", "/some/path/dir/another/dir/", "../../file" },

        { "/some/path/dir/file", "/some/path/dir/another/dir", "../../file" },
        { "/some/path/dir/file", "/some/path/dir/another/", "../file" },

        // different separators
        { "\\some\\path/dir/file", "\\some/path",  "dir/file" },
        { "/some/path\\dir\\file", "/some\\",  "path/dir/file" },
        { "\\some/path/dir\\file", "\\",  "some/path/dir/file" },
        { "\\some/path/dir/file", "",  "some/path/dir/file" },

        // windows tests
        { "C:\\some\\path\\dir\\file", "C:\\some\\path\\dir", "file" },
        { "C:\\some\\path\\dir\\file", "C:\\some\\path\\dir\\", "file" },
        { "C:\\some\\path\\dir\\file", "C:\\some\\path",  "dir/file" },
        { "C:\\some\\path\\dir\\file", "C:\\some\\",  "path/dir/file" },
        { "C:\\some\\path\\dir\\file", "C:\\some\\path\\dir\\another\\dir", 
                                       "../../file" },
        { "C:\\some\\path\\dir\\file", "C:\\some\\path\\dir\\another\\dir\\", 
                                       "../../file" },

        { "C:\\some\\path\\dir\\file", "D:\\some\\path\\dir", 
                                       "C:/some/path/dir/file" },
        { "C:\\some\\path\\dir\\file", "D:\\some\\path\\dir\\another\\dir", 
                                       "C:/some/path/dir/file" },
    };

    for ( uint i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i ) {
        QString rel = filePathToRelative( paths[i][0], paths[i][1] );
        QString abs = filePathToAbsolute( rel, paths[i][1] );
        QString fileName = paths[i][0].replace('\\', '/');
        std::cout << "Test " << i+1 << "\n-------\n";
        my_assert( rel == paths[i][2], 
                   QString("Relative \"%1\"").arg(rel) );
        my_assert( abs == fileName, 
                   QString("Absolute \"%1\"").arg(abs) );
        std::cout << "\n";
    }


    std::cout << "\n" << "Passed: " << PASSED << "\n" << 
                         "Failed: " << FAILED << "\n";
    return FAILED;
}
