
#include "FermaNextProject.h"
#include "SubsidiaryConstants.h"

#include <qtabwidget.h>
#include <qwidgetstack.h>

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/

const QString FermaNextProject::FormatExtension = ".fpr";

/*****************************************************************************/

FermaNextProject::FermaNextProject ( const QString& name_, QWidgetStack* stack ) :
    name(name_),
    widgetStack(stack),
    projectMainWidget( new QMainWindow(widgetStack, 0, 0) ),
    calcDataToolBar( new CalcDataToolBar(projectMainWidget) ),
    projectTab( new QTabWidget(projectMainWidget) ),
    justStrengthAnalisysWidget( new QWidget(projectTab) ),
    designerWindow( new TrussUnitDesignerWindow(name_, projectTab) ),
    trussWindowManager( new TrussUnitWindowManager )
{
    projectMainWidget->setRightJustification( true );
    projectMainWidget->setDockEnabled( DockLeft, false );
    projectMainWidget->setDockEnabled( DockRight, false );

    widgetStack->addWidget(projectMainWidget);
    projectMainWidget->setCentralWidget( projectTab );
  
    projectTab->setTabPosition( QTabWidget::Bottom );
    projectTab->addTab( designerWindow, tr("Designer") );
    projectTab->addTab( justStrengthAnalisysWidget, tr("Strength Analysis") );
      

    TrussUnitDesignerWidget& designerWidget = designerWindow->getDesignerWidget();

    // Catch trusses creation or deletion.
    connect( trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             &designerWidget, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( trussWindowManager, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             &designerWidget, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
    connect( trussWindowManager, SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)),     
             calcDataToolBar, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( trussWindowManager, SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             calcDataToolBar, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
}

FermaNextProject::~FermaNextProject ()
{
    if ( widgetStack )
        widgetStack->removeWidget( projectMainWidget );
    delete trussWindowManager;
    delete projectMainWidget;
}

QDomElement FermaNextProject::saveToXML ( QDomDocument& doc )
{
    QDomElement prjElem = XMLSerializableObject::saveToXML( doc );
    prjElem.setTagName( "FermaNextProject" );

    /**
     * Save project name
     ********************/
    prjElem.setAttribute( "name", getName() );

    /**
     * Save truss unit windows
     **************************/
    // Save window stack
    WindowList trussWindows = 
        getDesignerWindow().getDesignerWidget().getTrussUnitWindowList();
    WindowListIter wIter = trussWindows.begin();
    for ( ; wIter != trussWindows.end(); ++wIter )
        prjElem.appendChild( (*wIter)->saveToXML( doc ) );

    return prjElem;
}

void FermaNextProject::loadFromXML ( const QDomElement& prjElem ) 
    throw (LoadException)
{
    XMLSerializableObject::loadFromXML( prjElem );

    /**
     * Set project name
     ********************/
    if ( ! prjElem.hasAttribute( "name" ) )
        throw LoadException();
    setName( prjElem.attribute( "name" ) );


    /**
     * Create truss unit windows
     ****************************/
    TrussUnitWindowManager& mng = getTrussUnitWindowManager();
    QDomNodeList trussWindows = prjElem.elementsByTagName( "TrussUnitWindow" );
    for ( uint windsNum = 0; windsNum < trussWindows.count(); ++windsNum ) {
        QDomNode trussWindow = trussWindows.item( windsNum );
        if ( ! trussWindow.isElement() )
            throw LoadException();
        QDomElement windowElem = trussWindow.toElement();
        TrussUnitWindow& wnd = mng.createTrussUnitWindow( QString::null );
        wnd.loadFromXML( windowElem );
    }
}

void FermaNextProject::activate ()
{
    if ( widgetStack && widgetStack->visibleWidget() != projectMainWidget )
        widgetStack->raiseWidget( projectMainWidget );
}

const QString& FermaNextProject::getName () const
{
    return name;
}

void FermaNextProject::setName ( const QString& name_ )
{
    name = name_;
    emit onNameChange(name);
}

TrussUnitWindowManager& FermaNextProject::getTrussUnitWindowManager ()
{
    return *trussWindowManager;
}

TrussUnitDesignerWindow& FermaNextProject::getDesignerWindow ()
{
    return *designerWindow;
}

CalcDataToolBar& FermaNextProject::getCalcDataToolBar ()
{
    return *calcDataToolBar;
}

/*****************************************************************************/
