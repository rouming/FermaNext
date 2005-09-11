
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
                               double elasticityModule_ ) :
    materialName(name),
    workingStress(workingStress_),
    elasticityModule(elasticityModule_)
{}

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

/****************************************************************************/
