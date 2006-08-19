
#include <QFileDialog>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <QGridLayout>

#include "ProjectToolBox.h"
#include "WindowListBox.h"
#include "Global.h"
#include "TrussUnitActions.h"
#include "TrussCalcData.h"
//FIXME QT3TO4
//#include "CalcDataWidget.h"
//#include "CalculationInterface.h"

/*****************************************************************************
 * Project ToolBox
 *****************************************************************************/

ProjectToolBox::ProjectToolBox ( FermaNextWorkspace& ws, QWidget* parent, 
                                 Qt::WFlags f ) :
    QToolBox(parent, f),
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

    // Catch truss count and visibility changing
    TrussUnitWindowManager& mng =  prj.getTrussUnitWindowManager();
    QObject::connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)),
                            SLOT(afterTrussCountChange(TrussUnitWindow&)) );
    QObject::connect( &mng, SIGNAL(onTrussUnitWindowRevive(TrussUnitWindow&)),
                            SLOT(afterTrussCountChange(TrussUnitWindow&)) );
    QObject::connect( &mng, SIGNAL(onTrussUnitWindowDesist(TrussUnitWindow&)),
                            SLOT(afterTrussCountChange(TrussUnitWindow&)) );
    QObject::connect( &mng, SIGNAL(onTrussWindowVisibilityChange(bool)),
                            SLOT(afterTrussVisibilityChange()) );
    

    QIcon iconSet( Global::imagesPath() + "/project_toolbox.png"  );
    int result = addItem( page, iconSet, prj.getName() );    
    setCurrentWidget(page);
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
    QWidget* page = iter.value();
    int pageIndex = indexOf(page);
    projects.erase(iter);
    removeItem( pageIndex );
    if ( pageIndex > 0 )
        setCurrentIndex( pageIndex - 1 );        
    delete page;
    return pageIndex;
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
                setItemText( indx, newName );
        }
    } catch ( ... ) {}
}

void ProjectToolBox::projectIsActivated ( FermaNextProject& prj )
{
    // Double activation check
    if ( currentProject() == &prj )
        return;
    setCurrentWidget( projects[&prj] );
}

QWidget* ProjectToolBox::createSubsidiaryWidget ( FermaNextProject& prj )
{
    // Main widget
    QWidget* page = new QWidget;
    QVBoxLayout* pageLayout = new QVBoxLayout;
    pageLayout->setMargin(3);
    pageLayout->setSpacing(0);
    page->setLayout( pageLayout );

    QGroupBox* propertyBox = new QGroupBox( tr( "Project Info" ), page );
    pageLayout->addWidget( propertyBox );
    
    QGridLayout* propertyLayout = new QGridLayout( propertyBox );
    tokLabel = new QLabel( "", propertyBox );
    trussNumberLabel = new QLabel( "0", propertyBox );
    trussHiddenLabel = new QLabel( "0", propertyBox );
    propertyLayout->addWidget( new QLabel( tr( "TOK: ") ), 0, 0 );
    propertyLayout->addWidget( new QLabel( tr( "Total trusses: ") ), 1, 0 );
    propertyLayout->addWidget( new QLabel( tr( "Hidden trusses: ") ), 2, 0 );
    propertyLayout->addWidget( tokLabel, 0, 1 );
    propertyLayout->addWidget( trussNumberLabel, 1, 1 );
    propertyLayout->addWidget( trussHiddenLabel, 2, 1 );

    // Truss units group box
    QGroupBox* groupBoxTrussUnits = new QGroupBox( tr( "Trusses" ), page );

    QVBoxLayout* trussUnitsLayout = new QVBoxLayout( groupBoxTrussUnits );
    trussUnitsLayout->setMargin(3);
    trussUnitsLayout->setSpacing(0);
    pageLayout->addWidget( groupBoxTrussUnits );

    WindowListBox * listBox = new WindowListBox( prj, groupBoxTrussUnits );
    trussUnitsLayout->addWidget( listBox );
    QObject::connect( listBox, 
                      SIGNAL(onShowTrussResults(const TrussUnitWindow&)),
                      SIGNAL(onShowTrussResults(const TrussUnitWindow&)) );
    QObject::connect( this, 
                      SIGNAL(calculateTrussUnit(const TrussUnitWindow&)),
                      listBox,
                      SLOT(solveTrussUnit(const TrussUnitWindow&)) );
    // Buttons
    QPushButton* calculateAllButton = new QPushButton;    
    trussUnitsLayout->addSpacing(10);
    trussUnitsLayout->addWidget( calculateAllButton );
    calculateAllButton->setFlat( true );
    calculateAllButton->setText( tr("Calculate all") );    
    calculateAllButton->setIcon( 
                       QIcon(Global::imagesPath() + "/calculate_all.png") );
        
    connect( calculateAllButton, SIGNAL(clicked()), 
             SLOT(calculateAllIsPressed()) ); 

    // Calculate all action
    QAction* calculationAllAction = new QAction( tr("Calculate all"), this );
    calculationAllAction->setShortcut( tr("F5") );
    connect( calculationAllAction, SIGNAL(triggered()) , 
             SLOT(calculateAllIsPressed()) );

    QFrame* buttonsGroupBox = new QFrame;
    buttonsGroupBox->setFrameStyle( QFrame::Plain | QFrame::NoFrame );
    buttonsGroupBox->setMidLineWidth( 3 );
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(0);
    buttonsLayout->setMargin(0);
    buttonsGroupBox->setLayout( buttonsLayout );
    trussUnitsLayout->addWidget( buttonsGroupBox );

    QPushButton* buttonImport = new QPushButton;    
    buttonsLayout->addWidget( buttonImport );
    buttonImport->setFlat( true );
    buttonImport->setText( tr("Import") );
    buttonImport->setMinimumWidth( 60 );
    connect( buttonImport, SIGNAL(clicked()), SLOT(importIsPressed()) ); 

    QPushButton* buttonNewTruss = new QPushButton;
    buttonsLayout->addWidget( buttonNewTruss );
    buttonNewTruss->setFlat( true );
    buttonNewTruss->setText( tr("New") );
    buttonNewTruss->setMinimumWidth( 60 );
    connect( buttonNewTruss, SIGNAL(clicked()), SLOT(newTrussIsPressed()) ); 

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
    QWidget* page = currentWidget();
    QList<FermaNextProject*> keys = projects.keys();
    QList<FermaNextProject*>::iterator i = keys.begin();
    for ( ; i != keys.end(); ++i )
        if ( page == projects[*i] ) {
            currentPrj = *i;
            if ( ! currentPrj->isActivated() )
                currentPrj->activate();
            break;
        }

    QIcon activeIconSet( Global::imagesPath() + "/project_toolbox.png" );
    QIcon disablesIconSet( Global::imagesPath() + "/project_d.png" );
    for ( int ind = 0; ind < count(); ++ind )
        setItemIcon( ind, disablesIconSet );
    setItemIcon( index, activeIconSet );        
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

    QString fileName = QFileDialog::getOpenFileName( 
        this,
        tr( "Import trusses" ),
        QString(),
        "Ferma (*" + TrussUnitWindowManager::newFormatExtension()  +
        " );;Ferma old (*" + TrussUnitWindowManager::oldFormatExtension() +")"
         );

    if ( fileName.isEmpty() ) 
        return;

    try { 
        TrussUnitWindowManager& mng = currPrj->getTrussUnitWindowManager();
        mng.createTrussUnitWindowFromFile(fileName);

    } catch ( TrussUnitWindowManager::ReadFileException& ) {
        QMessageBox::critical( 
                   0, "TrussUnitWindowManager::ReadFileException",
                   QString("TrussUnitWindowManager::ReadFileException") );
        return;

    } catch ( TrussUnitWindowManager::WrongFormatException& ) {
        QMessageBox::critical( 
                   0, "TrussUnitWindowManager::WrongFormatException",
                   QString("TrussUnitWindowManager::WrongFormatException") );
    }
}

void ProjectToolBox::newTrussIsPressed ()
{
    FermaNextProject* currPrj = currentProject();
    if ( currPrj == 0 )
        return;

    QString trussName = QInputDialog::getText(
            this,
            tr("Truss unit creation"), 
            tr("Enter truss unit name:") );
    if ( !trussName.isEmpty() ) {
        TrussUnitWindowManager& trussMng = 
            currPrj->getTrussUnitWindowManager();
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

    // FIXME QT3TO4:
    /*
    PluginManager& plgManager = workspace.pluginManager();
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
    */
}

void ProjectToolBox::afterTrussCountChange ( TrussUnitWindow& w)
{
    TrussUnitWindowManager& mng = currentProject()->getTrussUnitWindowManager();
    if ( sender() != &mng )
        return;

    int numb = mng.getTrussUnitWindowList().size();
    trussNumberLabel->setText( QString::number(numb) );
    if ( ! w.isVisible() )
        afterTrussVisibilityChange();

}

void ProjectToolBox::afterTrussVisibilityChange ()
{
    TrussUnitWindowManager& mng = currentProject()->getTrussUnitWindowManager();
    if ( sender() != &mng )
        return;

    int numb = 0;
    WindowList trussWindows = mng.getTrussUnitWindowList();
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( ! (*iter)->isVisible() )
            ++numb;

    trussHiddenLabel->setText( QString::number(numb) );
}

bool ProjectToolBox::eventFilter( QObject*, QEvent* event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;
        if ( keyEvent->key() == Qt::Key_N || keyEvent->key() == Qt::Key_P ||
             keyEvent->key() == Qt::Key_F || keyEvent->key() == Qt::Key_L ||
             keyEvent->key() == Qt::Key_Escape ) {
            currentPrj->getDesignerWidget().aggKeyPressEvent( keyEvent );
            return true;
        }
    }
    return false;
}

/****************************************************************************/
