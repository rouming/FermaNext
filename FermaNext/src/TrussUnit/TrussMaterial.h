
#ifndef TRUSSMATERIAL_H
#define TRUSSMATERIAL_H

#include <qstring.h>

class TrussMaterial 
{
public:
    TrussMaterial ();
    TrussMaterial ( const QString&,
                    double workingStress,
                    double elasticityModule );

    virtual ~TrussMaterial ();

    virtual void setMaterialName ( const QString& );
    virtual void setWorkingStress ( double );
    virtual void setElasticityModule ( double );

    virtual const QString& getMaterialName () const;
    virtual double getWorkingStress () const;
    virtual double getElasticityModule () const;
    
private:
    QString materialName;
    double workingStress;
    double elasticityModule;
};

#endif //TRUSSMATERIAL_H
