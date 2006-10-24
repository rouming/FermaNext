
#include <QApplication>
#include <QStyleFactory>

#include "Splash.h"
#include "Global.h"
#include "FermaNextWorkspace.h"
#include "FermaNextMainWindow.h"
#include "Log4CXX.h"

/*****************************************************************************
 * Main 
 *****************************************************************************/

int main ( int argc, char* argv[] )
{
    try {
        // Configuration of the logging system
        std::string logFile = Global::loggingFileName().toStdString();
        log4cxx::PropertyConfigurator::configureAndWatch( logFile, 3000 );

        QApplication app(argc, argv);

        // Set default style
        //QApplication::setStyle( QStyleFactory::create ("plastique") );

        // Do some 'splash' job, so set wait cursor
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );

        QPixmap pixmap( Global::imagesPath() + 
                        Global::pathSeparator() + "splash.png" );
        Splash* splash = new Splash( pixmap, 
                                     Global::applicationName(),
                                     Global::applicationVersion() );
        splash->show();

        splash->message( "Creating main components .." );
        
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
        
        FermaNextMainWindow* fermaMainWindow = wsp.mainWindow();
        fermaMainWindow->showMaximized();
        splash->finish( fermaMainWindow );
        QApplication::restoreOverrideCursor();
        delete splash;
        return app.exec();

    } catch ( ... ) {
        // Try to do smth useful before crash
        throw;
    }
}

/*****************************************************************************/
