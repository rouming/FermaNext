
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <qstring.h>

#include "TrussUnitDesignerWindow.h"
#include "TrussUnitWindowManager.h"
#include "ObjectStateManager.h"

class FermaNextWorkspace;
class QWidgetStack;
class QTabWidget;

class FermaNextProject : public QObject
{
    Q_OBJECT
public:
    virtual ~FermaNextProject ();

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual void activate ();    
    virtual ObjectStateManager& getStateManager ();
    virtual TrussUnitWindowManager& getTrussUnitWindowManager ();
    virtual QTabWidget& getProjectTab ();
    virtual TrussUnitDesignerWindow& getDesignerWindow ();

signals:
    void onNameChange ( const QString& );    
  
private:
    friend class FermaNextWorkspace;
    
    FermaNextProject ( const QString& name, QWidgetStack* parent = 0 );

private:
    QString name;
    ObjectStateManager stateManager;
    TrussUnitWindowManager trussWindowManager;
    QTabWidget* projectTab;
    QWidget* justStrengthAnalisysWidget;

    // We own this object by pointer, because it can be destroyed
    // by Qt deletion mechanism when the application closes.
    // Nobody should own this member after deletion has happened.
    TrussUnitDesignerWindow* designerWindow;
};

#endif //FERMANEXTPROJECT_H
