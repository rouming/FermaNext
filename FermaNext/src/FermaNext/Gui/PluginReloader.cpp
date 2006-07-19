
#include <QApplication>
#include <QFileInfo>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QRect>
#include <QDesktopWidget>

#include "PluginReloader.h"
#include "PluginManager.h"

/*****************************************************************************
 * Reloader Dialog
 *****************************************************************************/

class ReloaderWidget : protected ReloaderWidgetPrivate
{
public:
    ReloaderWidget ( PluginManager& plgManager ) :
        plgMng( plgManager )
    {
        QWidget::setWindowFlags( Qt::Tool );
        resize( QSize(311, 60).expandedTo(minimumSizeHint()) );
        setMaximumSize( size() );
        QRect r( 0, 0, size().width(), size().height() );
        move(QApplication::desktop()->screenGeometry().center() - r.center());
        setWindowTitle( QObject::tr("Reload plugins") );

        QVBoxLayout* vboxLayout = new QVBoxLayout(this);
        vboxLayout->setSpacing(6);
        vboxLayout->setMargin(9);

        label = new QLabel(this);
        vboxLayout->addWidget(label);

        progressBar = new QProgressBar(this);
        progressBar->setTextVisible(false);
        progressBar->setValue(0);
        progressBar->setOrientation(Qt::Horizontal);
        vboxLayout->addWidget(progressBar);

        // Load connects 
        QObject::connect( &plgMng, SIGNAL(onBeforePluginsLoad(uint)),
                                   SLOT(beforePluginsLoad(uint)) );
        QObject::connect( &plgMng, SIGNAL(onAfterPluginsLoad(uint)), 
                                   SLOT(afterPluginsLoad(uint)) );
        QObject::connect( &plgMng, SIGNAL(onBeforePluginLoad(const QString&)),
                                   SLOT(beforePluginLoad(const QString&)) );
        QObject::connect( &plgMng, SIGNAL(onAfterPluginLoad(Plugin&)), 
                                   SLOT(afterPluginLoad(Plugin&)) );

        // Unload connects
        QObject::connect( &plgMng, SIGNAL(onBeforePluginsUnload(uint)),
                                   SLOT(beforePluginsUnload(uint)) );
        QObject::connect( &plgMng, SIGNAL(onAfterPluginsUnload(uint)), 
                                   SLOT(afterPluginsUnload(uint)) );
        QObject::connect( &plgMng, SIGNAL(onBeforePluginUnload(Plugin&)), 
                                   SLOT(beforePluginUnload(Plugin&)) );
        QObject::connect( &plgMng, SIGNAL(onAfterPluginUnload(const QString&)),
                                   SLOT(afterPluginUnload(const QString&)) );
    }

    void reloadPlugins ()
    {
        QApplication::setOverrideCursor( QCursor(Qt::WaitCursor) );
        show();
        qApp->processEvents( QEventLoop::ExcludeUserInputEvents );
        // Reload plugins
        plgMng.reloadPlugins();
        QApplication::restoreOverrideCursor();
    }

private:
    // Load
    virtual void beforePluginsLoad ( uint plgNum ) 
    {
        progressBar->setValue(0);
        progressBar->setMinimum(0);
        progressBar->setMaximum(plgNum);
        label->setText("");
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

    virtual void afterPluginsLoad ( uint ) 
    {
        progressBar->setValue(100);
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

    virtual void beforePluginLoad ( const QString& path ) 
    {
        label->setText( tr("Loading plugin: ") + QFileInfo(path).baseName() );
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

    virtual void afterPluginLoad ( Plugin& ) 
    {
        progressBar->setValue( progressBar->value() + 1 );
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

    // Unload
    virtual void beforePluginsUnload ( uint plgNum ) 
    {
        progressBar->setValue(0);
        progressBar->setMinimum(0);
        progressBar->setMaximum(plgNum);
        label->setText("");
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

    virtual void afterPluginsUnload ( uint ) 
    {
        progressBar->setValue(100);
        repaint();
        QApplication::flush();
    }

    virtual void beforePluginUnload ( Plugin& plg ) 
    {
        label->setText( tr("Unloading plugin: ") + 
                        QFileInfo(plg.pluginPath()).baseName() );
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

    virtual void afterPluginUnload ( const QString& ) 
    {
        progressBar->setValue( progressBar->value() + 1 );        
        repaint();
        QApplication::flush();
        QApplication::processEvents();
    }

private:
    PluginManager& plgMng;
    QLabel* label;
    QProgressBar* progressBar;
};


/*****************************************************************************
 * Plugin Reloader
 *****************************************************************************/

void PluginReloader::reloadPlugins ( PluginManager& plgManager )
{
    ReloaderWidget* reloaderWidget = new ReloaderWidget( plgManager );    
    reloaderWidget->reloadPlugins();    
    delete reloaderWidget;
}

/*****************************************************************************/
