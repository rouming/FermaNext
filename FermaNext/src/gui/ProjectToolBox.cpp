
#include "ProjectToolBox.h"
#include "SubsidiaryConstants.h"

#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qheader.h>
#include <qmessagebox.h>
#include <qiconset.h>

/*****************************************************************************
 * Project Remover
 *****************************************************************************/

ProjectRemover::ProjectRemover ( ProjectToolBox& tb, FermaNextProject& prj ) :
    QObject(&tb),
    projectToRemove(prj)    
{
    connect( this, SIGNAL(onProjectRemove(ProjectRemover&)),
             &tb, SLOT(removeProjectFromWorkspace(ProjectRemover&)) );
}

void ProjectRemover::removeProject () 
{
    emit onProjectRemove(*this);
}

FermaNextProject& ProjectRemover::getProjectToRemove ()
{
    return projectToRemove;
}

/*****************************************************************************
 * Project ToolBox
 *****************************************************************************/

ProjectToolBox::ProjectToolBox ( FermaNextWorkspace& ws, QWidget* parent, 
                                 const char* name, WFlags f ) :
    QToolBox(parent, name, f),
    lastRemover(0),
    currentPrj(0),
    workspace(ws)
{    
    // Fill in toolbox by existent projects
    if ( workspace.countProjects() ) {
        for ( int i=0; i < workspace.countProjects(); ++i ) {
            FermaNextProject& prj = workspace.getProject(i);
            addProject(prj);            
        }
    }
    connect( &workspace, SIGNAL(onReset()), SLOT(clear()) );
    connect( &workspace, SIGNAL(onProjectCreate(FermaNextProject&)), 
                         SLOT(addProject(FermaNextProject&)) );
    connect( &workspace, SIGNAL(onProjectRemove(FermaNextProject&)), 
                         SLOT(removeProject(FermaNextProject&)) );
    connect( this, SIGNAL(currentChanged(int)), 
                         SLOT(activateSelected(int)) );
}

int ProjectToolBox::addProject ( FermaNextProject& prj )
{
    if ( currentPrj != 0 )
        currentPrj->activate(false);
    currentPrj = &prj;
    QWidget* page = createSubsidiaryWidget(prj);
    projects[&prj] = page;

    QIconSet iconSet( QPixmap::fromMimeSource( imagesPath + "/project_toolbox.png" ) );
    int result = addItem( page, iconSet, prj.getName() );
    setCurrentItem(page);
    prj.activate(true);
    return result;
}

int ProjectToolBox::removeProject ( FermaNextProject& prj )
{
    // Zeroing currentPrj if we should remove it
    if ( currentPrj != 0 && currentPrj->getName() == prj.getName () )
      currentPrj = 0;
    ProjectMapIter iter = projects.find(&prj);
    if ( iter == projects.end() ) 
        return -1;
    QWidget* page = iter.data();
    projects.erase(iter);
    int result = removeItem(page);
    delete page;
    return result;
}

void ProjectToolBox::removeProjectFromWorkspace ( ProjectRemover& remover )
{
    if ( QMessageBox::question( this,
                                tr("Project removing - \"%1\"").
                                  arg(remover.getProjectToRemove().getName()),
                                tr("Are you sure?"),
                                tr("&Yes"), tr("&No"),
                                QString::null, 0, 1 ) ) 
        return;

    if ( lastRemover != 0 ) 
        delete lastRemover;
    lastRemover = &remover;
    workspace.removeProject( remover.getProjectToRemove() );
}

QWidget* ProjectToolBox::createSubsidiaryWidget ( FermaNextProject& prj )
{
    // Main widget
    QWidget* page = new QWidget( this, "page" );
    page->setBackgroundMode( QWidget::PaletteBackground );
    QVBoxLayout* pageLayout = new QVBoxLayout( page, 0, 0, "pageLayout"); 

    // Main group box
    QGroupBox* groupBox = new QGroupBox( page, "groupBox" );
    groupBox->setBackgroundMode(PaletteBase);
    groupBox->setLineWidth( 0 );
    groupBox->setColumnLayout(0, Qt::Vertical );
    groupBox->layout()->setSpacing( 0 );
    groupBox->layout()->setMargin( 0 );
    QVBoxLayout* groupBoxLayout = new QVBoxLayout( groupBox->layout() );
    groupBoxLayout->setAlignment( Qt::AlignTop );

    // Info group box
    QGroupBox* groupBoxInfo = new QGroupBox( groupBox, "groupBoxInfo" );
    groupBoxInfo->setBackgroundMode(PaletteBase);
    groupBoxInfo->setColumnLayout(0, Qt::Vertical );
    groupBoxInfo->layout()->setSpacing( 6 );
    groupBoxInfo->layout()->setMargin( 11 );
    QGridLayout* groupBoxInfoLayout = new QGridLayout( groupBoxInfo->layout() );
    groupBoxInfoLayout->setAlignment( Qt::AlignTop );

    // Windows group box
    QGroupBox* groupBoxWindows = new QGroupBox( groupBox, "groupBoxWindows" );
    groupBoxWindows->setBackgroundMode(PaletteBase);
    groupBoxWindows->setColumnLayout(0, Qt::Vertical );
    groupBoxWindows->layout()->setSpacing( 6 );
    groupBoxWindows->layout()->setMargin( 11 );
    QVBoxLayout* groupBoxWindowsLayout = new QVBoxLayout( groupBoxWindows->layout() );
    groupBoxWindowsLayout->setAlignment( Qt::AlignTop );

    // Labels for info group box
    QLabel* labelTrussNumberVal = new QLabel( groupBoxInfo, "labelTrussNumberVal" );
    labelTrussNumberVal->setBackgroundMode(PaletteBase);
    groupBoxInfoLayout->addWidget( labelTrussNumberVal, 1, 1 );

    QLabel* labelTOKVal = new QLabel( groupBoxInfo, "labelTOKVal" );
    labelTOKVal->setBackgroundMode(PaletteBase);
    groupBoxInfoLayout->addWidget( labelTOKVal, 0, 1 );
 
    QLabel* labelTOK = new QLabel( groupBoxInfo, "labelTOK" );    
    labelTOK->setBackgroundMode(PaletteBase);
    groupBoxInfoLayout->addWidget( labelTOK, 0, 0 );
 
    QLabel* labelVisibleTrusses = new QLabel( groupBoxInfo, "labelVisibleTrusses" );    
    labelVisibleTrusses->setBackgroundMode(PaletteBase); 
    groupBoxInfoLayout->addWidget( labelVisibleTrusses, 2, 0 );
 
    QLabel* labelTrussNumber = new QLabel( groupBoxInfo, "labelTrussNumber" );    
    labelTrussNumber->setBackgroundMode(PaletteBase); 
    groupBoxInfoLayout->addWidget( labelTrussNumber, 1, 0 );
 
    QLabel* labelVisibleTrussesVal = new QLabel( groupBoxInfo, "labelVisibleTrussesVal" );
    labelVisibleTrussesVal->setBackgroundMode(PaletteBase); 
    groupBoxInfoLayout->addWidget( labelVisibleTrussesVal, 2, 1 );

    // Windows list view
    QListView* listViewWindows = new QListView( groupBoxWindows, "listViewWindows" );
    groupBoxWindowsLayout->addWidget( listViewWindows );
    listViewWindows->header()->hide();
    listViewWindows->setLineWidth( 0 );
    listViewWindows->addColumn( tr( "Names" ) );    
    listViewWindows->setRootIsDecorated( TRUE );

    // Temp windows in list view
    QListViewItem* headerItem = new QListViewItem( listViewWindows, 0 );
    headerItem->setText(0,"Truss Unit Windows");
    (void) new QCheckListItem( headerItem, 0, "my_first_truss", QCheckListItem::CheckBox );
    (void) new QCheckListItem( headerItem, 0, "my_second_truss", QCheckListItem::CheckBox );
    (void) new QCheckListItem( headerItem, 0, "my_third_truss", QCheckListItem::CheckBox );
    
    // Fonts
    QFont labelFont( "arial", 9  );
    labelFont.setUnderline(true);
    QFont simpleFont( "arial", 9  );    

    // Legends     
    groupBoxInfo->setTitle( tr( "Project info" ) );
    groupBoxInfo->setFont(simpleFont);
    groupBoxWindows->setTitle( tr( "Trusses " ) );
    groupBoxWindows->setFont(simpleFont);
    labelTrussNumberVal->setText( tr( "6" ) );
    labelTOKVal->setText( tr( "1,2339682" ) );     
    labelTOK->setText( tr( "TOK" ) );     
    labelTOK->setFont(labelFont);     
    labelVisibleTrusses->setText( tr( "Visible trusses" ) );
    labelVisibleTrusses->setFont(labelFont);     
    labelTrussNumber->setText( tr( "Truss number" ) );
    labelTrussNumber->setFont(labelFont);
    labelVisibleTrussesVal->setText( tr( "5" ) );

    // Some buttons at the bottom
    QButtonGroup* buttons = new QButtonGroup( 1, Vertical, groupBox );
    buttons->setFrameStyle( QFrame::NoFrame );
    buttons->setEraseColor(green);
    buttons->setBackgroundMode(PaletteBase);
	
    QToolButton* buttonRemove = new QToolButton(buttons);
	buttonRemove->setBackgroundMode(PaletteBase);
	buttonRemove->setTextLabel( "Remove" );
	buttonRemove->setAutoRaise( TRUE );
	buttonRemove->setTextPosition( QToolButton::Right );
	buttonRemove->setUsesTextLabel( TRUE );
    buttonRemove->setFont( simpleFont );
    ProjectRemover* remover = new ProjectRemover( *this, prj );
    connect( buttonRemove, SIGNAL(clicked()), remover, SLOT(removeProject()) );    

    QToolButton* buttonSave = new QToolButton(buttons);
	buttonSave->setBackgroundMode(PaletteBase);
	buttonSave->setTextLabel( "Save" );
	buttonSave->setAutoRaise( TRUE );
	buttonSave->setTextPosition( QToolButton::Right );
	buttonSave->setUsesTextLabel( TRUE );
    buttonSave->setFont( simpleFont );

	buttons->insert( buttonRemove, 0 );
    buttons->insert( buttonSave, 0 );

    // Assembling
    pageLayout->addWidget( groupBox );
    groupBoxLayout->addWidget( groupBoxInfo );
    groupBoxLayout->addWidget( groupBoxWindows ); 
    groupBoxLayout->addWidget( buttons );

    return page;
}

FermaNextProject* ProjectToolBox::currentProject () const
{
    return currentPrj;
}

void ProjectToolBox::activateSelected ( int index )
{   
    if ( index == -1 ) 
        return;
    QWidget* page = currentItem();
    QValueList<FermaNextProject*> keys = projects.keys();
    QValueList<FermaNextProject*>::iterator i = keys.begin();
    for ( ; i != keys.end(); ++i ) {
        if ( page == projects[*i] )             
            (*i)->activate(true);        
        else
            (*i)->activate(false);
    }

    QIconSet activeIconSet( QPixmap::fromMimeSource( imagesPath + "/project_toolbox.png" ) );
    QIconSet disablesIconSet( QPixmap::fromMimeSource( imagesPath + "/project_d.png" ) );
    for ( int ind = 0; ind < count(); ++ind )
        setItemIconSet ( ind, disablesIconSet );
    setItemIconSet ( index, activeIconSet );        
}

void ProjectToolBox::clear ()
{
    for ( int i=0; i < workspace.countProjects(); ++i )
        removeProject( workspace.getProject(i) );        
    currentPrj = 0;
}

/****************************************************************************/
