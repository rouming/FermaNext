
#include "old_SimpleCalc.h"
#include "FermaNextPlugin.h"
#include <stdio.h>
#include <qsocketdevice.h>
#include <qfile.h>

/*****************************************************************************
 * Old Simple Calculation plugin (main init/fini routines)
 *****************************************************************************/

PluginInfo inf = { "SimpleOldCalc", "Just calculation", CALCULATION_PLUGIN };
FERMA_NEXT_PLUGIN(inf)

FERMA_NEXT_PLUGIN_INIT
{
    tempFile = createTempFile();
    tempSocket = startCalculationServer();
}

FERMA_NEXT_PLUGIN_FINI
{
    stopCalculationServer( tempSocket );
    destroyTempFile( tempFile );
}

PluginExport void StandardCall calculation ()
{
    // Send file name to calculation server
    QString fileName( "/home/roman/devel/FermaNext/1-64578.frm\n" );
    tempSocket->writeBlock( fileName, fileName.length() );
}

/*****************************************************************************/

QFile* createTempFile ()
{
    char* fileName = tmpnam(0);
    if ( fileName == 0 )
        return 0;
    QFile* file = new QFile( fileName );
    file->open( IO_WriteOnly );
    return file;
}

void destroyTempFile ( QFile*& file )
{
    if ( file ) {
        QFile::remove( file->name() + fermaResExt );
        file->remove();
        delete file;
        file = 0;        
    }
}

QSocketDevice* startCalculationServer ()
{
    CalcThread* calc = new CalcThread;
    calc->start();
    sleep_seconds(3);
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
    sleep_seconds(2);
    socket = 0;
}

/*****************************************************************************/
