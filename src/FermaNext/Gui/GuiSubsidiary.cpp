
#include <QString>

#include "GuiSubsidiary.h"
#include "TrussMaterial.h"

/*****************************************************************************
 * Material Combo box
 *****************************************************************************/

MaterialComboBox::MaterialComboBox ( QWidget* parent,
                                     const TrussMaterialLibrary* mLib ) :
    QComboBox ( parent ),
    materialLib( mLib )
{
    setArgList();
}

const TrussMaterialLibrary& MaterialComboBox::getMaterialLibrary () const
{
    return *materialLib;
}

void MaterialComboBox::setMaterialLibrary ( const TrussMaterialLibrary& mLib )
{
    if ( materialLib ) {
        disconnect( materialLib, 
                    SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                    this, SLOT(addMaterialItem(const TrussMaterial&)) );    
        disconnect( materialLib, 
                    SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                    this, SLOT(removeMaterialItem(const TrussMaterial&)) );  
    }
  
    materialLib = &mLib;
    connect( materialLib, SIGNAL(onAfterMaterialCreation(const TrussMaterial&)),
                          SLOT(addMaterialItem(const TrussMaterial&)) );    
    connect( materialLib, SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)),
                          SLOT(removeMaterialItem(const TrussMaterial&)) );
    setArgList();
}

const TrussMaterial* MaterialComboBox::getMaterialByIndex ( int idx ) const
{
    if ( idx < 0 || idx >= count() )
        return 0;
    
    QVariant material = itemData( idx );
    Q_ASSERT( qVariantCanConvert<const TrussMaterial*>(material) );
    const TrussMaterial* m = qVariantValue<const TrussMaterial*>(material);
    if ( ! m )
        return 0;
    return m;
}

const TrussMaterial* MaterialComboBox::getCurrentMaterial () const
{
    return getMaterialByIndex( currentIndex() );
}

void MaterialComboBox::setCurrentMaterial ( const TrussMaterial& m )
{
    int idx = getMaterialIndex( m );
    if ( idx >= 0 )
        setCurrentIndex( idx );
}

int MaterialComboBox::getMaterialIndex ( const TrussMaterial& material ) const
{
    for ( int i = 0; i < count(); ++i ) {
        const TrussMaterial* m = getMaterialByIndex( i );
        if ( m == &material )
            return i;
    }
    return -1;
}

void MaterialComboBox::setArgList ()
{
    if ( ! materialLib )
        return;

    clear();

    for ( int i = 0; i < materialLib->countMaterials(); ++i ) {
        TrussMaterial* m = materialLib->getMaterial( i );
        if ( m )
            addMaterialItem( *m );
    }
}

void MaterialComboBox::addMaterialItem ( const TrussMaterial& m )
{
    const TrussMaterialLibrary* lib = 
        dynamic_cast<const TrussMaterialLibrary*>(sender());

    if ( sender() && materialLib != lib )
        return;

    QVariant material;
    qVariantSetValue<const TrussMaterial*>( material, &m );
    addItem( m.getMaterialName(), material );

    connect( &m, SIGNAL(onAfterNameChange(const QString&)),
                 SLOT(updateMaterialItemName(const QString&)) );

    if ( count() == 1 )
        emit comboBoxIsEmpty ( false );
}

void MaterialComboBox::removeMaterialItem ( const TrussMaterial& m )
{
    int idx = getMaterialIndex( m );
    if ( idx >= 0 )
        removeItem( idx );

    if ( ! count() )
        emit comboBoxIsEmpty ( true );
}

void MaterialComboBox::updateMaterialItemName( const QString& name )
{
    const TrussMaterial* senderMaterial = 
        dynamic_cast<const TrussMaterial*>( sender() );

    if ( ! senderMaterial )
        return;
    
    int idx = getMaterialIndex( *senderMaterial );
    if ( idx >= 0 )
        setItemText( idx, name );
}

void MaterialComboBox::clearMaterialLibrary ()
{
    materialLib = 0;
}

/*****************************************************************************
 * Material Model
 *****************************************************************************/

MaterialModel::MaterialModel ( const TrussMaterialLibrary& lib )
{
	materialLib = &lib;
}

int MaterialModel::rowCount ( const QModelIndex& idx ) const
{
	if( ! idx.isValid() )
		return materialLib->countMaterials();
	return 0;
}

int MaterialModel::columnCount ( const QModelIndex& ) const
{
	return 1;
}

QVariant MaterialModel::data ( const QModelIndex &idx, int role ) const
{
    if ( ! idx.isValid( ))
        return QVariant();

    TrussMaterial* m = materialLib->getMaterial( idx.row() );

    if ( role == Qt::DisplayRole )
		return m->getMaterialName();

    if ( role == MaterialDataId::Elasticity )
		return m->getElasticityModule();

    if ( role == MaterialDataId::WorkingStress )
		return m->getWorkingStress();

    if ( role == MaterialDataId::Density )
		return m->getDensity();

    if ( role == MaterialDataId::Uuid )
		return m->getUUID();

/*
	if( role == Qt::DecorationRole )
		return m->getColor();
*/
    
    return QVariant();
}

void MaterialModel::synchronize()
{
	beginInsertRows(QModelIndex(), 0, 0);
	endInsertRows();
}

void MaterialModel::reset ()
{
    QAbstractTableModel::reset();
}


/*****************************************************************************
 * Range Validator
 *****************************************************************************/

RangeValidator::RangeValidator ( QObject* parent ) :
    QDoubleValidator( parent )
{}

RangeValidator::RangeValidator ( double bottom, double top, int decimals, 
                                  QObject* parent ) :
    QDoubleValidator( bottom, top, decimals, parent )
{}   
    
QValidator::State RangeValidator::validate ( QString& input, int& pos ) const
{
    State s = QDoubleValidator::validate( input, pos );
    if ( s == Intermediate && input == "-" )
        return Intermediate;
    if ( s != Acceptable )
       return Invalid;
    return s;
}

/*****************************************************************************
 * Docked Widget
 *****************************************************************************/

DockedWidget::DockedWidget( const QString& title, 
                            QWidget* parent /* = 0 */, 
                            Qt::WFlags flags /* = 0 */ ) :
    QDockWidget( title, parent, flags )
{}

QSize DockedWidget::sizeHint () const
{
    QSize size = QDockWidget::sizeHint(); 
    size.setWidth( 145 ); 
    return size;
}

/*****************************************************************************
 * Table Header
 *****************************************************************************/

TableHeader::TableHeader ( Qt::Orientation orientation, 
                           QWidget * parent /* = 0 */ ) :
    QHeaderView( orientation, parent )
{}

QSize TableHeader::sizeHint () const
{
    return QSize( 20, 20 );
}

/***************************************************************************/
