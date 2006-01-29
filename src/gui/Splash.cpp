
#include "Splash.h"

/*****************************************************************************
 * Splash
 *****************************************************************************/

Splash::Splash ( const QPixmap& p, WFlags f ) :
    QSplashScreen( p, f )
{}

void Splash::message ( PluginLoader& ldr, 
                       PluginManager::LoadingPriority priority )
{
    message( "Registering plugin loader for extension \"." + 
             ldr.pluginExtension() + "\"" );
}

void Splash::message ( Plugin& plg )
{
    message( "Loading plugin \"" + plg.pluginInfo().name + "\"" );
}

void Splash::message ( const QString& msg )
{
    QSplashScreen::message( msg, Qt::AlignRight | Qt::AlignBottom,
                            QColor("white") );
}

/*****************************************************************************/

