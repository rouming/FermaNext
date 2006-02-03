
#include "SubsidiaryConstants.h"
#include <qstringlist.h>
#include <iostream>

int PASSED = 0, FAILED = 0;

void my_assert( bool val, const QString& str ) 
{
    if ( val ) ++PASSED;
    else ++FAILED;
    
    std::cout << (val ? "    OK: " : "NOT OK: ") << str.ascii() << "\n";
}

int main ()
{
    QString paths[][3] = 
    { 
        // file, basedir, result
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
    };

    for ( uint i = 0; i < sizeof(paths) / sizeof(paths[0]); ++i ) {
        QString rel = filePathToRelative( paths[i][0], paths[i][1] );
        QString abs = filePathToAbsolute( rel, paths[i][1] );
        std::cout << "Test " << i+1 << "\n-------\n";
        my_assert( rel == paths[i][2], 
                   QString("Relative \"%1\"").arg(rel) );
        my_assert( abs == paths[i][0], 
                   QString("Absolute \"%1\"").arg(abs) );
        std::cout << "\n";
    }


    std::cout << "\n" << "Passed: " << PASSED << "\n" << 
                         "Failed: " << FAILED << "\n";
    return FAILED;
}
