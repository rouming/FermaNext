
#include "ProjectToolBox.h"
#include "WindowListBox.h"
#include "SubsidiaryConstants.h"
#include "TrussUnitActions.h"
#include "TrussCalcData.h"
#include "CalcDataWidget.h"
#include "CalculationInterface.h"

#include <qbuttongroup.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qheader.h>
#include <qmessagebox.h>
#include <qiconset.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qaction.h>

/*****************************************************************************
 * Project ToolBox
 *****************************************************************************/

ProjectToolBox::ProjectToolBox ( FermaNextWorkspace& ws, QWidget* parent, 
                                 const char* name, WFlags f ) :
    QToolBox(parent, name, f),
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
    connect( &workspace, SIGNAL(onProjectActivated(FermaNextProject&)), 
                         SLOT(projectIsActivated(FermaNextProject&)) );
    connect( &workspace, SIGNAL(onBeforeProjectRemove(FermaNextProject&)), 
                         SLOT(removeProject(FermaNextProject&)) );
    connect( this, SIGNAL(currentChanged(int)), 
                   SLOT(activateSelected(int)) );
}

int ProjectToolBox::addProject ( FermaNextProject& prj )
{
    currentPrj = &prj;
    QWidget* page = createSubsidiaryWidget(prj);
    projects[&prj] = page;

    // Catch name changing
    QObject::connect( &prj, SIGNAL(onNameChange(const QString&)),
                            SLOT(projectRename(const QString&)) );

    QIconSet iconSet( QPixmap::fromMimeSource( imagesPath() + "/project_toolbox.png" ) );
    int result = addItem( page, iconSet, prj.getName() );
    setCurrentItem(page);
    prj.activate();
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
    if ( result > 0 )
        setCurrentIndex( result - 1 );        
    delete page;
    return result;
}

void ProjectToolBox::projectRename ( const QString& newName )
{
    try { 
        QObject* prjSender = const_cast<QObject*>( sender() );
        FermaNextProject& prj = 
            dynamic_cast<FermaNextProject&>( *prjSender );
        if ( projects.contains( &prj ) ) {
            int indx = indexOf( projects[&prj] );
            if ( indx != -1 )
                setItemLabel( indx, newName );
        }
    } catch ( ... ) {}
}

void ProjectToolBox::projectIsActivated ( FermaNextProject& prj )
{
    // Double activation check
    if ( currentProject() == &prj )
        return;
    setCurrentItem( projects[&prj] );
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

    WindowListBox * listBox = new WindowListBox ( prj, groupBoxWindows, "listBox" );
    groupBoxWindowsLayout->addWidget( listBox );
    listBox->installEventFilter( this );

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
    labelTOKVal->setText( tr( "1,23" ) );     
    labelTOK->setText( tr( "TOK" ) );     
    labelTOK->setFont(labelFont);     
    labelVisibleTrusses->setText( tr( "Visible trusses" ) );
    labelVisibleTrusses->setFont(labelFont);     
    labelTrussNumber->setText( tr( "Truss number" ) );
    labelTrussNumber->setFont(labelFont);
    labelVisibleTrussesVal->setText( tr( "5" ) );

    // Some buttons at the bottom
    QToolButton* calculateAllButton = new QToolButton( groupBoxWindows );
    calculateAllButton->setText( tr("FFFFFFF") );
    groupBoxWindowsLayout->addWidget( calculateAllButton );
    calculateAllButton->setTextLabel( tr("Calculate all") );
    calculateAllButton->setAutoRaise( TRUE );
    calculateAllButton->setTextPosition( QToolButton::Right );
    calculateAllButton->setUsesTextLabel( TRUE );
    calculateAllButton->setFont( simpleFont );
    calculateAllButton->setBackgroundMode(PaletteBase);
    connect( calculateAllButton, SIGNAL(clicked()), SLOT(calculateAllIsPressed()) ); 

    // Calculate all action
    QAction* calculationAllAction = new QAction( "Calculate all", Key_F5, this );
    connect( calculationAllAction, SIGNAL( activated() ) , 
             SLOT( calculateAllIsPressed() ) );

    QButtonGroup* buttons = new QButtonGroup( 1, Vertical, groupBox );
    buttons->setFrameStyle( QFrame::NoFrame );
    buttons->setEraseColor(green);
    buttons->setBackgroundMode(PaletteBase);
        
    QToolButton* buttonImport = new QToolButton(buttons);
    buttonImport->setBackgroundMode(PaletteBase);
    buttonImport->setTextLabel( tr("Import") );
    buttonImport->setAutoRaise( TRUE );
    buttonImport->setTextPosition( QToolButton::Right );
    buttonImport->setUsesTextLabel( TRUE );
    buttonImport->setFont( simpleFont );
    connect( buttonImport, SIGNAL(clicked()), SLOT(importIsPressed()) ); 

    QToolButton* buttonNewTruss = new QToolButton(buttons);
    buttonNewTruss->setBackgroundMode(PaletteBase);
    buttonNewTruss->setTextLabel( tr("New") );
    buttonNewTruss->setAutoRaise( TRUE );
    buttonNewTruss->setTextPosition( QToolButton::Right );
    buttonNewTruss->setUsesTextLabel( TRUE );
    buttonNewTruss->setFont( simpleFont );
    connect( buttonNewTruss, SIGNAL(clicked()), SLOT(newTrussIsPressed()) ); 

    buttons->insert( buttonImport, 0 );
    buttons->insert( buttonNewTruss, 0 );

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
    for ( ; i != keys.end(); ++i )
        if ( page == projects[*i] ) {
            currentPrj = *i;
            if ( ! currentPrj->isActivated() )
                currentPrj->activate();
            break;
        }

    QIconSet activeIconSet( QPixmap::fromMimeSource( imagesPath() + "/project_toolbox.png" ) );
    QIconSet disablesIconSet( QPixmap::fromMimeSource( imagesPath() + "/project_d.png" ) );
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

void ProjectToolBox::importIsPressed ()
{
    FermaNextProject* currPrj = currentProject();
    if ( currPrj == 0 )
        return;

    QString fileName = QFileDialog::getOpenFileName( QString::null, 
        "Ferma (*" + TrussUnitWindowManager::newFormatExtension()  +
        " );;Ferma old (*" + TrussUnitWindowManager::oldFormatExtension() +")",
        this );

    if ( fileName.isEmpty() ) 
        return;

    try { 
        TrussUnitWindowManager& mng = currPrj->getTrussUnitWindowManager();
        mng.createTrussUnitWindowFromFile(fileName);

    } catch ( TrussUnitWindowManager::ReadFileException& ) {
        QMessageBox::critical( 0, "TrussUnitWindowManager::ReadFileException",
                               QString("TrussUnitWindowManager::ReadFileException") );
        return;

    } catch ( TrussUnitWindowManager::WrongFormatException& ) {
        QMessageBox::critical( 0, "TrussUnitWindowManager::WrongFormatException",
                               QString("TrussUnitWindowManager::WrongFormatException") );
    }
}

void ProjectToolBox::newTrussIsPressed ()
{
    FermaNextProject* currPrj = currentProject();
    if ( currPrj == 0 )
        return;

    bool ok;
    QString trussName = QInputDialog::getText(
            tr("Truss unit creation"), 
            tr("Enter truss unit name:"), QLineEdit::Normal,
            QString::null, &ok, this );
    if ( ok && !trussName.isEmpty() ) {
        TrussUnitWindowManager& trussMng = currPrj->getTrussUnitWindowManager();
        TrussUnitWindow& truss = trussMng.createTrussUnitWindow(trussName);
        truss.setTrussAreaSize( DoubleSize( 300, 300 ) );
    } else {
        // user entered nothing or pressed Cancel
    }
}

void ProjectToolBox::calculateAllIsPressed ()
{
    FermaNextProject* currPrj = currentProject();
    if ( currPrj == 0 )
        return;

    FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();
    PluginManager& plgManager = wsp.pluginManager();
    PluginList plugins = plgManager.loadedPluginsOfType( CALCULATION_TYPE );
    if ( plugins.size() == 0 ) {
        QMessageBox::warning( 0, tr("Plugin manager warning"), 
                                 tr("Calculation plugin was not found "
                                    "in the plugin dir.") );
        return;
    }

    CalcDataToolBar& calcToolBar = currPrj->getCalcDataToolBar();
    TrussUnitWindowManager& trussMng = currPrj->getTrussUnitWindowManager();
    WindowList trussWindows = trussMng.getTrussUnitWindowList();
    WindowListIter iter = trussWindows.begin();

    // Find first calculation plugin. 
    // TODO: plural calculation plugin support
    Plugin* plugin = plugins[0];

    try {
        CalculationInterface& calcPlugin = 
            dynamic_cast<CalculationInterface&>(*plugin);

        for ( ; iter != trussWindows.end(); ++iter ) {
            TrussUnitWindow* trussWindow = *iter;
            try {
                // Try to find truss calc data widget
                CalcDataWidget* calcForm = 
                    calcToolBar.findCalcDataWidget( *trussWindow );
            
                // Do calculation with new topology and calc data
                TrussCalcData calcData;
                TrussTopology& topology = trussWindow->createTopology();
                calcPlugin.calculate( topology, calcData );
                calcForm->initCalc( calcData );

                // TODO: toplogy manager
                topology.desist();
            }
            catch ( ... ) {
               QMessageBox::critical( 0, tr("Plugin manager error"),
                                      tr("Internal plugin error. \n"
                                      "Please, inform plugin developer.") );
            }
        }
    }
    catch ( std::exception& ) {
        QMessageBox::critical( 0, tr("Plugin manager error"),
                               tr("Plugin '"+  plugin->pluginInfo().name + 
                                  "' has violated type contract.") );
    }
}

bool ProjectToolBox::eventFilter( QObject* targetObj, QEvent* event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;
        if ( keyEvent->key() == Key_N || keyEvent->key() == Key_P ||
             keyEvent->key() == Key_F || keyEvent->key() == Key_L ||
             keyEvent->key() == Key_Escape ) {
            currentPrj->getDesignerWindow().
                getDesignerWidget().aggKeyPressEvent( keyEvent );
            return true;
        }
    }
    return false;
}

/****************************************************************************/
