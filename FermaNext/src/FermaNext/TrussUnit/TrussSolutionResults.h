
#ifndef TRUSSSOLUTIONRESULTS_H
#define TRUSSSOLUTIONRESULTS_H

#include <QList>
#include <QObject>
#include <XMLSerializableObject.h>

#include "Geometry.h"

class Plugin;
class TrussUnit;
class TrussUnitCopy;
class TrussUnitWindowManager;

/*****************************************************************************/

class LoadCaseResults : public XMLSerializableObject
{
public:
    LoadCaseResults ();
    ~LoadCaseResults ();

    // Numbers are not equal to indexes, so be careful 
    // and take note of the names of corresponding variables.
    // They are usually named "indx" and "numb" accordingly.
    // indx = numb - 1
    DoublePoint getDisplacement ( int indx, bool& valid ) const;
    double getStress ( int indx, bool& valid ) const;
    double getRequiredThickness ( int indx, bool& valid ) const;
    double getSafetyMargin ( int indx, bool& valid ) const;
    int getLoadCaseNumber () const;
    
    int countDisplacements () const;
    int countStresses () const;

    // XML serialization
    void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    QDomElement saveToXML ( QDomDocument& );
    // Can be used for loading results data in XML, but 
    // without uuid if flag uuidLoad wasn't set
    void loadAttributesFromXML ( const QDomElement&, bool uuidLoad ) 
                                            /*throw (LoadException)*/;
                                            
private:

    class NodeResults : public XMLSerializableObject
    {
    public:
        NodeResults ( double dispX_, double dispY_, int numb ) :
            dispX( dispX_ ), 
            dispY( dispY_ ), 
            nodeNumb( numb )
        {}
        // XML serialization
        void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
        QDomElement saveToXML ( QDomDocument& );
        // Can be used for loading results data in XML, but 
        // without uuid if flag uuidLoad wasn't set
        void loadAttributesFromXML ( const QDomElement&, bool uuidLoad ) 
                                                /*throw (LoadException)*/;

        double dispX, dispY;
        int nodeNumb;
    };

    class PivotResults : public XMLSerializableObject
    {
    public:
        PivotResults ( double stress_, double thick_, double margin_, int numb ) :
            stress( stress_ ), 
            requiredThick( thick_ ), 
            safetyMargin( margin_ ),
            pivotNumb( numb )
        {}

        // XML serialization
        void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
        QDomElement saveToXML ( QDomDocument& );
        // Can be used for loading results data in XML, but 
        // without uuid if flag uuidLoad wasn't set
        void loadAttributesFromXML ( const QDomElement&, bool uuidLoad ) 
                                                /*throw (LoadException)*/;

        double stress, requiredThick, safetyMargin;
        int pivotNumb;
    };

    typedef QList<NodeResults*> NodeResultsList;
    typedef NodeResultsList::iterator NodeResultsListIter;

    typedef QList<PivotResults*> PivotResultsList;
    typedef PivotResultsList::iterator PivotResultsListIter;

protected:
    NodeResults& addNodeResults ( double dispX, double dispY, int numb );
    PivotResults& addPivotResults ( double stress, double reqThick, 
                                    double sMargin, int numb );
    void setLoadCaseNumber ( int numb );
    void clean ();

private:
    NodeResultsList nodeResultsList;
    PivotResultsList pivotResultsList;
    int loadCaseNumb;
};

/*****************************************************************************/

class PluginResults : public XMLSerializableObject
{
public:
    typedef QList<LoadCaseResults*> LoadCaseResultsList;
    typedef QList<LoadCaseResults*>::iterator LoadCaseResultsListIter;

    PluginResults ();
    ~PluginResults ();
    
    const LoadCaseResults* getLoadCaseResults ( int numb ) const;
    const QString& getPluginName () const;
    double getForceWeight () const;
    double getMaterialVolume () const;
    double getMaxTensionStress () const;
    double getTrussMass () const;

    int countLoadCaseResults () const;

    // XML serialization
    void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    // Can be used for loading results data in XML, but 
    // without uuid if flag uuidLoad wasn't set
    void loadAttributesFromXML ( const QDomElement&, bool uuidLoad ) 
                                            /*throw (LoadException)*/;
    QDomElement saveToXML ( QDomDocument& );

protected:
    LoadCaseResults& createLoadCaseResults ();
    void setPluginName ( const QString& );
    void setForceWeight ( double );
    void setMaterialVolume ( double );
    void setMaxTensionStress ( double );
    void setTrussMass ( double );
    void clean ();

private:
    LoadCaseResultsList loadCaseResults;
    QString pluginName;
    double forceWeight, materialVolume, maxTension, trussMass;
};

/*****************************************************************************/

class TrussSolutionResults : public QObject, 
                             public XMLSerializableObject
{
    Q_OBJECT
public:
    typedef QList<PluginResults*> PluginResultsList;
    typedef PluginResultsList::iterator PluginResultsListIter;
    typedef PluginResultsList::const_iterator PluginResultsListConstIter;

    TrussSolutionResults ( const TrussUnitWindowManager& );
    ~TrussSolutionResults ();

    PluginResults& createPluginResults ();
    const PluginResults* getPluginResults ( int indx ) const;

    const QString& getTrussUnitUUID () const;
    TrussUnitCopy& getTrussUnitCopy () const;
    void setTrussUnit ( const QString& trussUnitUUID );    

    int countResults () const;
    
    // XML serialization
    void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    QDomElement saveToXML ( QDomDocument& );

protected:
    void clean ();

private:
    const TrussUnitWindowManager& windowMng;
    PluginResultsList pluginResults;
    QString trussUUID;
    TrussUnitCopy* trussCopy;
};  

#endif // TRUSSSOLUTIONRESULTS_H
