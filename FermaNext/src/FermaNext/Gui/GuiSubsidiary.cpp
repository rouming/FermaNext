
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
    materialLib = &mLib;
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

void MaterialComboBox::setCurrentMaterial( const TrussMaterial& m )
{
    int indx = findText( m.getMaterialName() );
    if ( indx != -1 )
        setCurrentIndex( indx );      
}

void MaterialComboBox::setArgList ()
{
    if ( ! materialLib )
        return;

    QStringList argList;
    for ( int i = 0; i < materialLib->countMaterials(); ++i ) {
        TrussMaterial* m = materialLib->getMaterial( i );
        if ( m )
            argList.append( m->getMaterialName() );
    }
    
    addItems( argList );
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

/***************************************************************************/
