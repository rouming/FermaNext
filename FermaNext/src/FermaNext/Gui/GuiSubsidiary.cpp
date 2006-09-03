
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

const TrussMaterial* MaterialComboBox::getCurrentMaterial () const
{
    QString materialName = currentText();
    TrussMaterial* m = materialLib->getMaterial( materialName );
    if ( ! m )
        return 0;
    return m;
}

void MaterialComboBox::setCurrentMaterial( const QString& name )
{
    if ( ! materialLib )
        return;

    int indx = findText( name );
    setCurrentIndex( indx );
}

void MaterialComboBox::setCurrentMaterial( const TrussMaterial& m )
{
    setCurrentMaterial( m.getMaterialName() );
}

void MaterialComboBox::setArgList ()
{
    if ( ! materialLib )
        return;

    clear();
    QStringList argList;
    for ( int i = 0; i < materialLib->countMaterials(); ++i ) {
        TrussMaterial* m = materialLib->getMaterial( i );
        if ( m )
            addMaterialItem( *m );
    }
    
    addItems( argList );
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
    const TrussMaterialLibrary* lib = 
        dynamic_cast<const TrussMaterialLibrary*>(sender());

    if ( materialLib != lib )
        return;

    int indx = findText( m.getMaterialName() );
    removeItem( indx );

    if ( ! count() )
        emit comboBoxIsEmpty ( true );
}

void MaterialComboBox::updateMaterialItemName( const QString& name )
{
    for ( int i = 0; i < count(); ++i ) {
        QVariant material = itemData( i );
        Q_ASSERT( qVariantCanConvert<const TrussMaterial*>(material) );
        const TrussMaterial* m = qVariantValue<const TrussMaterial*>(material);
        const TrussMaterial* senderMaterial = 
            dynamic_cast<const TrussMaterial*>(sender());
        if ( m == senderMaterial )
            setItemText( i, name );
    }
}

void MaterialComboBox::clearMaterialLibrary ()
{
    materialLib = 0;
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
