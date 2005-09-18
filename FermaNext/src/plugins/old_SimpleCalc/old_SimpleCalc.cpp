
#include "old_SimpleCalc.h"
#include "Plugin.h"
#include <stdio.h>
#include <qsocketdevice.h>
#include <qfile.h>

#if defined WINDOWS || defined WIN32  
  #include "win_SimpleCalc.h"
#else
  #include "unix_SimpleCalc.h"
#endif

/*****************************************************************************
 * Old Simple Calculation plugin (main init/fini routines)
 *****************************************************************************/

PluginInfo inf = { "SimpleOldCalc", "Just calculation", CALCULATION_PLUGIN };
FERMA_NEXT_PLUGIN(inf)

FERMA_NEXT_PLUGIN_INIT
{
    os_dependent_init();
    tempFile = createTempFile();
}

FERMA_NEXT_PLUGIN_FINI
{
    os_dependent_fini();
    destroyTempFile( tempFile );
}

PluginExport void StandardCall calculation ()
{
    os_dependent_calculation();
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
