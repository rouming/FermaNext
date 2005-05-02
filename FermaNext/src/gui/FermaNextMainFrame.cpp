
#include "FermaNextMainFrame.h"
#include "FermaNextWorkspace.h"
#include "ProjectToolBox.h"

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
    vbox = new QVBox(this); 
    workspaceWidget = new QWorkspace(vbox);

    // Do not move this line. FermaNextWorkspace should be initialized first.
    FermaNextWorkspace::workspace().setWorkspaceWidget(*workspaceWidget);

    vbox->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    vbox->setMargin( 0 );
    vbox->setLineWidth( 0);
    setCentralWidget(vbox);

    setCaption(mainFrameCaption);
    setMinimumSize( 640, 480 );
    statusBar()->addWidget(new QLabel( "Ready!!", this ));

    QMenuBar *mainMenu = menuBar();
    QPopupMenu *fileMenu = new QPopupMenu( this, "file" );
    fileMenu->insertItem( tr( "&Exit" ), this, SLOT( close() ),
			  QAccel::stringToKey( tr( "Ctrl+Q" ) ) );

    QPopupMenu *projectMenu = new QPopupMenu( this, "project" );
    projectMenu->insertItem( tr( "&Create" ), this, SLOT( createProject() ),
			  QAccel::stringToKey( tr( "Ctrl+C" ) ) );


    QPopupMenu *styleMenu = new QPopupMenu( this, "style" );
    styleMenu->setCheckable( TRUE );
    QActionGroup *ag = new QActionGroup( this, 0 );
    ag->setExclusive( TRUE );
    QSignalMapper *styleMapper = new QSignalMapper( this );
    connect( styleMapper, SIGNAL( mapped( const QString& ) ),
	     this, SLOT( setStyle( const QString& ) ) );

    QStringList list = QStyleFactory::keys();
    list.sort();
    QDict<int> stylesDict( 17, FALSE );
    for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
	QString style = *it;
	QString styleAccel = style;
	if ( stylesDict[styleAccel.left(1)] ) {
	    for ( uint i = 0; i < styleAccel.length(); i++ ) {
		if ( !stylesDict[styleAccel.mid( i, 1 )] ) {
		    stylesDict.insert(styleAccel.mid( i, 1 ), (const int *)1);
		    styleAccel = styleAccel.insert( i, '&' );
		    break;
		}
	    }
	} else {
	    stylesDict.insert(styleAccel.left(1), (const int *)1);
	    styleAccel = "&"+styleAccel;
	}
	QAction *a = new QAction( style, QIconSet(),
				  styleAccel, 0, ag, 0, ag->isExclusive() );
	connect( a, SIGNAL( activated() ), styleMapper, SLOT(map()) );
	styleMapper->setMapping( a, a->text() );
    }
    ag->addTo( styleMenu );

    mainMenu->insertItem( tr( "&File" ), fileMenu );
    //mainMenu->insertItem( tr( "St&yle" ), styleMenu );    
    mainMenu->insertItem( tr( "&Project" ), projectMenu );

    dw = new QDockWindow( QDockWindow::InDock, this );
    dw->setResizeEnabled( TRUE );
    dw->setVerticalStretchable( TRUE );
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
            "Project creation", "Enter project name:", QLineEdit::Normal,
            QString::null, &ok, this );
    if ( ok && !text.isEmpty() ) {
        FermaNextWorkspace::workspace().createProject( text ).getWindow().showMaximized();
    } else {
        // user entered nothing or pressed Cancel
    }
}

/*****************************************************************************/
