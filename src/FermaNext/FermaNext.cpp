
#include <QApplication>

#include "Splash.h"
#include "SubsidiaryConstants.h"
#include "FermaNextMainFrame.h"

/*****************************************************************************
 * Main 
 *****************************************************************************/

int main ( int argc, char* argv[] )
{
    QApplication app(argc, argv);
    QPixmap pixmap( imagesPath() + pathSeparator() + "splash.png" );
    Splash* splash = new Splash( pixmap );
    splash->show();

    PluginManager& plgMng = PluginManager::instance();
    // Catch plugin loaders registration
    QObject::connect( &plgMng, 
                      SIGNAL(onBeforePluginLoaderRegistration(const QString&)),
                      splash, 
                      SLOT(pluginLoaderMessage(const QString&)) );
    // Catch plugins loading
    QObject::connect( &plgMng, SIGNAL(onBeforePluginLoad(const QString&)), 
                      splash, SLOT(pluginMessage(const QString&)) );

    plgMng.registerPluginLoaders( pluginLoadersPath() );
    plgMng.loadPlugins( pluginsPath() );    

    splash->message( "Setting up GUI .." );
    FermaNextMainFrame ferma;
    ferma.showMaximized();
    splash->finish( &ferma );
    delete splash;
    return app.exec();
}

/*****************************************************************************/
