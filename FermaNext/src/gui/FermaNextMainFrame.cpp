
#include "FermaNextMainFrame.h"
#include "FermaNextWorkspace.h"
#include "ProjectToolBox.h"
#include "SubsidiaryConstants.h"
#include "UndoRedoListBox.h"
#include "TrussGeometryWindow.h"

#include <qapplication.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qaccel.h>
#include <qtoolbox.h>
#include <qpainter.h>
#include <qwidgetstack.h>
#include <qstylefactory.h>
#include <qaction.h>
#include <qsignalmapper.h>
#include <qdict.h>
#include <qdir.h>
#include <qtextcodec.h>
#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qinputdialog.h> 
#include <qmessagebox.h>
#include <qfiledialog.h>

const QString mainFrameCaption( QObject::tr( "Educational CAD System 'Ferma'" ) );

/*****************************************************************************
 * FermaNext Main Frame
 *****************************************************************************/

FermaNextMainFrame::FermaNextMainFrame ( QWidget* p, const char* n, 
                                         WFlags f ) :
    QMainWindow(p, n, f),
    pluginsMenu(0)
{      
    init();

    // Initial toolbar setup
    setupFileActions();
    setupEditActions();
    setupViewActions ();
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

    setCaption(mainFrameCaption);
    setMinimumSize( 640, 480 );
    statusBar()->addWidget(new QLabel( tr("Ready"), statusBar() ));

    undoRedoHistoryWidget = new QWidget( this, "undo_redo_history",
                                         WStyle_Tool | WType_TopLevel );

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
    undoRedoHistoryWidget->installEventFilter( this );

    geometryWindow = new TrussGeometryWindow( this, "truss_geometry",
                                              WStyle_Tool | WType_TopLevel );
    geometryWindow->move( QApplication::desktop()->width() - 265, 310 );
    
    projectsDockWindow = new QDockWindow( QDockWindow::InDock, this );
    projectsDockWindow->setResizeEnabled( TRUE );
    projectsDockWindow->setVerticalStretchable( TRUE );
    projectsDockWindow->setCaption( tr("Truss Unit Projects") );
    addDockWindow( projectsDockWindow, DockLeft );
    setDockEnabled( projectsDockWindow, DockTop, FALSE );
    setDockEnabled( projectsDockWindow, DockBottom, FALSE );
    projectsDockWindow->setCloseMode( QDockWindow::Always );

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
    projectsDockWindow->setFixedExtentWidth( 160 );
    projectsDockWindow->hide();

    setRightJustification( true );
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
        geometryWindow->show();
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
            tr("Project creation"), 
            tr("Enter project name:"), QLineEdit::Normal,
            QString::null, &ok, this );
    if ( ok && !text.isEmpty() ) {
        FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();
        FermaNextProject& prj = wsp.createProject( text );
        TrussUnitWindowManager& mng = prj.getTrussUnitWindowManager();

        connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
                 geometryWindow, SLOT(trussUnitWindowWasCreated(TrussUnitWindow&)) );

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
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("File Actions") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&File" ), menu );

    QAction *a;
    // New
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filenew.xpm" ), 
                     tr( "&New..." ), CTRL + Key_N, this, "fileNew" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Open
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/fileopen.xpm" ), 
                     tr( "&Open..." ), CTRL + Key_O, this, "fileOpen" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    a->addTo( tb );
    a->addTo( menu );    

    // Save
    saveProjectAction = 
        new QAction( QPixmap::fromMimeSource( imagesPath() + "/filesave.xpm" ),
                     tr( "&Save..." ), CTRL + Key_S, this, "fileSave" );
    connect( saveProjectAction, SIGNAL( activated() ), 
             this, SLOT( fileSave() ) );
    saveProjectAction->addTo( tb );
    saveProjectAction->addTo( menu );
    saveProjectAction->setDisabled(true);
    menu->insertSeparator();

    // Save As
    saveAsProjectAction = 
        new QAction( tr( "Save &As..." ), 0, this, "fileSaveAs" );
    connect( saveAsProjectAction, SIGNAL( activated() ), 
             this, SLOT( fileSaveAs() ) );
    saveAsProjectAction->addTo( menu );
    saveAsProjectAction->setDisabled(true);

    // Close
    closeProjectAction = 
        new QAction( tr( "&Close" ), 0, this, "fileClose" );
    connect( closeProjectAction, SIGNAL( activated() ), 
             this, SLOT( fileClose() ) );
    closeProjectAction->addTo( menu );
    closeProjectAction->setDisabled(true);
    menu->insertSeparator();

    // Open Wsp
    a = new QAction( tr( "Open Workspace..." ), SHIFT + Key_F2, 
                     this, "fileOpenWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpenWsp() ) );
    a->addTo( menu );

    // Save Wsp
    a = new QAction( tr( "Save Workspace" ), SHIFT + Key_F3, 
                     this, "fileSaveWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveWsp() ) );
    a->addTo( menu );

    // Save Wsp As
    a = new QAction( tr( "Save Workspace As..." ), 0, 
                     this, "fileSaveWspAs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveWspAs() ) );
    a->addTo( menu );

    // Close Wsp
    a = new QAction( tr( "Close Workspace" ), SHIFT + Key_F4, 
                     this, "fileCloseWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileCloseWsp() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Save All
    a = new QAction( tr( "Save All" ), CTRL + SHIFT + Key_S, 
                     this, "fileSaveAll" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAll() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Page Setup
    a = new QAction( tr( "Page setup..." ), 0, this, "filePageSetup" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePageSetup() ) );
    a->addTo( menu );
    a->setDisabled(true);

    // Print Preview
    a = new QAction( tr( "Print Preview" ), CTRL + ALT + Key_P, 
                     this, "fileprintpreview" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrintPreview() ) );
    a->addTo( menu );
    a->setDisabled(true);

    // Print
    printAction = new QAction( 
                 QPixmap::fromMimeSource( imagesPath() + "/fileprint.xpm" ), 
                 tr( "&Print..." ), CTRL + Key_P, this, "filePrint" );
    connect( printAction, SIGNAL( activated() ), this, SLOT( filePrint() ) );        
    printAction->addTo( menu );
    printAction->setDisabled(true);
    menu->insertSeparator();    

    // Exit
    a = new QAction( tr( "E&xit" ), CTRL + Key_Q, this, "fileExit" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    a->addTo( menu );
}

void FermaNextMainFrame::setupEditActions ()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( tr("Edit Actions") );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Edit" ), menu );

    // Undo
    undoAction = new QAction( 
                      QPixmap::fromMimeSource( imagesPath() + "/editundo.xpm" ), 
                      tr( "&Undo" ), CTRL + Key_Z, this, "editUndo" );
    connect( undoAction, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    undoAction->setDisabled(true);
    undoAction->addTo( tb );
    undoAction->addTo( menu );

    // Redo
    redoAction = new QAction( 
                      QPixmap::fromMimeSource( imagesPath() + "/editredo.xpm" ), 
                      tr( "&Redo" ), CTRL + Key_Y, this, "editRedo" );
    connect( redoAction, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    redoAction->setDisabled(true);
    redoAction->addTo( tb );
    redoAction->addTo( menu );
    menu->insertSeparator();
    tb->addSeparator();
}

void FermaNextMainFrame::setupViewActions ()
{
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&View" ), menu );

    // Contents
    showUndoRedoAction = new QAction( tr( "&Show History Window" ), 0, this );
    showUndoRedoAction->setToggleAction( true );
    showUndoRedoAction->setOn( true );
    showUndoRedoAction->setStatusTip( tr( "Show or hide history window" ) );
    showUndoRedoAction->addTo( menu );
    connect( showUndoRedoAction, SIGNAL( toggled( bool ) ), 
             undoRedoHistoryWidget, SLOT( setShown( bool ) ) );

    showGeometryWindowAction = 
        new QAction( tr( "&Show Truss Geometry Window" ), 0, this );
    showGeometryWindowAction->setToggleAction( true );
    showGeometryWindowAction->setOn( true );
    showGeometryWindowAction->
        setStatusTip( tr( "Show or hide truss geometry window" ) );
    showGeometryWindowAction->addTo( menu );
    connect( showGeometryWindowAction, SIGNAL( toggled( bool ) ), 
             geometryWindow, SLOT( setShown( bool ) ) );
    connect( geometryWindow, SIGNAL(onGeometryWindowClose()), 
             showGeometryWindowAction, SLOT(toggle()) );
}

void FermaNextMainFrame::setupProjectActions ()
{        
}

void FermaNextMainFrame::setupWindowActions ()
{
}

void FermaNextMainFrame::setupHelpActions ()
{
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Help" ), menu );

    QAction *a;
    // Contents
    a = new QAction( tr( "Contents..." ), Key_F1, this, "helpContents" );
    connect( a, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    a->addTo( menu );
    a->setDisabled(true);
    menu->insertSeparator();

    // About
    a = new QAction( tr( "About FermaNext" ), 0, this, "helpAbout" );
    connect( a, SIGNAL( activated() ), this, SLOT( helpAbout() ) );    
    a->addTo( menu );
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
        pluginsMenu = new QPopupMenu( this );
        menuBar()->insertItem( tr( "&Plugins" ), pluginsMenu );
    }
    else 
        pluginsMenu->clear();

    QStringList::iterator i = names.begin();
    for ( ; i != names.end(); ++i ) {
        pluginsMenu->insertItem( *i );
    }
    pluginsMenu->insertSeparator();

    // Reload plugins
    pluginsMenu->insertItem( tr("Reload plugins"), this, 
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

    geometryWindow->changeFocusWindow( 0 );
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

    geometryWindow->changeFocusWindow( &window );
}

bool FermaNextMainFrame::eventFilter( QObject* obj, QEvent* event )
{
    if ( obj == undoRedoHistoryWidget ) {
        if ( event->type() == QEvent::Close ) {
            showUndoRedoAction->toggle();
            return true;
        }
        else
            return false;
    } else
        // pass the event on to the parent class
        return QMainWindow::eventFilter( obj, event );
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
    QString fileName = QFileDialog::getOpenFileName( QString::null, 
        "Ferma project (*" + FermaNextProject::formatExtension() + ")",
        this, "open ferma project", tr("Open ferma project") );

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

    QString fileName = QFileDialog::getOpenFileName( QString::null, 
        "Ferma workspace (*" + FermaNextWorkspace::formatExtension() + ")", 
        this, "open ferma workspace", tr("Open ferma workspace") );

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
