
#include "FermaNextWorkspace.h"
#include "FermaNextMainFrame.h"
#include "SubsidiaryConstants.h"

#include <qwidgetstack.h>

/*****************************************************************************
 * FermaNext Workspace
 *****************************************************************************/

const QString FermaNextWorkspace::FormatExtension = ".fws";

/*****************************************************************************/

FermaNextWorkspace* FermaNextWorkspace::instance = 0;
QMutex FermaNextWorkspace::mutex;

FermaNextWorkspace::FermaNextWorkspace () :
    name( untitledWorkspaceName ),
    widgetStack(0),
    fermaConfig( configFileName() )
{}

FermaNextWorkspace::~FermaNextWorkspace ()
{
    clearProjects();
}

void FermaNextWorkspace::loadFromXML ( const QDomElement& elem )
    throw (LoadException)
{
    XMLSerializableObject::loadFromXML( elem );

    // Destroy existent projects
    reset();
}

QDomElement FermaNextWorkspace::saveToXML ( QDomDocument& doc )
{
    return XMLSerializableObject::saveToXML( doc );
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

void FermaNextWorkspace::clearProjects ()
{
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ) {
        bool res = removeProject(iter);
        if ( ! res )
            ++iter;
    }
}

void FermaNextWorkspace::reset ()
{
    emit onReset();
    name = untitledWorkspaceName;
    clearProjects();
}

void FermaNextWorkspace::createWidgetStack ( QMainWindow& parent )
{
    if ( widgetStack )
        return;
    widgetStack = new QWidgetStack(&parent);
    parent.setCentralWidget(widgetStack);
}

FermaNextProject& FermaNextWorkspace::createProject ( const QString& name )
    throw (WorkspaceIsNotInitedCorrectly)
{
    // Assuming widget stack was created.
    if ( widgetStack == 0 )
        throw WorkspaceIsNotInitedCorrectly();
    FermaNextProject* project = new FermaNextProject(name, widgetStack);
    projects.push_back(project);
    emit onProjectCreate(*project);    
    return *project;
}

FermaNextProject& FermaNextWorkspace::createProject ( QDomElement& elem ) 
    throw (WorkspaceIsNotInitedCorrectly, LoadException)
{
    FermaNextProject& prj = createProject( QString::null );
    try { prj.loadFromXML( elem ); }
    catch ( LoadException& ) {
        removeProject(prj);
        throw;
    }
    return prj;
}

bool FermaNextWorkspace::removeProject ( ProjectListIter& iter )
{
    if ( iter == projects.end() )
        return false;
    FermaNextProject* prj = *iter;
    emit onBeforeProjectRemove(*prj);
    projects.erase(iter);
    delete prj;
    emit onAfterProjectRemove();
    return true;        
}

bool FermaNextWorkspace::removeProject ( FermaNextProject& project )
{    
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) 
        if ( (*iter) == &project )
            return removeProject(iter);    
    return false; 
}

bool FermaNextWorkspace::removeProject ( const QString& name )
{ 
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) 
        if ( (*iter)->getName() == name ) {
            return removeProject( iter );
    }
    return false; 
}

void FermaNextWorkspace::activateProject ( const QString& prjName )
{
    FermaNextProject* prj = findProjectByName(prjName);
    if ( prj )
        prj->activate();
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

FermaNextConfig& FermaNextWorkspace::config ()
{
    return fermaConfig;
}

bool FermaNextWorkspace::loadPlugins ()
{
    plgManager.loadPlugins( pluginsPath() );
    return true;
}

PluginManager& FermaNextWorkspace::pluginManager ()
{
    return plgManager;
}

/*****************************************************************************/
