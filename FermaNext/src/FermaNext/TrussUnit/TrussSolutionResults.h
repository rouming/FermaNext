
#ifndef TRUSSSOLUTIONRESULTS_H
#define TRUSSSOLUTIONRESULTS_H

#include <QList>
#include <QObject>
#include <XMLSerializableObject.h>

#include "Geometry.h"

class Plugin;
class TrussUnit;
class TrussUnitCopy;

/*****************************************************************************/

class LoadCaseResults : public XMLSerializableObject
{
public:
    typedef QList<DoublePoint*> DispList;
    typedef QList<DoublePoint*>::iterator DispListIter;
    typedef QList<double> StressList;
    typedef QList<double>::iterator StressListIter;
    
    LoadCaseResults ( int );
    ~LoadCaseResults ();

    void addDisplacement ( double, double, int );
    DoublePoint getDisplacement ( int, bool& ) const;
    void removeDisplacement ( int );

    void addStress ( double, int );
    double getStress ( int, bool& ) const;
    void removeStress ( int );

    void setForceWeight ( double );
    double getForceWeight () const;

    int countDisplacements () const;
    int countStresses () const;

    void clean ();

    int getLoadCaseNumber () const;

    // XML serialization
    void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    QDomElement saveToXML ( QDomDocument& );

private:
    DispList displacements;
    StressList stresses;
    double forceWeight;
    int loadCaseNumb;
};

/*****************************************************************************/

class PluginResults : public XMLSerializableObject
{
public:
    typedef QList<LoadCaseResults*> LoadCaseResultsList;
    typedef QList<LoadCaseResults*>::iterator LoadCaseResultsListIter;

    PluginResults ( const QString& );
    ~PluginResults ();
    
    void addLoadCaseResults ( LoadCaseResults& );
    const LoadCaseResults* getLoadCaseResults ( int numb ) const;
    int countLoadCaseResults () const;
    void clean ();

    const QString& getPluginName () const;

    // XML serialization
    void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    QDomElement saveToXML ( QDomDocument& );

private:
    LoadCaseResultsList loadCaseResults;
    QString pluginName;
};

/*****************************************************************************/

class TrussSolutionResults : public QObject, 
                             public XMLSerializableObject
{
    Q_OBJECT
public:
    typedef QList<const PluginResults*> PluginResultsList;
    typedef PluginResultsList::iterator PluginResultsListIter;
    typedef PluginResultsList::const_iterator PluginResultsListConstIter;

    TrussSolutionResults ( const TrussUnit& );
    ~TrussSolutionResults ();

    void addPluginResults ( const PluginResults& );

    const TrussUnit& getTrussUnit () const;
    TrussUnitCopy& getTrussUnitCopy () const;
    const PluginResults* getPluginResults ( int indx ) const;
    int countResults () const;

    // XML serialization
    void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    QDomElement saveToXML ( QDomDocument& );

protected:
    void clean ();

private:
    PluginResultsList pluginResults;
    TrussUnitCopy* trussCopy;
    const TrussUnit& trussUnit;
};  

#endif // TRUSSSOLUTIONRESULTS_H
