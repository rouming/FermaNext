
#ifndef TRUSSMATERIAL_H
#define TRUSSMATERIAL_H

#include <QString>
#include <QList>
#include <QMap>
#include <QObject>
#include <QMetaType>

#include "XMLSerializableObject.h"

class TrussMaterial : public XMLSerializableObject
{
public:
    TrussMaterial ();
    TrussMaterial ( const QString&,
                    double workingStress,
                    double elasticityModule,
                    double density );
    TrussMaterial& operator= ( const TrussMaterial& );

    virtual ~TrussMaterial ();

    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    virtual void setMaterialName ( const QString& );
    virtual void setWorkingStress ( double );
    virtual void setElasticityModule ( double );
    virtual void setDensity ( double );

    virtual const QString& getMaterialName () const;
    virtual double getWorkingStress () const;
    virtual double getElasticityModule () const;
    virtual double getDensity () const;
    
private:
    QString materialName;
    double workingStress;
    double elasticityModule;
    double density;
};

/*****************************************************************************/

class TrussMaterialLibrary : public QObject,
                             public XMLSerializableObject
{
    Q_OBJECT
public:
    // Material library exceptions
    class WrongMaterialNameException {};
    class WrongCharacteristicException {};

    typedef QMap<QString, TrussMaterial*> TrussMaterialUUIDMap;

    TrussMaterialLibrary ();
    virtual ~TrussMaterialLibrary ();

    virtual TrussMaterial& createMaterial ( const QString&, 
                                            double, double, double ); 
                                            /*throw (WrongMaterialNameException, 
                                              WrongCharacteristicException)*/

    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    virtual bool removeMaterial ( TrussMaterial& );
    virtual bool removeMaterial ( int indx );
    
    virtual TrussMaterial* getMaterial ( int indx ) const;
    virtual TrussMaterial* getMaterial ( const QString& name ) const;

    virtual int countMaterials () const;

    virtual const TrussMaterialUUIDMap& getMaterialUUIDMap () const;

protected:
    virtual void clean ();

signals:
    void onAfterMaterialCreation ( TrussMaterial& );
    void onAfterMaterialRemoval ();
    
private:
    typedef QList<TrussMaterial*> TrussMaterialList;
    typedef TrussMaterialList::iterator TrussMaterialListIter;
    typedef TrussMaterialList::const_iterator TrussMaterialListConstIter;

    TrussMaterialList materials;
    // Tie UUID with truss materials to link materials with pivots
    TrussMaterialUUIDMap materialUUIDMap;
};

/************************* Declare meta types ********************************/

Q_DECLARE_METATYPE (TrussMaterial);
Q_DECLARE_METATYPE (const TrussMaterial*);

#endif //TRUSSMATERIAL_H
