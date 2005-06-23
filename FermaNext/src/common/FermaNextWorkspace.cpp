
#include "FermaNextWorkspace.h"
#include "FermaNextMainFrame.h"

#include <qapplication.h>
#include <qwidgetstack.h>

const QString untitledName( QObject::tr("Unnamed workspace") );

/*****************************************************************************
 * FermaNext Workspace
 *****************************************************************************/

FermaNextWorkspace* FermaNextWorkspace::instance = 0;
QMutex FermaNextWorkspace::mutex;

FermaNextWorkspace::FermaNextWorkspace () :
    name(untitledName),
    widgetStack( new QWidgetStack() )
{
    // Singleton desist by Qt quit signal
    connect( qApp, SIGNAL(lastWindowClosed()), SLOT(clear()) );
}

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
    FermaNextProject* project = new FermaNextProject(name, widgetStack);
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

void FermaNextWorkspace::activateProject ( FermaNextProject& prj )
{
    widgetStack->raiseWidget( (QWidget*)&prj.getProjectTab() );
}

void FermaNextWorkspace::activateProject ( const QString& prjName )
{
    FermaNextProject* prj = findProjectByName(prjName);
    if ( prj )
        widgetStack->raiseWidget( (QWidget*)&prj->getProjectTab() );
}

FermaNextProject* FermaNextWorkspace::findProjectByName ( const QString& name )
{
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) 
        if ( (*iter)->getName() == name )
            return *iter;    
    return 0;
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

QWidgetStack& FermaNextWorkspace::getWidgetStack ()
{
    return *widgetStack;
}

FermaNextConfig& FermaNextWorkspace::config ()
{
    return fermaConfig;
}

/*****************************************************************************/
