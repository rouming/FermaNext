
#include "FermaNextWorkspace.h"

FermaNextWorkspace* FermaNextWorkspace::instance = 0;
QMutex FermaNextWorkspace::mutex;

FermaNextWorkspace::FermaNextWorkspace ()
{}

FermaNextWorkspace::~FermaNextWorkspace ()
{}

FermaNextWorkspace& FermaNextWorkspace::workspace ()
{
    if ( instance == 0 ) {
        mutex.lock();
        if ( instance == 0 ) 
            instance = new FermaNextWorkspace;
        mutex.unlock();
    }
    return *instance;
}

void FermaNextWorkspace::reset ()
{}

FermaNextProject& FermaNextWorkspace::createProject ( const QString& name )
{
    return *new FermaNextProject(name);
}

bool FermaNextWorkspace::removeProject ( FermaNextProject& )
{
    return false;
}

bool FermaNextWorkspace::removeProject ( const QString& name )
{ 
    return false;
}


