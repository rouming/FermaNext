
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <QString>

#include "XMLSerializableObject.h"
#include "TrussDesignerWidget.h"
#include "TrussUnitWindowManager.h"
#include "ObjectStateManager.h"
// FIXME QT3TO4:
//#include "CalcDataToolBar.h"

class FermaNextWorkspace;
class TrussSolutionResults;
class TrussUnitWindow;
class TrussUnitCopy;
class QStackedWidget;
class QTabWidget;
class QFrame;

class FermaNextProject : public QObject, public XMLSerializableObject
{
    Q_OBJECT
public:
    // File format extension
    static const QString& formatExtension ();

public:
    // Exceptions
    class IOException {};
    class WrongXMLDocException {};
    class FileNameIsNotDefinedException {};

    virtual ~FermaNextProject ();

    virtual void loadFromFile ( const QString& ) /*throw (IOException, 
                                                          WrongXMLDocException,
                                                          LoadException)*/;
    virtual void saveToFile () /*throw (FileNameIsNotDefinedException, 
                                        IOException)*/;
    virtual void saveToFile ( const QString& ) /*throw (IOException)*/;
    
    virtual const QString& getProjectFileName () const;
    virtual bool isFileNameDefined () const;

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual void activate ();
    virtual bool isActivated () const;

    virtual TrussUnitWindowManager& getTrussUnitWindowManager ();    
    virtual TrussDesignerWidget& getDesignerWidget ();
// FIXME QT3TO4:
//    virtual CalcDataToolBar& getCalcDataToolBar ();
    virtual FermaNextWorkspace& getWorkspace();

    virtual TrussMaterialLibrary& getMaterialLibrary () const;

    virtual TrussSolutionResults* getResultsForTrussUnit ( 
                                         const TrussUnit& ) const;

protected:
    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) /*throw (LoadException)*/;
    virtual QDomElement saveToXML ( QDomDocument& );

    virtual void setProjectFileName ( const QString& );

public slots:
    virtual void addSolutionResults ( TrussSolutionResults& );
    virtual void removeSolutionResults ( TrussUnitWindow& );

signals:
    void onActivate ( FermaNextProject& );
    void onNameChange ( const QString& );
    void onProjectFileNameChange ( const QString& );

    void tabWasChanged ( int );
    void afterTrussCreation ();
    void afterTrussRemoval ();
  
private:
    friend class FermaNextWorkspace;
    
    FermaNextProject ( FermaNextWorkspace&, 
                       const QString& name, 
                       QStackedWidget* parent = 0 );

private:
    typedef QList<TrussSolutionResults*> TrussResultsList;
    typedef TrussResultsList::const_iterator TrussResultsListConstIter;

    FermaNextWorkspace& currentWorkspace;
    QString name;
    QString projectFileName;
    QStackedWidget* stackedWidget;
// FIXME QT3TO4:
//    CalcDataToolBar* calcDataToolBar;
    QFrame* projectWidget;
    TrussDesignerWidget* designerWidget;
    TrussMaterialLibrary* materialLibrary;
    TrussUnitWindowManager* trussWindowManager;
    TrussResultsList trussResults;
};

#endif //FERMANEXTPROJECT_H
