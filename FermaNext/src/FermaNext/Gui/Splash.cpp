
#include <QFileInfo>
#include <QPainter>
#include <QApplication>

#include "Splash.h"

/*****************************************************************************
 * Splash
 *****************************************************************************/

Splash::Splash ( const QPixmap& p, const QString& appName_,
                 const QString& appVersion_ ) :
    QSplashScreen(p),
    appName(appName_),
    appVersion(appVersion_)
{
    // Force drawing app name and app version
    QSplashScreen::showMessage( QString() );
}

void Splash::pluginLoaderMessage ( const QString& path )
{
    message( "Registering loader: '" + QFileInfo(path).baseName() + "'" );
}

void Splash::pluginMessage ( const QString& path )
{
    message( "Loading plugin: '" + QFileInfo(path).baseName() + "'" );
}

void Splash::message ( const QString& msg )
{
    QSplashScreen::showMessage( msg, Qt::AlignLeft | Qt::AlignTop,
                                QColor("red") );
    QApplication::processEvents();
}

void Splash::drawContents ( QPainter* painter )
{
    QFont f("Helvetica", 13);
    f.setBold( true );
    painter->setFont( f );
    painter->setPen( Qt::white );    
    painter->drawText( QPoint(10,120), appName );
    f.setBold( false );
    f.setPixelSize( 11 );
    painter->setFont( f );
    painter->drawText( QPoint(10,133), appVersion );
    QSplashScreen::drawContents( painter );
}

/*****************************************************************************/
