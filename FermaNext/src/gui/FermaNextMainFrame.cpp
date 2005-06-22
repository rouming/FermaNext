
#include "FermaNextMainFrame.h"
#include "FermaNextWorkspace.h"
#include "ProjectToolBox.h"
#include "SubsidiaryConstants.h"

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
#include <qworkspace.h>

const QString mainFrameCaption( QObject::tr( "Educational CAD System 'Ferma'" ) );

/*****************************************************************************
 * FermaNext Main Frame
 *****************************************************************************/

FermaNextMainFrame::FermaNextMainFrame ( QWidget* p, const char* n, 
                                         WFlags f ) :
    QMainWindow(p, n, f)    
{      
    init();

    // Initial toolbar setup
    setupFileActions();
    setupEditActions();
    setupProjectActions();
    setupWindowActions();
    setupHelpActions();
}

void FermaNextMainFrame::init ()
{
    widgetStack = new QWidgetStack(this);
    setCentralWidget(widgetStack);

    // Do not move this line. FermaNextWorkspace should be initialized first.
    FermaNextWorkspace::workspace().setWidgetStack(*widgetStack);

    setCaption(mainFrameCaption);
    setMinimumSize( 640, 480 );
    statusBar()->addWidget(new QLabel( "Ready", statusBar() ));

    dw = new QDockWindow( QDockWindow::InDock, this );
    dw->setResizeEnabled( TRUE );
    dw->setVerticalStretchable( TRUE );
    dw->setCaption( tr("Truss Unit Projects") );
    addDockWindow( dw, DockLeft );
    setDockEnabled( dw, DockTop, FALSE );
    setDockEnabled( dw, DockBottom, FALSE );
    dw->setCloseMode( QDockWindow::Always );

    FermaNextWorkspace& ws = FermaNextWorkspace::workspace();
    QToolBox* projectToolBox = new ProjectToolBox( ws, dw );
    connect( &ws, SIGNAL(onProjectRemove(FermaNextProject&)), SLOT(someProjectRemoved()) );
    connect( &ws, SIGNAL(onProjectCreate(FermaNextProject&)), SLOT(someProjectCreated()) );
    dw->setWidget( projectToolBox );
    dw->hide();
}

void FermaNextMainFrame::someProjectRemoved ()
{
    if ( 1 == FermaNextWorkspace::workspace().countProjects() )
        dw->hide();
}

void FermaNextMainFrame::someProjectCreated ()
{
    if ( !dw->isVisible() && 
         0 < FermaNextWorkspace::workspace().countProjects() )
        dw->show();
}

void FermaNextMainFrame::createProject ()
{
    bool ok;
    QString text = QInputDialog::getText(
            tr("Project creation"), 
            tr("Enter project name:"), QLineEdit::Normal,
            QString::null, &ok, this );
    if ( ok && !text.isEmpty() ) {
        FermaNextWorkspace::workspace().createProject( text ).activate( true );
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
    tb->setLabel( "File Actions" );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&File" ), menu );

    QAction *a;
    // New
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filenew.xpm" ), 
                     tr( "&New..." ), CTRL + Key_N, this, "fileNew" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Open
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/fileopen.xpm" ), 
                     tr( "&Open..." ), CTRL + Key_O, this, "fileOpen" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpen() ) );
    a->addTo( tb );
    a->addTo( menu );    

    // Save
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filesave.xpm" ), 
                     tr( "&Save..." ), CTRL + Key_S, this, "fileSave" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    a->addTo( tb );
    a->addTo( menu );    

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
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/fileopenwsp.xpm" ), 
                     tr( "Open Workspace..." ), SHIFT + Key_F2, this, "fileOpenWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpenWsp() ) );
    a->addTo( menu );

    // Save Wsp
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filesavewsp.xpm" ), 
                     tr( "Save Workspace" ), SHIFT + Key_F3, this, "fileSaveWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveWsp() ) );
    a->addTo( menu );

    // Save Wsp As
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filesavewspas.xpm" ), 
                     tr( "Save Workspace As..." ), 0, this, "fileSaveWspAs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveWspAs() ) );
    a->addTo( menu );

    // Close Wsp
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/fileclosewsp.xpm" ), 
                     tr( "Close Workspace" ), SHIFT + Key_F4, this, "fileCloseWsp" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileCloseWsp() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Save All
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filesaveall.xpm" ), 
                     tr( "Save All" ), CTRL + Key_F3, this, "fileSaveAll" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAll() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Page Setup
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filepagesetup.xpm" ), 
                     tr( "Page setup..." ), 0, this, "filePageSetup" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePageSetup() ) );
    a->addTo( menu );

    // Print Preview
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/fileprintpreview.xpm" ), 
                     tr( "Print Preview" ), CTRL + ALT + Key_P, this, "fileprintpreview" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrintPreview() ) );
    a->addTo( menu );

    // Print
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/fileprint.xpm" ), 
                     tr( "&Print..." ), CTRL + Key_P, this, "filePrint" );
    connect( a, SIGNAL( activated() ), this, SLOT( filePrint() ) );        
    a->addTo( menu );
    menu->insertSeparator();

    // Recent Prjs
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filerecentprjs.xpm" ), 
                     tr( "Recent Projects" ), 0, this, "fileRecentPrjs" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileRecentPrjs() ) );
    a->addTo( menu );

    // Recent Wsps
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/filerecentwsps.xpm" ), 
                     tr( "Recent Workspaces" ), 0, this, "fileRecentWsps" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileRecentWsps() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // Exit
    a = new QAction( tr( "E&xit" ), 0, this, "fileExit" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    a->addTo( menu );
}


void FermaNextMainFrame::setupEditActions ()
{
    QToolBar *tb = new QToolBar( this );
    tb->setLabel( "Edit Actions" );
    QPopupMenu *menu = new QPopupMenu( this );
    menuBar()->insertItem( tr( "&Edit" ), menu );

    QAction *a;
    // Undo
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editundo.xpm" ), 
                     tr( "&Undo" ), CTRL + Key_Z, this, "editUndo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Redo
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editredo.xpm" ), 
                     tr( "&Redo" ), CTRL + Key_Y, this, "editRedo" );
    connect( a, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    a->addTo( tb );
    a->addTo( menu );
    menu->insertSeparator();
    tb->addSeparator();

    // Copy
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editcopy.xpm" ), 
                     tr( "&Copy" ), CTRL + Key_C, this, "editCopy" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Paste
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editpaste.xpm" ), 
                     tr( "&Paste" ), CTRL + Key_V, this, "editPaste" );
    connect( a, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Cut
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editcut.xpm" ), 
                     tr( "Cu&t" ), CTRL + Key_X, this, "editCut" );
    connect( a, SIGNAL( activated() ), this, SLOT( editCut() ) );
    a->addTo( tb );
    a->addTo( menu );

    // Delete
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editdelete.png" ), 
                     tr( "&Delete" ), Key_Delete, this, "editDelete" );
    connect( a, SIGNAL( activated() ), this, SLOT( editDelete() ) );    
    a->addTo( menu );
    a->addTo( tb );
    menu->insertSeparator();

    // Select All
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/editselectall.xpm" ), 
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
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/helpcontents.xpm" ), 
                     tr( "Contents..." ), Key_F1, this, "helpContents" );
    connect( a, SIGNAL( activated() ), this, SLOT( helpContents() ) );
    a->addTo( menu );
    menu->insertSeparator();

    // About
    a = new QAction( QPixmap::fromMimeSource( imagesPath + "/helpabout.xpm" ), 
                     tr( "About FermaNext" ), 0, this, "helpAbout" );
    connect( a, SIGNAL( activated() ), this, SLOT( helpAbout() ) );    
    a->addTo( menu );    
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
    qWarning("Not implmented yet!");
}


void FermaNextMainFrame::editUndo ()
{
    qWarning("Not implmented yet!");
}

void FermaNextMainFrame::editRedo ()
{
    qWarning("Not implmented yet!");
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




/*****************************************************************************/
