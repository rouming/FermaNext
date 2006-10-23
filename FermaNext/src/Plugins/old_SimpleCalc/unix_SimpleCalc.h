
#ifndef UNIX_SIMPLECALC_H
#define UNIX_SIMPLECALC_H

#include <unistd.h>
#include <QTcpSocket>
#include <QHostAddress>
#include <QThread>
#include <QMutex>
#include <QProcess>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QQueue>
#include <QDateTime>

#include "old_SimpleCalc.h"

// Calculation thread
class CalcThread : public QThread 
{
    Q_OBJECT
public:
    CalcThread () :
        m_calculating(true)
    {}

    ~CalcThread ()
    {
        stopCalculating();
    }
    
    virtual void run()
    {
        // Wine call. Assuming wine is installed
        QProcess::startDetached("wine plugins/Simple_f.exe 1212");
          //> /dev/null 2>&1
        
        // Wait for server is up
        sleep(3);

        QTcpSocket socket;

        socket.connectToHost( QHostAddress("127.0.0.1"), 1212 );
        socket.waitForConnected();
        
        QString okMsg("OK\n");
            
        while ( m_calculating ) {
            
            QDataStream out( &socket );
            out.setVersion( QDataStream::Qt_4_0 );
            QString fileName = nextToCalculate();
            
            if ( fileName.isEmpty() )
                continue;
            
            out.writeRawData( qPrintable(fileName), fileName.length() );
            socket.waitForBytesWritten();
                
            while ( socket.bytesAvailable() < okMsg.length() )
                socket.waitForReadyRead();
            
            // Ok!
            socket.readAll();
            
            m_sync.wakeAll();
        }

        QString quitMsg("quit\n");
        
        QDataStream out( &socket );
        out.setVersion( QDataStream::Qt_4_0 );            
        out.writeRawData( qPrintable(quitMsg), quitMsg.length() );
        socket.flush();

        // Wait for server is down
        sleep(3);
    }

    void calculate ( const QString& fileName ) const
    {
        if ( fileName.isEmpty() )
            return;
        QMutexLocker locker(&m_mutex);
        m_calculateQueue.append(fileName);
        m_sync.wait(&m_mutex);
    }

public slots:
    void stopCalculating ()
    {
        m_calculating = false;
        while ( isRunning() )
            usleep(200);
    }
    
private:
    QString nextToCalculate () 
    {
        if ( m_calculateQueue.isEmpty() )
            return QString();
        return m_calculateQueue.dequeue();
    }

private:
    volatile bool m_calculating;
    mutable QMutex m_mutex;
    mutable QWaitCondition m_sync;
    mutable QQueue<QString> m_calculateQueue;    
};

class os_dependent_SimpleCalcPlugin : public SimpleCalcPlugin
{
public:    
    os_dependent_SimpleCalcPlugin ( PluginManager& mng, const QString& path ) :
        SimpleCalcPlugin( mng, path )
    {
        calcThread.start();
    }

    void startCalculation ( const QString& fileName ) const
    {
        // Send file name to calculation server
        QString fileToCalc( fileName + "\n" );
        calcThread.calculate( fileToCalc );
    }
    
private:
    CalcThread calcThread;
};

#endif //UNIX_SIMPLECALC_H
