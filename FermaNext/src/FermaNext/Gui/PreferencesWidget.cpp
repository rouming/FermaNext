
#include <QCheckBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "PreferencesWidget.h"
#include "Global.h"

/*****************************************************************************
 * Preferences Tab Widget
 *****************************************************************************/

PreferencesTabWidget::PreferencesTabWidget ( QWidget* parent ) :
    QTabWidget( parent )
{
    initTrussViewTab();
}

void PreferencesTabWidget::initTrussViewTab ()
{
    QFrame* parentFrame = new QFrame;
    QGroupBox* viewGroupBox = 
        new QGroupBox( tr( "Truss view settings" ), parentFrame );
    showNodeNumbsCb = new QCheckBox( tr( "Show node numbers" ) );
    showPivotNumbsCb = new QCheckBox( tr( "Show pivot numbers" ) );
    showFixationsCb = new QCheckBox( tr( "Show fixations" ) );
    showLoadsCb = new QCheckBox( tr( "Show loads" ) );

    QVBoxLayout* parenLayout = new QVBoxLayout( parentFrame );
    parenLayout->addWidget( viewGroupBox );
    parenLayout->addStretch( 1 );
    parenLayout->setMargin( 10 );

    QGridLayout *viewLayout = new QGridLayout( viewGroupBox );
    viewLayout->addWidget( showNodeNumbsCb, 0, 0 );
    viewLayout->addWidget( showPivotNumbsCb, 1, 0 );
    viewLayout->addWidget( showFixationsCb, 0, 1 );
    viewLayout->addWidget( showLoadsCb, 1, 1 );
    viewLayout->setSpacing( 5 );
    viewLayout->setMargin( 15 );

    addTab( parentFrame, tr( "View" ) );

    connect( showNodeNumbsCb, SIGNAL( toggled(bool) ),
                              SLOT( showNodeNumbers(bool) ) );
    connect( showPivotNumbsCb, SIGNAL( toggled(bool) ),
                               SLOT( showPivotNumbers(bool) ) );
    connect( showFixationsCb, SIGNAL( toggled(bool) ),
                              SLOT( showFixations(bool) ) );
    connect( showLoadsCb, SIGNAL( toggled(bool) ),
                          SLOT( showLoads(bool) ) );    

}

void PreferencesTabWidget::loadPreferences ()
{
    QSettings appSettings;

    appSettings.beginGroup( "Preferences" );

    showNodeNumbsCb->setChecked( appSettings.value( "ShowNodeNumbers" ).toBool() );
    showPivotNumbsCb->setChecked(appSettings.value( "ShowPivotNumbers" ).toBool() );
    showFixationsCb->setChecked( appSettings.value( "ShowFixations" ).toBool() );
    showLoadsCb->setChecked( appSettings.value( "ShowLoads" ).toBool() );

    appSettings.endGroup();
}

void PreferencesTabWidget::savePreferences ()
{
    QSettings appSettings;

    appSettings.beginGroup( "Preferences" );

    appSettings.setValue( "ShowNodeNumbers", showNodeNumbsCb->isChecked() );
    appSettings.setValue( "ShowPivotNumbers", showPivotNumbsCb->isChecked() );
    appSettings.setValue( "ShowFixations", showFixationsCb->isChecked() );
    appSettings.setValue( "ShowLoads", showLoadsCb->isChecked() );

    appSettings.endGroup();
}

void PreferencesTabWidget::showNodeNumbers ( bool status )
{
    Global::showNodeNumbers = status;
}

void PreferencesTabWidget::showPivotNumbers ( bool status )
{
    Global::showPivotNumbers = status;
}

void PreferencesTabWidget::showFixations ( bool status )
{
    Global::showFixations = status;
}

void PreferencesTabWidget::showLoads ( bool status )
{
    Global::showLoads = status;
}
   
/*****************************************************************************
 * Preferences Widget
 *****************************************************************************/

PreferencesWidget::PreferencesWidget ( QWidget* parent, Qt::WFlags f ) :
    QDialog( parent, f ),
    tabWidget( 0 )
{
    init();
}

void PreferencesWidget::init ()
{
    tabWidget = new PreferencesTabWidget;

    QPushButton* okButton = new QPushButton( tr( "OK" ) );
    connect( okButton, SIGNAL(clicked()),
                          SLOT(accept()) );

    QPushButton* cancelButton = new QPushButton( tr( "Cancel" ) );
    connect( cancelButton, SIGNAL(clicked()),
                             SLOT(reject()) );

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget( okButton );
    buttonLayout->addWidget( cancelButton );

    QVBoxLayout* parentLayout = new QVBoxLayout( this );
    parentLayout->addWidget( tabWidget );
    parentLayout->addLayout( buttonLayout );

    tabWidget->loadPreferences();

    resize( 250, 100 );
    setWindowTitle( tr("Application Preferences") );
}

void PreferencesWidget::accept ()
{
    tabWidget->savePreferences();
    done( 1 );
}

void PreferencesWidget::reject ()
{
    done( 0 );
}
 
/*****************************************************************************/
