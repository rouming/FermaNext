
#include <QFileInfo>
#include <QPainter>

#include "Splash.h"

/*****************************************************************************
 * Splash
 *****************************************************************************/

Splash::Splash ( const QPixmap& p, Qt::WFlags f ) :
    QSplashScreen( p, f )
{}

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
    QSplashScreen::showMessage( msg, Qt::AlignRight | Qt::AlignBottom,
                                QColor("white") );
}

void Splash::drawContents ( QPainter* painter )
{
    QFont f("Helvetica", 10);
    painter->setFont( f );
    QSplashScreen::drawContents( painter );
}

/*****************************************************************************/
