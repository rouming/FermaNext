
#ifndef WIN_SIMPLECALC_H
#define WIN_SIMPLECALC_H

#include "DynaLoader.h"

const QString calcDLLName = "plugins/win_Simple_f.dll";
const QString calcProcName = "SimpleFerma";
typedef void  ( __stdcall * SimpleFerma ) ( const char* );

// Calculation loader
DynaLoader* loader = 0;
SimpleFerma simpleCalc = 0;

void os_dependent_init ()
{
    loader = new DynaLoader( calcDLLName );
    simpleCalc = (SimpleFerma)loader->getProcAddress( calcProcName );    
}

void os_dependent_fini ()
{
    delete loader;
}

void os_dependent_calculation ()
{
    simpleCalc( "1.frm" );
}


#endif //WIN_SIMPLECALC_H
