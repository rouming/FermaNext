
#include <QApplication>
#include <QStyleFactory>
#include <QSettings>

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
        QApplication app(argc, argv);

        // Configuration of the logging system
        std::string logFile = Global::loggingFileName().toStdString();
        log4cxx::PropertyConfigurator::configureAndWatch( logFile, 3000 );

        QCoreApplication::setOrganizationName( "Scumbag & Brainfucker Inc." );
        QCoreApplication::setApplicationName( Global::applicationName() );

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
        
        // Setup user preferences
        QSettings appSettings;

        appSettings.beginGroup( "Preferences" );

        if ( ! appSettings.contains( "ShowNodeNumbers" ) ) {
            appSettings.setValue( "ShowNodeNumbers", true );
            Global::showNodeNumbers = true;
        }
        else
            Global::showNodeNumbers =  
                appSettings.value( "ShowNodeNumbers" ).toBool();

        if ( ! appSettings.contains( "ShowPivotNumbers" ) ) {
            appSettings.setValue( "ShowPivotNumbers", true );
            Global::showPivotNumbers = true;
        }
        else
            Global::showPivotNumbers = 
                appSettings.value( "ShowPivotNumbers" ).toBool();

        if ( ! appSettings.contains( "ShowFixations" ) ) {
            appSettings.setValue( "ShowFixations", true );
            Global::showFixations = true;
        }
        else
            Global::showFixations = 
                appSettings.value( "ShowFixations" ).toBool();

        if ( ! appSettings.contains( "ShowLoads" ) ) {
            appSettings.setValue( "ShowLoads", true );
            Global::showLoads = true;
        }
        else
            Global::showLoads = 
                appSettings.value( "ShowLoads" ).toBool();

        appSettings.endGroup();

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
