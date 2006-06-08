
#include <QApplication>
#include <QFileInfo>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

#include "PluginReloader.h"
#include "PluginManager.h"

/*****************************************************************************
 * Reloader Dialog
 *****************************************************************************/

class ReloaderDialog : protected ReloaderDialogPrivate
{
public:
    ReloaderDialog ( PluginManager& plgManager ) :
        plgMng( plgManager )
    {
        QDialog::setWindowFlags( Qt::WindowTitleHint |
                                 Qt::WindowStaysOnTopHint );
        resize( QSize(311, 60).expandedTo(minimumSizeHint()) );
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
        setCursor(Qt::WaitCursor);
        show();
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
        // Start
        plgMng.unloadPlugins();
        plgMng.loadPlugins( Global::pluginsPath() );
        setCursor(Qt::ArrowCursor);
    }

private:
    // Load
    virtual void beforePluginsLoad ( uint plgNum ) 
    {
        progressBar->setValue(0);
        progressBar->setMinimum(0);
        progressBar->setMaximum(plgNum);
        label->setText("");
        label->repaint();
    }

    virtual void afterPluginsLoad ( uint plgNum ) 
    {
        progressBar->setValue(100);
    }

    virtual void beforePluginLoad ( const QString& path ) 
    {
        label->setText( tr("Loading plugin: ") + QFileInfo(path).baseName() );
        label->repaint();
    }
    virtual void afterPluginLoad ( Plugin& plg ) 
    {
        progressBar->setValue( progressBar->value() + 1 );
    }

    // Unload
    virtual void beforePluginsUnload ( uint plgNum ) 
    {
        progressBar->setValue(0);
        progressBar->setMinimum(0);
        progressBar->setMaximum(plgNum);
        label->setText("");
        label->repaint();
    }

    virtual void afterPluginsUnload ( uint plgNum ) 
    {
        progressBar->setValue(100);
    }

    virtual void beforePluginUnload ( Plugin& plg ) 
    {
        label->setText( tr("Unloading plugin: ") + 
                        QFileInfo(plg.pluginPath()).baseName() );
        label->repaint();
    }

    virtual void afterPluginUnload ( const QString& path ) 
    {
        progressBar->setValue( progressBar->value() + 1 );
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
    ReloaderDialog* reloaderDialog = new ReloaderDialog( plgManager );    
    reloaderDialog->reloadPlugins();    
    delete reloaderDialog;
}

/*****************************************************************************/
