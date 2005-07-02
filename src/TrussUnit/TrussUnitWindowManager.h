
#ifndef TRUSSUNITWINDOWMANAGER_H
#define TRUSSUNITWINDOWMANAGER_H

#include <vector>
#include <qobject.h>
#include <qfile.h>
#include <qstring.h>

#include "StatefulObject.h"
#include "TrussUnitWindow.h"

typedef std::vector<TrussUnitWindow*>  WindowList;
typedef std::vector<TrussUnitWindow*>::iterator  WindowListIter;

class TrussUnitWindowManager : public StatefulObject
{
    Q_OBJECT
public:
    // TrussUnitWindowManager exceptions
    class ReadFileException  {};
    class WriteFileException {};
    class WrongFormatException {};

    TrussUnitWindowManager ( ObjectStateManager* );
    virtual ~TrussUnitWindowManager ();

protected:
    virtual void clearTrussUnitWindows ();

    virtual void loadOldVersion ( TrussUnit&, QFile& ) throw (WrongFormatException);
    virtual void load ( TrussUnit&, const QFile& ) throw (WrongFormatException);

public slots:
    virtual TrussUnitWindow& createTrussUnitWindow ( const QString& name );
    virtual bool removeTrussUnitWindow ( TrussUnitWindow& );

public:
    virtual TrussUnitWindow& createTrussUnitWindowFromFile ( const QString& fileName ) 
                                                      throw (ReadFileException,
                                                             WrongFormatException);


signals:
    void onTrussUnitWindowCreate ( TrussUnitWindow& );
    void onTrussUnitWindowRemove ( const TrussUnitWindow& );

private:
    static const QString NEW_EXTENSION;
    static const QString OLD_EXTENSION;

    WindowList trussWindows;
};

#endif //TRUSSUNITWINDOWMANAGER_H
