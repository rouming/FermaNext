
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <qobject.h>
#include <qstring.h>
#include <qmainwindow.h>

#include "TrussUnitDesignerWindow.h"
#include "TrussUnitManager.h"
#include "ObjectStateManager.h"

class FermaNextWorkspace;
class QWorkspace;

class FermaNextProject : public QObject
{
    Q_OBJECT
public:
    virtual ~FermaNextProject ();

    virtual const QString& getName () const;
    virtual void setName ( const QString& );

    virtual void activate ( bool );    
    virtual ObjectStateManager& getStateManager ();    

signals:
    void onNameChange ( const QString& );    

private slots:
    void markWindowDestroyed ();
    
private:
    friend class FermaNextWorkspace;
    
    bool eventFilter( QObject* o, QEvent* e );
    FermaNextProject ( const QString& name, QWorkspace* qWsp = 0 );


private:
    bool maximizedDesginerWindow;
    QString name;
    ObjectStateManager stateManager;
    TrussUnitManager trussManager;

    // We own this object by pointer, because it should be destroyed
    // by Qt deletion mechanism when all the application closes.
    // Nobody should call getWindow after deletion has happened.
    TrussUnitDesignerWindow* designerWindow;

    // If Qt has already destroyed designer window it should not be
    // destroyed twice.
    bool windowIsAlreadyDestroyed;
};

#endif //FERMANEXTPROJECT_H
