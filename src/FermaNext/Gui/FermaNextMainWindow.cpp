
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
}

FermaNextMainWindow::~FermaNextMainWindow ()
{}

void FermaNextMainWindow::init ()
{
    setWindowTitle( fermaTitle);
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
                             SLOT(refreshUndoRedoActions()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshProjectActions()) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshGeometryAndPropertyWindows()) );
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
    QObject::connect( pluginsSigMapper, SIGNAL(mapped(const QString &)),
                                        SLOT(showPluginInfo(const QString &)));
}

void FermaNextMainWindow::initUndoRedoWindow ()
{
    undoRedoHistoryWidget = new QWidget( this, Qt::Window | Qt::Tool );

    undoRedoHistoryWidget->setFixedSize( 196, 120 );
    // Pretty history widget offset from the end point of the screen
    undoRedoHistoryWidget->move( QApplication::desktop()->width() - 215, 100 );
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
    geometryWindow->setFixedSize( 196, 170 );
    geometryWindow->setWindowTitle( tr("Truss Geometry") );
    geometryTabWidget = new GeometryTabWidget( geometryWindow );
    geometryWindow->move( QApplication::desktop()->width() - 215, 250 );
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
    trussPropertyWindow->setFixedSize( 196, 224 );
    trussPropertyWindow->setWindowTitle( tr("Truss Properties") );
    trussPropTabWidget = new TrussPropertyTabWidget( trussPropertyWindow );
    trussPropertyWindow->move( QApplication::desktop()->width() - 215, 450 );
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
    QWidget* statusBarWidget = new QWidget;
    QHBoxLayout* statusBarLayout = new QHBoxLayout( statusBarWidget );
    statusBarLayout->setMargin(2);
    statusBarLayout->setSpacing(2);

    hintLabel = new QLabel( "  Ready" );
    // statusBar()->addWidget( hintLabel, 1 );

    coordLabel = new QLabel( "  999.99 : 999.99  " );
    coordLabel->setAlignment( Qt::AlignCenter );
    coordLabel->setMinimumSize( coordLabel->sizeHint() );
    coordLabel->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    //statusBar()->addPermanentWidget( coordLabel );

    elementLabel = new QLabel( " pivot #999 " );
    elementLabel->setAlignment( Qt::AlignCenter );
    elementLabel->setMinimumSize( elementLabel->sizeHint() );
    elementLabel->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    //statusBar()->addPermanentWidget( elementLabel );

    modLabel = new QLabel( " MOD " );
    modLabel->setEnabled( false );
    modLabel->setAlignment( Qt::AlignCenter );
    modLabel->setMinimumSize( modLabel->sizeHint() );
    modLabel->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    // statusBar()->addWidget( modLabel );

    statusBarLayout->addWidget( hintLabel, 1 );
    statusBarLayout->addWidget( coordLabel );
    statusBarLayout->addWidget( elementLabel );
    statusBarLayout->addWidget( modLabel );

    statusBar()->addPermanentWidget( statusBarWidget, 1 );

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
        showUndoRedoAction->setEnabled( false );
        showTrussPropWindowAction->setEnabled( false );
        showGeometryWindowAction->setEnabled( false );
    }
    TrussDesignerWidget& designerWidget = prj.getDesignerWidget();
    designerWidget.disconnect( this );
}

void FermaNextMainWindow::someProjectCreated ( FermaNextProject& prj )
{
    if ( !projectsDockWidget->isVisible() && 0 < workspace.countProjects() ) {
        projectsDockWidget->show();
        showUndoRedoAction->setEnabled( true );
        showTrussPropWindowAction->setEnabled( true );
        showGeometryWindowAction->setEnabled( true );
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
    connect( &designerWidget, SIGNAL(onHintChange(const QString&)),
                              SLOT(setStatusBarHint(const QString&)) );

    TrussUnitWindowManager& mng = prj.getTrussUnitWindowManager();
    connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             geometryTabWidget, 
                    SLOT(trussUnitWindowWasCreated(TrussUnitWindow&)) );
    connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             trussPropTabWidget, 
                    SLOT(trussUnitWindowWasCreated(TrussUnitWindow&)) );
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
        TrussMaterial* m = &materialLib.createMaterial( tr( "Aluminum Alloy" ), 
                                                       30000, 7000000, 0.028 );
        materialLib.createMaterial( tr( "Steel" ), 70000, 20000000, 0.078 );

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
    QToolBar* tb = addToolBar( tr("File Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&File" ) );

    QAction *a;
    // New
    a = new QAction( QIcon(Global::imagesPath() + "/filenew.png"), 
                     tr( "&New..." ), this );
    a->setShortcut( tr("CTRL+N") );
    connect( a, SIGNAL(triggered()), SLOT(fileNew()) );
    tb->addAction( a );
    menu->addAction( a );

    // Open
    a = new QAction( QIcon(Global::imagesPath() + "/fileopen.png"), 
                      tr( "&Open..." ), this );
    a->setShortcut( tr("CTRL+O") );
    connect( a, SIGNAL(triggered()), SLOT(fileOpen()) );
    tb->addAction( a );
    menu->addAction( a );   

    // Save
    saveProjectAction = 
        new QAction( QIcon(Global::imagesPath() + "/filesave.png"),
                     tr( "&Save..." ), this );
    saveProjectAction->setShortcut( tr("CTRL+S") );
    connect( saveProjectAction, SIGNAL(triggered()), SLOT(fileSave()) );
    tb->addAction( saveProjectAction );
    menu->addAction( saveProjectAction );
    saveProjectAction->setDisabled(true);
    menu->addSeparator();

    // Save All
    saveAllAction = 
        new QAction( QIcon(Global::imagesPath() + "/filesaveall.png"),
                     tr( "&Save All..." ), this );
    saveAllAction->setShortcut( tr("CTRL+SHIFT+S") );
    connect( saveAllAction, SIGNAL(triggered()), SLOT(fileSaveAll()) );
    tb->addAction( saveAllAction );
    menu->addAction( saveAllAction );
    saveAllAction->setDisabled(true);
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
    printAction = new QAction( QIcon(Global::imagesPath() + "/print.png"), 
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
    QToolBar* tb = addToolBar( tr("Edit Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&Edit" ) );

    // Undo
    undoAction = new QAction( QIcon(Global::imagesPath() + "/undo.png"), 
                              tr( "&Undo" ), this );
    undoAction->setShortcut( tr("CTRL+Z") );                               
    connect( undoAction, SIGNAL(triggered()), SLOT(editUndo()) );
    undoAction->setDisabled(true);
    tb->addAction( undoAction );
    menu->addAction( undoAction );

    // Redo
    redoAction = new QAction( QIcon(Global::imagesPath() + "/redo.png"), 
                              tr( "&Redo" ), this );
    redoAction->setShortcut( tr("CTRL+Y") );
    connect( redoAction, SIGNAL(triggered()), SLOT(editRedo()) );
    redoAction->setDisabled(true);
    tb->addAction( redoAction );
    menu->addAction( redoAction );
    menu->addSeparator();
    tb->addSeparator();

    // Material Editor
    materialEditorAction = new QAction( tr( "&Material Editor" ), this );
    materialEditorAction->setShortcut( tr("CTRL+M") );
    connect( materialEditorAction, SIGNAL(triggered()), 
                                   SLOT(editMaterials()) );
    menu->addAction( materialEditorAction );
    materialEditorAction->setEnabled( false );

    // Preferences
    QAction* prefAction = new QAction( tr( "&Preferences..." ), this );
    prefAction->setShortcut( tr("CTRL+K") );
    connect( prefAction, SIGNAL(triggered()), SLOT(editPreferences()) );
    menu->addAction( prefAction );
}

void FermaNextMainWindow::setupViewActions ()
{
    QToolBar* tb = addToolBar( tr("View Actions") );
    QMenu* menu = menuBar()->addMenu( tr( "&View" ) );
    
    // Contents
    showUndoRedoAction = new QAction( QIcon(Global::imagesPath() + 
                                      "/undo_redo_window.png"),
                                      tr( "&Show History Window" ), this );
    showUndoRedoAction->setStatusTip( tr( "Show or hide history window" ) );
    showUndoRedoAction->setCheckable( true );
    showUndoRedoAction->setChecked( true );
    menu->addAction( showUndoRedoAction );
    tb->addAction( showUndoRedoAction );
    connect( showUndoRedoAction, SIGNAL(toggled(bool)), 
             undoRedoHistoryWidget, SLOT(setVisible(bool)) );
    showUndoRedoAction->setEnabled( false );

    showGeometryWindowAction = 
        new QAction( QIcon(Global::imagesPath() + "/truss_geom.png"),
                     tr( "&Show Truss Geometry Window" ), this );
    showGeometryWindowAction->
        setStatusTip( tr( "Show or hide truss geometry window" ) );
    showGeometryWindowAction->setCheckable( true );
    showGeometryWindowAction->setChecked( true );
    menu->addAction( showGeometryWindowAction );
    tb->addAction( showGeometryWindowAction );
    connect( showGeometryWindowAction, SIGNAL(toggled(bool)),
             geometryWindow, SLOT(setVisible(bool) ) );
    showGeometryWindowAction->setEnabled( false );
    
    showTrussPropWindowAction = 
        new QAction( QIcon(Global::imagesPath() + "/truss_prop.png"),
                     tr( "&Show Truss Property Window" ), this );
    showTrussPropWindowAction->
        setStatusTip( tr( "Show or hide truss property window" ) );
    showTrussPropWindowAction->setCheckable( true );
    showTrussPropWindowAction->setChecked( true );
    menu->addAction( showTrussPropWindowAction );
    tb->addAction( showTrussPropWindowAction );
    connect( showTrussPropWindowAction, SIGNAL(toggled(bool)), 
             trussPropertyWindow, SLOT(setVisible(bool)) );
    showTrussPropWindowAction->setEnabled( false );  
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
    saveAllAction->setEnabled(enableActions);
    saveAsProjectAction->setEnabled(enableActions);
    closeProjectAction->setEnabled(enableActions);
    printAction->setEnabled(enableActions);
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

void FermaNextMainWindow::setStatusBarHint ( const QString& text )
{
    hintLabel->setText( " " + text );
}

void FermaNextMainWindow::projectModified ()
{}

void FermaNextMainWindow::trussWindowLostFocus ( TrussUnitWindow& window )
{
    ObjectStateManager* mng = window.getStateManager();
    if ( mng )
        mng->disconnect( this );

    refreshUndoRedoActions();

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
    preferencesWidget->resize( 300, 300 );
    preferencesWidget->setWindowTitle( tr("Preferences") );
    preferencesWidget->exec();    
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
        if ( ! QMessageBox::question( 0, tr("Calculate truss unit"),
                                      tr("Calculate truss \"%1\"?").
                                      arg(w.getTrussName()),
                                      tr("&Yes"), tr("&No"),
                                      QString::null, 0, 1 ) )
            emit calculateTrussUnit( w );
        else
            QMessageBox::information( 0, tr( "Calculate truss unit" ), 
                            tr( "There are no results for current truss" ), 
                            QMessageBox::Ok );
    }
    else {
        if ( ! w.isCalculated() ) {
            if ( ! QMessageBox::question( 0, tr("Recalculate truss"),
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
void FermaNextMainWindow::closeEvent ( QCloseEvent* )
{
    workspace.quitFermaNextApplication();
}

/*****************************************************************************/
