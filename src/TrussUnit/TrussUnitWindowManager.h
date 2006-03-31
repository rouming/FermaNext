
#ifndef TRUSSUNITWINDOWMANAGER_H
#define TRUSSUNITWINDOWMANAGER_H

#include <vector>
#include <QObject>
#include <QFile>
#include <QString>

#include "TrussUnitWindow.h"

typedef std::vector<TrussUnitWindow*> WindowList;
typedef WindowList::iterator WindowListIter;
typedef WindowList::const_iterator WindowListConstIter;
typedef WindowList::reverse_iterator WindowListRevIter;

class TrussUnitWindowManager : public QObject
{
    Q_OBJECT
public:
    // File format extensions
    static const QString& newFormatExtension ();
    static const QString& oldFormatExtension ();

public:
    // TrussUnitWindowManager exceptions
    class ReadFileException  {};
    class WriteFileException {};
    class WrongFormatException {};

    TrussUnitWindowManager ();
    virtual ~TrussUnitWindowManager ();

    virtual TrussUnitWindow& createTrussUnitWindowFromFile ( 
         const QString& fileName ) throw (ReadFileException,
                                          WrongFormatException);

    virtual WindowList getTrussUnitWindowList ();

    virtual TrussUnitWindow& createTrussUnitWindow ( const QString& name );    
    virtual bool removeTrussUnitWindow ( TrussUnitWindow& );

protected:
    virtual void clearTrussUnitWindows ();
    virtual void suspendedClean ();

    // Returns iter after erase
    virtual WindowListIter removeTrussUnitWindow ( WindowListIter );

    virtual void loadOldVersion ( TrussUnit&, QFile& ) 
        throw (WrongFormatException);
    virtual void loadNewVersion ( TrussUnit&, QFile& ) 
        throw (WrongFormatException);

    // If 'silence' is true, truss manager doesn't emit 
    // 'onTrussUnitWindowCreate' signal. "Quiet" creation used to create truss
    // and then to construct all its memebers (e.g. reading from file). While
    // members construction exception can occur, so if it happens we should 
    // free all the memory, remove truss and nobody should know about this 
    // failed creation attempt.
    virtual TrussUnitWindow& createTrussUnitWindow ( bool silence,
                                                     const QString& name );

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
    // Every truss has it's own state manager.
    typedef QMap<TrussUnitWindow*, ObjectStateManager*> StateManagerMap;
    typedef StateManagerMap::Iterator StateManagerMapIter;

    WindowList trussWindows;
    StateManagerMap stateManagerMap;
};

#endif //TRUSSUNITWINDOWMANAGER_H
