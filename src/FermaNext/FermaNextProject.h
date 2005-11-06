
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <qstring.h>

#include "XMLSerializableObject.h"
#include "TrussUnitDesignerWindow.h"
#include "TrussUnitWindowManager.h"
#include "ObjectStateManager.h"
#include "CalcDataToolBar.h"

class FermaNextWorkspace;
class QWidgetStack;
class QTabWidget;

class FermaNextProject : public QObject, public XMLSerializableObject
{
    Q_OBJECT
public:
    // File format extension
    static const QString FormatExtension;

    virtual ~FermaNextProject ();

    // XML serialization
    virtual QDomElement saveToXML ( QDomDocument& );

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual void activate ();
    virtual TrussUnitWindowManager& getTrussUnitWindowManager ();    
    virtual TrussUnitDesignerWindow& getDesignerWindow ();
    virtual CalcDataToolBar& getCalcDataToolBar ();

protected:
    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);

signals:
    void onNameChange ( const QString& );    
  
private:
    friend class FermaNextWorkspace;
    
    FermaNextProject ( const QString& name, QWidgetStack* parent = 0 );

private:
    QString name;
    QWidgetStack* widgetStack;
    QMainWindow* projectMainWidget;
    CalcDataToolBar* calcDataToolBar;
    QTabWidget* projectTab;
    QWidget* justStrengthAnalisysWidget;
    TrussUnitDesignerWindow* designerWindow;
    TrussUnitWindowManager* trussWindowManager;
};

#endif //FERMANEXTPROJECT_H
