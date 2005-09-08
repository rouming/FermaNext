
#ifndef OLD_SIMPLECALC_H
#define OLD_SIMPLECALC_H

#include <qthread.h>

#if defined WINDOWS || defined WIN32  
  #include <windows.h>
  #define sleep_seconds(secs) Sleep(secs*1000)
#else
  #include <unistd.h>
  #define sleep_seconds(secs) sleep(secs)
#endif

class QFile;
class QSocket;

// Temp file
QFile* tempFile = 0;
// Temp socket
QSocket* tempSocket = 0;

const char* fermaResExt = ".vyv";

// Manages temp file
QFile* createTempFile ();
void destroyTempFile ( QFile*& );

// Manages calculation server
QSocket* startCalculationServer ();
void stopCalculationServer ( QSocket*& );


// Calculation thread
class CalcThread : public QThread 
{
public:
    virtual void run()
    {
#if defined WINDOWS || defined WIN32  
        system("plugins/Simple_f.exe 1212");
#else
        // Wine call. Assuming it is installed
        system("wine plugins/Simple_f.exe 1212");
#endif
    }
};



#endif //OLD_SIMPLECALC_H
