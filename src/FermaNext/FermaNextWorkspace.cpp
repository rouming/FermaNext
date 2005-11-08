
#include "FermaNextWorkspace.h"
#include "FermaNextMainFrame.h"
#include "SubsidiaryConstants.h"

#include <qwidgetstack.h>
#include <qfileinfo.h>
#include <qdir.h>

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

void FermaNextWorkspace::loadFromFile ( const QString& fileName )
    throw (IOException, WrongXMLDocException, LoadException)
{
    QFile xmlFile( fileName );
    if ( ! xmlFile.open( IO_ReadOnly ) )
        throw IOException();

    QIODevice* xmlIODev = &xmlFile;
    QDomDocument doc;
    if ( !doc.setContent( xmlIODev ) )
        throw WrongXMLDocException();

    QDomElement docElem = doc.documentElement();
    if ( docElem.isNull() )
        throw WrongXMLDocException();

    loadFromXML( docElem );

    setWorkspaceFileName( fileName );
}

void FermaNextWorkspace::saveToFile ( const QString& fileName )
    throw (IOException, ProjectIsNotSavedException)
{
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) {
        const QString& prjFileName = (*iter)->getProjectFileName();
        if ( prjFileName.isNull() || prjFileName.isEmpty() )
            throw ProjectIsNotSavedException();
    }

    QFile xmlFile( fileName );
    if ( ! xmlFile.open( IO_WriteOnly ) )
        throw IOException();

    QTextStream stream( &xmlFile );

    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );

    QDomElement wspElement = saveToXML( doc );

    doc.appendChild( wspElement );
    doc.save( stream, 4 );
    setWorkspaceFileName( fileName );
}

void FermaNextWorkspace::loadFromXML ( const QDomElement& wspElem )
    throw (LoadException)
{
    XMLSerializableObject::loadFromXML( wspElem );

    // Destroy existent projects
    reset();

    QFileInfo wspFileInfo( getWorkspaceFileName() );    
    QDir workspaceDir( wspFileInfo.dirPath(true) );

    /**
     * Create projects
     ****************************/
    FermaNextProject* selectedProject = 0;
    
    QDomNodeList projects = wspElem.elementsByTagName( "FermaNextProjects" );
    for ( uint prjsNum = 0; prjsNum < projects.count(); ++prjsNum ) {
        QDomNode project = projects.item( prjsNum );
        if ( ! project.isElement() )
            throw LoadException();
        QDomElement projectElem = project.toElement();
        if ( ! projectElem.hasAttribute( "projectURL" ) )
            throw LoadException();
        QString prjUrl = projectElem.attribute( "projectURL" );
        QString prjFileName = workspaceDir.filePath( prjUrl, true );
        QFileInfo prjFileInf( prjFileName );
        if ( ! prjFileInf.exists() || ! prjFileInf.isReadable() )
            throw LoadException();
        
    }

    // Activate project
    if ( selectedProject )
        selectedProject->activate();
}

QDomElement FermaNextWorkspace::saveToXML ( QDomDocument& doc )
{
    QDomElement wspElem = XMLSerializableObject::saveToXML( doc );
    wspElem.setTagName( "FermaNextWorkspace" );

    QFileInfo wspFileInfo( getWorkspaceFileName() );
    QDir workspaceDir( wspFileInfo.dirPath(true) );

    /**
     * Save projects URLs
     *********************/
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) {
        const QString& prjFileName = (*iter)->getProjectFileName();
        QDomElement prjElem = doc.createElement( "FermaNextProject" );        
        prjElem.setAttribute( "projectURL", prjFileName );
        wspElem.appendChild( prjElem );
    }
    return wspElem;
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

const QString& FermaNextWorkspace::getWorkspaceFileName () const
{
    return workspaceFileName;
}

void FermaNextWorkspace::setWorkspaceFileName ( const QString& fileName )
{
    workspaceFileName = fileName;
    emit onWorkspaceFileNameChange( workspaceFileName );
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
