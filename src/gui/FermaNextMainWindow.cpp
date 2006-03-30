
// Qt3 Support classes
#include <Q3DockWindow>

#include <QApplication>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog> 
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QSignalMapper>
#include <QStatusBar>
#include <QStyleFactory>
#include <QTextCodec>
#include <QToolButton>
#include <QToolBox>
#include <QDesktopWidget>
#include <QMenu>
#include <QToolBar>

#include "FermaNextMainWindow.h"
#include "FermaNextWorkspace.h"
#include "ProjectToolBox.h"
#include "SubsidiaryConstants.h"
#include "UndoRedoListBox.h"
#include "GeometryTabWidget.h"

const QString fermaTitle( QObject::tr( "Educational CAD System 'Ferma'" ) );

/*****************************************************************************
 * FermaNext Main Frame
 *****************************************************************************/

FermaNextMainWindow::FermaNextMainWindow ( FermaNextWorkspace& wsp ) :
    Q3MainWindow(0, 0, Qt::Window),
    workspace(wsp),
    pluginsMenu(0)
{     
    init();

    // Initial toolbar setup
    setupFileActions();
    setupEditActions();
    setupViewActions ();
    setupProjectActions();
    setupWindowActions();
    setupHelpActions();

        // Refresh plugins menu
    PluginManager& plgManager = workspace.pluginManager();
    QObject::connect( &plgManager, SIGNAL(onAfterPluginsLoad(uint)), 
                                   SLOT(refreshPluginsActions()) );
    QObject::connect( &plgManager, SIGNAL(onAfterPluginsUnload(uint)), 
                                   SLOT(refreshPluginsActions()) );
}

void FermaNextMainWindow::init ()
{
    setCaption( fermaTitle);
    setMinimumSize( 640, 480 );
    statusBar()->addWidget(new QLabel( tr("Ready"), statusBar() ));

    undoRedoHistoryWidget = new QWidget( this, "undo_redo_history",
                                         // FIXME QT3TO4
                                         // WStyle_Tool | WType_TopLevel 
                                         Qt::Window | Qt::Tool );

    int undoRedoWidth = 140, undoRedoHeight = 110;
    undoRedoHistoryWidget->setFixedSize( undoRedoWidth, undoRedoHeight );
    // Pretty history widget offset from the end point of the screen
    undoRedoHistoryWidget->move( QApplication::desktop()->width() - 
                                 int(undoRedoWidth*1.5), 
                                 0 + int(undoRedoHeight*1.5) );
    undoRedoHistoryWidget->setCaption( tr("History") );

    QVBoxLayout* vboxHistoryWidget = new QVBoxLayout( undoRedoHistoryWidget );
    undoRedoListBox = new UndoRedoListBox( undoRedoHistoryWidget );
    vboxHistoryWidget->addWidget( undoRedoListBox );
    undoRedoListBox->installEventFilter( this );
    undoRedoHistoryWidget->installEventFilter( this );
    
    geometryWindow = new QWidget( this, Qt::Window | Qt::Tool );
    geometryWindow->setFixedSize( 195, 174 );
    geometryWindow->setCaption( tr("Truss Geometry") );
    geometryTabWidget = new GeometryTabWidget( geometryWindow );
    geometryWindow->move( QApplication::desktop()->width() - 265, 310 );
    geometryWindow->installEventFilter( this );

    QVBoxLayout* tabLayout = new QVBoxLayout;
    tabLayout->addWidget( geometryTabWidget );
    QVBoxLayout* parentLayout = new QVBoxLayout( geometryWindow );
    parentLayout->addLayout( tabLayout );
    tabLayout->setMargin( 1 );
    tabLayout->setSpacing( 1 );
    parentLayout->setMargin( 2 );
    parentLayout->setSpacing( 2 );
    
    projectsDockWindow = new Q3DockWindow( Q3DockWindow::InDock, this );
    projectsDockWindow->setResizeEnabled( true );
    projectsDockWindow->setVerticalStretchable( true );
    projectsDockWindow->setCaption( tr("Truss Unit Projects") );
    addDockWindow( projectsDockWindow, Qt::DockLeft );
    setDockEnabled( projectsDockWindow, Qt::DockTop, false );
    setDockEnabled( projectsDockWindow, Qt::DockBottom, false );
    projectsDockWindow->setCloseMode( Q3DockWindow::Always );

    projectToolBox = new ProjectToolBox( workspace, projectsDockWindow );
    connect( &workspace, SIGNAL(onBeforeProjectRemove(FermaNextProject&)), 
             SLOT(someProjectRemoved(FermaNextProject&)) );
    connect( &workspace, SIGNAL(onProjectCreate(FermaNextProject&)), 
             SLOT(someProjectCreated(FermaNextProject&)) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshUndoRedoActions()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshProjectActions()) );
    projectsDockWindow->setWidget( projectToolBox );
    projectsDockWindow->setFixedExtentWidth( 160 );

    projectsDockWindow->hide();

    setRightJustification( true );
}

void FermaNextMainWindow::someProjectRemoved ( FermaNextProject& prj )
{
    if ( 1 == workspace.countProjects() ) {
        projectsDockWindow->hide();
        undoRedoHistoryWidget->hide();
        geometryWindow->hide();
    }
    TrussUnitDesignerWidget& designerWidget = prj.getDesignerWidget();
    designerWidget.disconnect( this );
}

void FermaNextMainWindow::someProjectCreated ( FermaNextProject& prj )
{
    if ( !projectsDockWindow->isVisible() && 0 < workspace.countProjects() ) {
        projectsDockWindow->show();
        undoRedoHistoryWidget->show();
        geometryWindow->show();
    }

    TrussUnitDesignerWidget& designerWidget = prj.getDesignerWidget();

    connect( &designerWidget, SIGNAL(onFocusLose(TrussUnitWindow&)),
                            SLOT(trussWindowLostFocus(TrussUnitWindow&)) );
    connect( &designerWidget, SIGNAL(onFocusReceive(TrussUnitWindow&)),
                            SLOT(trussWindowReceivedFocus(TrussUnitWindow&)) );
}

void FermaNextMainWindow::createProject ()
{
    bool ok;
    QString text = QInputDialog::getText(
            tr("Project creation"), 
            tr("Enter project name:"), QLineEdit::Normal,
            QString::null, &ok, this );
    if ( ok && !text.isEmpty() ) {
        FermaNextProject& prj = workspace.createProject( text );
        TrussUnitWindowManager& mng = prj.getTrussUnitWindowManager();

        connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
                 geometryTabWidget, 
                        SLOT(trussUnitWindowWasCreated(TrussUnitWindow&)) );


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

void FermaNextMainWindow::setupFileActions ()
{
    QToolBar* tb = new QToolBar( tr("File Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&File" ) );

    QAction *a;
    // New
    a = new QAction( QIcon(imagesPath() + "/filenew.xpm"), 
                     tr( "&New..." ), this );
    a->setShortcut( tr("CTRL+N") );
    connect( a, SIGNAL(triggered()), SLOT(fileNew()) );
    tb->addAction( a );
    menu->addAction( a );

    // Open
    a = new QAction( QIcon(imagesPath() + "/fileopen.xpm"), 
                      tr( "&Open..." ), this );
    a->setShortcut( tr("CTRL+O") );
    connect( a, SIGNAL(triggered()), SLOT(fileOpen()) );
    tb->addAction( a );
    menu->addAction( a );   

    // Save
    saveProjectAction = new QAction( QIcon(imagesPath() + "/filesave.xpm"),
                                     tr( "&Save..." ), this );
    saveProjectAction->setShortcut( tr("CTRL+S") );
    connect( saveProjectAction, SIGNAL(triggered()), SLOT(fileSave()) );
    tb->addAction( saveProjectAction );
    menu->addAction( saveProjectAction );
    saveProjectAction->setDisabled(true);
    menu->addSeparator();
  
    // Save As
    saveAsProjectAction = new QAction( tr( "Save &As..." ), this );
    connect( saveAsProjectAction, SIGNAL(triggered()), SLOT(fileSaveAs()) );
    menu->addAction( saveAsProjectAction );
    saveAsProjectAction->setDisabled(true);

    // Close
    closeProjectAction = new QAction( tr( "&Close" ), this );
    connect( closeProjectAction, SIGNAL(triggered()), SLOT(fileClose()) );
    menu->addAction( closeProjectAction );
    closeProjectAction->setDisabled(true);
    menu->addSeparator();

    // Open Wsp
    a = new QAction( tr( "Open Workspace..." ), this );
    a->setShortcut( tr("SHIFT+F2") );
    connect( a, SIGNAL(triggered()), SLOT(fileOpenWsp()) );
    menu->addAction( a );

    // Save Wsp
    a = new QAction( tr( "Save Workspace" ), this );
    a->setShortcut( tr("SHIFT+F3") );
    connect( a, SIGNAL(triggered()), SLOT(fileSaveWsp()) );
    menu->addAction( a );

    // Save Wsp As
    a = new QAction( tr( "Save Workspace As..." ), this );
    connect( a, SIGNAL(triggered()), SLOT(fileSaveWspAs()) );
    menu->addAction( a );

    // Close Wsp
    a = new QAction( tr( "Close Workspace" ), this );
    a->setShortcut( tr("SHIFT+F4") );
    connect( a, SIGNAL(triggered()), SLOT(fileCloseWsp()) );
    menu->addAction( a );
    menu->addSeparator();

    // Save All
    a = new QAction( tr( "Save All" ), this );
    a->setShortcut( tr("CTRL+SHIFT+S") );
    connect( a, SIGNAL(triggered()), SLOT(fileSaveAll()) );
    menu->addAction( a );
    menu->addSeparator();
    
    // Page Setup
    a = new QAction( tr( "Page setup..." ), this );
    connect( a, SIGNAL(triggered()), SLOT(filePageSetup()) );
    menu->addAction( a );
    a->setDisabled(true);

    // Print Preview
    a = new QAction( tr( "Print Preview" ), this );
    a->setShortcut( tr("CTRL+ALT+P") );
    connect( a, SIGNAL(triggered()), SLOT(filePrintPreview()) );
    menu->addAction( a );
    a->setDisabled(true);

    // Print
    printAction = new QAction( QIcon(imagesPath() + "/fileprint.xpm"), 
                               tr( "&Print..." ),  this ); 
    printAction->setShortcut( tr("CTRL+P") );
    connect( printAction, SIGNAL(triggered()), SLOT(filePrint()) );        
    menu->addAction( printAction );
    printAction->setDisabled(true);
    menu->addSeparator();    

    // Exit
    a = new QAction( tr( "E&xit" ), this );
    a->setShortcut( tr("CTRL+Q") );
    connect( a, SIGNAL(triggered()), SLOT(fileExit()) );
    menu->addAction( a );
}

void FermaNextMainWindow::setupEditActions ()
{
    QToolBar* tb = new QToolBar( tr("Edit Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&Edit" ) );

    // Undo
    undoAction = new QAction( QIcon(imagesPath() + "/editundo.xpm"), 
                              tr( "&Undo" ), this );
        undoAction->setShortcut( tr("CTRL+Z") );                               
    connect( undoAction, SIGNAL(triggered()), SLOT(editUndo()) );
    undoAction->setDisabled(true);
        tb->addAction( undoAction );
        menu->addAction( undoAction );

    // Redo
    redoAction = new QAction( QIcon(imagesPath() + "/editredo.xpm"), 
                              tr( "&Redo" ), this );
        undoAction->setShortcut( tr("CTRL+Y") );
    connect( redoAction, SIGNAL(triggered()), SLOT(editRedo()) );
    redoAction->setDisabled(true);
        tb->addAction( redoAction );
        menu->addAction( redoAction );
    menu->addSeparator();
    tb->addSeparator();
}

void FermaNextMainWindow::setupViewActions ()
{
    QMenu* menu = menuBar()->addMenu( tr( "&View" ) );

    // Contents
    showUndoRedoAction = new QAction( tr( "&Show History Window" ), this );
    showUndoRedoAction->setToggleAction( true );
    showUndoRedoAction->setOn( true );
    showUndoRedoAction->setStatusTip( tr( "Show or hide history window" ) );
    menu->addAction( showUndoRedoAction );
    connect( showUndoRedoAction, SIGNAL(toggled(bool)), 
             undoRedoHistoryWidget, SLOT(setShown(bool)) );

    showGeometryWindowAction = 
        new QAction( tr( "&Show Truss Geometry Window" ), this );
    showGeometryWindowAction->setToggleAction( true );
    showGeometryWindowAction->setOn( true );
    showGeometryWindowAction->
        setStatusTip( tr( "Show or hide truss geometry window" ) );
    menu->addAction( showGeometryWindowAction );
    connect( showGeometryWindowAction, SIGNAL( toggled( bool ) ), 
             geometryWindow, SLOT( setShown( bool ) ) );
}

void FermaNextMainWindow::setupProjectActions ()
{        
}

void FermaNextMainWindow::setupWindowActions ()
{
}

void FermaNextMainWindow::setupHelpActions ()
{
    QMenu* menu = menuBar()->addMenu( tr( "&Help" ) );

    QAction *a;
    // Contents
    a = new QAction( tr( "Contents..." ), this );
        a->setShortcut( tr("F1") );
    connect( a, SIGNAL(triggered()), SLOT(helpContents()) );
        menu->addAction( a );
    a->setDisabled(true);
    menu->insertSeparator();

    // About
    a = new QAction( tr( "About FermaNext" ), this );
    connect( a, SIGNAL(triggered()), SLOT(helpAbout()) );
        menu->addAction( a );    
    a->setDisabled(true);
}

void FermaNextMainWindow::reloadPlugins ()
{
    reloadPlugins( true );
}

void FermaNextMainWindow::reloadPlugins ( bool reload )
{
    PluginManager& plgManager = workspace.pluginManager();

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

void FermaNextMainWindow::refreshUndoRedoActions ()
{
    bool enableUndo = false;
    bool enableRedo = false;

    ObjectStateManager* mng = 0;
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj != 0 ) {
        TrussUnitWindow* trussWindow = 
            prj->getDesignerWidget().getFocusedWindow();
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

void FermaNextMainWindow::refreshProjectActions ()
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

void FermaNextMainWindow::refreshPluginsActions ()
{
    // Just read info about plugins
    reloadPlugins( false )  ;
}

void FermaNextMainWindow::trussWindowLostFocus ( TrussUnitWindow& window )
{
    ObjectStateManager* mng = window.getStateManager();
    if ( mng )
        mng->disconnect( this );

    refreshUndoRedoActions();

    geometryTabWidget->changeFocusWindow( 0 );
}

void FermaNextMainWindow::trussWindowReceivedFocus ( TrussUnitWindow& window )
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
    
    geometryTabWidget->changeFocusWindow( &window );
}

/*****************************************************************************
 * Actions
 *****************************************************************************/

void FermaNextMainWindow::fileNew ()
{
    createProject();
}

void FermaNextMainWindow::fileOpen ()
{
    QString fileName = QFileDialog::getOpenFileName( QString::null, 
        "Ferma project (*" + FermaNextProject::formatExtension() + ")",
        this, "open ferma project", tr("Open ferma project") );

    if ( fileName.isEmpty() )
        return;

    try { workspace.createProjectFromFile( fileName ); }
    catch ( FermaNextWorkspace::LoadException& ) {
        QMessageBox::critical( this, tr("Error reading project file"),
                               tr("Wrong XML format.") );
    }
}

bool FermaNextMainWindow::fileSave ()
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

bool FermaNextMainWindow::fileSaveAs ()
{
    FermaNextProject* currentPrj = projectToolBox->currentProject();
    if ( currentPrj == 0 )
        return false;

    QString prjName = currentPrj->getName();
    QString fileName = QFileDialog::getSaveFileName( 
        prjName + FermaNextProject::formatExtension(),
        "Ferma project (*" + FermaNextProject::formatExtension() + ")", 
        this, "save ferma project", tr("Save ferma project (%1)").
          arg(prjName) );

    if ( fileName.isEmpty() )
        return false;

    QRegExp rx( ".*\\" + FermaNextProject::formatExtension() + "$" );
    if ( -1 == rx.search( fileName ) )
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

void FermaNextMainWindow::fileClose ()
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
    
    workspace.removeProject( *currentPrj );
}

void FermaNextMainWindow::fileOpenWsp ()
{
    if ( QMessageBox::question( this,
                                tr("Open workspace"),                                  
                                tr("Open new workspace?"),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) )
        return;

    QString fileName = QFileDialog::getOpenFileName( QString::null, 
        "Ferma workspace (*" + FermaNextWorkspace::formatExtension() + ")", 
        this, "open ferma workspace", tr("Open ferma workspace") );

    if ( fileName.isEmpty() )
        return;

    try { workspace.loadFromFile( fileName ); }
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

bool FermaNextMainWindow::fileSaveWsp ()
{
    // All projects should have their file names
    FermaNextProject* currentSelected = projectToolBox->currentProject();
    FermaNextWorkspace::ProjectList projects = workspace.getProjectList();
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

    try { workspace.saveToFile(); }
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

bool FermaNextMainWindow::fileSaveWspAs ()
{
    QString fileName = QFileDialog::getSaveFileName( QString::null, 
        "Ferma workspace (*" + FermaNextWorkspace::formatExtension() + ")",
        this, "save ferma workspace", tr("Save ferma workspace") );

    if ( fileName.isEmpty() )
        return false;

    QRegExp rx( ".*\\" + FermaNextWorkspace::formatExtension() + "$" );
    if ( -1 == rx.search( fileName ) )
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
    FermaNextWorkspace::ProjectList projects = workspace.getProjectList();
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

    try { workspace.saveToFile( fileName ); }
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

void FermaNextMainWindow::fileCloseWsp ()
{
    workspace.reset();
}

bool FermaNextMainWindow::fileSaveAll ()
{
    // Save projects at first
    FermaNextProject* currentSelected = projectToolBox->currentProject();
    FermaNextWorkspace::ProjectList projects = workspace.getProjectList();
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

void FermaNextMainWindow::filePageSetup ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::filePrintPreview ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::filePrint ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;
    prj->getDesignerWidget().print();
}

void FermaNextMainWindow::fileExit ()
{
    workspace.quitFermaNextApplication();
}

void FermaNextMainWindow::editUndo ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;

    TrussUnitWindow* trussWindow = 
        prj->getDesignerWidget().getFocusedWindow();
    if ( trussWindow == 0 )
        return;

    ObjectStateManager* mng = trussWindow->getStateManager();
    if ( mng == 0 )
        return;
    
    try {
        mng->undo();
        prj->getDesignerWidget().update();
        refreshUndoRedoActions();
    } 
    catch ( ObjectStateManager::UndoException& )
    {
        QMessageBox::information( this, tr("Can't undo"),
                                        tr("Nothing to undo") );
    }
}

void FermaNextMainWindow::editRedo ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;

    TrussUnitWindow* trussWindow = 
        prj->getDesignerWidget().getFocusedWindow();
    if ( trussWindow == 0 )
        return;

    ObjectStateManager* mng = trussWindow->getStateManager();
    if ( mng == 0 )
        return;
    
    try {
        mng->redo();
        prj->getDesignerWidget().update();
        refreshUndoRedoActions();
    } 
    catch ( ObjectStateManager::RedoException& )
    {
        QMessageBox::information( this, tr("Can't redo"), 
                                        tr("Nothing to redo") );
    }
}

void FermaNextMainWindow::editCopy ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::editPaste ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::editCut ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::editSelectAll ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::helpContents ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::helpAbout ()
{
    qWarning("Not implmented yet!");
}

/*****************************************************************************
 * Events
 *****************************************************************************/

bool FermaNextMainWindow::eventFilter( QObject* targetObj, QEvent* event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent* keyEvent = (QKeyEvent*)event;
        if ( keyEvent->modifiers() & Qt::ControlModifier ||
             keyEvent->modifiers() & Qt::ShiftModifier ) {
            setActiveWindow();
            menuBar()->setFocus();
            keyPressEvent( keyEvent );
            return true;
        }
        else if ( keyEvent->key() == Qt::Key_N || 
                  keyEvent->key() == Qt::Key_P ||
                  keyEvent->key() == Qt::Key_F || 
                  keyEvent->key() == Qt::Key_L ||
                  keyEvent->key() == Qt::Key_Escape ) {
            FermaNextProject* prj = projectToolBox->currentProject();
            if ( prj ) {
                setActiveWindow();
                prj->getDesignerWidget().aggKeyPressEvent( keyEvent );
                return true;
            }
        }
        return false;
    }
    else if ( event->type() == QEvent::Close ) {
        if ( targetObj == geometryWindow )
            showGeometryWindowAction->toggle();
        else if ( targetObj == undoRedoHistoryWidget )
            showUndoRedoAction->toggle();
        return true;
    } 
    else
        return Q3MainWindow::eventFilter( targetObj, event );
}

/*
 *  Correct clean.
 */
void FermaNextMainWindow::closeEvent ( QCloseEvent* event )
{
    workspace.quitFermaNextApplication();
}

/*****************************************************************************/
