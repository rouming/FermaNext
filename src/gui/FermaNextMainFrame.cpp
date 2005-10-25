
#include "FermaNextMainFrame.h"
#include "FermaNextWorkspace.h"
#include "ProjectToolBox.h"
#include "SubsidiaryConstants.h"
#include "UndoRedoListBox.h"

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
    connect( &ws, SIGNAL(onProjectRemove(FermaNextProject&)), 
                  SLOT(someProjectRemoved(FermaNextProject&)) );
    connect( &ws, SIGNAL(onProjectCreate(FermaNextProject&)), 
                  SLOT(someProjectCreated(FermaNextProject&)) );
    connect( projectToolBox, SIGNAL(currentChanged(int)), 
                             SLOT(refreshUndoRedoActions()) );
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

        prj.activate();

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
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filesave.xpm" ), 
                     tr( "&Save..." ), CTRL + Key_S, this, "fileSave" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    a->addTo( tb );
    a->addTo( menu );    

    // Delete
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/editdelete.png" ), 
                     tr( "&Delete" ), Key_Delete, this, "editDelete" );
    connect( a, SIGNAL( activated() ), this, SLOT( editDelete() ) );    
    a->addTo( menu );
    a->addTo( tb );
    menu->insertSeparator();

    // Save As
    a = new QAction( tr( "Save &As..." ), 0, this, "fileSaveAs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    a->addTo( menu );

    // Close
    a = new QAction( tr( "&Close" ), CTRL + ALT + Key_F4, this, "fileClose" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileClose() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Open Wsp
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/fileopenwsp.xpm" ), 
                     tr( "Open Workspace..." ), SHIFT + Key_F2, this, "fileOpenWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpenWsp() ) );
    a->addTo( menu );

    // Save Wsp
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filesavewsp.xpm" ), 
                     tr( "Save Workspace" ), SHIFT + Key_F3, this, "fileSaveWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveWsp() ) );
    a->addTo( menu );

    // Save Wsp As
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filesavewspas.xpm" ), 
                     tr( "Save Workspace As..." ), 0, this, "fileSaveWspAs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveWspAs() ) );
    a->addTo( menu );

    // Close Wsp
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/fileclosewsp.xpm" ), 
                     tr( "Close Workspace" ), SHIFT + Key_F4, this, "fileCloseWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileCloseWsp() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Save All
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filesaveall.xpm" ), 
                     tr( "Save All" ), CTRL + Key_F3, this, "fileSaveAll" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAll() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Page Setup
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filepagesetup.xpm" ), 
                     tr( "Page setup..." ), 0, this, "filePageSetup" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePageSetup() ) );
    a->addTo( menu );

    // Print Preview
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/fileprintpreview.xpm" ), 
                     tr( "Print Preview" ), CTRL + ALT + Key_P, this, "fileprintpreview" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrintPreview() ) );
    a->addTo( menu );

    // Print
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/fileprint.xpm" ), 
                     tr( "&Print..." ), CTRL + Key_P, this, "filePrint" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrint() ) );        
    a->addTo( menu );
    menu->insertSeparator();

    // Recent Prjs
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filerecentprjs.xpm" ), 
                     tr( "Recent Projects" ), 0, this, "fileRecentPrjs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileRecentPrjs() ) );
    a->addTo( menu );

    // Recent Wsps
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/filerecentwsps.xpm" ), 
                     tr( "Recent Workspaces" ), 0, this, "fileRecentWsps" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileRecentWsps() ) );
    a->addTo( menu );
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

    QAction *a;
    // Copy
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/editcopy.xpm" ), 
                     tr( "&Copy" ), CTRL + Key_C, this, "editCopy" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Paste
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/editpaste.xpm" ), 
                     tr( "&Paste" ), CTRL + Key_V, this, "editPaste" );
    connect( a, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Cut
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/editcut.xpm" ), 
                     tr( "Cu&t" ), CTRL + Key_X, this, "editCut" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCut() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Select All
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/editselectall.xpm" ), 
                     tr( "Select A&ll" ), CTRL + Key_A, this, "editSelectAll" );
    connect( a, SIGNAL( activated() ), this, SLOT( editSelectAll() ) );    
    a->addTo( menu );
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
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/helpcontents.xpm" ), 
                     tr( "Contents..." ), Key_F1, this, "helpContents" );
    connect( a, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // About
    a = new QAction( QPixmap::fromMimeSource( imagesPath() + "/helpabout.xpm" ), 
                     tr( "About FermaNext" ), 0, this, "helpAbout" );
    connect( a, SIGNAL( activated() ), this, SLOT( helpAbout() ) );    
    a->addTo( menu );    
}

void FermaNextMainFrame::setupPluginActions ()
{
    reloadPlugins();
}

void FermaNextMainFrame::reloadPlugins ()
{
    FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();
    bool pluginsLoaded = wsp.loadPlugins();
    if ( ! pluginsLoaded )
        return;
    PluginManager& plgManager = wsp.pluginManager();
    QStringList names = plgManager.loadedPluginsNames();

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
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileSave ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileSaveAs ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileClose ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileOpenWsp ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileSaveWsp ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileSaveWspAs ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileCloseWsp ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileSaveAll ()
{
    qWarning("Not implmented yet!");
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
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileRecentPrjs ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::fileRecentWsps ()
{
    qWarning("Not implmented yet!");
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

void FermaNextMainFrame::editDelete ()
{
    FermaNextProject* currentPrj = projectToolBox->currentProject();
    if ( currentPrj == 0 )
        return;
    if ( QMessageBox::question( this,
                                tr("Project deleting - \"%1\"").
                                  arg(currentPrj->getName()),
                                tr("Delete current project?"),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) )
        return;
    
    FermaNextWorkspace::workspace().removeProject( *currentPrj );
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
