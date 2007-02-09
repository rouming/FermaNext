
#include <QFileDialog>
#include <QIcon>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QGroupBox>
#include <QFrame>
#include <QGridLayout>
#include <QDoubleSpinBox>

#include "ProjectToolBox.h"
#include "WindowListBox.h"
#include "Global.h"
#include "TrussUnitActions.h"

/*****************************************************************************
 * Project ToolBox Page
 *****************************************************************************/

CreateTrussDialog::CreateTrussDialog ( TrussUnitWindowManager& mng,
                                       QWidget* parent /* = 0 */ ) :
    QDialog( parent ),
    wndMng( mng )
{
    QVBoxLayout* parenLayout = new QVBoxLayout( this );

    QVBoxLayout* nameLayout = new QVBoxLayout;
    QLabel* nameLabel = new QLabel( tr( "Enter truss name:" ) );
    nameEdit = new QLineEdit;
    connect( nameEdit, SIGNAL(textEdited(const QString&)),
                          SLOT(checkDialogState()) );
    
    nameLayout->addWidget( nameLabel );
    nameLayout->addWidget( nameEdit );

    QHBoxLayout* sizeLayout = new QHBoxLayout;
    QGroupBox* sizeGroupBox = new QGroupBox( tr( "Area size" ), this );
    QLabel* xSizeLabel = new QLabel( tr( "Size by X:" ), sizeGroupBox );
    QLabel* ySizeLabel = new QLabel( tr( "Size by Y:" ), sizeGroupBox );
    xSizeEdit = new QDoubleSpinBox;
    xSizeEdit->setMaximum( Global::areaMaxDimension );
    xSizeEdit->setValue( 300.0 );
    connect( xSizeEdit, SIGNAL(valueChanged(double)),
                          SLOT(checkDialogState()) );

    ySizeEdit = new QDoubleSpinBox;
    ySizeEdit->setMaximum( Global::areaMaxDimension );
    ySizeEdit->setValue( 300.0 );
    connect( ySizeEdit, SIGNAL(valueChanged(double)),
                          SLOT(checkDialogState()) );

    sizeLayout->addWidget( sizeGroupBox );

    QGridLayout *grid = new QGridLayout( sizeGroupBox );
    grid->addWidget( xSizeLabel, 0, 0 );
    grid->addWidget( ySizeLabel, 1, 0 );
    grid->addWidget( xSizeEdit, 0, 1 );
    grid->addWidget( ySizeEdit, 1, 1 );
    grid->setSpacing( 5 );
    grid->setMargin( 15 );

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                          SLOT(onOk()) );
    okButton->setEnabled( false );
    
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                             SLOT(onCancel()) );
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    parenLayout->addLayout( nameLayout );
    parenLayout->addWidget( sizeGroupBox );
    parenLayout->addLayout( buttonLayout );
    parenLayout->addStretch( 1 );
    parenLayout->setMargin( 10 );

    setWindowTitle( tr( "Create truss unit" ) );
    nameEdit->setFocus( Qt::OtherFocusReason );
}

void CreateTrussDialog::checkDialogState ()
{
    if ( ! nameEdit->text().isEmpty() &&
         xSizeEdit->value() > 0 && 
         ySizeEdit->value() > 0)
        okButton->setEnabled( true );
    else
        okButton->setEnabled( false ); 
}

void CreateTrussDialog::onOk ()
{
    TrussUnitWindow& truss = wndMng.createTrussUnitWindow( nameEdit->text() );
    truss.setTrussAreaSize( DoubleSize( xSizeEdit->value(), ySizeEdit->value() ) );  
    done( 1 );
}

void CreateTrussDialog::onCancel ()
{
    done( 0 );
}

/*****************************************************************************
 * Project ToolBox Page
 *****************************************************************************/

ProjectToolBoxPage::ProjectToolBoxPage ( FermaNextProject& prj,
                                         QWidget* parent, Qt::WFlags f ) :
    QWidget( parent, f ),
    pageProject( prj ),
    listBox( 0 )
{
    init();
}

void ProjectToolBoxPage::init ()
{
    QVBoxLayout* pageLayout = new QVBoxLayout( this );
    pageLayout->setMargin(3);
    pageLayout->setSpacing(0);

    QGroupBox* propertyBox = new QGroupBox( tr( "Project Info" ), this );
    pageLayout->addWidget( propertyBox );
    
    QGridLayout* propertyLayout = new QGridLayout( propertyBox );
    tokLabel = new QLabel( "", propertyBox );
    trussNumberLabel = new QLabel( "0", propertyBox );
    trussHiddenLabel = new QLabel( "0", propertyBox );
    //propertyLayout->addWidget( new QLabel( tr( "TOK: ") ), 0, 0 );
    propertyLayout->addWidget( new QLabel( tr( "Total trusses: ") ), 1, 0 );
    propertyLayout->addWidget( new QLabel( tr( "Hidden trusses: ") ), 2, 0 );
    //propertyLayout->addWidget( tokLabel, 0, 1 );
    propertyLayout->addWidget( trussNumberLabel, 1, 1 );
    propertyLayout->addWidget( trussHiddenLabel, 2, 1 );

    // Truss units group box
    QGroupBox* groupBoxTrussUnits = new QGroupBox( tr( "Trusses" ), this );

    QVBoxLayout* trussUnitsLayout = new QVBoxLayout( groupBoxTrussUnits );
    trussUnitsLayout->setMargin(3);
    trussUnitsLayout->setSpacing(0);
    pageLayout->addWidget( groupBoxTrussUnits );

    listBox = new WindowListBox( pageProject, groupBoxTrussUnits );
    trussUnitsLayout->addWidget( listBox );

    // Buttons
    QPushButton* calculateAllButton = new QPushButton;    
    trussUnitsLayout->addSpacing(10);
    //trussUnitsLayout->addWidget( calculateAllButton );
    calculateAllButton->setFlat( true );
    calculateAllButton->setStatusTip( tr( "Calculates all the project trusses" ) );
    calculateAllButton->setToolTip( tr( "Calculate All (F5)" ) );
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
    buttonImport->setStatusTip( tr( "Imports a truss of the previous format" ) );
    buttonImport->setToolTip( tr( "Import Truss" ) );
    buttonImport->setMinimumWidth( 60 );
    connect( buttonImport, SIGNAL(clicked()), 
                             SLOT(importIsPressed()) ); 

    QPushButton* buttonNewTruss = new QPushButton;
    buttonsLayout->addWidget( buttonNewTruss );
    buttonNewTruss->setFlat( true );
    buttonNewTruss->setText( tr("New") );
    buttonNewTruss->setStatusTip( tr( "Creates a new truss" ) );
    buttonNewTruss->setToolTip( tr( "New Truss" ) );
    buttonNewTruss->setMinimumWidth( 60 );
    connect( buttonNewTruss, SIGNAL(clicked()), 
                               SLOT(newTrussIsPressed()) ); 

    // Catch truss count and visibility changing
    TrussUnitWindowManager& mng =  pageProject.getTrussUnitWindowManager();
    connect( &mng, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)),
                     SLOT(afterTrussCountChange(TrussUnitWindow&)) );
    connect( &mng, SIGNAL(onTrussUnitWindowRevive(TrussUnitWindow&)),
                     SLOT(afterTrussCountChange(TrussUnitWindow&)) );
    connect( &mng, SIGNAL(onTrussUnitWindowDesist(TrussUnitWindow&)),
                     SLOT(afterTrussCountChange(TrussUnitWindow&)) );
    connect( &mng, SIGNAL(onTrussWindowVisibilityChange(bool)),
                     SLOT(afterTrussVisibilityChange()) );
}

FermaNextProject& ProjectToolBoxPage::project () const
{
    return pageProject;
}

WindowListBox& ProjectToolBoxPage::getWindowListBox () const
{
    return *listBox;
}

void ProjectToolBoxPage::setTrussNumber ( uint numb )
{
    trussNumberLabel->setText( QString::number(numb) );
}

uint ProjectToolBoxPage::getTrussNumber () const
{
    return trussNumberLabel->text().toUInt();
}

void ProjectToolBoxPage::setHiddenTrussNumber ( uint numb )
{
    trussHiddenLabel->setText( QString::number(numb) );
}

uint ProjectToolBoxPage::getHiddenTrussNumber () const
{
    return trussHiddenLabel->text().toUInt();
}

void ProjectToolBoxPage::setTOK ( double numb )
{
    tokLabel->setText( QString::number(numb) );
}

double ProjectToolBoxPage::getTOK () const
{
    return tokLabel->text().toDouble();
}

void ProjectToolBoxPage::importIsPressed ()
{
    QString fileName = QFileDialog::getOpenFileName( 
        this,
        tr( "Import trusses" ),
        QString(),
        "FermaNext, Ferma old (*" + TrussUnitWindowManager::newFormatExtension()  +
        " *" + TrussUnitWindowManager::oldFormatExtension() +")"
         );

    if ( fileName.isEmpty() ) 
        return;

    try { 
        TrussUnitWindowManager& mng = pageProject.getTrussUnitWindowManager();
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

void ProjectToolBoxPage::newTrussIsPressed ()
{
    CreateTrussDialog* createDlg = 
        new CreateTrussDialog( pageProject.getTrussUnitWindowManager(), this );

    createDlg->exec();
}

void ProjectToolBoxPage::calculateAllIsPressed ()
{
    QMessageBox::warning( this, "Not implemented", "Not implemented" );
}

void ProjectToolBoxPage::afterTrussCountChange ( TrussUnitWindow& w )
{
    TrussUnitWindowManager& mng = pageProject.getTrussUnitWindowManager();
    setTrussNumber( mng.getTrussUnitWindowList().size() );
    if ( ! w.isVisible() )
        afterTrussVisibilityChange();

}

void ProjectToolBoxPage::afterTrussVisibilityChange ()
{
    TrussUnitWindowManager& mng = pageProject.getTrussUnitWindowManager();
    int numb = 0;
    WindowList trussWindows = mng.getTrussUnitWindowList();
    WindowListIter iter = trussWindows.begin();
    for ( ; iter != trussWindows.end(); ++iter )
        if ( ! (*iter)->isVisible() )
            ++numb;

    setHiddenTrussNumber( numb );
}

/*****************************************************************************
 * Project ToolBox
 *****************************************************************************/

ProjectToolBox::ProjectToolBox ( FermaNextWorkspace& ws, QWidget* parent, 
                                 Qt::WFlags f ) :
    QToolBox(parent, f),
    workspace(ws)
{    
    setMouseTracking( true );
        
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

FermaNextProject* ProjectToolBox::currentProject () const
{
    ProjectToolBoxPage* currPage = 
        dynamic_cast<ProjectToolBoxPage*>( currentWidget() );

    if ( currPage )
        return &currPage->project();

    return 0;
}

ProjectToolBoxPage* ProjectToolBox::getPageForProject ( 
                                         const FermaNextProject& prj ) const
{
    for ( int i = 0; i < count(); ++i ) {
        ProjectToolBoxPage* page = 
            dynamic_cast<ProjectToolBoxPage*>( widget( i ) );
        if ( ! page )
            continue;

        if ( &page->project() == &prj )
            return page;        
    }
    return 0;
}

int ProjectToolBox::addProject ( FermaNextProject& prj )
{
    ProjectToolBoxPage* page = new ProjectToolBoxPage( prj );

    // Catch name changing
    connect( &prj, SIGNAL(onNameChange(const QString&)),
                     SLOT(projectRename(const QString&)) );

    QIcon iconSet( Global::imagesPath() + "/project_toolbox.png"  );
    int result = addItem( page, iconSet, prj.getName() );    
    setCurrentWidget(page);
    prj.activate();

    // FIXME: kill this long chain of signal delegations
    connect( &page->getWindowListBox(), 
                SIGNAL(onShowTrussResults(const TrussUnitWindow&)),
                SIGNAL(onShowTrussResults(const TrussUnitWindow&)) );

    connect( this, 
             SIGNAL(calculateTrussUnit(const TrussUnitWindow&)),
             &page->getWindowListBox(), 
             SLOT(solveTrussUnit(const TrussUnitWindow&)) );

    return result;
}

int ProjectToolBox::removeProject ( FermaNextProject& prj )
{
    ProjectToolBoxPage* page = getPageForProject( prj );
    if ( ! page )
        return -1;

    int pageIndex = indexOf( page );
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

        ProjectToolBoxPage* page = getPageForProject( prj );
        if ( ! page )
            return;

        int pageIndex = indexOf( page );

        if ( pageIndex != -1 )
            setItemText( pageIndex, newName );
    } 
    catch ( ... ) {}
}

void ProjectToolBox::projectIsActivated ( FermaNextProject& prj )
{
    ProjectToolBoxPage* page = getPageForProject( prj );
    if ( ! page && page == currentWidget() )
        return;

    setCurrentWidget( page );
}

void ProjectToolBox::activateSelected ( int index )
{   
    if ( index == -1 ) 
        return;

    FermaNextProject* currPrj = currentProject();
    if ( ! currPrj )
        return;

    if ( ! currPrj->isActivated() )
        currPrj->activate();

    QIcon activeIconSet( Global::imagesPath() + "/project_toolbox.png" );
    QIcon disablesIconSet( Global::imagesPath() + "/project_d.png" );
    for ( int i = 0; i < count(); ++i )
        setItemIcon( i, disablesIconSet );

    setItemIcon( index, activeIconSet );        
}

void ProjectToolBox::clear ()
{
    for ( int i=0; i < workspace.countProjects(); ++i )
        removeProject( workspace.getProject(i) );        
}

bool ProjectToolBox::eventFilter( QObject*, QEvent* event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;
        if ( keyEvent->key() == Qt::Key_N || keyEvent->key() == Qt::Key_P ||
             keyEvent->key() == Qt::Key_F || keyEvent->key() == Qt::Key_L ||
             keyEvent->key() == Qt::Key_Escape ) {
            FermaNextProject* prj = currentProject();
            if ( prj )
                prj->getDesignerWidget().aggKeyPressEvent( keyEvent );
            return true;
        }
    }
    return false;
}

/****************************************************************************/
