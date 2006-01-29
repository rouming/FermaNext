
#include <qapplication.h>
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
    QObject::connect( &plgMng, SIGNAL(onPluginLoaderRegistration(PluginLoader&,
                                        PluginManager::LoadingPriority)), 
                      splash, SLOT(message(PluginLoader&, 
                                        PluginManager::LoadingPriority)) );
    // Catch plugins loading
    QObject::connect( &plgMng, SIGNAL(onPluginLoad(Plugin&)), 
                      splash, SLOT(message(Plugin&)) );

    plgMng.registerPluginLoaders( pluginLoadersPath() );
    plgMng.loadPlugins( pluginsPath() );    

    splash->message( "Building main objects .." );
    FermaNextMainFrame ferma;
    app.setMainWidget(&ferma);
    splash->message( "Loading gui .." );
    ferma.showMaximized();
    splash->finish( &ferma );
    delete splash;
    return app.exec();
}

/*****************************************************************************/
