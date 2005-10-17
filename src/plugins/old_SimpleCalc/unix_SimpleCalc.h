
#ifndef UNIX_SIMPLECALC_H
#define UNIX_SIMPLECALC_H

#include <unistd.h>
#include <qsocketdevice.h>
#include <qthread.h>
#include <qdatetime.h>

class os_dependent_SimpleCalcPlugin : public SimpleCalcPlugin
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
    os_dependent_SimpleCalcPlugin ()
    {
        calcThread.start();
        // Should wait a little to be sure server is started
        sleep(3);        
        socket.connect( QHostAddress("127.0.0.1"), 1212 );
    }

    ~os_dependent_SimpleCalcPlugin ()
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

    void startCalculation ( const QString& fileName ) const
    {
        // Send file name to calculation server
        QString fileToCalc( fileName + "\n" );
        socket.writeBlock( fileToCalc, fileToCalc.length() );

        QString vyvFile( tempFileName() + fermaResExt );
        // Wait file appearance. 
        // 'Wine' should spend some time to flush the file.
        uint curr = QDateTime::currentDateTime().toTime_t();
        while ( ! QFile::exists( vyvFile ) && 
                5 > QDateTime::currentDateTime().toTime_t() - curr );
    }
    
private:
    mutable QSocketDevice socket;
    CalcThread calcThread;
};

#endif //UNIX_SIMPLECALC_H
