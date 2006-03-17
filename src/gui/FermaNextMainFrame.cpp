
#include "FermaNextMainFrame.h"
#include "FermaNextWorkspace.h"
#include "ProjectToolBox.h"
#include "SubsidiaryConstants.h"
#include "UndoRedoListBox.h"

#include <QApplication>
#include <QMenu>
#include <QMenuBar>
#include <QShortcut>
#include <QToolBox>
#include <QPainter>
#include <QStackedWidget>
#include <QStyleFactory>
#include <QAction>
#include <QSignalMapper>
#include <QDir>
#include <QTextCodec>
#include <QButtonGroup>
#include <QToolButton>
#include <QStatusBar>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog> 
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopWidget>
#include <QDockWidget>

const QString mainFrameCaption( QObject::tr( "Educational CAD System 'Ferma'" ) );

/*****************************************************************************
 * FermaNext Main Frame
 *****************************************************************************/

FermaNextMainFrame::FermaNextMainFrame ( QWidget* p, Qt::WFlags f ) :
    QMainWindow(p, f),
    pluginsMenu(0)
{      
    init();

    // Initial toolbar setup
    setupFileActions();
    setupEditActions();
    setupProjectActions();
    setupWindowActions();
    setupPluginActions();
    setupHelpActions();
}

/*
 *  Last cleanup before quit.
 */
void FermaNextMainFrame::cleanBeforeQuit ()
{
    // Manually destroy all projects.
    FermaNextWorkspace::workspace().clearProjects();
    // Manually unregister all plugin loaders.
    PluginManager::instance().unregisterPluginLoaders();
}

void FermaNextMainFrame::init ()
{
    // Do not move this line. Should be the first for correct inition.
    FermaNextWorkspace::workspace().createWidgetStack( *this );

    setWindowTitle(mainFrameCaption);
    setMinimumSize( 640, 480 );
    statusBar()->addWidget(new QLabel( tr("Ready"), statusBar() ));

    undoRedoHistoryWidget = new QWidget( this,
                                        // FIXME QT3TO4
                                        // WStyle_Tool | WType_TopLevel 
                                        Qt::Window | Qt::Tool );

    int undoRedoWidth = 140, undoRedoHeight = 110;
    undoRedoHistoryWidget->setFixedSize( undoRedoWidth, undoRedoHeight );
    // Pretty history widget offset from the end point of the screen
    undoRedoHistoryWidget->move( QApplication::desktop()->width() - 
                                 int(undoRedoWidth*1.5), 
                                 0 + int(undoRedoHeight*1.5) );
    undoRedoHistoryWidget->setWindowTitle( tr("History") );

    QVBoxLayout* vboxHistoryWidget = new QVBoxLayout( undoRedoHistoryWidget );
    undoRedoListBox = new UndoRedoListBox( undoRedoHistoryWidget );
    vboxHistoryWidget->addWidget( undoRedoListBox );

    projectsDockWindow = new QDockWidget( // FIXME QT3TO4 QDockWidget::InDock, 
                                          this );
    //FIXME QT3TO4
    //projectsDockWindow->setResizeEnabled( TRUE );
    //projectsDockWindow->setVerticalStretchable( TRUE );
    projectsDockWindow->setWindowTitle( tr("Truss Unit Projects") );
    //FIXME QT3TO4
    // addDockWindow
    setCentralWidget( projectsDockWindow );
    //FIXME QT3TO4
    //setDockEnabled( projectsDockWindow, DockTop, FALSE );
    //setDockEnabled( projectsDockWindow, DockBottom, FALSE );
    //projectsDockWindow->setCloseMode( QDockWidget::Always );

    FermaNextWorkspace& ws = FermaNextWorkspace::workspace();
    projectToolBox = new ProjectToolBox( ws, projectsDockWindow );
    connect( &ws, SIGNAL(onBeforeProjectRemove(FermaNextProject&)), 
                  SLOT(someProjectRemoved(FermaNextProject&)) );
    connect( &ws, SIGNAL(onProjectCreate(FermaNextProject&)), 
                  SLOT(someProjectCreated(FermaNextProject&)) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshUndoRedoActions()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshProjectActions()) );
    projectsDockWindow->setWidget( projectToolBox );
    //FIXME QT3TO4
    //projectsDockWindow->setFixedExtentWidth( 160 );
    projectsDockWindow->hide();

    //FIXME QT3TO4
    //setRightJustification( true );
}

void FermaNextMainFrame::someProjectRemoved ( FermaNextProject& prj )
{
    if ( 1 == FermaNextWorkspace::workspace().countProjects() ) {
        projectsDockWindow->hide();
        undoRedoHistoryWidget->hide();
    }
    TrussUnitDesignerWidget& designerWidget = 
        prj.getDesignerWindow().getDesignerWidget();
    designerWidget.disconnect( this );
}

void FermaNextMainFrame::someProjectCreated ( FermaNextProject& prj )
{
    if ( !projectsDockWindow->isVisible() && 
        0 < FermaNextWorkspace::workspace().countProjects() ) {
        projectsDockWindow->show();
        undoRedoHistoryWidget->show();
    }

    TrussUnitDesignerWidget& designerWidget = 
        prj.getDesignerWindow().getDesignerWidget();

    connect( &designerWidget, SIGNAL(onFocusLose(TrussUnitWindow&)),
                            SLOT(trussWindowLostFocus(TrussUnitWindow&)) );
    connect( &designerWidget, SIGNAL(onFocusReceive(TrussUnitWindow&)),
                            SLOT(trussWindowReceivedFocus(TrussUnitWindow&)) );
}

void FermaNextMainFrame::createProject ()
{
    bool ok;
    QString text = QInputDialog::getText(
            this,
            tr("Project creation"), 
            tr("Enter project name:"), 
            QLineEdit::Normal,
            QString(), &ok );
    if ( ok && !text.isEmpty() ) {
        FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();
        FermaNextProject& prj = wsp.createProject( text );
        TrussUnitWindowManager& mng = prj.getTrussUnitWindowManager();

//TODO: remove this in future
/*********** TEMP TRUSS UNIT **************************/
#ifndef NDEBUG
        TrussUnitWindow& trussWindow = mng.createTrussUnitWindow("Truss unit");
        trussWindow.setTrussAreaSize( DoubleSize( 300, 300 ) );

        TrussNode& node1 = trussWindow.createNode ( 280, 30 );
        TrussNode& node2 = trussWindow.createNode( 0, 0 );
        TrussNode& node3 = trussWindow.createNode( 130, 130 );
        trussWindow.createPivot( node2, node3 );

        TrussNode& node4 = trussWindow.createNode( 250, 300 );
        trussWindow.createPivot ( node4, node3 );

        TrussNode& node5 = trussWindow.createNode( 0, 300 );
        trussWindow.createPivot ( node5, node3 );

        trussWindow.createPivot ( node5, node4 );

        trussWindow.createPivot ( node5, node2 );

        node1.setFixation( Node::FixationByX );
        node2.setFixation( Node::FixationByY );
        node3.setFixation( Node::FixationByXY );

        TrussUnit::LoadCase& currentCase = 
            trussWindow.getLoadCases().createLoadCase();
        trussWindow.getLoadCases().setCurrentLoadCase ( currentCase );
        currentCase.addLoad ( node4, 300, 100 );
#endif
/*********** TEMP TRUSS UNIT **************************/

    } else {
        // user entered nothing or pressed Cancel
    }
}

/*****************************************************************************
 * Initial ToolBar And Actions Setup
 *****************************************************************************/

void FermaNextMainFrame::setupFileActions ()
{
    //FIXME QT3TO4 whole change
    QToolBar* tb = addToolBar( tr("File Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&File" ) );

    QAction *a;
    // New
    a = new QAction( QIcon( imagesPath() + "/filenew.xpm" ), 
                     tr( "&New..." ), this );
    a->setShortcut( tr("Ctrl+N") );
    a->setStatusTip( tr("Create new project") );
    connect( a, SIGNAL(activated()), SLOT(fileNew()) );
    tb->addAction( a );
    menu->addAction( a );

    // Open
    a = new QAction( QIcon( imagesPath() + "/fileopen.xpm" ),
                     tr( "&Open..." ), this );
    a->setShortcut( tr("Ctrl+O") );
    a->setStatusTip( tr("Open project") );
    connect( a, SIGNAL(activated()), SLOT(fileOpen()) );
    tb->addAction( a );
    menu->addAction( a );

    // Save
    saveProjectAction = new QAction( QIcon( imagesPath() + "/filesave.xpm" ),
                                     tr( "&Save..." ), this );
    saveProjectAction->setShortcut( tr("Ctrl+S") );
    saveProjectAction->setStatusTip( tr("Save project") );
    connect( saveProjectAction, SIGNAL(activated()), SLOT(fileSave()) );
    tb->addAction( saveProjectAction );
    menu->addAction( saveProjectAction );
    saveProjectAction->setDisabled(true);
    menu->addSeparator();

    // Save As
    saveAsProjectAction = new QAction( tr( "Save &As..." ), this );
    saveAsProjectAction->setStatusTip( tr("Save project as") );
    connect( saveAsProjectAction, SIGNAL(activated()), SLOT(fileSaveAs()) );
    menu->addAction( saveAsProjectAction );
    saveAsProjectAction->setDisabled(true);

    // Close
    closeProjectAction = new QAction( tr( "&Close" ), this );
    closeProjectAction->setStatusTip( tr("Close project") );
    connect( closeProjectAction, SIGNAL(activated()), SLOT(fileClose()) );
    menu->addAction( closeProjectAction );
    closeProjectAction->setDisabled(true);
    menu->addSeparator();

    // Open Wsp
    a = new QAction( tr( "Open Workspace..." ), this );
    a->setShortcut( tr("Shift+F2") );
    a->setStatusTip( tr("Open workspace") );
    connect( a, SIGNAL(activated()), SLOT(fileOpenWsp()) );
    menu->addAction( a );

    // Save Wsp
    a = new QAction( tr( "Save Workspace" ), this );
    a->setShortcut( tr("Shift+F3") );
    a->setStatusTip( tr("Save workspace") );
    connect( a, SIGNAL(activated()), SLOT(fileSaveWsp()) );
    menu->addAction( a );

    // Save Wsp As
    a = new QAction( tr( "Save Workspace As..." ), this );
    a->setStatusTip( tr("Save workspace as") );
    connect( a, SIGNAL(activated()), SLOT(fileSaveWspAs()) );
    menu->addAction( a );

    // Close Wsp
    a = new QAction( tr( "Close Workspace" ), this );
    a->setShortcut( tr("Shift+F4") );
    a->setStatusTip( tr("Close workspace") );
    connect( a, SIGNAL(activated()), SLOT(fileCloseWsp()) );
    menu->addAction( a );
    menu->addSeparator();

    // Save All
    a = new QAction( tr( "Save All" ), this );
    a->setShortcut( tr("Ctrl+Shift+S") );
    a->setStatusTip( tr("Save workspace and projects") );
    connect( a, SIGNAL(activated()), SLOT(fileSaveAll()) );
    menu->addAction( a );
    menu->addSeparator();

    // Page Setup
    a = new QAction( tr( "Page setup..." ), this );
    a->setStatusTip( tr("Satup page") );
    connect( a, SIGNAL(activated()), SLOT(filePageSetup()) );
    menu->addAction( a );
    a->setDisabled(true);

    // Print Preview
    a = new QAction( tr( "Print Preview" ), this );
    a->setShortcut( tr("Ctrl+Alt+P") );
    a->setStatusTip( tr("Print preview") );
    connect( a, SIGNAL(activated()), SLOT(filePrintPreview()) );
    menu->addAction( a );
    a->setDisabled(true);

    // Print
    printAction = new QAction( QIcon( imagesPath() + "/fileprint.xpm" ), 
                               tr( "&Print..." ), this );
    printAction->setShortcut( tr("Ctrl+P") );
    printAction->setStatusTip( tr("Print project") );
    connect( printAction, SIGNAL(activated()), SLOT(filePrint()) );        
    menu->addAction( printAction );
    printAction->setDisabled(true);
    menu->addSeparator();    

    // Exit
    a = new QAction( tr( "E&xit" ), this );
    a->setShortcut( tr("Ctrl+Q") );
    a->setStatusTip( tr("Exit") );
    connect( a, SIGNAL(activated()), SLOT(fileExit()) );
    menu->addAction( a );
}

void FermaNextMainFrame::setupEditActions ()
{
    //FIXME QT3TO4 whole change
    QToolBar* tb = addToolBar( tr("Edit Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&Edit" ) );

    // Undo
    undoAction = new QAction( QIcon( imagesPath() + "/editundo.xpm" ), 
                              tr( "&Undo" ), this );
    undoAction->setShortcut( tr("Ctrl+Z") );
    undoAction->setStatusTip( tr("Undo last action") );
    connect( undoAction, SIGNAL(activated()), SLOT(editUndo()) );
    undoAction->setDisabled(true);
    tb->addAction( undoAction );
    menu->addAction( undoAction );

    // Redo
    redoAction = new QAction( QIcon( imagesPath() + "/editredo.xpm" ), 
                              tr( "&Redo" ), this );
    redoAction->setShortcut( tr("Ctrl+Y") );
    redoAction->setStatusTip( tr("Redo last action") );
    connect( redoAction, SIGNAL(activated()), SLOT(editRedo()) );
    redoAction->setDisabled(true);
    tb->addAction( redoAction );
    menu->addAction( redoAction );
    menu->addSeparator();
    tb->addSeparator();
}

void FermaNextMainFrame::setupProjectActions ()
{        
}

void FermaNextMainFrame::setupWindowActions ()
{
}

void FermaNextMainFrame::setupHelpActions ()
{
    //FIXME QT3TO4 whole change
    QMenu* menu = menuBar()->addMenu( tr( "&Help" ) );

    QAction *a;
    // Contents
    a = new QAction( tr( "Contents..." ), this );
    a->setShortcut( tr("F1") );
    a->setStatusTip( tr("Contents") );
    connect( a, SIGNAL(activated()), SLOT(helpContents()) );
    menu->addAction( a );
    a->setDisabled(true);
    menu->addSeparator();

    // About
    a = new QAction( tr( "About FermaNext" ), this );
    a->setStatusTip( tr("Help") );
    connect( a, SIGNAL(activated()), SLOT(helpAbout()) );    
    menu->addAction( a );
    a->setDisabled(true);
}

void FermaNextMainFrame::setupPluginActions ()
{
    reloadPlugins( false );
}

void FermaNextMainFrame::reloadPlugins ()
{
    reloadPlugins( true );
}

void FermaNextMainFrame::reloadPlugins ( bool reload )
{
    FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();
    PluginManager& plgManager = wsp.pluginManager();

    if ( reload ) {        
        plgManager.unloadPlugins();
        plgManager.loadPlugins( pluginsPath() );
    }

    QStringList names;
    PluginList plugins = plgManager.loadedPlugins();
    PluginListConstIter plgIt = plugins.begin();

    for ( ; plgIt != plugins.end(); ++plgIt )
        names.push_back( (*plgIt)->pluginInfo().name );

    if ( pluginsMenu == 0 ) {
        pluginsMenu = menuBar()->addMenu( tr( "&Plugins" ) );
    }
    else 
        pluginsMenu->clear();

    QStringList::iterator i = names.begin();
    for ( ; i != names.end(); ++i ) {
        pluginsMenu->addAction( *i );
    }
    pluginsMenu->addSeparator();

    // Reload plugins
    pluginsMenu->addAction( tr("Reload plugins"), this, 
                            SLOT(reloadPlugins()) );
}

void FermaNextMainFrame::refreshUndoRedoActions ()
{
    bool enableUndo = false;
    bool enableRedo = false;

    ObjectStateManager* mng = 0;
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj != 0 ) {
        TrussUnitWindow* trussWindow = 
            prj->getDesignerWindow().getDesignerWidget().getFocusedWindow();
        if ( trussWindow ) {
            mng = trussWindow->getStateManager();
            if ( mng ) {    
                enableUndo = (mng->countStatesToUndo() > 0);
                enableRedo = (mng->countStatesToRedo() > 0);
            }
        }
    }
    undoRedoListBox->setStateManager( mng );
    undoAction->setEnabled(enableUndo);
    redoAction->setEnabled(enableRedo);
}

void FermaNextMainFrame::refreshProjectActions ()
{
    bool enableActions = false;

    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj )
        enableActions = true;

    saveProjectAction->setEnabled(enableActions);
    saveAsProjectAction->setEnabled(enableActions);
    closeProjectAction->setEnabled(enableActions);
    printAction->setEnabled(enableActions);
}

void FermaNextMainFrame::trussWindowLostFocus ( TrussUnitWindow& window )
{
    ObjectStateManager* mng = window.getStateManager();
    if ( mng )
        mng->disconnect( this );

    refreshUndoRedoActions();
}

void FermaNextMainFrame::trussWindowReceivedFocus ( TrussUnitWindow& window )
{
    ObjectStateManager* mng = window.getStateManager();
    if ( mng == 0 )
        return;

    // Catch basic state manager signals
    connect( mng, SIGNAL(onStateBlockIsEnded(ObjectStateManager&)), 
                  SLOT(refreshUndoRedoActions()) );
    connect( mng, SIGNAL(onRemoveState(ObjectStateManager&, ObjectState&)), 
                  SLOT(refreshUndoRedoActions()) );
    connect( mng, SIGNAL(afterUndo(ObjectStateManager&)), 
                  SLOT(refreshUndoRedoActions()) );
    connect( mng, SIGNAL(afterRedo(ObjectStateManager&)), 
                  SLOT(refreshUndoRedoActions()) );
    
    refreshUndoRedoActions();
}

/*****************************************************************************
 * Actions
 *****************************************************************************/

void FermaNextMainFrame::fileNew ()
{
    createProject();
}

void FermaNextMainFrame::fileOpen ()
{
    QString fileName = QFileDialog::getOpenFileName( 
        this, 
        tr("Open ferma project"),
        applicationDirPath(),
        "Ferma project (*" + FermaNextProject::formatExtension() + ")" );

    if ( fileName.isEmpty() )
        return;

    try { FermaNextWorkspace::workspace().createProjectFromFile( fileName ); }
    catch ( FermaNextWorkspace::WorkspaceIsNotInitedCorrectly& ) {
        QMessageBox::critical( this, tr("FermaNextWorkspace exception"),
                               tr("Workspace is not inited correctly.") );
    } catch ( FermaNextWorkspace::LoadException& ) {
        QMessageBox::critical( this, tr("Error reading project file"),
                               tr("Wrong XML format.") );
    }
}

bool FermaNextMainFrame::fileSave ()
{
    FermaNextProject* currentPrj = projectToolBox->currentProject();
    if ( currentPrj == 0 )
        return false;

    try { currentPrj->saveToFile(); }
    catch ( FermaNextProject::FileNameIsNotDefinedException& ) {
        return fileSaveAs();
    }
    return true;
}

bool FermaNextMainFrame::fileSaveAs ()
{
    FermaNextProject* currentPrj = projectToolBox->currentProject();
    if ( currentPrj == 0 )
        return false;

    QString prjName = currentPrj->getName();
    QString fileName = QFileDialog::getSaveFileName( 
        this,
        tr("Save ferma project (%1)").arg(prjName),
        prjName + FermaNextProject::formatExtension(),
        "Ferma project (*" + FermaNextProject::formatExtension() + ")" );

    if ( fileName.isEmpty() )
        return false;

    QRegExp rx( ".*\\" + FermaNextProject::formatExtension() + "$" );
    if ( -1 == rx.indexIn( fileName ) )
        fileName += FermaNextProject::formatExtension();

    QFileInfo prjFileInfo( fileName );

    if ( prjFileInfo.exists() ) {
        if ( QMessageBox::question( this,
                                    tr("File exists"),
                                    tr("Rewrite file \"%1\"?").arg(fileName),
                                    tr("&Yes"), tr("&No"),
                                    QString::null, 0, 1 ) )
            return false;
    }

    try { currentPrj->saveToFile( fileName ); }
    catch ( FermaNextProject::IOException& ) {
        QMessageBox::critical( this, tr("Write file error"),
                             tr("Can't write to file: \"%1\"").arg(fileName) );
        return false;
    }
    return true;
}

void FermaNextMainFrame::fileClose ()
{
    FermaNextProject* currentPrj = projectToolBox->currentProject();
    if ( currentPrj == 0 )
        return;
    if ( QMessageBox::question( this,
                                tr("Project closing - \"%1\"").
                                  arg(currentPrj->getName()),
                                tr("Close current project?"),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) )
        return;
    
    FermaNextWorkspace::workspace().removeProject( *currentPrj );
}

void FermaNextMainFrame::fileOpenWsp ()
{
    if ( QMessageBox::question( this,
                                tr("Open workspace"),
                                tr("Open new workspace?"),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) )
        return;


    QString fileName = QFileDialog::getOpenFileName( 
       this,
       tr("Open ferma workspace"),
       applicationDirPath(),
        "Ferma workspace (*" + FermaNextWorkspace::formatExtension() + ")" );

    if ( fileName.isEmpty() )
        return;

    try { FermaNextWorkspace::workspace().loadFromFile( fileName ); }
    catch ( FermaNextWorkspace::IOException& ) {
        QMessageBox::critical( this, tr("Error reading workspace file"),
                               tr("Can't open file.") );
    } 
    catch ( FermaNextWorkspace::WrongXMLDocException& ) {
        QMessageBox::critical( this, tr("Error reading workspace file"),
                               tr("Wrong XML format.") );
    }
    catch ( FermaNextWorkspace::LoadException& ) {
        QMessageBox::critical( this, tr("Error reading workspace file"),
                               tr("Wrong workspace format.") );
    }
}

bool FermaNextMainFrame::fileSaveWsp ()
{
    // All projects should have their file names
    FermaNextProject* currentSelected = projectToolBox->currentProject();
    FermaNextWorkspace::ProjectList projects = 
        FermaNextWorkspace::workspace().getProjectList();
    FermaNextWorkspace::ProjectListIter prjIt = projects.begin();
    for ( ; prjIt != projects.end(); ++prjIt ) {
        FermaNextProject* prj = *prjIt;
        if ( ! prj->isFileNameDefined() ) {
            prj->activate();
            if ( ! fileSaveAs() )
                return false;
        }            
    }

    // Restore selected
    if ( currentSelected )
        currentSelected->activate();

    try { FermaNextWorkspace::workspace().saveToFile(); }
    catch ( FermaNextWorkspace::FileNameIsNotDefinedException& ) {
        return fileSaveWspAs();
    }
    catch ( FermaNextWorkspace::ProjectFileNameIsNotDefinedException& ) {
        // Hm. Strange
        QMessageBox::critical( this, tr("FermaNextWorkspace exception"),
                               tr("Some project file name is not defined.\n"
                                  "Please, save all projects." ) );
        return false;
    }
    return true;
}

bool FermaNextMainFrame::fileSaveWspAs ()
{
    QString fileName = QFileDialog::getSaveFileName( 
        this,
        tr("Save ferma workspace"),
        applicationDirPath(),
        "Ferma workspace (*" + FermaNextWorkspace::formatExtension() + ")" );

    if ( fileName.isEmpty() )
        return false;

    QRegExp rx( ".*\\" + FermaNextWorkspace::formatExtension() + "$" );
    if ( -1 == rx.indexIn( fileName ) )
        fileName += FermaNextWorkspace::formatExtension();

    QFileInfo wspFileInfo( fileName );

    if ( wspFileInfo.exists() ) {
        if ( QMessageBox::question( this,
                                    tr("File exists"),
                                    tr("Rewrite file \"%1\"?").arg(fileName),
                                    tr("&Yes"), tr("&No"),
                                    QString::null, 0, 1 ) )
            return false;
    }
    
    // All projects should have their file names
    FermaNextProject* currentSelected = projectToolBox->currentProject();
    FermaNextWorkspace::ProjectList projects = 
        FermaNextWorkspace::workspace().getProjectList();
    FermaNextWorkspace::ProjectListIter prjIt = projects.begin();
    for ( ; prjIt != projects.end(); ++prjIt ) {
        FermaNextProject* prj = *prjIt;
        if ( ! prj->isFileNameDefined() ) {
            prj->activate();
            if ( ! fileSaveAs() )
                return false;
        }            
    }

    // Restore selected
    if ( currentSelected )
        currentSelected->activate();

    try { FermaNextWorkspace::workspace().saveToFile( fileName ); }
    catch ( FermaNextWorkspace::ProjectFileNameIsNotDefinedException& ) {
        // Hm. Strange
        QMessageBox::critical( this, tr("FermaNextWorkspace exception"),
                               tr("Some project file name is not defined.\n"
                                  "Please, save all projects." ) );
        return false;        
    }
    catch ( FermaNextWorkspace::IOException& ) {
        // Hm. Strange
        QMessageBox::critical( this, tr("Error writing to workspace file"),
                               tr("Can't open file for writing.") );
        return false;
    }
    return true;
}

void FermaNextMainFrame::fileCloseWsp ()
{
    FermaNextWorkspace::workspace().reset();
}

bool FermaNextMainFrame::fileSaveAll ()
{
    // Save projects at first
    FermaNextProject* currentSelected = projectToolBox->currentProject();
    FermaNextWorkspace::ProjectList projects = 
        FermaNextWorkspace::workspace().getProjectList();
    FermaNextWorkspace::ProjectListIter prjIt = projects.begin();
    for ( ; prjIt != projects.end(); ++prjIt ) {
        FermaNextProject* prj = *prjIt;
        try { prj->saveToFile(); }
        catch ( FermaNextProject::IOException& ) {
            const QString& fileName = prj->getProjectFileName();
            QMessageBox::critical( this, tr("Write file error"),
                             tr("Can't write to file: \"%1\"").arg(fileName) );
            return false;
        }
        catch ( FermaNextProject::FileNameIsNotDefinedException& ) {
            prj->activate();
            if ( !fileSaveAs() )
                return false;
        }
    }

    // Restore selected
    if ( currentSelected )
        currentSelected->activate();

    return fileSaveWsp();
}

void FermaNextMainFrame::filePageSetup ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::filePrintPreview ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::filePrint ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;
    prj->getDesignerWindow().getDesignerWidget().print();
}

void FermaNextMainFrame::fileExit ()
{
    cleanBeforeQuit();
    qApp->quit();
}

void FermaNextMainFrame::editUndo ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;

    TrussUnitWindow* trussWindow = 
        prj->getDesignerWindow().getDesignerWidget().getFocusedWindow();
    if ( trussWindow == 0 )
        return;

    ObjectStateManager* mng = trussWindow->getStateManager();
    if ( mng == 0 )
        return;
    
    try {
        mng->undo();
        prj->getDesignerWindow().getDesignerWidget().update();
        refreshUndoRedoActions();
    } 
    catch ( ObjectStateManager::UndoException& )
    {
        QMessageBox::information( this, tr("Can't undo"),
                                        tr("Nothing to undo") );
    }
}

void FermaNextMainFrame::editRedo ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;

    TrussUnitWindow* trussWindow = 
        prj->getDesignerWindow().getDesignerWidget().getFocusedWindow();
    if ( trussWindow == 0 )
        return;

    ObjectStateManager* mng = trussWindow->getStateManager();
    if ( mng == 0 )
        return;
    
    try {
        mng->redo();
        prj->getDesignerWindow().getDesignerWidget().update();
        refreshUndoRedoActions();
    } 
    catch ( ObjectStateManager::RedoException& )
    {
        QMessageBox::information( this, tr("Can't redo"), 
                                        tr("Nothing to redo") );
    }
}

void FermaNextMainFrame::editCopy ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::editPaste ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::editCut ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::editSelectAll ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::helpContents ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::helpAbout ()
{
    qWarning("Not implmented yet!");
}

/*****************************************************************************
 * Events
 *****************************************************************************/

/*
 *  Correct clean.
 */
void FermaNextMainFrame::closeEvent ( QCloseEvent* event )
{
    cleanBeforeQuit();
    event->accept();
}

/*****************************************************************************/
