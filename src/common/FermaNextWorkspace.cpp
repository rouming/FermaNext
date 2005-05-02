
#include "FermaNextWorkspace.h"

#include <qworkspace.h>

FermaNextWorkspace* FermaNextWorkspace::instance = 0;
QMutex FermaNextWorkspace::mutex;

const QString untitledName( QObject::tr("Unnamed workspace") );

/*****************************************************************************
 * FermaNext Workspace
 *****************************************************************************/

FermaNextWorkspace::FermaNextWorkspace () :
    name(untitledName),
    qWsp(0)
{}

FermaNextWorkspace::~FermaNextWorkspace ()
{
    clear();
}

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

void FermaNextWorkspace::clear ()
{
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter )       
      delete *iter;    
    projects.clear();
}

void FermaNextWorkspace::reset ()
{
    emit onReset();
    name = untitledName;    
    clear();    
}

FermaNextProject& FermaNextWorkspace::createProject ( const QString& name )
{
    FermaNextProject* project = new FermaNextProject(name, qWsp);
    projects.push_back(project);
    emit onProjectCreate(*project);    
    return *project;
}

bool FermaNextWorkspace::removeProject ( FermaNextProject& project )
{    
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) 
        if ( (*iter) == &project ) {  
            FermaNextProject* prj = *iter;
            emit onProjectRemove(*prj);
            projects.erase(iter);
            delete prj;
            return true;
    }
    return false; 
}

bool FermaNextWorkspace::removeProject ( const QString& name )
{ 
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) 
        if ( (*iter)->getName() == name ) {  
            FermaNextProject* prj = *iter;
            emit onProjectRemove(*prj);
            projects.erase(iter);
            delete prj;
            return true;
    }
    return false; 
}

int FermaNextWorkspace::countProjects () const
{
    return (int)projects.size();
}

FermaNextProject& FermaNextWorkspace::getProject ( int index )
{
    FermaNextProject* prj = projects.at((size_t)index);
    return *prj;
}

const QString& FermaNextWorkspace::getName () const
{
    return name;
}

void FermaNextWorkspace::setName ( const QString& name_ )
{
    name = name_;
    emit onNameChange(name);
}

void FermaNextWorkspace::setWorkspaceWidget ( QWorkspace& qWsp_ )
{
    qWsp = &qWsp_;    
}

/*****************************************************************************/
