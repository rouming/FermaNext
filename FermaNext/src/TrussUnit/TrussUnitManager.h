
#ifndef TRUSSUNITMANAGER_H
#define TRUSSUNITMANAGER_H

#include <vector>
#include <qobject.h>
#include <qfile.h>
#include <qstring.h>

#include "TrussUnit.h"

typedef std::vector<TrussUnit*>  TrussUnitList;
typedef std::vector<TrussUnit*>::iterator  TrussUnitListIter;

class TrussUnitManager : public QObject
{
    Q_OBJECT
public:
    // TrussUnitManager exceptions
    class ReadFileException  {};
    class WriteFileException {};
    class WrongFormatException {};

    virtual ~TrussUnitManager ();

protected:
    virtual void clearTrussUnits ();

    virtual void loadOldVersion ( TrussUnit&, QFile& ) throw (WrongFormatException);
    virtual void load ( TrussUnit&, const QFile& ) throw (WrongFormatException);

public slots:
    virtual TrussUnit& createTrussUnit ( const QString& name );
    virtual bool removeTrussUnit ( const TrussUnit& );

public:
    virtual TrussUnit& createTrussUnitFromFile ( const QString& fileName ) 
                                                  throw (ReadFileException,
                                                         WrongFormatException);


signals:
    void onTrussUnitCreate ( TrussUnit& );
    void onTrussUnitRemove ( const TrussUnit& );

private:
    static const QString NEW_EXTENSION;
    static const QString OLD_EXTENSION;

    TrussUnitList trussUnits;
};

#endif //TRUSSUNITMANAGER_H
