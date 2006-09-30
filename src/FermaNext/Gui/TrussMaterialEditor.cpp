
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
                                                 int type /* = Type */ ) :
    QTreeWidgetItem( type ),
    material( m )
{
    setText( 0, m.getMaterialName() );
    setTextAlignment( 0, Qt::AlignLeft );
    
    connect( &m, SIGNAL(onAfterNameChange(const QString&)) ,
                 SLOT(updateName(const QString&)) );
}

const TrussMaterial& MaterialTreeWidgetItem::getItemMaterial () const
{
    return material;
}

void MaterialTreeWidgetItem::updateName ( const QString& name )
{
    setText( 0, name );
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
 * Material Edit Widget
 *****************************************************************************/

MaterialEditWidget::MaterialEditWidget ( QWidget* parent /* = 0 */, 
                                         Qt::WFlags f /* = 0 */ ) :
    QWidget( parent, f ),
    material( 0 )
{
    init();
}

void MaterialEditWidget::init ()
{
    QGroupBox* materialPropBox = new QGroupBox( tr( "Material Properties" ) );
    nameLineEdit = new QLineEdit( materialPropBox );
    stressSpinBox = new QDoubleSpinBox( materialPropBox );
    stressSpinBox->setRange( 0, Global::maxWorkingStress );
    stressSpinBox->setSingleStep( 1000 );
    elasticitySpinBox = new QDoubleSpinBox( materialPropBox );
    elasticitySpinBox->setRange( 0, Global::maxElasticityModule );
    elasticitySpinBox->setSingleStep( 100000 );
    densitySpinBox = new QDoubleSpinBox( materialPropBox );
    densitySpinBox->setRange( 0, Global::maxMaterialDensity );
    densitySpinBox->setDecimals( 3 );
    densitySpinBox->setSingleStep( 0.001 );

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

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( materialPropBox );

    connect( nameLineEdit, SIGNAL( editingFinished() ),
                             SLOT( updateName() ) );

    connect( elasticitySpinBox, SIGNAL( editingFinished() ),
                                  SLOT( updateElasticityModule() ) );

    connect( stressSpinBox, SIGNAL( editingFinished() ),
                              SLOT( updateWorkingStress() ) );

    connect( densitySpinBox, SIGNAL( editingFinished() ),
                               SLOT( updateDensity() ) );

    showEditors( false );

    nameLineEdit->installEventFilter( this );
    elasticitySpinBox->installEventFilter( this );
    stressSpinBox->installEventFilter( this );
    densitySpinBox->installEventFilter( this );
}

void MaterialEditWidget::setMaterial ( TrussMaterial& m, 
                                       bool clearEditors /* = false */ )
{
    showEditors( true );
    if ( ! clearEditors ) {
        nameLineEdit->setText( m.getMaterialName() );
        elasticitySpinBox->setValue( m.getElasticityModule() );
        stressSpinBox->setValue( m.getWorkingStress() );
        densitySpinBox->setValue( m.getDensity() );
    }
    else {
        nameLineEdit->clear();
        elasticitySpinBox->clear();
        stressSpinBox->clear();
        densitySpinBox->clear();
    }
    material = &m;
}

void MaterialEditWidget::updateName ()
{
    if ( ! material )
        return;
    
    try {
        material->setMaterialName( nameLineEdit->text() );
    } 
    catch ( TrussMaterialLibrary::WrongMaterialNameException& ) {
        ( QMessageBox::critical( this, tr("Material creation"),
          tr("There is another material with name %1.").arg(
                                        nameLineEdit->text()) ) );
        nameLineEdit->setFocus();
        return;
    }
}

void MaterialEditWidget::updateWorkingStress ()
{
    if ( ! material )
        return;

    material->setWorkingStress( stressSpinBox->value() );
}

void MaterialEditWidget::updateElasticityModule ()
{
    if ( ! material )
        return;

    material->setElasticityModule( elasticitySpinBox->value() );
}

void MaterialEditWidget::updateDensity ()
{
    if ( ! material )
        return;

    material->setDensity( densitySpinBox->value() );
}

void MaterialEditWidget::showEditors ( bool showStatus )
{
    nameLineEdit->clear();
    elasticitySpinBox->clear();
    stressSpinBox->clear();
    densitySpinBox->clear();   
    
    nameLineEdit->setEnabled( showStatus );
    elasticitySpinBox->setEnabled( showStatus );
    stressSpinBox->setEnabled( showStatus );
    densitySpinBox->setEnabled( showStatus );
}

bool MaterialEditWidget::eventFilter ( QObject* targetObj, QEvent* event )
{
    if ( event->type() == QEvent::MouseButtonRelease ||
         event->type() == QEvent::KeyRelease ) {
        if ( targetObj ) 
            return true;
        return QWidget::eventFilter( targetObj, event );
    }
    if ( event->type() == QEvent::FocusOut ) {
        if ( targetObj == nameLineEdit ) {
            if ( nameLineEdit->text().isEmpty() ) {
                QMessageBox::critical( this, tr("Material creation"),
                                 tr("Material name wasn't entered.") );
                nameLineEdit->setFocus( Qt::OtherFocusReason );
                return true;
            }
        }
        else if ( targetObj == elasticitySpinBox ) {
            if ( elasticitySpinBox->value() <= 0 ) {
                QMessageBox::critical( this, tr("Material creation"),
                          tr("Wrong working stress was entered.") );
                elasticitySpinBox->setFocus( Qt::OtherFocusReason );
                return true;
            }
        }
        else if ( targetObj == stressSpinBox ) {
            if ( stressSpinBox->value() <= 0 ) {
                QMessageBox::critical( this, tr("Material creation"),
                                tr("Wrong elasticity module was entered.") );
                stressSpinBox->setFocus( Qt::OtherFocusReason );
                return true;
            }
        }
        else if ( targetObj == densitySpinBox ) {
            if ( densitySpinBox->value() <= 0 ) {
                QMessageBox::critical( this, tr("Material creation"),
                                 tr("Wrong density was entered.") );
                densitySpinBox->setFocus( Qt::OtherFocusReason );
                return true;
            }
        }
        return false;
    }
    else
        return QWidget::eventFilter( targetObj, event );
}

/*****************************************************************************
 * Truss Material Creation Dialog
 *****************************************************************************/

MaterialCreationDialog::MaterialCreationDialog ( QWidget* parent /* = 0 */, 
                                                 Qt::WFlags f /* = 0 */ ) :
    QDialog( parent, f ),
    material( 0 )
{
    init();
}

void MaterialCreationDialog::init ()
{
    materialEditWidget = new MaterialEditWidget;
    
    QPushButton* okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                       SLOT(applyChanges()) );

    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                           SLOT(cancelChanges()) );

    QVBoxLayout* editLayout = new QVBoxLayout;
    editLayout->addWidget( materialEditWidget );
   
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addLayout( editLayout );
    mainLayout->addLayout( buttonLayout );

    setWindowTitle( "Material Creation" );
    setFixedSize( 250, 200 ); 
}

void MaterialCreationDialog::setMaterial ( TrussMaterial& m )
{
    materialEditWidget->setMaterial( m, true );
    material = &m;
}

void MaterialCreationDialog::applyChanges ()
{
    emit onMaterialCreationApply( *material );
    hide();
}

void MaterialCreationDialog::cancelChanges ()
{ 
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
    removeButton( 0 ),
    applyButton( 0 )
{
    init();
}

void TrussMaterialEditor::init ()
{
    materialLibTreeList->setEditTriggers( QAbstractItemView::NoEditTriggers );

    QHeaderView *header = materialLibTreeList->header();
    header->setClickable( false );
    QStringList labels;
    labels << tr("Project/Material");
    materialLibTreeList->setHeaderLabels( labels );

    editWidget = new MaterialEditWidget;
    
    createDialog = new MaterialCreationDialog( this );
    createDialog->hide();

    QPushButton* okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                          SLOT(hide()) );
    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                             SLOT(cancelChanges()) );
    applyButton = new QPushButton( tr( "Apply" ) );
    connect( applyButton, SIGNAL(clicked()),
                            SLOT(applyChanges()) );
    applyButton->setEnabled( false );
    addButton = new QPushButton( tr( "Add" ) );
    connect( addButton, SIGNAL(clicked()),
                        SLOT(addMaterial()) );
    removeButton = new QPushButton( tr( "Remove" ) );
    connect( removeButton, SIGNAL(clicked()),
                           SLOT(removeMaterial()) );
    removeButton->setEnabled( false );
    
    // init layouts
    QVBoxLayout* treeLayout = new QVBoxLayout;
    treeLayout->addWidget( materialLibTreeList );

    QVBoxLayout* editLayout = new QVBoxLayout;
    editLayout->addWidget( editWidget );

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget( addButton );
    buttonLayout->addWidget( removeButton );
    buttonLayout->addStretch( 1 );
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );
    buttonLayout->addWidget( applyButton );

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addLayout( treeLayout );
    topLayout->addLayout( editLayout );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addLayout( topLayout );
    mainLayout->addLayout( buttonLayout );

    connect( materialLibTreeList, SIGNAL(itemSelectionChanged()),
                                  SLOT(checkButtons()) );
    
    connect( createDialog, SIGNAL(onMaterialCreationApply(const TrussMaterial&)),
                         SLOT(applyMaterialCreation(const TrussMaterial&)) );

    connect( createDialog, SIGNAL(onMaterialCreationCancel(TrussMaterial&)),
                         SLOT(cancelMaterialCreation(TrussMaterial&)) );
    
    connect( materialLibTreeList, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                                    SLOT(fillEditorFields(QTreeWidgetItem*, int)));

    setWindowTitle( "Material Editor" );
    setFixedSize( 500, 250 );    
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

void TrussMaterialEditor::fillEditorFields ( QTreeWidgetItem* item, int )
{
    MaterialTreeWidgetItem* materialItem = 
        dynamic_cast<MaterialTreeWidgetItem*>( item );
    if ( ! materialItem )
        return;

    // since we need to edit it in future
    TrussMaterial& material = 
        const_cast<TrussMaterial&>( materialItem->getItemMaterial() );
    
    editWidget->setMaterial( material );
}

void TrussMaterialEditor::saveMaterialProperties ()
{
    materialProperties.clear();
    
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );

        ProjectTreeWidgetItem* projItem = 
            dynamic_cast<ProjectTreeWidgetItem*>(item);
        if ( ! projItem )
            return;

        const FermaNextProject& proj = projItem->getItemProject();
        TrussMaterialLibrary& mLib = proj.getMaterialLibrary();
        QDomNode oldData = materialPropDoc.firstChild();
        if ( ! oldData.isNull() )
            materialPropDoc.removeChild( oldData );
        QDomElement xmlProps = mLib.saveToXML( materialPropDoc );
        materialProperties.insert( &proj, xmlProps );
    }    
}

void TrussMaterialEditor::loadMaterialProperties ()
{
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );

        ProjectTreeWidgetItem* projItem = 
            dynamic_cast<ProjectTreeWidgetItem*>(item);
        if ( ! projItem )
            return;

        const FermaNextProject& proj = projItem->getItemProject();
        TrussMaterialLibrary& mLib = proj.getMaterialLibrary();
        mLib.clean();
        mLib.loadFromXML( materialProperties.value( &proj ) );
    }    
}

void TrussMaterialEditor::exec ()
{
    saveMaterialProperties();
    QDialog::exec();
}

void TrussMaterialEditor::cancelChanges ()
{
    loadMaterialProperties();
    hide();
}

void TrussMaterialEditor::applyChanges ()
{
    saveMaterialProperties();
    hide();
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

    createDialog->setMaterial( *m );
    createDialog->exec();
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
        editWidget->showEditors( true );
        removeButton->setEnabled( true );
    }
    else {
        editWidget->showEditors( false );
        removeButton->setEnabled( false );
    }
}

/***************************************************************************/
