
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <QString>

#include "XMLSerializableObject.h"
#include "TrussUnitDesignerWindow.h"
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
    virtual TrussUnitDesignerWindow& getDesignerWindow ();
    virtual CalcDataToolBar& getCalcDataToolBar ();

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
    
    FermaNextProject ( const QString& name, QStackedWidget* parent = 0 );

private:
    QString name;
    QString projectFileName;
    QStackedWidget* widgetStack;
    QMainWindow* projectMainWidget;
    CalcDataToolBar* calcDataToolBar;
    QTabWidget* projectTab;
    QWidget* justStrengthAnalisysWidget;
    TrussUnitDesignerWindow* designerWindow;
    TrussUnitWindowManager* trussWindowManager;
};

#endif //FERMANEXTPROJECT_H
