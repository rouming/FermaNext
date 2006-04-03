
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <QString>

#include "XMLSerializableObject.h"
#include "TrussDesignerWidget.h"
#include "TrussUnitWindowManager.h"
#include "ObjectStateManager.h"
#include "CalcDataToolBar.h"

class FermaNextWorkspace;
class QStackedWidget;
class QTabWidget;

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

    virtual void loadFromFile ( const QString& ) throw (IOException, 
                                                        WrongXMLDocException,
                                                        LoadException);
    virtual void saveToFile () throw (FileNameIsNotDefinedException, 
                                      IOException);
    virtual void saveToFile ( const QString& ) throw (IOException);
    
    virtual const QString& getProjectFileName () const;
    virtual bool isFileNameDefined () const;

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual void activate ();
    virtual bool isActivated () const;

    virtual TrussUnitWindowManager& getTrussUnitWindowManager ();    
    virtual TrussDesignerWidget& getDesignerWidget ();
    virtual CalcDataToolBar& getCalcDataToolBar ();
    virtual FermaNextWorkspace& getWorkspace();

protected:
    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    virtual void setProjectFileName ( const QString& );

signals:
    void onActivate ( FermaNextProject& );
    void onNameChange ( const QString& );
    void onProjectFileNameChange ( const QString& );
  
private:
    friend class FermaNextWorkspace;
    
    FermaNextProject ( FermaNextWorkspace&, 
                       const QString& name, 
                       QStackedWidget* parent = 0 );

private:
    FermaNextWorkspace& currentWorkspace;
    QString name;
    QString projectFileName;
    QStackedWidget* stackedWidget;
    CalcDataToolBar* calcDataToolBar;
    QTabWidget* projectTab;
    QWidget* justStrengthAnalisysWidget;
    TrussDesignerWidget* designerWidget;
    TrussUnitWindowManager* trussWindowManager;
};

#endif //FERMANEXTPROJECT_H
