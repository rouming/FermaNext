
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
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

//#include "../../3rdparty/QtSolutions/qtcolorcombobox.h"
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

    connect( &material, SIGNAL( onAfterNameChange( const QString& ) ),
                          SLOT( updateName() ) );
}

const TrussMaterial& MaterialTreeWidgetItem::getItemMaterial () const
{
    return material;
}

void MaterialTreeWidgetItem::updateName ()
{
    setText( 0, material.getMaterialName() );
}

/*****************************************************************************
 * Project Tree Widget Item
 *****************************************************************************/

ProjectTreeWidgetItem::ProjectTreeWidgetItem ( const QString& projName,
                                               TrussMaterialLibrary& mLib,
                                               bool current /* = false */,
                                               int type /* = Type */) :
    QTreeWidgetItem( type ),
    lib( mLib )
{
    setText( 0, "Project: " + projName );
    setCurrent( current );
}


TrussMaterialLibrary& ProjectTreeWidgetItem::getMaterialLibrary() const
{
    return lib;
}

void ProjectTreeWidgetItem::setCurrent ( bool status )
{
    isCurrent = status;

    QFont currentFont = QTreeWidgetItem::font( 0 );
    if ( isCurrent ) {
        currentFont.setBold( true );
        currentFont.setPixelSize( 13 );
        setIcon( 0, QIcon( Global::imagesPath() + "/project_des.png" ) );
    } else {
        currentFont.setBold( false );
        currentFont.setPixelSize( 12 );
        setIcon( 0, QIcon( Global::imagesPath() + "/project_d.png" ) );
    }

    setFont( 0, currentFont );
}

/*****************************************************************************
 * Material Tree Widget
 *****************************************************************************/

MaterialTreeWidget::MaterialTreeWidget ( QWidget* parent /* = 0 */ ) :
    QTreeWidget( parent )
{
    setAcceptDrops( true );
}

QTreeWidgetItem* MaterialTreeWidget::getSelectedItem( bool& materialSelected ) const
{
    QList<QTreeWidgetItem*> selItems = selectedItems();

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

ProjectTreeWidgetItem* MaterialTreeWidget::getSelectedProjectItem () const
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

MaterialTreeWidgetItem* MaterialTreeWidget::getSelectedMaterialItem () const
{
    bool materialSelected;
    QTreeWidgetItem* item = getSelectedItem( materialSelected );
    if ( ! item || ! materialSelected )
        return 0;

    return dynamic_cast<MaterialTreeWidgetItem*>(item);;
}

MaterialTreeWidgetItem* MaterialTreeWidget::getMaterialItem ( 
                                const TrussMaterial& m,
                                const ProjectTreeWidgetItem& projItem ) const
{
    for ( int i = 0; i < projItem.childCount(); ++i ) {
        MaterialTreeWidgetItem* materialItem = 
            dynamic_cast<MaterialTreeWidgetItem*>( projItem.child( i ) );
        if ( ! materialItem )
            continue;

        if ( &materialItem->getItemMaterial() == &m )
            return materialItem;
    }
    return 0;
}

void MaterialTreeWidget::addMaterialItem ( const TrussMaterial& m )
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    MaterialTreeWidgetItem* mItem = new MaterialTreeWidgetItem( m );
    projItem->addChild( mItem );
    setCurrentItem( mItem );
}

void MaterialTreeWidget::removeMaterialItem ( const TrussMaterial& m )
{
    ProjectTreeWidgetItem* projItem = getSelectedProjectItem();
    if ( ! projItem )
        return;

    MaterialTreeWidgetItem* materialItem = getMaterialItem( m, *projItem );
    int indx = projItem->indexOfChild( materialItem );
    delete projItem->takeChild( indx );
}

void MaterialTreeWidget::removeItemWithMaterial ( QTreeWidgetItem& item )
{
    ProjectTreeWidgetItem* projItem = 
        dynamic_cast<ProjectTreeWidgetItem*>( item.parent() );
    if ( ! projItem )
        return;

    MaterialTreeWidgetItem* mItem = 
        dynamic_cast<MaterialTreeWidgetItem*>( &item );
    if ( ! mItem )
        return;
    
    TrussMaterialLibrary& mLib = projItem->getMaterialLibrary();
    mLib.removeMaterial( mItem->getItemMaterial().getUUID() );

    int indx = projItem->indexOfChild( mItem );
    delete projItem->takeChild( indx );
}

void MaterialTreeWidget::mousePressEvent ( QMouseEvent* mEvent )
{
    if ( mEvent->button() == Qt::LeftButton ) 
        dragPos = mEvent->pos();
    QTreeWidget::mousePressEvent( mEvent );
}

void MaterialTreeWidget::mouseMoveEvent ( QMouseEvent* mEvent )
{
    if ( mEvent->buttons() & Qt::LeftButton ) {
        int dist = ( mEvent->pos() - dragPos ).manhattanLength();
        if ( dist > QApplication::startDragDistance() )
            startDrag();
    }
    QTreeWidget::mouseMoveEvent( mEvent );
}

void MaterialTreeWidget::startDrag ()
{
    MaterialTreeWidgetItem* mItem = getSelectedMaterialItem();
    if ( ! mItem )
        return;
    
    acceptDrag = true;

    ProjectTreeWidgetItem* prjItem = getSelectedProjectItem();
    if ( ! prjItem )
        return;

    TrussMaterialLibrary& mLib = prjItem->getMaterialLibrary();

    QDrag* materialDrag = new QDrag( this );
    QMimeData* mimeData = new QMimeData;
    mimeData->setText( mLib.getMaterialXml( mItem->getItemMaterial() ) );
    materialDrag->setHotSpot( visualItemRect( currentItem() ).topLeft() );
    materialDrag->setMimeData( mimeData );

    if ( materialDrag->start( Qt::MoveAction ) == 
                Qt::MoveAction | Qt::CopyAction && acceptDrag )
        removeItemWithMaterial( *mItem );
}

void MaterialTreeWidget::dragEnterEvent( QDragEnterEvent* event )
{
    if ( event->mimeData()->hasText() )
        event->accept();
    else
        event->ignore();
}

void MaterialTreeWidget::dragMoveEvent( QDragMoveEvent *event )
{
    if ( event->mimeData()->hasText() ) {
        event->setDropAction( Qt::MoveAction );
        event->accept();
    } else
        event->ignore();
}

bool MaterialTreeWidget::dropMimeData ( QTreeWidgetItem* parent, int index, 
                                        const QMimeData* data, 
                                        Qt::DropAction action )
{
    Q_UNUSED(index);
    Q_UNUSED(action);

    ProjectTreeWidgetItem* pItem = 0;

    MaterialTreeWidgetItem* mItem = 
        dynamic_cast<MaterialTreeWidgetItem*>( parent );

    if ( mItem )
        pItem = dynamic_cast<ProjectTreeWidgetItem*>( parent->parent() );
    else
        pItem = dynamic_cast<ProjectTreeWidgetItem*>( parent );

    if ( ! pItem || pItem == getSelectedProjectItem() ) {
        acceptDrag = false;
        return false;
    }

    TrussMaterialLibrary& mLib = pItem->getMaterialLibrary();
    TrussMaterial& m = mLib.createMaterial( data->text() );
    MaterialTreeWidgetItem* newItem = new MaterialTreeWidgetItem( m );
    pItem->addChild( newItem );

    return true;
}

QStringList MaterialTreeWidget::mimeTypes () const
{
    QStringList strList;

    strList.append( "application/x-qabstractitemmodeldatalist" );
    strList.append( "text/html" );
    strList.append( "text/plain" );
    strList.append( "text/uri-list" );
    strList.append( "text/directory" );
    strList.append("text/xml" );

    return strList;
}

Qt::DropActions MaterialTreeWidget::supportedDropActions () const
{
    return Qt::MoveAction | Qt::CopyAction;
}

/*****************************************************************************
 * Material Edit Widget
 *****************************************************************************/

MaterialEditWidget::MaterialEditWidget ( QWidget* parent /* = 0 */, 
                                         Qt::WFlags f /* = 0 */ ) :
    QWidget( parent, f ),
    material( 0 ),
    widgetStack( new QStackedWidget( this ) )
{
    init();
}

void MaterialEditWidget::init ()
{
    // Init material page
    QWidget* materialPage = new QWidget;
    QGroupBox* materialPropBox =
            new QGroupBox( tr( "Material properties" ), materialPage );
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
    //materialColorCmb = new QtColorComboBox( materialPropBox );
    //materialColorCmb->addColor( QColor( 0, 0, 0 ), tr( "Black" ) );

    // init material property group box layouts
    QVBoxLayout* parentLayout = new QVBoxLayout( materialPropBox );
   
    QHBoxLayout* nameLayout = new QHBoxLayout;
    nameLayout->addWidget( new QLabel(tr("Name: "), materialPropBox ) );
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

    //materialPropLayout->addWidget( new QLabel( tr("Material Color: "),
    //                                           materialPropBox), 4, 0 );
    //materialPropLayout->addWidget( materialColorCmb );

    parentLayout->addLayout( nameLayout);
    parentLayout->addLayout( materialPropLayout);

    QVBoxLayout* materialPageLayout = new QVBoxLayout( materialPage );
    materialPageLayout->addWidget( materialPropBox );

    connect( nameLineEdit, SIGNAL( editingFinished() ),
                             SLOT( updateName() ) );

    connect( elasticitySpinBox, SIGNAL( editingFinished() ),
                                  SLOT( updateElasticityModule() ) );

    connect( stressSpinBox, SIGNAL( editingFinished() ),
                              SLOT( updateWorkingStress() ) );

    connect( densitySpinBox, SIGNAL( editingFinished() ),
                               SLOT( updateDensity() ) );

    widgetStack->addWidget( materialPage );

    // Init empty page
    QWidget* emptyPage = new QWidget;
    widgetStack->addWidget( emptyPage );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( widgetStack );
}

void MaterialEditWidget::setMaterial ( TrussMaterial& m )
{
    nameLineEdit->setText( m.getMaterialName() );
    elasticitySpinBox->setValue( m.getElasticityModule() );
    stressSpinBox->setValue( m.getWorkingStress() );
    densitySpinBox->setValue( m.getDensity() );

    material = &m;

    widgetStack->setCurrentIndex( 0 );
}

void MaterialEditWidget::updateName ()
{
    if ( ! material || nameLineEdit->text().isEmpty() )
        return;
    
    material->setMaterialName( nameLineEdit->text() );
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

void MaterialEditWidget::clearPage ()
{
    widgetStack->setCurrentIndex( 1 );
}

/*****************************************************************************
 * Truss Material Editor
 *****************************************************************************/

TrussMaterialEditor::TrussMaterialEditor ( QString& startPage,
                                           QWidget* parent /* = 0 */, 
                                           Qt::WFlags f /* = 0 */ ) :
    QDialog( parent, f ),
    materialLibTreeList( new MaterialTreeWidget ),
    addButton( 0 ),
    removeButton( 0 ),
    startPageUUID( startPage )
{
    init();
}

void TrussMaterialEditor::init ()
{
    materialLibTreeList->setEditTriggers( QAbstractItemView::NoEditTriggers );

    QHeaderView *header = materialLibTreeList->header();
    header->hide();
    QStringList labels;
    labels << tr("Project/Material");
    materialLibTreeList->setHeaderLabels( labels );

    editWidget = new MaterialEditWidget;

    QPushButton* okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                          SLOT(applyChanges()) );

    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                             SLOT(cancelChanges()) );

    addButton = new QPushButton( tr( "Add" ) );
    connect( addButton, SIGNAL(clicked()),
                        SLOT(addMaterial()) );

    removeButton = new QPushButton( tr( "Remove" ) );
    connect( removeButton, SIGNAL(clicked()),
                           SLOT(removeSelectedMaterial()) );
    
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

    QHBoxLayout* topLayout = new QHBoxLayout;
    topLayout->addLayout( treeLayout );
    topLayout->addLayout( editLayout );

    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addLayout( topLayout );
    mainLayout->addLayout( buttonLayout );

    setWindowTitle( "Material Editor" );
    setFixedSize( 500, 250 );
    setAttribute( Qt::WA_DeleteOnClose );

    connect( materialLibTreeList, SIGNAL(itemSelectionChanged()),
                                    SLOT(fillEditorFields()) );
    
    // Setup context menu
    materialLibTreeList->setContextMenuPolicy( Qt::ActionsContextMenu );

    actAddMaterial = new QAction( tr( "Add material" ), this );
    connect( actAddMaterial, SIGNAL(triggered()), this, SLOT(addMaterial()) );

    actRemoveMaterial = new QAction( tr( "Remove material" ), this );
    connect( actRemoveMaterial, SIGNAL(triggered()), 
                          this, SLOT(removeSelectedMaterial()) );

    materialLibTreeList->addAction( actAddMaterial );
    materialLibTreeList->addAction( actRemoveMaterial );
}

TrussMaterialEditor::~TrussMaterialEditor ()
{
    clearCopyLibs();
}

void TrussMaterialEditor::clearCopyLibs ()
{
    // Delete all material library copies
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );

        ProjectTreeWidgetItem* projItem = 
            dynamic_cast<ProjectTreeWidgetItem*>( item );
        if ( ! projItem )
            return;

        TrussMaterialLibrary* mLib = &projItem->getMaterialLibrary();

        delete mLib;
    }  
}

void TrussMaterialEditor::addProject ( FermaNextProject& proj, 
                                       bool current /* = false */ )
{
    TrussMaterialLibrary& mLib = proj.getMaterialLibrary();
    originalMaterialLibs.insert( &proj, &mLib );

    QDomDocument materialDoc;
    QDomElement xmlProps = mLib.saveToXML( materialDoc );
    materialDoc.appendChild( xmlProps );

    TrussMaterialLibrary* copyLib = new TrussMaterialLibrary();
    copyLib->loadFromXML( xmlProps );

    ProjectTreeWidgetItem* projItem = 
            new ProjectTreeWidgetItem( proj.getName(), *copyLib, current );

    materialLibTreeList->addTopLevelItem( projItem );
    materialLibTreeList->setCurrentItem( projItem );

    for ( int i = 0; i < copyLib->countMaterials(); ++i ) {
        TrussMaterial* m = copyLib->getMaterial( i );
        if ( m )
            materialLibTreeList->addMaterialItem( *m );
    }
}

void TrussMaterialEditor::fillEditorFields ()
{
    QList<QTreeWidgetItem*> selItems = materialLibTreeList->selectedItems();
    if ( selItems.size() != 1 )
        return;

    QTreeWidgetItem* selItem = selItems[0];
    ProjectTreeWidgetItem* projItem = 
        materialLibTreeList->getSelectedProjectItem();

    if ( projItem && projItem == selItem ) {
        editWidget->clearPage();
        removeButton->setEnabled( false );
        actRemoveMaterial->setEnabled( false );
        return;
    }

    removeButton->setEnabled( true );
    actRemoveMaterial->setEnabled( true );
    
    MaterialTreeWidgetItem* materialItem = 
        materialLibTreeList->getSelectedMaterialItem();
    if ( ! materialItem )
        return;

    TrussMaterial& material = 
        const_cast<TrussMaterial&>( materialItem->getItemMaterial() );
    editWidget->setMaterial( material );
}

void TrussMaterialEditor::copyLib ( TrussMaterialLibrary& libFrom, 
                                    TrussMaterialLibrary& libTo )
{
    for ( int i = 0; i < libFrom.countMaterials(); ++i ) {
        TrussMaterial* mFrom = libFrom.getMaterial( i );
        if ( ! mFrom )
            continue;

        QDomDocument materialDoc;
        QDomElement copyProps = mFrom->saveToXML( materialDoc );

        TrussMaterial* mTo = libTo.getMaterial( mFrom->getUUID() );
        if ( ! mTo )
            mTo = &libTo.createMaterial();
        mTo->loadFromXML( copyProps );
    }

    // Take removed materials from the original library
    QList<TrussMaterial*> materialToRemoveList;

    for ( int i = 0; i < libTo.countMaterials(); ++i ) {
        TrussMaterial* mTo = libTo.getMaterial( i );
        if ( ! mTo )
            continue;

        TrussMaterial* mFrom = libFrom.getMaterial( mTo->getUUID() );

        if ( ! mFrom ) 
            materialToRemoveList.append( mTo );            
    }

    foreach ( TrussMaterial* m, materialToRemoveList )
        libTo.removeMaterial( *m );
}

void TrussMaterialEditor::applyChanges ()
{
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );

        ProjectTreeWidgetItem* projItem = 
            dynamic_cast<ProjectTreeWidgetItem*>(item);
        if ( ! projItem )
            return;

        TrussMaterialLibrary& mLib = projItem->getMaterialLibrary();

        QList<TrussMaterialLibrary*> originalMaterialLibList =
            originalMaterialLibs.values();
        foreach ( TrussMaterialLibrary* originalLib, originalMaterialLibList )
            if ( originalLib->getUUID() == mLib.getUUID() )
                copyLib( mLib, *originalLib );
    }  

    MaterialTreeWidgetItem* mItem = 
        materialLibTreeList->getSelectedMaterialItem();
    if ( mItem )
        startPageUUID =  mItem->getItemMaterial().getUUID();

    done( 1 );
}

void TrussMaterialEditor::cancelChanges ()
{
    done( 0 );
}

void TrussMaterialEditor::addMaterial ()
{ 
    ProjectTreeWidgetItem* projItem = 
        materialLibTreeList->getSelectedProjectItem();
    if ( ! projItem )
        return;

    TrussMaterialLibrary& lib = projItem->getMaterialLibrary();

    TrussMaterial& m = lib.createMaterial();

    materialLibTreeList->addMaterialItem( m );
}

void TrussMaterialEditor::removeSelectedMaterial ()
{
    MaterialTreeWidgetItem* materialItem = 
        materialLibTreeList->getSelectedMaterialItem();
    if ( ! materialItem )
        return;
    
    QString name = materialItem->getItemMaterial().getMaterialName();

    if ( QMessageBox::question( this, tr("Material remove confirmation"),
                                tr("Remove material %1?").arg( name ),
                                tr("&Yes"), tr("&No"), QString::null, 0, 1 ) )
        return;
    
    materialLibTreeList->removeItemWithMaterial( *materialItem );
}

void TrussMaterialEditor::exec ()
{
    // Set start page
    for ( int i = 0; i < materialLibTreeList->topLevelItemCount(); ++i ) {
        QTreeWidgetItem* item = materialLibTreeList->topLevelItem( i );

        ProjectTreeWidgetItem* projItem = 
            dynamic_cast<ProjectTreeWidgetItem*>(item);
        if ( ! projItem )
            return;

        TrussMaterialLibrary& mLib = projItem->getMaterialLibrary();
        TrussMaterial* m = mLib.getMaterial( startPageUUID );
        if ( ! m )
            continue;

        MaterialTreeWidgetItem* mItem = 
            materialLibTreeList->getMaterialItem( *m, *projItem );
        if ( mItem )
            materialLibTreeList->setCurrentItem( mItem, 0 );

    } 
    
    fillEditorFields();
    
    QDialog::exec();
}

/***************************************************************************/
