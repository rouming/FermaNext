
#include <QStackedWidget>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QApplication>

#include "FermaNextWorkspace.h"
#include "Global.h"

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
    name( Global::untitledWorkspaceName ),
    fermaConfig( Global::config() ),
    pluginMng( fermaConfig ),
    fermaMainWindow(0),
    plgExecutorDialog(0),
    stackedWidget(0),
    clipboardObject(0)
{
    fermaMainWindow = new FermaNextMainWindow( *this );
    stackedWidget = new QStackedWidget( fermaMainWindow );
    plgExecutorDialog = new PluginExecutorDialog( pluginMng, fermaMainWindow );
    fermaMainWindow->setCentralWidget( stackedWidget );
}

FermaNextWorkspace::~FermaNextWorkspace ()
{
    quitFermaNextApplication();
    clearClipboard();
}

void FermaNextWorkspace::loadFromFile ( const QString& fileName )
    /*throw (IOException, WrongXMLDocException, LoadException)*/
{
    QFile xmlFile( fileName );
    if ( ! xmlFile.open( QIODevice::ReadOnly ) )
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
    /*throw (FileNameIsNotDefinedException,
             IOException, 
             ProjectFileNameIsNotDefinedException)*/
{
    const QString& fileName = getWorkspaceFileName();
    if ( ! isFileNameDefined() )
        throw FileNameIsNotDefinedException();
    
    saveToFile( fileName );
}

void FermaNextWorkspace::saveToFile ( const QString& fileName )
    /*throw (IOException, ProjectFileNameIsNotDefinedException)*/
{
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) {
        if ( ! (*iter)->isFileNameDefined() )
            throw ProjectFileNameIsNotDefinedException();
    }

    QFile xmlFile( fileName );
    if ( ! xmlFile.open( QIODevice::WriteOnly ) )
        throw IOException();

    setWorkspaceFileName( fileName );

    QTextStream stream( &xmlFile );

    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );

    QDomElement wspElement = saveToXML( doc );

    doc.appendChild( wspElement );
    doc.save( stream, 4 );
}

void FermaNextWorkspace::loadFromXML ( const QDomElement& wspElem,
                                       const QString& wspFileName )
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( wspElem );

    // Destroy existent projects
    reset();

    QFileInfo wspFileInfo( wspFileName );
    QDir workspaceDir( wspFileInfo.absoluteDir() );

    /**
     * Create projects
     ****************************/
    FermaNextProject* selectedProject = 0;
    
    QDomNodeList projects = wspElem.elementsByTagName( "FermaNextProject" );
    for ( int prjsNum = 0; prjsNum < projects.count(); ++prjsNum ) {
        QDomNode project = projects.item( prjsNum );
        if ( ! project.isElement() )
            throw LoadException();
        QDomElement projectElem = project.toElement();
        if ( ! projectElem.hasAttribute( "projectURL" ) )
            throw LoadException();
        QString prjUrl = projectElem.attribute( "projectURL" );
        QString prjFileName = workspaceDir.absoluteFilePath( prjUrl );
        QFileInfo prjFileInf( prjFileName );
        if ( ! prjFileInf.exists() || ! prjFileInf.isReadable() )
            throw LoadException();
        FermaNextProject& prj = createProject( QString() );
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
    QString wspDirPath = wspFileInfo.absolutePath();

    /**
     * Save projects URLs
     *********************/
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) {
        FermaNextProject* prj = *iter;
        const QString& prjFileName = prj->getProjectFileName();
        QString relPrjFileName = 
            Global::filePathToRelative( prjFileName, wspDirPath );
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
    for ( ; iter != projects.end(); )
        iter = removeProject( iter );

}

void FermaNextWorkspace::reset ()
{
    emit onReset();
    name = Global::untitledWorkspaceName;
    clearProjects();
    setWorkspaceFileName( "" );
}

void FermaNextWorkspace::quitFermaNextApplication ()
{
    // Manually destroy all projects.
    clearProjects();
    // Manually unregister all plugin loaders.
    pluginManager().unregisterPluginLoaders();

    qApp->exit();
}

void FermaNextWorkspace::projectIsActivated ( FermaNextProject& prj )
{
    emit onProjectActivated( prj );
}

FermaNextProject& FermaNextWorkspace::createProject ( const QString& name )
{
    FermaNextProject* project = new FermaNextProject( *this, name, 
                                                      stackedWidget );

    // Activation signal catch
    QObject::connect( project, SIGNAL(onActivate(FermaNextProject&)),
                      SLOT(projectIsActivated(FermaNextProject&)) );

    projects.push_back(project);
    emit onProjectCreate(*project);    
    return *project;
}

FermaNextProject& FermaNextWorkspace::createProjectFromFile ( 
    const QString& fileName ) 
    /*throw (LoadException)*/
{
    FermaNextProject& prj = createProject( QString() );
    try { prj.loadFromFile( fileName ); }
    catch ( ... ) {
        removeProject(prj);
        throw LoadException();
    }
    return prj;
}

FermaNextWorkspace::ProjectListIter FermaNextWorkspace::removeProject ( 
    ProjectListIter iter )
{
    if ( iter == projects.end() )
        return projects.end();
    FermaNextProject* prj = *iter;
    emit onBeforeProjectRemove(*prj);
    iter = projects.erase(iter);
    delete prj;
    emit onAfterProjectRemove();
    return iter;        
}

bool FermaNextWorkspace::removeProject ( FermaNextProject& project )
{    
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter ) 
        if ( (*iter) == &project ) {
            ProjectListIter removedIt = removeProject(iter);
            if ( removedIt == projects.end() )
                return false;
            else
                return true;
        }
    return false; 
}

bool FermaNextWorkspace::removeProject ( const QString& name )
{ 
    ProjectListIter iter = projects.begin();
    for ( ; iter != projects.end(); ++iter )
        if ( (*iter)->getName() == name ) {
            ProjectListIter removedIt = removeProject(iter);
            if ( removedIt == projects.end() )
                return false;
            else
                return true;
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

Config& FermaNextWorkspace::config ()
{
    return fermaConfig;
}

PluginManager& FermaNextWorkspace::pluginManager ()
{
    return pluginMng;
}

FermaNextMainWindow* FermaNextWorkspace::mainWindow () const
{
    return fermaMainWindow;
}

PluginExecutorDialog* FermaNextWorkspace::pluginExecutor () const
{
    return plgExecutorDialog;
}

void FermaNextWorkspace::setClipboardObject ( const QString& xmlStr,
                                              QStringList materials )
{
    clipboardObject = new ClipBoardObject( xmlStr, materials );
}

const ClipBoardObject* FermaNextWorkspace::getClipboardObject () const
{
    return clipboardObject;
}

void FermaNextWorkspace::clearClipboard ()
{
    delete clipboardObject;
}

/*****************************************************************************/
