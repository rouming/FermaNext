
#ifndef TRUSSUNITWINDOWMANAGER_H
#define TRUSSUNITWINDOWMANAGER_H

#include <vector>
#include <qobject.h>
#include <qfile.h>
#include <qstring.h>

#include "StatefulObject.h"
#include "TrussUnitWindow.h"

typedef std::vector<TrussUnitWindow*> WindowList;
typedef WindowList::iterator WindowListIter;

class TrussUnitWindowManager : public QObject
{
    Q_OBJECT
public:
    // TrussUnitWindowManager exceptions
    class ReadFileException  {};
    class WriteFileException {};
    class WrongFormatException {};

    TrussUnitWindowManager ( ObjectStateManager& );
    virtual ~TrussUnitWindowManager ();

    virtual TrussUnitWindow& createTrussUnitWindowFromFile ( 
         const QString& fileName ) throw (ReadFileException,
                                          WrongFormatException);

    virtual WindowList getTrussUnitWindowList ();

protected:
    virtual void clearTrussUnitWindows ();
    virtual void suspendedClean ();
    virtual bool removeTrussUnitWindow ( WindowListIter& );

    virtual void loadOldVersion ( TrussUnit&, QFile& ) throw (WrongFormatException);
    virtual void load ( TrussUnit&, const QFile& ) throw (WrongFormatException);

public slots:
    virtual TrussUnitWindow& createTrussUnitWindow ( const QString& name );    
    virtual bool removeTrussUnitWindow ( TrussUnitWindow& );

protected slots:
    // Just wrappers to cast stateful object to truss window
    // and to emit the signal further
    virtual void trussWindowAfterRevive ( StatefulObject& );
    virtual void trussWindowAfterDesist ( StatefulObject& );

signals:
    void onTrussUnitWindowCreate ( TrussUnitWindow& );
    void onTrussUnitWindowRemove ( TrussUnitWindow& );
    void onTrussUnitWindowRevive ( TrussUnitWindow& );
    void onTrussUnitWindowDesist ( TrussUnitWindow& );

private:
    static const QString NEW_EXTENSION;
    static const QString OLD_EXTENSION;

    WindowList trussWindows;
    ObjectStateManager& stateManager;
};

#endif //TRUSSUNITWINDOWMANAGER_H
