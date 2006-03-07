
#include <QWidgetStack>
#include <QFileInfo>
#include <QDir>

#include "FermaNextWorkspace.h"
#include "FermaNextMainFrame.h"
#include "SubsidiaryConstants.h"

/*****************************************************************************
 * FermaNext Workspace
 *****************************************************************************/

FermaNextWorkspace* FermaNextWorkspace::instance = 0;
QMutex FermaNextWorkspace::mutex;

/*****************************************************************************/

const QString& FermaNextWorkspace::formatExtension ()
{
    static QString extension = ".fws";
    return extension;
}

/*****************************************************************************/

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

    loadFromXML( docElem, fileName );

    setWorkspaceFileName( fileName );
}

void FermaNextWorkspace::saveToFile ()
    throw (FileNameIsNotDefinedException,
           IOException, 
           ProjectFileNameIsNotDefinedException)
{
    const QString& fileName = getWorkspaceFileName();
    if ( ! isFileNameDefined() )
        throw FileNameIsNotDefinedException();
    
    saveToFile( fileName );
}

void FermaNextWorkspace::saveToFile ( const QString& fileName )
    throw (IOException, ProjectFileNameIsNotDefinedException)
{
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) {
        if ( ! (*iter)->isFileNameDefined() )
            throw ProjectFileNameIsNotDefinedException();
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

void FermaNextWorkspace::loadFromXML ( const QDomElement& wspElem,
                                       const QString& wspFileName )
    throw (LoadException)
{
    XMLSerializableObject::loadFromXML( wspElem );

    // Destroy existent projects
    reset();

    QFileInfo wspFileInfo( wspFileName );
    QDir workspaceDir( wspFileInfo.dirPath(true) );

    /**
     * Create projects
     ****************************/
    FermaNextProject* selectedProject = 0;
    
    QDomNodeList projects = wspElem.elementsByTagName( "FermaNextProject" );
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
        FermaNextProject& prj = createProject( QString::null );
        try { prj.loadFromFile( prjFileName ); }
        catch ( ... ) {
            throw LoadException();
        }
        if ( projectElem.hasAttribute( "selected" ) &&
             projectElem.attribute( "selected" ) == "Yes" ) {
            selectedProject = &prj;
        }
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
    QString wspDirPath = wspFileInfo.dirPath(true);

    /**
     * Save projects URLs
     *********************/
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) {
        FermaNextProject* prj = *iter;
        const QString& prjFileName = prj->getProjectFileName();
        QString relPrjFileName = filePathToRelative( prjFileName, wspDirPath );
        QDomElement prjElem = doc.createElement( "FermaNextProject" );        
        prjElem.setAttribute( "projectURL", relPrjFileName );
        if ( prj->isActivated() )
            prjElem.setAttribute( "selected", "Yes" );
        else
            prjElem.removeAttribute( "selected" );
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

FermaNextWorkspace::ProjectList FermaNextWorkspace::getProjectList () const
{
    return projects;
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
    setWorkspaceFileName( "" );
}

void FermaNextWorkspace::projectIsActivated ( FermaNextProject& prj )
{
    emit onProjectActivated( prj );
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

    // Activation signal catch
    QObject::connect( project, SIGNAL(onActivate(FermaNextProject&)),
                      SLOT(projectIsActivated(FermaNextProject&)) );

    projects.push_back(project);
    emit onProjectCreate(*project);    
    return *project;
}

FermaNextProject& FermaNextWorkspace::createProjectFromFile ( 
    const QString& fileName ) 
    throw (WorkspaceIsNotInitedCorrectly, LoadException)
{
    FermaNextProject& prj = createProject( QString::null );
    try { prj.loadFromFile( fileName ); }
    catch ( ... ) {
        removeProject(prj);
        throw LoadException();
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

FermaNextProject* FermaNextWorkspace::currentActivatedProject () const
{
    ProjectListConstIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter )
        if ( (*iter)->isActivated() )
            return *iter;
    return 0;
}

FermaNextProject* FermaNextWorkspace::findProjectByName ( 
    const QString& name ) const
{
    ProjectListConstIter iter = projects.begin();
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

bool FermaNextWorkspace::isFileNameDefined () const
{
    return (! workspaceFileName.isNull() && ! workspaceFileName.isEmpty() );
}

void FermaNextWorkspace::setWorkspaceFileName ( const QString& fileName )
{
    if ( workspaceFileName != fileName ) {
        workspaceFileName = fileName;
        emit onWorkspaceFileNameChange( workspaceFileName );
    }
}

FermaNextConfig& FermaNextWorkspace::config ()
{
    return fermaConfig;
}

PluginManager& FermaNextWorkspace::pluginManager ()
{
    return PluginManager::instance();
}

/*****************************************************************************/
