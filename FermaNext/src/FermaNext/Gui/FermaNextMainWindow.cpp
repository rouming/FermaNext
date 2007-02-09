
#include <QDockWidget>
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
#include <QSignalMapper>
#include <QSettings>

#include "AggTrussToolBar.h"
#include "FermaNextMainWindow.h"
#include "FermaNextWorkspace.h"
#include "GeometryTabWidget.h"
#include "Global.h"
#include "GuiSubsidiary.h"
#include "PreferencesWidget.h"
#include "ProjectToolBox.h"
#include "PluginReloader.h"
#include "ResultsTabWidget.h"
#include "TrussPropertyTabWidget.h"
#include "TrussMaterialEditor.h"
#include "TrussResultsManager.h"
#include "UndoRedoListBox.h"

#include "Plugin.h"
#include <QTextStream>

const QString fermaTitle( QObject::tr( "Educational CAD System 'Ferma'" ) );

/*****************************************************************************
 * FermaNext Main Frame
 *****************************************************************************/

FermaNextMainWindow::FermaNextMainWindow ( FermaNextWorkspace& wsp ) :
    workspace(wsp),
    pluginsMenu(0),
    pluginsSigMapper( new QSignalMapper(this) ),
    materialEditorStartPage( QString() )
{     
    init();

    // Initial toolbar setup
    setupFileActions();
    setupEditActions();
    setupViewActions ();
    setupProjectActions();
    setupWindowActions();
    setupPluginsActions();
    setupHelpActions();

    // Refresh plugins menu
    PluginManager& plgManager = workspace.pluginManager();
    QObject::connect( &plgManager, SIGNAL(onAfterPluginsLoad(uint)), 
                                   SLOT(refreshPluginsActions()) );
    QObject::connect( &plgManager, SIGNAL(onAfterPluginsUnload(uint)), 
                                   SLOT(refreshPluginsActions()) );
    
    restoreApplicationSettings();
}

FermaNextMainWindow::~FermaNextMainWindow ()
{}

void FermaNextMainWindow::init ()
{
    setWindowTitle( fermaTitle );
    setMinimumSize( 640, 480 );

    projectsDockWidget = new DockedWidget( tr("Projects"), this );
    projectsDockWidget->setVisible(false);
    projectsDockWidget->setAllowedAreas( Qt::LeftDockWidgetArea );
    projectsDockWidget->setMinimumWidth( 145 );
    projectsDockWidget->setWindowTitle( tr("Projects") );

    projectToolBox = new ProjectToolBox( workspace, projectsDockWidget );
    connect( &workspace, SIGNAL(onBeforeProjectRemove(FermaNextProject&)), 
             SLOT(someProjectRemoved(FermaNextProject&)) );
    connect( &workspace, SIGNAL(onProjectCreate(FermaNextProject&)), 
             SLOT(someProjectCreated(FermaNextProject&)) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(currentProjectChanged()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(currentProjectChanged()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(currentProjectChanged()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(currentProjectChanged()) );

    connect( projectToolBox, SIGNAL(onShowTrussResults(const TrussUnitWindow&)),
                             SLOT(showResultsWindow(const TrussUnitWindow&)) );

    connect( this, 
                SIGNAL(calculateTrussUnit(const TrussUnitWindow&)),
             projectToolBox, 
                SIGNAL(calculateTrussUnit(const TrussUnitWindow&)) );

    projectsDockWidget->setWidget( projectToolBox );    

    // Clear and init plugins menu only from main event loop
    connect( this, SIGNAL(reloadPluginsFromMainEventLoop()),
                   SLOT(onReloadPluginsFromMainEventLoop()),
             Qt::QueuedConnection );

    preferencesWidget = new PreferencesWidget( this );
    
    resultsWindow = new QDialog( this );
    resultsWindow->setWindowTitle( "Calculation Results" );
    resultsWindow->setFixedWidth( 760 );
    QRect wGeometry = resultsWindow->geometry();
    wGeometry.setHeight( 600 );
    resultsWindow->setGeometry( wGeometry );
    QHBoxLayout* resultsLayout = new QHBoxLayout( resultsWindow );
    resultsTabWidget = new ResultsTabWidget;
    resultsLayout->addWidget( resultsTabWidget );
    resultsLayout->setMargin( 1 );

    addDockWidget( Qt::LeftDockWidgetArea, projectsDockWidget );

    initUndoRedoWindow();
    initGeometryWindow();
    initTrussPropertyWindow();
    createStatusBar();

    // Connect plugins signal mapper to show plugin info
    connect( pluginsSigMapper, SIGNAL(mapped(const QString &)),
                                 SLOT(showPluginInfo(const QString &)));
}

void FermaNextMainWindow::initUndoRedoWindow ()
{
    undoRedoHistoryWidget = new QWidget( this, Qt::Window | Qt::Tool );
    undoRedoHistoryWidget->setFixedWidth( 196 );
    undoRedoHistoryWidget->setMinimumHeight( 120 );
    undoRedoHistoryWidget->setWindowTitle( tr("History") );
    undoRedoListBox = new UndoRedoListBox( undoRedoHistoryWidget );
    undoRedoHistoryWidget->installEventFilter( this );
    
    QVBoxLayout* listBoxLayout = new QVBoxLayout;
    listBoxLayout->addWidget( undoRedoListBox );
    QVBoxLayout* widgetLayout = new QVBoxLayout( undoRedoHistoryWidget );
    widgetLayout->addLayout( listBoxLayout );
    widgetLayout->setMargin( 2 );
}

void FermaNextMainWindow::initGeometryWindow ()
{
    geometryWindow = new QWidget( this, Qt::Window | Qt::Tool );
    geometryWindow->setFixedWidth( 196 );
    geometryWindow->setMinimumHeight( 170 );
    geometryWindow->setWindowTitle( tr("Truss Geometry") );
    geometryTabWidget = new GeometryTabWidget( geometryWindow );
    geometryWindow->installEventFilter( this );

    QVBoxLayout* tabLayout = new QVBoxLayout;
    tabLayout->addWidget( geometryTabWidget );
    QVBoxLayout* parentLayout = new QVBoxLayout( geometryWindow );
    parentLayout->addLayout( tabLayout );
    tabLayout->setMargin( 1 );
    tabLayout->setSpacing( 1 );
    parentLayout->setMargin( 2 );
    parentLayout->setSpacing( 2 );
}

void FermaNextMainWindow::initTrussPropertyWindow ()
{
    trussPropertyWindow = new QWidget( this, Qt::Window | Qt::Tool );
    trussPropertyWindow->setFixedWidth( 196 );
    trussPropertyWindow->setMinimumHeight( 224 );
    trussPropertyWindow->setWindowTitle( tr("Truss Properties") );
    trussPropTabWidget = new TrussPropertyTabWidget( trussPropertyWindow );
    trussPropertyWindow->installEventFilter( this );

    QVBoxLayout* tabLayout = new QVBoxLayout;
    tabLayout->addWidget( trussPropTabWidget );
    QVBoxLayout* parentLayout = new QVBoxLayout( trussPropertyWindow );
    parentLayout->addLayout( tabLayout );
    tabLayout->setMargin( 1 );
    tabLayout->setSpacing( 1 );
    parentLayout->setMargin( 2 );
    parentLayout->setSpacing( 2 );
}

void FermaNextMainWindow::createStatusBar ()
{
    statusBar()->setStyle( new FlatStyle() );

    hintLabel = new QLabel( "  Ready" );
    statusBar()->addWidget( hintLabel, 1 );

    coordLabel = new QLabel( "  999.99 : 999.99  " );
    coordLabel->setAlignment( Qt::AlignCenter );
    coordLabel->setMinimumSize( coordLabel->sizeHint() );
    coordLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    statusBar()->addPermanentWidget( coordLabel );

    elementLabel = new QLabel( " pivot #999 " );
    elementLabel->setAlignment( Qt::AlignCenter );
    elementLabel->setMinimumSize( elementLabel->sizeHint() );
    elementLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    statusBar()->addPermanentWidget( elementLabel );

    modLabel = new QLabel( " MOD " );
    modLabel->setEnabled( false );
    modLabel->setAlignment( Qt::AlignCenter );
    modLabel->setMinimumSize( modLabel->sizeHint() );
    modLabel->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    statusBar()->addPermanentWidget( modLabel );

    updateStatusBar();
}

void FermaNextMainWindow::someProjectRemoved ( FermaNextProject& prj )
{
    if ( 1 == workspace.countProjects() ) {
        projectsDockWidget->hide();
        undoRedoHistoryWidget->hide();
        geometryWindow->hide();
        trussPropertyWindow->hide();
        trussPropTabWidget->clearMaterialComboBox();

        prefAction->setEnabled( false );
        showUndoRedoAction->setEnabled( false );
        showTrussPropWindowAction->setEnabled( false );
        showGeometryWindowAction->setEnabled( false );
        projectsDockAction->setEnabled( false );

        copyAction->setEnabled( false );
        cutAction->setEnabled( false );
        pasteAction->setEnabled( false );
    }
    TrussDesignerWidget& designerWidget = prj.getDesignerWidget();
    designerWidget.disconnect( this );
}

void FermaNextMainWindow::someProjectCreated ( FermaNextProject& prj )
{
    if ( !projectsDockWidget->isVisible() && 0 < workspace.countProjects() ) {
        prefAction->setEnabled( true );
        showUndoRedoAction->setEnabled( true );
        showTrussPropWindowAction->setEnabled( true );
        showGeometryWindowAction->setEnabled( true );
        projectsDockAction->setEnabled( true );

        QSettings appSettings;

        appSettings.beginGroup( "Preferences" );

        bool showUndoRedo = true;
        if ( appSettings.contains( "ShowUndoRedo" ) )
            showUndoRedo = appSettings.value( "ShowUndoRedo" ).toBool();

        bool showGeometryWnd = true;
        if ( appSettings.contains( "ShowGeometryWindow" ) )
            showGeometryWnd = appSettings.value( "ShowGeometryWindow" ).toBool();

        bool showPropertyWnd = true;
        if ( appSettings.contains( "ShowPropertyWindow" ) )
            showPropertyWnd = appSettings.value( "ShowPropertyWindow" ).toBool();

        bool showProjectBar = true;
        if ( appSettings.contains( "ShowProjectsBar" ) )
            showProjectBar = appSettings.value( "ShowProjectsBar" ).toBool();

        appSettings.endGroup();

        showUndoRedoAction->setChecked( showUndoRedo );
        showTrussPropWindowAction->setChecked( showGeometryWnd );
        showGeometryWindowAction->setChecked( showPropertyWnd );
        projectsDockAction->setChecked( showProjectBar );

        if ( showUndoRedoAction->isChecked() )
            undoRedoHistoryWidget->show();
        if ( showTrussPropWindowAction->isChecked() )
            geometryWindow->show();
        if ( showGeometryWindowAction->isChecked() )
            trussPropertyWindow->show();
    }

    TrussDesignerWidget& designerWidget = prj.getDesignerWidget();

    connect( &designerWidget, SIGNAL(onFocusLose(TrussUnitWindow&)),
                            SLOT(trussWindowLostFocus(TrussUnitWindow&)) );
    connect( &designerWidget, SIGNAL(onFocusReceive(TrussUnitWindow&)),
                            SLOT(trussWindowReceivedFocus(TrussUnitWindow&)) );
    connect( &designerWidget, SIGNAL(cursorMoved()),
                            SLOT(updateStatusBar()) );

    connect( &designerWidget.getToolBar(), 
                SIGNAL(onShowStatusTip(const QString&)),
             statusBar(), SLOT(showMessage(const QString&)) );

    TrussUnitWindowManager& mng = prj.getTrussUnitWindowManager();
    connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             geometryTabWidget, 
                    SLOT(trussUnitWindowWasCreated(TrussUnitWindow&)) );
    connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             trussPropTabWidget, 
                    SLOT(trussUnitWindowWasCreated(TrussUnitWindow&)) );

    connect( &prj, SIGNAL(modified()),
                     SLOT(projectModified()) );
    connect( &prj, SIGNAL(saved()),
                     SLOT(projectSaved()) );    
}

void FermaNextMainWindow::currentProjectChanged ()
{
    refreshUndoRedoActions();
    refreshProjectActions();
    refreshGeometryAndPropertyWindows();
    refreshCopyPaste();

    FermaNextProject* currProj = workspace.currentActivatedProject();
    if ( ! currProj )
        return;  

    modLabel->setEnabled( currProj->isProjectModified() );
}

void FermaNextMainWindow::createProject ()
{
    QString text = QInputDialog::getText(
            this,
            tr("Project creation"), 
            tr("Enter project name:") );
    if ( !text.isEmpty() ) {
        FermaNextProject& prj = workspace.createProject( text );
        TrussUnitWindowManager& mng = prj.getTrussUnitWindowManager();

//TODO: remove this in future
/*********** TEMP TRUSS UNIT **************************/
//#ifdef DEBUG
#if 1
        // Create truss
        TrussUnitWindow& trussWindow = mng.createTrussUnitWindow("demo Truss unit");
        // Set area size
        trussWindow.setTrussAreaSize( DoubleSize( 200.0, 300.0 ) );

        // Create material
        TrussMaterialLibrary& materialLib = prj.getMaterialLibrary();
        TrussMaterial* m = materialLib.getMaterial( 0 );
        if ( ! m )
            return;

        // Create nodes
        TrussNode& node1 = trussWindow.createNode ( 100.0, 0.0 );
        TrussNode& node2 = trussWindow.createNode ( 200.0, 150.0 );
        TrussNode& node3 = trussWindow.createNode ( 200.0, 300.0 );
        TrussNode& node4 = trussWindow.createNode ( 0.0, 0.0 );
        TrussNode& node5 = trussWindow.createNode ( 0.0, 300.0 );
        TrussNode& node6 = trussWindow.createNode ( 108.77, 50.55 );
        TrussNode& node7 = trussWindow.createNode ( 168.25, 169.81 );
        TrussNode& node8 = trussWindow.createNode ( 104.0, 85.0 );

        // Create pivots
        trussWindow.createPivot( node5, node3 ).setMaterial( m );
        trussWindow.createPivot( node3, node2 ).setMaterial( m );
        trussWindow.createPivot( node4, node1 ).setMaterial( m );
        trussWindow.createPivot( node6, node1 ).setMaterial( m );
        trussWindow.createPivot( node4, node6 ).setMaterial( m );
        trussWindow.createPivot( node6, node2 ).setMaterial( m );
        trussWindow.createPivot( node3, node7 ).setMaterial( m );
        trussWindow.createPivot( node5, node7 ).setMaterial( m );
        trussWindow.createPivot( node7, node2 ).setMaterial( m );
        trussWindow.createPivot( node4, node8 ).setMaterial( m );
        trussWindow.createPivot( node8, node6 ).setMaterial( m );
        trussWindow.createPivot( node5, node8 ).setMaterial( m );
        trussWindow.createPivot( node8, node7 ).setMaterial( m );

        // Set fixation
        node4.setFixation( Node::FixationByXY );
        node5.setFixation( Node::FixationByXY );

        TrussUnit::LoadCase& lc1 = *trussWindow.getLoadCases().findLoadCase( 1 );

        // Create 2 additional cases
        TrussUnit::LoadCase& lc2 = trussWindow.getLoadCases().createLoadCase();
        TrussUnit::LoadCase& lc3 = trussWindow.getLoadCases().createLoadCase();
       

        // Add loads       
        lc1.addLoad( node1, 0.0, -10000.0 );
        lc2.addLoad( node2, 0.0,  20000.0 );
        lc3.addLoad( node2, 0.0,  30000.0 );

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
    fileActions = addToolBar( tr("File Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&File" ) );

    QAction *a;
    // New
    a = new QAction( QIcon(Global::imagesPath() + "/filenew.png"), 
                     tr( "&New..." ), this );
    a->setShortcut( tr("CTRL+N") );
    a->setStatusTip( tr( "Creates a new project" ) );
    connect( a, SIGNAL(triggered()), SLOT(fileNew()) );
    fileActions->addAction( a );
    menu->addAction( a );

    // Open
    a = new QAction( QIcon(Global::imagesPath() + "/fileopen.png"), 
                      tr( "&Open..." ), this );
    a->setShortcut( tr("CTRL+O") );
    a->setStatusTip( tr( "Opens an existing project" ) );
    connect( a, SIGNAL(triggered()), SLOT(fileOpen()) );
    fileActions->addAction( a );
    menu->addAction( a );   

    // Save
    saveProjectAction = 
        new QAction( QIcon(Global::imagesPath() + "/filesave.png"),
                     tr( "&Save..." ), this );
    saveProjectAction->setShortcut( tr("CTRL+S") );
    saveProjectAction->setStatusTip( tr( "Saves the current project" ) );
    connect( saveProjectAction, SIGNAL(triggered()), SLOT(fileSave()) );
    fileActions->addAction( saveProjectAction );
    menu->addAction( saveProjectAction );
    saveProjectAction->setDisabled(true);
    menu->addSeparator();

    // Save All
    saveAllAction = 
        new QAction( QIcon(Global::imagesPath() + "/filesaveall.png"),
                     tr( "&Save All..." ), this );
    saveAllAction->setShortcut( tr("CTRL+SHIFT+S") );
    saveAllAction->setStatusTip( tr( "Saves all the open files" ) );
    connect( saveAllAction, SIGNAL(triggered()), SLOT(fileSaveAll()) );
    fileActions->addAction( saveAllAction );
    menu->addAction( saveAllAction );
    saveAllAction->setDisabled(true);
    menu->addSeparator();

    // Save As
    saveAsProjectAction = new QAction( tr( "Save &As..." ), this );
    saveAsProjectAction->setStatusTip( tr( "Saves the project with a new name" ) );
    connect( saveAsProjectAction, SIGNAL(triggered()), SLOT(fileSaveAs()) );
    menu->addAction( saveAsProjectAction );
    saveAsProjectAction->setDisabled(true);

    // Close
    closeProjectAction = new QAction( tr( "&Close" ), this );
    connect( closeProjectAction, SIGNAL(triggered()), SLOT(fileClose()) );
    closeProjectAction->setStatusTip( tr( "Closes the curent project" ) );
    menu->addAction( closeProjectAction );
    closeProjectAction->setDisabled(true);
    menu->addSeparator();

    // Open Wsp
    a = new QAction( tr( "Open Workspace..." ), this );
    a->setShortcut( tr("SHIFT+F2") );
    a->setStatusTip( tr( "Opens an existing workspace" ) );
    connect( a, SIGNAL(triggered()), SLOT(fileOpenWsp()) );
    menu->addAction( a );

    // Save Wsp
    a = new QAction( tr( "Save Workspace" ), this );
    a->setShortcut( tr("SHIFT+F3") );
    a->setStatusTip( tr( "Saves the workspace" ) );
    connect( a, SIGNAL(triggered()), SLOT(fileSaveWsp()) );
    menu->addAction( a );

    // Save Wsp As
    a = new QAction( tr( "Save Workspace As..." ), this );
    a->setStatusTip( tr( "Saves the workspace with a new name" ) );
    connect( a, SIGNAL(triggered()), SLOT(fileSaveWspAs()) );
    menu->addAction( a );

    // Close Wsp
    a = new QAction( tr( "Close Workspace" ), this );
    a->setStatusTip( tr( "Closes the workspace" ) );
    a->setShortcut( tr("SHIFT+F4") );
    connect( a, SIGNAL(triggered()), SLOT(fileCloseWsp()) );
    menu->addAction( a );
    menu->addSeparator();
    /*
    // Page Setup
    a = new QAction( tr( "Page setup..." ), this );
    a->setStatusTip( tr( "Changes the page layout settings" ) );
    connect( a, SIGNAL(triggered()), SLOT(filePageSetup()) );
    menu->addAction( a );
    a->setDisabled(true);

    // Print Preview
    a = new QAction( tr( "Print Preview" ), this );
    a->setShortcut( tr("CTRL+ALT+P") );
    a->setStatusTip( tr( "Shows the print preview" ) );
    connect( a, SIGNAL(triggered()), SLOT(filePrintPreview()) );
    menu->addAction( a );
    a->setDisabled(true);

    // Print
    printAction = new QAction( QIcon(Global::imagesPath() + "/print.png"), 
                               tr( "&Print..." ),  this ); 
    printAction->setShortcut( tr("CTRL+P") );
    printAction->setStatusTip( tr( "Prints the designer widget area" ) );
    connect( printAction, SIGNAL(triggered()), SLOT(filePrint()) );        
    menu->addAction( printAction );
    printAction->setDisabled(true);
    menu->addSeparator();    
*/
    // Exit
    a = new QAction( tr( "E&xit" ), this );
    a->setShortcut( tr("CTRL+Q") );
    a->setStatusTip( tr( "Quits the application; promts to save documents" ) );
    connect( a, SIGNAL(triggered()), SLOT(fileExit()) );
    menu->addAction( a );
}

void FermaNextMainWindow::setupEditActions ()
{
    editActions = addToolBar( tr("Edit Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&Edit" ) );

    // Undo
    undoAction = new QAction( QIcon(Global::imagesPath() + "/undo.png"), 
                              tr( "&Undo" ), this );
    undoAction->setStatusTip( tr( "Undoes the last action" ) );
    undoAction->setShortcut( tr("CTRL+Z") );                               
    connect( undoAction, SIGNAL(triggered()), SLOT(editUndo()) );
    undoAction->setDisabled(true);
    editActions->addAction( undoAction );
    menu->addAction( undoAction );

    // Redo
    redoAction = new QAction( QIcon(Global::imagesPath() + "/redo.png"), 
                              tr( "&Redo" ), this );
    redoAction->setShortcut( tr("CTRL+Y") );
    redoAction->setStatusTip( tr( "Redoes the previously undone action" ) );
    connect( redoAction, SIGNAL(triggered()), SLOT(editRedo()) );
    redoAction->setDisabled(true);
    editActions->addAction( redoAction );
    menu->addAction( redoAction );
    menu->addSeparator();
    editActions->addSeparator();

    // Copy
    copyAction = new QAction( QIcon(Global::imagesPath() + "/editcopy.png"), 
                              tr( "&Copy" ), this );
    copyAction->setShortcut( tr("CTRL+C") );
    copyAction->setStatusTip( tr( "Copies the selected truss to the Clipboard" ) );
    connect( copyAction, SIGNAL(triggered()), SLOT(editCopy()) );
    copyAction->setDisabled(true);
    editActions->addAction( copyAction );
    menu->addAction( copyAction );

    // Copy
    cutAction = new QAction( QIcon(Global::imagesPath() + "/editcut.png"), 
                              tr( "Cu&t" ), this );
    cutAction->setShortcut( tr("CTRL+X") );
    cutAction->setStatusTip( tr( "Cuts the selected truss and moves it to the Clipboard" ) );
    connect( cutAction, SIGNAL(triggered()), SLOT(editCut()) );
    cutAction->setDisabled(true);
    editActions->addAction( cutAction );
    menu->addAction( cutAction );

    // Paste
    pasteAction = new QAction( QIcon(Global::imagesPath() + "/editpaste.png"), 
                              tr( "&Paste" ), this );
    pasteAction->setShortcut( tr("CTRL+V") );
    pasteAction->setStatusTip( tr( "Inserts the truss from the Clipboard" ) );
    connect( pasteAction, SIGNAL(triggered()), SLOT(editPaste()) );
    pasteAction->setDisabled(true);
    editActions->addAction( pasteAction );
    menu->addAction( pasteAction );
    menu->addSeparator();
 
    // Material Editor
    materialEditorAction = new QAction( tr( "&Material Editor" ), this );
    materialEditorAction->setShortcut( tr("CTRL+M") );
    materialEditorAction->setStatusTip( tr( "Opens the material editor" ) );
    connect( materialEditorAction, SIGNAL(triggered()), 
                                   SLOT(editMaterials()) );
    menu->addAction( materialEditorAction );
    materialEditorAction->setEnabled( false );

    // Preferences
    prefAction = new QAction( tr( "P&references..." ), this );
    prefAction->setShortcut( tr("CTRL+R") );
    prefAction->setStatusTip( tr( "Edits the application preferences" ) );
    prefAction->setEnabled( false );
    connect( prefAction, SIGNAL(triggered()), SLOT(editPreferences()) );
    menu->addAction( prefAction ); 
}

void FermaNextMainWindow::setupViewActions ()
{
    viewActions = addToolBar( tr("View Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&View" ) );
    
    showUndoRedoAction = new QAction( QIcon(Global::imagesPath() + 
                                      "/undo_redo_window.png"),
                                      tr( "&History" ), this );
    showUndoRedoAction->setStatusTip( tr( "Shows history window" ) );
    showUndoRedoAction->setCheckable( true );
    menu->addAction( showUndoRedoAction );
    viewActions->addAction( showUndoRedoAction );
    connect( showUndoRedoAction, SIGNAL(toggled(bool)), 
             undoRedoHistoryWidget, SLOT(setVisible(bool)) );
    showUndoRedoAction->setEnabled( false );

    showGeometryWindowAction = 
        new QAction( QIcon(Global::imagesPath() + "/truss_geom.png"),
                     tr( "Truss &Geometry" ), this );
    showGeometryWindowAction->
        setStatusTip( tr( "Shows truss geometry window" ) );
    showGeometryWindowAction->setCheckable( true );
    menu->addAction( showGeometryWindowAction );
    viewActions->addAction( showGeometryWindowAction );
    connect( showGeometryWindowAction, SIGNAL(toggled(bool)),
             geometryWindow, SLOT(setVisible(bool) ) );
    showGeometryWindowAction->setEnabled( false );
    
    showTrussPropWindowAction = 
        new QAction( QIcon(Global::imagesPath() + "/truss_prop.png"),
                     tr( "Truss &Properties" ), this );
    showTrussPropWindowAction->
        setStatusTip( tr( "Shows truss properties window" ) );
    showTrussPropWindowAction->setCheckable( true );
    menu->addAction( showTrussPropWindowAction );
    viewActions->addAction( showTrussPropWindowAction );
    connect( showTrussPropWindowAction, SIGNAL(toggled(bool)), 
             trussPropertyWindow, SLOT(setVisible(bool)) );
    showTrussPropWindowAction->setEnabled( false ); 

    menu->addSeparator();

    QMenu* toolBarMenu = menu->addMenu( tr( "Tool Bar" ) );

    showFileActions = new QAction( tr( "&File Actions" ), this );
    showFileActions->setStatusTip( tr( "Shows file bar" ) );
    showFileActions->setCheckable( true );
    connect( showFileActions, SIGNAL(triggered(bool)), 
                 fileActions, SLOT(setVisible(bool)) );
    toolBarMenu->addAction( showFileActions ); 

    showEditActions = new QAction( tr( "&Edit Actions" ), this );
    showEditActions->setStatusTip( tr( "Shows edit bar" ) );
    showEditActions->setCheckable( true );
    connect( showEditActions, SIGNAL(triggered(bool)), 
                 editActions, SLOT(setVisible(bool)) );
    toolBarMenu->addAction( showEditActions ); 

    showViewActions = new QAction( tr( "&View Actions" ), this );
    showViewActions->setStatusTip( tr( "Shows view bar" ) );
    showViewActions->setCheckable( true );
    connect( showViewActions, SIGNAL(triggered(bool)), 
                 viewActions, SLOT(setVisible(bool)) );
    toolBarMenu->addAction( showViewActions ); 

    projectsDockAction = new QAction( tr( "Pro&jects" ), this );
    projectsDockAction->setStatusTip( tr( "Shows projects bar" ) );
    projectsDockAction->setCheckable( true );
    projectsDockAction->setEnabled( false );
    connect( projectsDockAction, SIGNAL(triggered(bool)), 
             projectsDockWidget, SLOT(setVisible(bool)) );
    menu->addAction( projectsDockAction ); 
}

void FermaNextMainWindow::setupProjectActions ()
{        
}

void FermaNextMainWindow::setupWindowActions ()
{
}

void FermaNextMainWindow::setupPluginsActions ()
{
    pluginsMenu = menuBar()->addMenu( tr( "&Plugins" ) );
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
    menu->addSeparator();

    // About
    a = new QAction( tr( "About FermaNext" ), this );
    connect( a, SIGNAL(triggered()), SLOT(helpAbout()) );
        menu->addAction( a );    
    a->setDisabled(true);
}

bool FermaNextMainWindow::onCopyTrussUnitWindow ()
{
    FermaNextProject* currProj = workspace.currentActivatedProject();
    if ( ! currProj )
        return false;

    TrussUnitWindow* selectedWindow = 
        currProj->getDesignerWidget().getFocusedWindow();
    if ( ! selectedWindow )
        return false;

    QDomDocument doc;
    QDomElement copyProps = selectedWindow->saveToXML( doc );
    doc.appendChild( copyProps );
    QString wndXml = doc.toString();

    TrussMaterialLibrary& mLib = currProj->getMaterialLibrary();

    // Parse truss materials 
    QStringList materialXmls;
    foreach ( TrussPivot* pivot, selectedWindow->getPivotList() ) {
        const TrussMaterial* m = pivot->getMaterial();
        if ( ! m )
            continue;

        materialXmls.append( mLib.getMaterialXml( *m ) );
    }

    workspace.setClipboardObject( wndXml, materialXmls );

    return true;
}

void FermaNextMainWindow::copyMaterialsForPivot ( QDomElement pivotDomElem,
                                                  QStringList materialXmls )
{
    if ( ! pivotDomElem.hasAttribute( "materialID" ) )
        return;

    QString pivotMaterialUUID = pivotDomElem.attribute( "materialID" );

    FermaNextProject* currProj = workspace.currentActivatedProject();
    if ( ! currProj )
        return;    

    TrussMaterialLibrary& mLib = currProj->getMaterialLibrary();

    // Get material for the current pivot 
    foreach ( QString mXml, materialXmls ) {
        QDomDocument doc;
        if ( ! doc.setContent( mXml ) )
            return;

        QDomNodeList mNodeList = doc.elementsByTagName( "TrussMaterial" );
        if ( mNodeList.isEmpty() )
            return;
        
        int nodeNumb = mNodeList.count();
        for ( int i = 0; i < nodeNumb; ++i ) {
            QDomElement mElem = mNodeList.at( i ).toElement();
            if ( ! mElem.hasAttribute( "uuid" ) )
                return;

            QString mUUID = mElem.attribute( "uuid" );

            if ( mUUID != pivotMaterialUUID )
                continue;

            TrussMaterial* sameMaterial = 
                mLib.getMaterialWithSameProperties( mXml );

            // No material with such properties was found, so we should create it
            if ( ! sameMaterial ) {
                TrussMaterial& newMaterial = mLib.createMaterial( mXml );
                pivotMaterialUUID = newMaterial.getUUID();
            }
            else 
                pivotMaterialUUID = sameMaterial->getUUID();

            // Replace pivot material UUID with UUID of the same material in the
            // current library
            pivotDomElem.setAttribute( "materialID", pivotMaterialUUID );
        }
    }
}

TrussUnitWindow* FermaNextMainWindow::onPasteTrussUnitWindow ()
{
    FermaNextProject* currProj = workspace.currentActivatedProject();
    if ( ! currProj )
        return 0;    

    const ClipBoardObject* clipboardObj = workspace.getClipboardObject();
    if ( ! clipboardObj )
        return 0;

    QString objXml = clipboardObj->getObjectXML();
    if ( objXml.isEmpty() )
        return 0;

    QDomDocument doc;
    if ( ! doc.setContent( objXml ) )
        return 0;

    QDomNodeList wndElemList = doc.elementsByTagName( "TrussUnitWindow" );
    if ( wndElemList.isEmpty() )
        return 0;

    QDomElement wndProps = wndElemList.at( 0 ).toElement();


    //------------ Set materials from other project if necessary ------------//

    QStringList materialXmls = clipboardObj->getMaterialXMLs();

    QDomNodeList pivotElemList = wndProps.elementsByTagName( "TrussPivot" );
    int elemNumb = pivotElemList.count();
    for ( int i = 0; i < elemNumb; ++i ) {
        QDomElement pivotElem = pivotElemList.at( i ).toElement();
        if ( pivotElem.isNull() )
            continue;

        copyMaterialsForPivot( pivotElem, materialXmls );
    }


    //------------------------ Create truss window --------------------------//

    TrussUnitWindowManager& wndMng = currProj->getTrussUnitWindowManager();

    TrussUnitWindow& copyWnd = wndMng.createTrussUnitWindow( "" );
    copyWnd.setVisible( false );
    
    // Save UUID to restore it after copy properties from XML string
    QString wndUuid = copyWnd.getUUID();

    try {
        copyWnd.loadFromXML( wndProps );
        copyWnd.setUUID( wndUuid );
    } 
    catch ( ... ) {
        wndMng.removeTrussUnitWindow( copyWnd );
        return 0;
    }

    QString copyWndName = copyWnd.getTrussName();
    QPoint copyWndPos = copyWnd.getWindowLeftTopPos();

    foreach ( TrussUnitWindow* wnd, wndMng.getTrussUnitWindowList() ) {
        if ( wnd != &copyWnd && wnd->getWindowLeftTopPos() == copyWndPos ) {
            // Add single indent to the window position
            copyWndPos = QPoint( copyWndPos + QPoint( 15, 10 ) );
            copyWnd.setWindowPosition( copyWndPos );
        }
        if ( wnd != &copyWnd && wnd->getTrussName() == copyWndName ) {
            copyWndName.append( "_copy" );
            copyWnd.setTrussName( copyWndName );
        }
    }

    return &copyWnd;
}

void FermaNextMainWindow::reloadPlugins ()
{
    emit reloadPluginsFromMainEventLoop();
}

void FermaNextMainWindow::onReloadPluginsFromMainEventLoop ()
{
    reloadPlugins( true );
}

void FermaNextMainWindow::showPluginInfo ( const QString& plgPath )
{
    Plugin* pluginToShow = 0;
    PluginManager& plgManager = workspace.pluginManager();
    PluginList plugins = plgManager.loadedPlugins();
    foreach ( Plugin* plugin, plugins ) {
        if ( plugin->pluginPath() == plgPath ) {
            pluginToShow = plugin;
            break;
        }
    }

    if ( pluginToShow != 0 ) {
        // Show plugin info
        QString name = pluginToShow->pluginInfo().name;
        QString desc = pluginToShow->pluginInfo().description;
        QString type = pluginToShow->pluginInfo().type;

        QString text = QString("<table><tr><td align=\"right\"><b>Name:</b>"
                                "</td><td>%2</td></tr><tr><td align=\"right\">"
                                "<b>Path:</b></td><td>%1</td></tr><tr>"
                                "<td align=\"right\"><b>Type:</b></td>"
                                "<td>%4</td></tr><tr><td align=\"right\">"
                                "<b>Descr:</b></td><td>%3</td></tr></table>")
                         .arg(plgPath).arg(name).arg(desc).arg(type);

        QMessageBox::information( this, "Plugin information", 
                                  text, QMessageBox::Ok );
    }
    else {
        // Was not found
        QMessageBox::warning( this, "Plugin was not found", 
                              QString("<b>Wrong plugin path:</b><br>%1")
                                .arg(plgPath) );
    }

}

void FermaNextMainWindow::reloadPlugins ( bool reload )
{
    PluginManager& plgManager = workspace.pluginManager();

    if ( reload ) {
        PluginReloader::reloadPlugins( plgManager );
    }

    pluginsMenu->clear();

    PluginList plugins = plgManager.loadedPlugins();
    foreach ( Plugin* plugin, plugins ) {
        QString plgName = plugin->pluginInfo().name;
        QString plgPath = plugin->pluginPath();
    
        QAction* act = pluginsMenu->addAction( QIcon(Global::imagesPath() + 
                                                     "/calculate.png"),
                                               plgName, pluginsSigMapper, 
                                               SLOT(map()) );
        pluginsSigMapper->setMapping( act, plgPath );
    }
    pluginsMenu->addSeparator();

    // Reload plugins
    QAction* a = new QAction( tr("Reload plugins"), this );
    a->setStatusTip( tr( "Reloads all available plugins" ) );
    connect( a, SIGNAL(triggered()), SLOT(reloadPlugins()) );
    pluginsMenu->addAction( a );
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
    saveAllAction->setEnabled(enableActions);
    saveAsProjectAction->setEnabled(enableActions);
    closeProjectAction->setEnabled(enableActions);
    //printAction->setEnabled(enableActions);
    materialEditorAction->setEnabled( enableActions );
}

void FermaNextMainWindow::refreshPluginsActions ()
{
    // Just read info about plugins
    reloadPlugins( false )  ;
}

void FermaNextMainWindow::refreshGeometryAndPropertyWindows ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( ! prj )
        return;
    
    TrussUnitWindow* window = prj->getDesignerWidget().getFocusedWindow();
    geometryTabWidget->changeFocusWindow( window );
    trussPropTabWidget->changeFocusWindow( window );
    trussPropTabWidget->changeMaterialLibrary( prj->getMaterialLibrary() );
}

void FermaNextMainWindow::refreshCopyPaste ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( ! prj )
        return;
    
    bool windowSelected = false;
    TrussUnitWindow* window = prj->getDesignerWidget().getFocusedWindow();
    if ( window )
        windowSelected = true;

    copyAction->setEnabled( windowSelected );
    cutAction->setEnabled( windowSelected );
}

void FermaNextMainWindow::updateStatusBar ()
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( ! prj ) {
        coordLabel->setText( " " );
        elementLabel->setText( " " );
        return;
    }

    TrussUnitWindow* window = 0;
    WindowList windows = prj->getDesignerWidget().getTrussUnitWindowList();
    WindowListConstIter wIter = windows.begin();
    for ( ; wIter != windows.end(); ++wIter ) {
        if ( (*wIter)->getCursorCoord().x() != -1.0 ) {
            window = *wIter;
            break;
        }
    }

    if ( ! window ) {
        coordLabel->setText( " " );
        elementLabel->setText( " " );
        return;
    }

    DoublePoint cursorCoord = window->getCursorCoord();
    QString str;
    str = QString( "%1" ).arg( cursorCoord.x(),0,'f',2 );
    str.append(" : ");
    str.append( QString( "%1" ).arg( cursorCoord.y(),0,'f',2 ) );  
    coordLabel->setText( str );

    const TrussNode* node = window->getFocusedNode();
    const TrussPivot* pivot = window->getFocusedPivot();
    if ( node && ! pivot )
        elementLabel->setText( tr( "Node #" ) + 
                               QString::number(node->getNumber()) );
    else if ( pivot )
        elementLabel->setText( tr( "Pivot #" ) + 
                               QString::number(pivot->getNumber()) );
    else
        elementLabel->setText( " " );
}

void FermaNextMainWindow::projectModified ()
{
    modLabel->setEnabled( true );
}

void FermaNextMainWindow::projectSaved ()
{
    modLabel->setEnabled( false );
}

void FermaNextMainWindow::trussWindowLostFocus ( TrussUnitWindow& window )
{
    ObjectStateManager* mng = window.getStateManager();
    if ( mng )
        mng->disconnect( this );

    refreshUndoRedoActions();
    
    copyAction->setEnabled( false );
    cutAction->setEnabled( false );

    geometryTabWidget->changeFocusWindow( 0 );
    trussPropTabWidget->changeFocusWindow( 0 );
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

    copyAction->setEnabled( true );
    cutAction->setEnabled( true );
    
    geometryTabWidget->changeFocusWindow( &window );
    trussPropTabWidget->changeFocusWindow( &window );
}

void FermaNextMainWindow::pluginWasExecuted ( 
    Plugin* plg, 
    const Plugin::ExecutionResult& exRes )
{
    FermaNextProject* currentPrj = projectToolBox->currentProject();
    TrussResultsManager& resMng = currentPrj->getTrussResultsManager();
    
    QString errMsg;

    if ( ! resMng.parseExecutionResults(exRes, errMsg) ) {
        QMessageBox::critical( this, tr( "Plugin internal error" ), 
                               tr("%1: %2").arg(plg->pluginInfo().name).
                               arg(errMsg) );
    }
    else {
        const TrussSolutionResults& res = *resMng.getResultsList().back();
        TrussUnitWindow* w = resMng.findTrussByResults( res );
        if ( w )
            showResultsWindow( *w );
    }
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
    QString fileName = QFileDialog::getOpenFileName( 
        this,
        tr("Open ferma project"),
        QString(),
        "Ferma project (*" + FermaNextProject::formatExtension() + ")" );

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
/*
    if ( prjFileInfo.exists() ) {
        if ( QMessageBox::question( this,
                                    tr("File exists"),
                                    tr("Rewrite file \"%1\"?").arg(fileName),
                                    tr("&Yes"), tr("&No"),
                                    QString::null, 0, 1 ) )
            return false;
    }
*/
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

    if ( currentPrj->isProjectModified() )
        if ( ! QMessageBox::question( this,
                                tr("Project closing - \"%1\"").
                                    arg(currentPrj->getName()),
                                tr("Save project \"%1\"?").
                                    arg(currentPrj->getName()),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) ) 
        {
            try { currentPrj->saveToFile(); }
            catch ( FermaNextProject::FileNameIsNotDefinedException& ) {
                fileSaveAs();
            }
        }
    
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

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open ferma workspace"),
        QString(),
        "Ferma workspace (*" + FermaNextWorkspace::formatExtension() + ")" );

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
    QString fileName = QFileDialog::getSaveFileName( 
        this,
        tr("Save ferma workspace"),
        QString(),
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

void FermaNextMainWindow::restoreApplicationSettings ()
{
    QSettings appSettings;

    //******************* Geometry 

    appSettings.beginGroup( "Geometry" );

    // Main window

    if ( appSettings.contains( "MainWndWidth" ) &&
        appSettings.contains( "MainWndHeight" ) ) {
        int w = appSettings.value( "MainWndWidth" ).toInt();
        int h = appSettings.value( "MainWndHeight" ).toInt();
        resize( w, h );
    }
        
    if ( appSettings.contains( "MainWndX" ) &&
        appSettings.contains( "MainWndY" ) ) {
        int x = appSettings.value( "MainWndX" ).toInt();
        int y = appSettings.value( "MainWndY" ).toInt();
        move( x, y );
    }

    // Undo-redo

    int height;
    if ( appSettings.contains( "UndoRedoHeight" ) )
        height = appSettings.value( "UndoRedoHeight" ).toInt();
    else
        height = 120;

    int xPos, yPos;
    if ( appSettings.contains( "UndoRedoXPosition" ) )
        xPos = appSettings.value( "UndoRedoXPosition" ).toInt();
    else
        xPos = QApplication::desktop()->width() - 215;

    if ( appSettings.contains( "UndoRedoYPosition" ) )
        yPos = appSettings.value( "UndoRedoYPosition" ).toInt();
    else
        yPos = 100;

    undoRedoHistoryWidget->resize( 196, height );
    undoRedoHistoryWidget->move( xPos, yPos );

    // Geometry window

    if ( appSettings.contains( "GeometryWindowHeight" ) )
        height = appSettings.value( "GeometryWindowHeight" ).toInt();
    else
        height = 170;

    if ( appSettings.contains( "GeometryWindowXPosition" ) )
        xPos = appSettings.value( "GeometryWindowXPosition" ).toInt();
    else
        xPos = QApplication::desktop()->width() - 215;

    if ( appSettings.contains( "GeometryWindowYPosition" ) )
        yPos = appSettings.value( "GeometryWindowYPosition" ).toInt();
    else
        yPos = 250;

    geometryWindow->resize( 196, height );
    geometryWindow->move( xPos, yPos );

    if ( appSettings.contains( "PropertyWindowHeight" ) )
        height = appSettings.value( "PropertyWindowHeight" ).toInt();
    else
        height = 224;

    // Truss property window

    if ( appSettings.contains( "PropertyWindowXPosition" ) )
        xPos = appSettings.value( "PropertyWindowXPosition" ).toInt();
    else
        xPos = QApplication::desktop()->width() - 215;

    if ( appSettings.contains( "PropertyWindowYPosition" ) )
        yPos = appSettings.value( "PropertyWindowYPosition" ).toInt();
    else
        yPos = 450;

    trussPropertyWindow->resize( 196, height );
    trussPropertyWindow->move( xPos, yPos );

    appSettings.endGroup();

    appSettings.beginGroup( "Preferences" );

    if ( ! appSettings.contains( "ShowFileActions" ) )
        showFileActions->setChecked( true );
    else
        showFileActions->setChecked( 
                appSettings.value( "ShowFileActions" ).toBool() );

    if ( ! appSettings.contains( "ShowEditActions" ) )
        showEditActions->setChecked( true );
    else
        showEditActions->setChecked( 
                appSettings.value( "ShowEditActions" ).toBool() );

    if ( ! appSettings.contains( "ShowViewActions" ) )
        showViewActions->setChecked( true );
    else
        showViewActions->setChecked( 
                appSettings.value( "ShowViewActions" ).toBool() );

    appSettings.endGroup();
}

void FermaNextMainWindow::saveApplicationSettings ()
{
    QSettings appSettings;

    appSettings.beginGroup( "Geometry" );

    appSettings.setValue( "MainWndWidth", width() );
    appSettings.setValue( "MainWndHeight", height() );

    appSettings.setValue( "MainWndX", pos().x() );
    appSettings.setValue( "MainWndY", pos().y() );

    appSettings.setValue( "UndoRedoHeight", undoRedoHistoryWidget->height() );
    appSettings.setValue( "UndoRedoXPosition", undoRedoHistoryWidget->pos().x() );
    appSettings.setValue( "UndoRedoYPosition", undoRedoHistoryWidget->pos().y() );
    
    appSettings.setValue( "GeometryWindowHeight", geometryWindow->height() );
    appSettings.setValue( "GeometryWindowXPosition", geometryWindow->pos().x() );
    appSettings.setValue( "GeometryWindowYPosition", geometryWindow->pos().y() );

    appSettings.setValue( "PropertyWindowHeight",
                trussPropertyWindow->height() );
    appSettings.setValue( "PropertyWindowXPosition", 
                trussPropertyWindow->pos().x() );
    appSettings.setValue( "PropertyWindowYPosition", 
                trussPropertyWindow->pos().y() );

    appSettings.endGroup();

    appSettings.beginGroup( "Preferences" );

    appSettings.setValue( "ShowFileActions", showFileActions->isChecked() );
    appSettings.setValue( "ShowEditActions", showEditActions->isChecked() );
    appSettings.setValue( "ShowViewActions", showViewActions->isChecked() );
    appSettings.setValue( "ShowProjectsBar", projectsDockAction->isChecked() );
    appSettings.setValue( "ShowUndoRedo", showUndoRedoAction->isChecked() );
    appSettings.setValue( "ShowGeometryWindow", 
                showGeometryWindowAction->isChecked() );
    appSettings.setValue( "ShowPropertyWindow", 
                showTrussPropWindowAction->isChecked() );

    appSettings.endGroup();
}

bool FermaNextMainWindow::checkProjectsBeforeClose ()
{
    FermaNextWorkspace::ProjectList projects = workspace.getProjectList();
    FermaNextWorkspace::ProjectListIter iter =  projects.begin();

    for ( ; iter < projects.end(); ++iter ) {
        FermaNextProject* proj = *iter;
        if ( ! proj->isProjectModified() ) 
            continue;

        int res = QMessageBox::question( this, 
                            tr("Project closing - \"%1\"").arg(proj->getName()),
                            tr("Save project \"%1\"?").arg(proj->getName()),
                            tr("&Yes"), tr("&No"), tr("&Cancel"), 0, 2 );

        // No button was pressed
        if ( res == 1 )
            continue;

        // Cancel button was pressed
        else if ( res == 2 )
            return false;

        try { proj->saveToFile(); }
        catch ( FermaNextProject::FileNameIsNotDefinedException& ) {
            if ( ! fileSaveAs() )
                return false;
        }
    }

    return true;
}

void FermaNextMainWindow::fileExit ()
{
    // Return if user press Cancel
    if ( ! checkProjectsBeforeClose() )
        return;

    saveApplicationSettings();
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
    bool copyResult = onCopyTrussUnitWindow();
    pasteAction->setEnabled( copyResult );
}

void FermaNextMainWindow::editCut ()
{
    bool copyResult = onCopyTrussUnitWindow();
    if ( copyResult ) {
        FermaNextProject* currProj = workspace.currentActivatedProject();
        TrussUnitWindowManager& wndMng = 
            currProj->getTrussUnitWindowManager();
        TrussUnitWindow* focusedWindow = 
            currProj->getDesignerWidget().getFocusedWindow();
        if ( ! focusedWindow )
            return;

        wndMng.removeTrussUnitWindow( *focusedWindow );
        pasteAction->setEnabled( true );
    }    
}

void FermaNextMainWindow::editPaste ()
{
    onPasteTrussUnitWindow();
}

void FermaNextMainWindow::editSelectAll ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainWindow::editMaterials ()
{
    TrussMaterialEditor* materialEditor = 
        new TrussMaterialEditor( materialEditorStartPage, this );
    FermaNextProject* currProject = workspace.currentActivatedProject();

    foreach ( FermaNextProject* project, workspace.getProjectList() )
        if ( project == currProject )
            materialEditor->addProject( *project, true );
        else
            materialEditor->addProject( *project, false );

    materialEditor->exec();

}

void FermaNextMainWindow::editPreferences ()
{   
    preferencesWidget = new PreferencesWidget( this );
    int result = preferencesWidget->exec();    

    if ( result ) {
        FermaNextProject* prj = projectToolBox->currentProject();
        if ( prj == 0 )
            return;
        prj->getDesignerWidget().redrawAllTrussUnits();
    }
}

void FermaNextMainWindow::showResultsWindow ( const TrussUnitWindow& w )
{
    FermaNextProject* prj = projectToolBox->currentProject();
    if ( prj == 0 )
        return;

    TrussResultsManager& resMng = prj->getTrussResultsManager();
    TrussSolutionResults* trussResults = 
        resMng.getResultsForTrussUnit( w.getUUID() );
    if ( ! trussResults ) {
        if ( ! QMessageBox::question( this, tr("Calculate truss unit"),
                                      tr("Calculate truss \"%1\"?").
                                      arg(w.getTrussName()),
                                      tr("&Yes"), tr("&No"),
                                      QString::null, 0, 1 ) )
            emit calculateTrussUnit( w );
        else
            QMessageBox::information( this, tr( "Calculate truss unit" ), 
                            tr( "There are no results for current truss" ), 
                            QMessageBox::Ok );
    }
    else {
        if ( ! w.isCalculated() ) {
            if ( ! QMessageBox::question( this, tr("Recalculate truss"),
                                          tr("Truss \"%1\" was changed.\n"
                                             "Recalculate truss?").
                                          arg(w.getTrussName()),
                                          tr("&Yes"), tr("&No"),
                                          QString::null, 0, 1 ) )
                emit calculateTrussUnit( w );
            else {
                resultsWindow->setWindowTitle( tr( "Solve results for" ) + 
                               w.getTrussName() );
                resultsTabWidget->setTrussSolutionResults( *trussResults );
                resultsWindow->exec();
            }

        } 
        else {
            resultsWindow->setWindowTitle( tr( "Results for \"%1\"" ). 
                           arg( w.getTrussName() ) );
            resultsTabWidget->setTrussSolutionResults( *trussResults );
            resultsWindow->exec();
        }
    }
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
                prj->getDesignerWidget().aggKeyPressEvent( keyEvent );
                return true;
            }
        }
        return false;
    }
    else if ( event->type() == QEvent::Close ) {
        if ( targetObj == undoRedoHistoryWidget )
            showUndoRedoAction->toggle();
        else if ( targetObj == geometryWindow )
            showGeometryWindowAction->toggle();            
        else if ( targetObj == trussPropertyWindow )
            showTrussPropWindowAction->toggle();
        return true;
    } 
    else
        return QMainWindow::eventFilter( targetObj, event );
}

/*
 *  Correct clean.
 */
void FermaNextMainWindow::closeEvent ( QCloseEvent* event )
{
    // Do nothing if user had pressed Cancel
    if ( ! checkProjectsBeforeClose() ) {
        event->ignore();
        return;
    }

    saveApplicationSettings();
    workspace.quitFermaNextApplication();
}

/*****************************************************************************/
