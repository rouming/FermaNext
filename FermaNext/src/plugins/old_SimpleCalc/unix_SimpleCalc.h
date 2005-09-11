
#ifndef UNIX_SIMPLECALC_H
#define UNIX_SIMPLECALC_H

#include <unistd.h>

// Temp socket
QSocketDevice* tempSocket = 0;

// Calculation thread
class CalcThread : public QThread 
{
public:
    virtual void run()
    {
        // Wine call. Assuming wine is installed
        system("wine plugins/Simple_f.exe 1212");
    }
};


// Manages calculation server
QSocketDevice* startCalculationServer ()
{
    CalcThread* calc = new CalcThread;
    calc->start();
    // Should wait a little to be sure server is started
    sleep(3);
    QSocketDevice* socket = new QSocketDevice();
    socket->connect( QHostAddress("127.0.0.1"), 1212 );
    return socket;
}

void stopCalculationServer ( QSocketDevice*& socket )
{
    QString msg("quit\n");
    socket->writeBlock( msg, msg.length() );
    socket->close();
    delete socket;
    // Should wait a little
    sleep(2);
    socket = 0;
}

void os_dependent_init ()
{
    tempSocket = startCalculationServer();
}

void os_dependent_fini ()
{
    stopCalculationServer( tempSocket );
}

void os_dependent_calculation ()
{
    // Send file name to calculation server
    QString fileName( "/home/roman/devel/FermaNext/1-64578.frm\n" );
    tempSocket->writeBlock( fileName, fileName.length() );
}

#endif //UNIX_SIMPLECALC_H
