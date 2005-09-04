
#include "old_SimpleCalc.h"
#include "FermaNextPlugin.h"
#include <stdio.h>
#include <qfile.h>

/*****************************************************************************
 * Old Simple Calculation plugin (main init/fini routines)
 *****************************************************************************/

PluginInfo inf = { "SimpleOldCalc", "Just calculation", CALCULATION_PLUGIN };
FERMA_NEXT_PLUGIN(inf)

FERMA_NEXT_PLUGIN_INIT
{
    tempFile = createTempFile();
}

FERMA_NEXT_PLUGIN_FINI
{
    destroyTempFile( tempFile );
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

/*****************************************************************************/
