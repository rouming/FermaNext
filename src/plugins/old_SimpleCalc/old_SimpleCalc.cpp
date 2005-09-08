
#include "old_SimpleCalc.h"
#include "FermaNextPlugin.h"
#include <stdio.h>
#include <qsocket.h>
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
    // write to calculation server
    QTextStream os(tempSocket);
    os << "/home/roman/devel/FermaNext/1-64578.frm" << "\n";    
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

QSocket* startCalculationServer ()
{
    CalcThread* calc = new CalcThread;
    calc->start();
    sleep_seconds(3);
    QSocket* socket = new QSocket();
    socket->connectToHost( "localhost", 1212 );
    return socket;
}

void stopCalculationServer ( QSocket*& socket )
{
    QTextStream os(socket);
    os << "quit";
    socket->close();
    sleep_seconds(3);
    delete socket;
    socket = 0;
}

/*****************************************************************************/
