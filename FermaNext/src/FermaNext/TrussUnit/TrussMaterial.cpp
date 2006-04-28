
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
    throw (LoadException)
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

    double elast = materialElem.attribute( "elasticityModule" ).toDouble( &ok );
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
    materialName = name;
}

void TrussMaterial::setWorkingStress ( double ws )
{
    workingStress = ws;
}

void TrussMaterial::setElasticityModule ( double em )
{
    elasticityModule = em;    
}

void TrussMaterial::setDensity ( double d )
{
    density = d;    
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

TrussMaterialLibrary::TrussMaterialLibrary ()
{
    createMaterial( tr( "Aluminum Alloy" ), 30000, 7000000, 0.028 );
    createMaterial( tr( "Steel" ), 70000, 20000000, 0.078 );
}

TrussMaterialLibrary::~TrussMaterialLibrary ()
{
    clean();
}

void TrussMaterialLibrary::loadFromXML ( const QDomElement& projElem ) 
    throw (LoadException)
{
    XMLSerializableObject::loadFromXML( projElem );

    clean();

    /** 
     * Create materials
     ********************/
    QDomNodeList trussMaterials = projElem.elementsByTagName( "TrussMaterial" );
    for ( int i = 0; i < trussMaterials.count(); ++i ) {
        QDomNode material = trussMaterials.item( i );
        if ( ! material.isElement() )
            throw LoadException();
        QDomElement materialElem = material.toElement();
        TrussMaterial& newMaterial = createMaterial( "new", 1, 1, 1 );
        newMaterial.loadFromXML( materialElem );
        materialUUIDMap[newMaterial.getUUID()] = &newMaterial;
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

TrussMaterial& TrussMaterialLibrary::createMaterial ( const QString& name, 
                                                      double s, double e,
                                                      double d ) /*throw 
                                                 (WrongMaterialNameException, 
                                                  WrongCharacteristicException)*/
{
    TrussMaterial* material = 0;
    TrussMaterialListConstIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter ) {
        material = *iter;
        if ( material->getMaterialName() == name )
            throw WrongMaterialNameException();
    }
    
    if ( s <= 0 || e <= 0 || d <= 0 )
        throw WrongCharacteristicException();
    
    material = new TrussMaterial( name, s, e, d );
    materials.push_back( material );
    emit onAfterMaterialCreation( *material );
    return *material;
}

bool TrussMaterialLibrary::removeMaterial ( TrussMaterial& material )
{
    TrussMaterialListIter iter = std::find( materials.begin(),
                                            materials.end(),
                                            &material );
    if ( iter == materials.end() )
        return false;

    emit onAfterMaterialRemoval();
    return true;
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

TrussMaterial* TrussMaterialLibrary::getMaterial ( const QString& name ) const
{
    TrussMaterialListConstIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter ) {
        TrussMaterial* material = *iter;
        if ( material->getMaterialName() == name )
            return material;
    }
    return 0;
}

int TrussMaterialLibrary::countMaterials () const
{
    return materials.size();
}

const QMap<QString, TrussMaterial*>& 
                            TrussMaterialLibrary::getMaterialUUIDMap () const
{
    return materialUUIDMap;
}

void TrussMaterialLibrary::clean ()
{
    TrussMaterialListIter iter = materials.begin();
    for ( ; iter != materials.end(); ++iter )
        materials.erase(iter);
    materials.clear();
}

/****************************************************************************/
