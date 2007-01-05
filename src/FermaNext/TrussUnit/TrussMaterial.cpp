
#include <algorithm>

#include "TrussMaterial.h"

/*****************************************************************************
 * Truss Material
 *****************************************************************************/

TrussMaterial::TrussMaterial () :
    workingStress(0),
    elasticityModule(0)
{}

TrussMaterial::TrussMaterial ( const QString& name,
                               double workingStress_,
                               double elasticityModule_,
                               double density_ ) :
    materialName(name),
    workingStress(workingStress_),
    elasticityModule(elasticityModule_),
    density(density_)
{}

TrussMaterial::TrussMaterial ( const TrussMaterial& mat ) :
    QObject(),
    XMLSerializableObject(),
    materialName( mat.materialName ),
    workingStress( mat.workingStress ),
    elasticityModule( mat.elasticityModule ),
    density( mat.density )
{}

TrussMaterial& TrussMaterial::operator= ( const TrussMaterial& mat )
{
    materialName = mat.materialName;
    workingStress = mat.workingStress;
    elasticityModule = mat.elasticityModule;
    return *this;
}

TrussMaterial::~TrussMaterial ()
{}

void TrussMaterial::loadFromXML ( const QDomElement& materialElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( materialElem );

    /** 
     * Set name
     **************/
    if ( ! materialElem.hasAttribute( "name" ) )
        throw LoadException();

    QString name = materialElem.attribute( "name" );
    setMaterialName( name );

    /** 
     * Set working stress
     ***************************/
    if ( ! materialElem.hasAttribute( "workingStress" ) )
        throw LoadException();

    bool ok;
    double stress = materialElem.attribute( "workingStress" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    setWorkingStress( stress );

    /** 
     * Set elasticity module
     ***************************/
    if ( ! materialElem.hasAttribute( "elasticityModule" ) )
        throw LoadException();

    double elast = 
        materialElem.attribute( "elasticityModule" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    setElasticityModule( elast );

    /** 
     * Set density
     ******************/
    if ( ! materialElem.hasAttribute( "density" ) )
        throw LoadException();

    double dens = materialElem.attribute( "density" ).toDouble( &ok );
    if ( !ok ) throw LoadException();

    setDensity( dens );
}

QDomElement TrussMaterial::saveToXML ( QDomDocument& doc )
{
    QDomElement materialElem = XMLSerializableObject::saveToXML( doc );
    materialElem.setTagName( "TrussMaterial" );

    /** 
     * Save name
     **************/
    materialElem.setAttribute( "name", materialName );

    /** 
     * Save working stress
     ***************************/
    materialElem.setAttribute( "workingStress", workingStress );

    /** 
     * Save elasticity module
     ***************************/
    materialElem.setAttribute( "elasticityModule", elasticityModule );

    /** 
     * Save density
     ******************/
    materialElem.setAttribute( "density", density );    

    return materialElem;
}

void TrussMaterial::setMaterialName ( const QString& name )
{
    emit onBeforeNameChange( name );
    materialName = name;
    emit onAfterNameChange( name );
}

void TrussMaterial::setWorkingStress ( double ws )
{
    emit onBeforeWorkingStressChange( ws );
    workingStress = ws;
    emit onAfterWorkingStressChange( ws );
}

void TrussMaterial::setElasticityModule ( double em )
{
    emit onBeforeElasticityModuleChange( em );
    elasticityModule = em;   
    emit onAfterElasticityModuleChange( em );
}

void TrussMaterial::setDensity ( double d )
{
    emit onBeforeDensityChange( d );
    density = d;
    emit onAfterDensityChange( d );
}

const QString& TrussMaterial::getMaterialName () const
{
    return materialName;
}

double TrussMaterial::getWorkingStress () const
{
    return workingStress;
}

double TrussMaterial::getElasticityModule () const
{
    return elasticityModule;
}

double TrussMaterial::getDensity () const
{
    return density;
}

/*****************************************************************************
 * Truss Material Library
 *****************************************************************************/

TrussMaterialLibrary::TrussMaterialLibrary () :
    selectedMaterial( 0 )
{}

TrussMaterialLibrary::~TrussMaterialLibrary ()
{
    clean();
}

void TrussMaterialLibrary::loadFromXML ( const QDomElement& projElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( projElem );

    clean();

    /** 
     * Create materials
     ********************/
    QDomNodeList trussMaterials = 
        projElem.elementsByTagName( "TrussMaterial" );
    for ( int i = 0; i < trussMaterials.count(); ++i ) {
        QDomNode material = trussMaterials.item( i );
        if ( ! material.isElement() )
            throw LoadException();
        QDomElement materialElem = material.toElement();
        TrussMaterial& newMaterial = createMaterial( 0, 0, 0, "" );
        newMaterial.loadFromXML( materialElem );
    }
}

QDomElement TrussMaterialLibrary::saveToXML ( QDomDocument& doc )
{
    QDomElement materialLibElem = XMLSerializableObject::saveToXML( doc );
    materialLibElem.setTagName( "TrussMaterialLibrary" );

    /** 
     * Save material
     *****************/
    TrussMaterialListIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter )
        materialLibElem.appendChild( (*iter)->saveToXML( doc ) );

    return materialLibElem;
}

TrussMaterial& TrussMaterialLibrary::createMaterial ( double ws, 
                                                      double em,
                                                      double d,
                                                      const QString& name )
{
    QString mName = name;
    if ( mName.isNull() )
        mName = getDefaultMaterialName( "Unnamed material" );

    TrussMaterial* material = new TrussMaterial( mName, ws, em, d );
    materials.push_back( material );

    emit onAfterMaterialCreation( *material );

    return *material;
}

TrussMaterial& TrussMaterialLibrary::createMaterial ( const QString& xmlProps )
{
    // Create empty material
    TrussMaterial& material = createMaterial( 0, 0, 0, "" );

    // Save material UUID 
    QString mUUID = material.getUUID();

    QDomDocument doc;
    if ( ! doc.setContent( xmlProps ) )
        return material;

    QDomNodeList nodeList = doc.elementsByTagName( "TrussMaterial" );
    if ( nodeList.isEmpty() )
        return material;
    
    QDomElement materialElem = nodeList.at( 0 ).toElement();
    if ( materialElem.isNull() )
        return material;

    material.loadFromXML( materialElem );

    // Restore UUID
    material.setUUID( mUUID );

    return material;
}

bool TrussMaterialLibrary::removeMaterial ( TrussMaterial& material )
{
    TrussMaterialListIter iter = std::find( materials.begin(),
                                            materials.end(),
                                            &material );
    if ( iter == materials.end() )
        return false;

    TrussMaterial* m = *iter;
    emit onBeforeMaterialRemoval( *m );
    materials.erase( iter );
    delete m;
    emit onAfterMaterialRemoval();

    return true;
}

bool TrussMaterialLibrary::removeMaterial ( const QString& uuid )
{
    TrussMaterialListConstIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter ) {
        TrussMaterial* material = *iter;
        if ( material->getUUID() == uuid )
            return removeMaterial( *material );
    }
    return false;
}

bool TrussMaterialLibrary::removeMaterial ( int indx )
{
    if ( indx > materials.size() )
        return false;

    try {
        return removeMaterial( *materials.at(indx) );
    }
    catch ( ... ) {
        return false;
    }
}

TrussMaterial* TrussMaterialLibrary::getMaterial ( int indx ) const
{
    if ( indx > materials.size() )
        return 0;

    try {
        return materials.at(indx);
    }
    catch ( ... ) {
        return 0;
    }
}

TrussMaterial* TrussMaterialLibrary::getMaterial ( const QString& uuid ) const
{
    TrussMaterialListConstIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter ) {
        TrussMaterial* material = *iter;
        if ( material->getUUID() == uuid )
            return material;
    }
    return 0;
}

QString TrussMaterialLibrary::getMaterialXml ( const TrussMaterial& material )
{
    QDomDocument doc;
    TrussMaterial& m = const_cast<TrussMaterial&>( material );
    QDomElement mProps = m.saveToXML( doc );
    doc.appendChild( mProps );
    return doc.toString();
}

int TrussMaterialLibrary::countMaterials () const
{
    return materials.size();
}

void TrussMaterialLibrary::selectMaterial ( const TrussMaterial& m )
{
    selectedMaterial = &m;
}

void TrussMaterialLibrary::selectMaterial ( int idx )
{
    selectedMaterial = getMaterial( idx );
}

void TrussMaterialLibrary::selectMaterial ( const QString& uuid )
{
    selectedMaterial = getMaterial( uuid );
}

const TrussMaterial* TrussMaterialLibrary::getSelectedMaterial () const
{
    return selectedMaterial;
}

TrussMaterial* TrussMaterialLibrary::getMaterialWithSameProperties ( 
                                              const TrussMaterial& m ) const
{
    foreach ( TrussMaterial* material, materials )
        if ( material->getMaterialName() == m.getMaterialName() && 
             material->getWorkingStress() == m.getWorkingStress() &&
             material->getElasticityModule() == m.getElasticityModule() &&
             material->getDensity() == m.getDensity() )
            return material;  
    return 0;
}

TrussMaterial* TrussMaterialLibrary::getMaterialWithSameProperties ( 
                                          const QString& xmlProps ) const
{
    QDomDocument doc;
    if ( ! doc.setContent( xmlProps ) )
        return false;

    QDomNodeList nodeList = doc.elementsByTagName( "TrussMaterial" );
    if ( nodeList.isEmpty() )
        return false; 
    
    QDomElement materialElem = nodeList.at( 0 ).toElement();
    if ( materialElem.isNull() )
        return false;

    // Create fake material to compare with the library materials
    TrussMaterial* fakeMaterial = new TrussMaterial();
    fakeMaterial->loadFromXML( materialElem );

    TrussMaterial* sameMaterial = getMaterialWithSameProperties( *fakeMaterial );

    delete fakeMaterial;

    return sameMaterial;
}

QString TrussMaterialLibrary::getDefaultMaterialName ( 
                                                const QString& nameBasis, 
                                                uint startIdx /* = 0 */ ) const
{
    QString defaultName = nameBasis;
    foreach ( TrussMaterial* material, materials ) {
        if ( material->getMaterialName() == defaultName ) {
            if ( startIdx != 0 ) {

                QString numb = QString::number( startIdx );

                // should remove previous index
                if ( startIdx > 1 )
                    defaultName.chop( 3 + numb.size() );

                defaultName = defaultName + " (" + numb + ")";
            }
            
            defaultName = getDefaultMaterialName( defaultName, ++startIdx );
        }
    }

    return defaultName;
}

void TrussMaterialLibrary::clean ()
{
    TrussMaterialListIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter )
        removeMaterial( **iter );
    materials.clear();
}

/****************************************************************************/
