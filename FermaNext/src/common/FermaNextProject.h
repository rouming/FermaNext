
#ifndef FERMANEXTPROJECT_H
#define FERMANEXTPROJECT_H

#include <qobject.h>
#include <qstring.h>
#include <qmainwindow.h>
#include "TrussUnitDesignerWindow.h"

class FermaNextWorkspace;

class FermaNextProject : public QObject
{
    Q_OBJECT
private:
    friend class FermaNextWorkspace;
    
    FermaNextProject ( const QString& name );

    QString name;
    TrussUnitDesignerWindow designerWindow;
};

#endif //FERMANEXTPROJECT_H
