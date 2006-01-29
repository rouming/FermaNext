
#ifndef SPLASH_H
#define SPLASH_H

#include <qsplashscreen.h>
#include "PluginManager.h"

class Splash : public QSplashScreen
{
    Q_OBJECT
public:
    Splash ( const QPixmap& pixmap=QPixmap(), WFlags f=0 );

public slots:
    void message ( PluginLoader&, PluginManager::LoadingPriority );
    void message ( Plugin& );
    void message ( const QString& );
};

#endif //SPLASH_H
