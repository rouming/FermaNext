
#include <QApplication>
#include <QStyleFactory>

#include "Splash.h"
#include "Global.h"
#include "FermaNextWorkspace.h"
#include "FermaNextMainWindow.h"

/*****************************************************************************
 * Main 
 *****************************************************************************/

int main ( int argc, char* argv[] )
{
    QApplication app(argc, argv);

    // Set default style
    //QApplication::setStyle( QStyleFactory::create ("plastique") );

    QPixmap pixmap( Global::imagesPath() + 
                    Global::pathSeparator() + "splash.png" );
    Splash* splash = new Splash( pixmap, 
                                 Global::applicationName(),
                                 Global::applicationVersion() );
    splash->show();

    FermaNextWorkspace& wsp = FermaNextWorkspace::workspace();

    PluginManager& plgMng = wsp.pluginManager();
    // Catch plugin loaders registration
    QObject::connect( &plgMng, 
                      SIGNAL(onBeforePluginLoaderRegistration(const QString&)),
                      splash, 
                      SLOT(pluginLoaderMessage(const QString&)) );
    // Catch plugins loading
    QObject::connect( &plgMng, SIGNAL(onBeforePluginLoad(const QString&)), 
                      splash, SLOT(pluginMessage(const QString&)) );

    plgMng.registerPluginLoaders( Global::pluginLoadersPath() );
    plgMng.loadPlugins( Global::pluginsPath() );    

    splash->message( "Setting up GUI .." );

    FermaNextMainWindow& fermaMainWindow = wsp.mainWindow();
    fermaMainWindow.showMaximized();
    splash->finish( &fermaMainWindow );
    delete splash;
    return app.exec();
}

/*****************************************************************************/
