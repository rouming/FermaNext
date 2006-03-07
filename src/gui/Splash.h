
#ifndef SPLASH_H
#define SPLASH_H

#include <QSplashScreen>

#include "PluginManager.h"

class Splash : public QSplashScreen
{
    Q_OBJECT
public:
    Splash ( const QPixmap& pixmap=QPixmap(), WFlags f=0 );

public slots:
    void pluginLoaderMessage ( const QString& );
    void pluginMessage ( const QString& );
    void message ( const QString& );

protected:
    void drawContents ( QPainter* );
};

#endif //SPLASH_H
