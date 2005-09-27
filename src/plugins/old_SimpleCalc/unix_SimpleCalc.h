
#ifndef UNIX_SIMPLECALC_H
#define UNIX_SIMPLECALC_H

#include <unistd.h>
#include <qsocketdevice.h>
#include <qthread.h>

class UnixSimpleCalcPlugin : public SimpleCalcPlugin
{
private:
    // Calculation thread
    class CalcThread : public QThread 
    {
    public:
        virtual void run()
        {
            // Wine call. Assuming wine is installed
            system("wine plugins/Simple_f.exe 1212 "); //> /dev/null 2>&1
        }
    };

public:    
    UnixSimpleCalcPlugin ()
    {
        calcThread.start();
        // Should wait a little to be sure server is started
        sleep(3);        
        socket.connect( QHostAddress("127.0.0.1"), 1212 );
    }

    ~UnixSimpleCalcPlugin ()
    {
        QString msg("quit\n");
        socket.writeBlock( msg, msg.length() );
        socket.close();

        // Workaround of vague segmentation fault after direct call
        // of wait of calculation thread.
        class Waiter : public QThread 
        {
        public:
            Waiter ( QThread& t_ ) : t(t_) {}
            virtual void run()
            { t.wait(); }                
        private:
            QThread& t;
        };
        Waiter w(calcThread);
        w.start();
        w.wait();
    }

    void startCalculation ( const QString& fileName ) 
    {
        // Send file name to calculation server
        socket.writeBlock( fileName, fileName.length() ); 
    }
    
private:
    QSocketDevice socket;
    CalcThread calcThread;
};

UnixSimpleCalcPlugin plugin;

#endif //UNIX_SIMPLECALC_H
