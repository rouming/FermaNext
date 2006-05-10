
#include <algorithm>

#include <QApplication>
#include <QColor>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "TrussMaterialEditor.h"
#include "TrussMaterial.h"
#include "FermaNextProject.h"
#include "GuiSubsidiary.h"

/*****************************************************************************
 * Material Tree Widget Item
 *****************************************************************************/

MaterialTreeWidgetItem::MaterialTreeWidgetItem ( const TrussMaterial& m, 
                                                 int type /* = Type */) :
    QTreeWidgetItem( type ),
    material( m )
{
    setText( 0, m.getMaterialName() );
    setTextAlignment( 0, Qt::AlignLeft );
    setText( 1, QString::number( m.getWorkingStress() ) );
    setTextAlignment( 1, Qt::AlignRight );
    setText( 2, QString::number( m.getElasticityModule() ) );
    setTextAlignment( 2, Qt::AlignRight );
    setText( 3, QString::number( m.getDensity() ) );
    setTextAlignment( 3, Qt::AlignRight );
    
    connect( &m, SIGNAL(onAfterNameChange(const QString&)) ,
                 SLOT(updateName(const QString&)) );
    connect( &m, SIGNAL(onAfterWorkingStressChange(double)) ,
                 SLOT(updateWorkingStress(double)) );
    connect( &m, SIGNAL(onAfterElasticityModuleChange(double)) ,
                 SLOT(updateElasticityModule(double)) );
    connect( &m, SIGNAL(onAfterDensityChange(double)) ,
                 SLOT(updateDensity(double)) );
}

const TrussMaterial& MaterialTreeWidgetItem::getItemMaterial () const
{
    return material;
}

void MaterialTreeWidgetItem::updateName ( const QString& name )
{
    setText( 0, name );
}

void MaterialTreeWidgetItem::updateWorkingStress ( double ws )
{
    setText( 1, QString::number( ws ) );
}

void MaterialTreeWidgetItem::updateElasticityModule ( double em )
{
    setText( 2, QString::number( em ) );
}

void MaterialTreeWidgetItem::updateDensity ( double d )
{
    setText( 3, QString::number( d ) );
}

/*****************************************************************************
 * Project Tree Widget Item
 *****************************************************************************/

ProjectTreeWidgetItem::ProjectTreeWidgetItem ( const FermaNextProject& p, 
                                               int type /* = Type */) :
    QTreeWidgetItem( type ),
    project( p )
{
    lib = &p.getMaterialLibrary();
    setText( 0, p.getName() );

    connect( &p, SIGNAL(onNameChange(const QString&)),
                 SLOT(updateName(const QString&)) );
}

const FermaNextProject& ProjectTreeWidgetItem::getItemProject () const
{
    return project;
}

TrussMaterialLibrary& ProjectTreeWidgetItem::getMaterialLibrary() const
{
    return *lib;
}

void ProjectTreeWidgetItem::setCurrent ( bool status )
{
    isCurrent = status;

    QFont currentFont = font( 0 );
    if ( isCurrent ) {
        currentFont.setBold( true );
        currentFont.setPixelSize( 13 );
    } else {
        currentFont.setBold( false );
        currentFont.setPixelSize( 12 );
    }

    setFont( 0, currentFont );
}

void ProjectTreeWidgetItem::updateName ( const QString& name )
{
    setText( 0, name );
}

/*****************************************************************************
 * Truss Material Edit Dialog
 *****************************************************************************/

TrussMaterialEditDialog::TrussMaterialEditDialog ( QWidget* parent /* = 0 */, 
                                                   Qt::WFlags f /* = 0 */ ) :
    QDialog( parent, f ),
    material( 0 ),
    onMaterialCreate( false )
{
    init();
}

void TrussMaterialEditDialog::init ()
{
    int maxElasticityModule = 999999999.99;
    int maxWorkingStress = 9999999.99;
    int maxMaterialDensity = 999.99;

    QGroupBox* materialPropBox = new QGroupBox( tr( "Material Properties" ) );
    nameLineEdit = new QLineEdit( materialPropBox );
    stressSpinBox = new QDoubleSpinBox( materialPropBox );
    stressSpinBox->setRange( 0, maxWorkingStress );
    stressSpinBox->setSingleStep( 1000 );
    elasticitySpinBox = new QDoubleSpinBox( materialPropBox );
    elasticitySpinBox->setRange( 0, maxElasticityModule );
    elasticitySpinBox->setSingleStep( 100000 );
    densitySpinBox = new QDoubleSpinBox( materialPropBox );
    densitySpinBox->setRange( 0, maxMaterialDensity );
    densitySpinBox->setDecimals( 3 );
    densitySpinBox->setSingleStep( 0.001 );
    QPushButton* okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                       SLOT(applyChanges()) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                           SLOT(cancelChanges()) );

    // init material property group box layouts
    QVBoxLayout* parentLayout = new QVBoxLayout( materialPropBox );
   
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget( new QLabel(tr("Name: "), materialPropBox) );
    nameLayout->addWidget( nameLineEdit );

    QGridLayout* materialPropLayout = new QGridLayout;
    materialPropLayout->addWidget( new QLabel( tr("Working Stress: "), 
                                               materialPropBox ), 1, 0 );
    materialPropLayout->addWidget( stressSpinBox, 1, 1 );
    materialPropLayout->addWidget( new QLabel( tr("Elasticity Module: "), 
                                               materialPropBox ), 2, 0 );
    materialPropLayout->addWidget( elasticitySpinBox, 2, 1 );
    materialPropLayout->addWidget( new QLabel( tr("Material Density: "), 
                                               materialPropBox ), 3, 0 );
    materialPropLayout->addWidget( densitySpinBox, 3, 1 );

    parentLayout->addLayout( nameLayout);
    parentLayout->addLayout( materialPropLayout);

    // init layout for buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( materialPropBox );
    mainLayout->addLayout( buttonLayout );

    setWindowTitle( "Edit Material" );
    setFixedSize( 250, 200 ); 
}

void TrussMaterialEditDialog::setMaterial ( TrussMaterial& m, bool newMaterial )
{
    material = &m;
    onMaterialCreate = newMaterial;
    if ( newMaterial ) {
        nameLineEdit->clear();
        elasticitySpinBox->setValue(0);
        stressSpinBox->setValue(0);
        densitySpinBox->setValue(0);
    } else {
        nameLineEdit->setText( material->getMaterialName() );
        elasticitySpinBox->setValue( material->getElasticityModule() );
        stressSpinBox->setValue( material->getWorkingStress() );
        densitySpinBox->setValue( material->getDensity() );
    }
}

void TrussMaterialEditDialog::applyChanges ()
{
    if ( nameLineEdit->text().isEmpty() ) 
    {
        ( QMessageBox::critical( this, tr("Material creation"),
                                 tr("Material name wasn't entered.") ) );
        return;
    } 
    else 
    {
        try {
            material->setMaterialName( nameLineEdit->text() );
        } 
        catch ( TrussMaterialLibrary::WrongMaterialNameException& ) {
            ( QMessageBox::critical( this, tr("Material creation"),
              tr("There is another material with name %1.").arg(
                                            nameLineEdit->text()) ) );
            return;
        }
    }

    try {
        material->setWorkingStress( stressSpinBox->value() );
    } 
    catch ( TrussMaterialLibrary::WrongWorkingStressException& ) {
        ( QMessageBox::critical( this, tr("Material creation"),
          tr("Wrong working stress was entered.") ) );
        return;
    }

    try {
        material->setElasticityModule( elasticitySpinBox->value() );
    } 
    catch ( TrussMaterialLibrary::WrongElasticityModuleException& ) {
        ( QMessageBox::critical( this, tr("Material creation"),
          tr("Wrong elasticity module was entered.") ) );
        return;
    }

    try {
        material->setDensity( densitySpinBox->value() );
    } 
    catch ( TrussMaterialLibrary::WrongDensityException& ) {
        ( QMessageBox::critical( this, tr("Material creation"),
          tr("Wrong density was entered.") ) );
        return;
    }
    
    if ( onMaterialCreate )
        emit onMaterialCreationApply( *material );
    
    hide();
}

void TrussMaterialEditDialog::cancelChanges ()
{ 
    if ( onMaterialCreate )
        emit onMaterialCreationCancel( *material );
    
    hide();
}

/*****************************************************************************
 * Truss Material Editor
 *****************************************************************************/

TrussMaterialEditor::TrussMaterialEditor ( QWidget* parent /* = 0 */, 
                                           Qt::WFlags f /* = 0 */ ) :
    QDialog( parent, f ),
    materialLibTreeList( new QTreeWidget ),
    addButton( 0 ),
    editButton( 0 ),
    removeButton( 0 )
{
    init();
}

void TrussMaterialEditor::init ()
{
    materialLibTreeList->setColumnCount(4);
    materialLibTreeList->setEditTriggers( QAbstractItemView::NoEditTriggers );

    QHeaderView *header = materialLibTreeList->header();
    header->setClickable( false );
    header->setMovable( false );
    QStringList labels;
    labels << tr("Project/Material") << tr("Stress") << 
              tr("Elasticity") << tr("Density");
    materialLibTreeList->setHeaderLabels( labels );
    header->resizeSection( 0, 150 );
    header->resizeSection( 1, 60 );
    header->resizeSection( 2, 60 );
    header->resizeSection( 3, 60 );
    //header->setResizeMode( QHeaderView::Custom );
    QPushButton* okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                       SLOT(hide()) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                           SLOT(hide()) );
    addButton = new QPushButton( tr( "Add" ) );
    connect( addButton, SIGNAL(clicked()),
                        SLOT(addMaterial()) );
    editButton = new QPushButton( tr( "Edit" ) );
    connect( editButton, SIGNAL(clicked()),
                         SLOT(editMaterial()) );
    editButton->setEnabled( false );
    removeButton = new QPushButton( tr( "Remove" ) );
    connect( removeButton, SIGNAL(clicked()),
                           SLOT(removeMaterial()) );
    removeButton->setEnabled( false );
    
    // init layouts
    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addWidget( materialLibTreeList );
    QVBoxLayout* rigthLayout = new QVBoxLayout;
    rigthLayout->addWidget( addButton );
    rigthLayout->addWidget( editButton );    
    rigthLayout->addWidget( removeButton );
    rigthLayout->addStretch( 1 );
    rigthLayout->addWidget( okButton );
    rigthLayout->addWidget( cancelButton );
    QHBoxLayout* mainLayout = new QHBoxLayout( this );
    mainLayout->addLayout( leftLayout );
    mainLayout->addLayout( rigthLayout );

    editDialog = new TrussMaterialEditDialog( this );
    editDialog->hide();

    connect( materialLibTreeList, SIGNAL(itemSelectionChanged()),
                                  SLOT(checkButtons()) );
    
    connect( editDialog, SIGNAL(onMaterialCreationApply(const TrussMaterial&)),
                         SLOT(applyMaterialCreation(const TrussMaterial&)) );

    connect( editDialog, SIGNAL(onMaterialCreationCancel(TrussMaterial&)),
                         SLOT(cancelMaterialCreation(TrussMaterial&)) );

    setWindowTitle( "Material Editor" );
    setFixedSize( 445, 250 );    
}

QTreeWidgetItem* TrussMaterialEditor::getSelectedItem( 
                                                bool& materialSelected ) const
{
    QList<QTreeWidgetItem*> selItems = materialLibTreeList->selectedItems();

    if ( selItems.size() != 1 )
        return 0;

    QTreeWidgetItem* item = selItems[0];
    if ( ! item )
        return 0;

    if ( item->parent() )
        materialSelected = true;
    else
        materialSelected = false;
    
    return item;
}

ProjectTreeWidgetItem* TrussMaterialEditor::getSelectedProjectItem () const
{
    bool materialSelected;
    QTreeWidgetItem* item = getSelectedItem( materialSelected );
    if ( ! item )
        return 0;

    if ( materialSelected ) {
        QTreeWidgetItem* projectItem = item->parent();
        if ( ! projectItem )
            return 0;

        return dynamic_cast<ProjectTreeWidgetItem*>(projectItem);;
    }
    return dynamic_cast<ProjectTreeWidgetItem*>(item);
}

MaterialTreeWidgetItem* TrussMaterialEditor::getSelectedMaterialItem () const
{
    bool materialSelected;
    QTreeWidgetItem* item = getSelectedItem( materialSelected );
    if ( ! item || ! materialSelected )
        return 0;

    return dynamic_cast<MaterialTreeWidgetItem*>(item);;
}

TrussMaterial* TrussMaterialEditor::getSelectedMaterial () const
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    MaterialTreeWidgetItem* materialItem = getSelectedMaterialItem();
    if ( ! projItem || ! materialItem )
        return 0;
    
    TrussMaterialLibrary& lib = projItem->getMaterialLibrary();
    return lib.getMaterial( materialItem->text(0) );
}

MaterialTreeWidgetItem* TrussMaterialEditor::getMaterialItem ( 
                                const TrussMaterial& m,
                                const ProjectTreeWidgetItem& projItem ) const
{
    for ( int i = 0; i < projItem.childCount(); ++i ) {
        MaterialTreeWidgetItem* materialItem = 
            dynamic_cast<MaterialTreeWidgetItem*>( projItem.child( i ) );
        if ( ! materialItem )
            continue;

        if ( &materialItem->getItemMaterial() == &m ) {
            materialLibTreeList->setItemHidden( materialItem, false );
            return materialItem;
        }
    }
    return 0;
}

ProjectTreeWidgetItem* TrussMaterialEditor::getProjectItem ( 
                                    const FermaNextProject& proj ) const
{
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );
        ProjectTreeWidgetItem* projItem =
            dynamic_cast<ProjectTreeWidgetItem*>( item );
        if ( ! projItem )
            continue;

        if ( &projItem->getItemProject() == &proj )
            return projItem;
    }
    return 0;
}

void TrussMaterialEditor::addMaterialItem ( const TrussMaterial& m )
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    projItem->addChild( new MaterialTreeWidgetItem( m ) );
}

void TrussMaterialEditor::removeMaterialItem ( const TrussMaterial& m )
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    MaterialTreeWidgetItem* materialItem = getMaterialItem( m, *projItem );
    int indx = projItem->indexOfChild( materialItem );
    delete projItem->takeChild( indx );
}

void TrussMaterialEditor::addProjectItem ( FermaNextProject& proj )
{
    ProjectTreeWidgetItem* projItem = new ProjectTreeWidgetItem( proj );
    materialLibTreeList->addTopLevelItem( projItem );
    materialLibTreeList->setCurrentItem( projItem );
    setCurrentProjectItem( proj );
    
    TrussMaterialLibrary& lib = projItem->getMaterialLibrary();
    for ( int i = 0; i < lib.countMaterials(); ++i ) {
        TrussMaterial* m = lib.getMaterial( i );
        if ( m )
            addMaterialItem( *m );
    }
    connect( &lib, SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                   SLOT(addMaterialItem(const TrussMaterial&)) );
    connect( &lib, SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                   SLOT(removeMaterialItem(const TrussMaterial&)) );
}

void TrussMaterialEditor::removeProjectItem ( FermaNextProject& proj )
{
    ProjectTreeWidgetItem* projItem = getProjectItem( proj );
    if ( ! projItem )
        return;
    
    int indx = materialLibTreeList->indexOfTopLevelItem( projItem );
    delete materialLibTreeList->takeTopLevelItem( indx );
}

void TrussMaterialEditor::setCurrentProjectItem ( FermaNextProject& proj )
{
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );
        if ( ! item )
            return;

        ProjectTreeWidgetItem* projItem = 
            dynamic_cast<ProjectTreeWidgetItem*>(item);
        if ( ! projItem )
            return;

        if ( &projItem->getItemProject() == &proj )
            projItem->setCurrent( true );
        else
            projItem->setCurrent( false );
    }
}

void TrussMaterialEditor::addMaterial ()
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    TrussMaterial* m = 0;
    TrussMaterialLibrary& lib = projItem->getMaterialLibrary();
    
    try {
        m = &lib.createMaterial( "", 1.0, 1.0, 1.0 );
    } 
    catch ( TrussMaterialLibrary::WrongMaterialNameException& ) {
        ( QMessageBox::critical( this, tr("Error"),
                                 tr("Can't create material.") ) );
        return;
    }

    MaterialTreeWidgetItem* materialItem = getMaterialItem( *m, *projItem);
    if ( materialItem )
        materialLibTreeList->setItemHidden( materialItem, true );

    editDialog->setMaterial( *m, true );
    editDialog->exec();
}

void TrussMaterialEditor::editMaterial ()
{
    TrussMaterial* m = getSelectedMaterial();
    if ( m )
        editDialog->setMaterial( *m, false );
    editDialog->exec();
}

void TrussMaterialEditor::removeMaterial ()
{
    QTreeWidgetItem* materialItem = getSelectedMaterialItem();
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! materialItem || ! projItem )
        return;
    
    QString name = materialItem->text(0);

    if ( QMessageBox::question( this, tr("Material remove confirmation"),
                                tr("Remove material %1?").arg(name),
                                tr("&Yes"), tr("&No"), QString::null, 0, 1 ) )
        return;
    
    projItem->getMaterialLibrary().removeMaterial( name );
    int indx = projItem->indexOfChild( materialItem );
    delete projItem->takeChild( indx );
}

void TrussMaterialEditor::applyMaterialCreation ( const TrussMaterial& m )
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    MaterialTreeWidgetItem* materialItem = getMaterialItem( m, *projItem);
    if ( materialItem )
        materialLibTreeList->setItemHidden( materialItem, false );
}

void TrussMaterialEditor::cancelMaterialCreation ( TrussMaterial& m )
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    projItem->getMaterialLibrary().removeMaterial( m );
}

void TrussMaterialEditor::checkButtons ()
{
    if ( getSelectedMaterialItem() ) {
        editButton->setEnabled( true );
        removeButton->setEnabled( true );
    } else {
        editButton->setEnabled( false );
        removeButton->setEnabled( false );
    }
}

/***************************************************************************/
