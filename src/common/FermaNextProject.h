
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <qobject.h>
#include <qstring.h>
#include <qmainwindow.h>

#include "TrussUnitDesignerWindow.h"
#include "TrussUnitWindowManager.h"
#include "ObjectStateManager.h"

class FermaNextWorkspace;
class QWidgetStack;

class FermaNextProject : public QObject
{
    Q_OBJECT
public:
    virtual ~FermaNextProject ();

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual void activate ( bool );    
    //TODO: virtual ObjectStateManager& getStateManager ();
    virtual TrussUnitWindowManager& getTrussUnitWindowManager ();

signals:
    void onNameChange ( const QString& );    

private slots:
    void markWindowDestroyed ();
    
private:
    friend class FermaNextWorkspace;
    
    bool eventFilter( QObject* o, QEvent* e );
    FermaNextProject ( const QString& name, QWidgetStack* parent = 0 );


private:
    bool maximizedDesginerWindow;
    QString name;
    //TODO: ObjectStateManager stateManager;
    TrussUnitWindowManager trussWindowManager;

    // We own this object by pointer, because it can be destroyed
    // by Qt deletion mechanism when the application closes.
    // Nobody should own this member after deletion has happened.
    TrussUnitDesignerWindow* designerWindow;

    // If Qt has already destroyed designer window it should not be
    // destroyed twice.
    bool windowIsAlreadyDestroyed;
};

#endif //FERMANEXTPROJECT_H
