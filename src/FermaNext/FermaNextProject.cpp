
#include <QTabWidget>
#include <QStackedWidget>
#include <QTextStream>

#include "FermaNextProject.h"
#include "SubsidiaryConstants.h"

/*****************************************************************************
 * FermaNext Project
 *****************************************************************************/

const QString& FermaNextProject::formatExtension ()
{
    static QString extension = ".fpr";
    return extension;
}

/*****************************************************************************/

FermaNextProject::FermaNextProject ( FermaNextWorkspace& wsp,
                                     const QString& name_, 
                                     QStackedWidget* stack ) :
    currentWorkspace(wsp),
    name(name_),
    stackedWidget(stack),
    // FIXME QT3TO4:
    //calcDataToolBar( new CalcDataToolBar(projectMainWidget) ),
    projectTab( new QTabWidget(stackedWidget) ),
    justStrengthAnalisysWidget( new QWidget ),
    designerWidget( new TrussUnitDesignerWidget ),
    trussWindowManager( new TrussUnitWindowManager )
{
    /*
      // FIXME QT3TO4:
    projectMainWidget->setRightJustification( true );
    projectMainWidget->setDockEnabled( DockLeft, false );
    projectMainWidget->setDockEnabled( DockRight, false );
    */

    stackedWidget->addWidget(projectTab);
  
    projectTab->setTabPosition( QTabWidget::South );
    projectTab->addTab( designerWidget, tr("Designer") );
    projectTab->addTab( justStrengthAnalisysWidget, tr("Strength Analysis") );
      
    // Catch trusses creation or deletion.
    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             designerWidget, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             designerWidget, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
    // FIXME QT3TO4: 
    /*
    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)),     
             calcDataToolBar, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             calcDataToolBar, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );
     */
}

FermaNextProject::~FermaNextProject ()
{
    if ( stackedWidget )
        stackedWidget->removeWidget( projectTab );
    delete trussWindowManager;
    delete projectTab;
}

void FermaNextProject::loadFromFile ( const QString& fileName )
    throw (IOException, WrongXMLDocException, LoadException)
{
    QFile xmlFile( fileName );
    if ( ! xmlFile.open( QIODevice::ReadOnly ) )
        throw IOException();

    QIODevice* xmlIODev = &xmlFile;
    QDomDocument doc;
    if ( !doc.setContent( xmlIODev ) )
        throw WrongXMLDocException();

    QDomElement docElem = doc.documentElement();
    if ( docElem.isNull() )
        throw WrongXMLDocException();

    loadFromXML( docElem );

    setProjectFileName( fileName );
}

void FermaNextProject::saveToFile () throw (FileNameIsNotDefinedException, 
                                            IOException)
{
    const QString& fileName = getProjectFileName();
    if ( ! isFileNameDefined() )
        throw FileNameIsNotDefinedException();
    
    saveToFile( fileName );
}

void FermaNextProject::saveToFile ( const QString& fileName )
    throw (IOException)
{
    QFile xmlFile( fileName );
    if ( ! xmlFile.open( QIODevice::WriteOnly ) )
        throw IOException();

    QTextStream stream( &xmlFile );

    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );

    QDomElement prjElement = saveToXML( doc );

    doc.appendChild( prjElement );
    doc.save( stream, 4 );
    setProjectFileName( fileName );
}
    
const QString& FermaNextProject::getProjectFileName () const
{
    return projectFileName;
}

bool FermaNextProject::isFileNameDefined () const
{
    return (! projectFileName.isNull() && ! projectFileName.isEmpty() );
}

void FermaNextProject::setProjectFileName ( const QString& fileName )
{
    if ( projectFileName != fileName ) {
        projectFileName = fileName;
        emit onProjectFileNameChange( projectFileName );
    }
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
    // Windows in creation order
    WindowList trussWindows = 
        getTrussUnitWindowManager().getTrussUnitWindowList();

    // Windows in layout order
    WindowList layoutOrderedWindows = designerWidget->getTrussUnitWindowList();
    typedef QMap<TrussUnitWindow*, uint> LayoutOrderMap;
    
    // Build layout order. i.e. last window in the stack is a first
    // focused window to the user
    LayoutOrderMap layoutMap;
    WindowListIter orderIter = layoutOrderedWindows.begin();
    for ( uint order = 1; orderIter != layoutOrderedWindows.end(); 
          ++orderIter, ++order )
        layoutMap[*orderIter] = order;

    WindowListIter wIter = trussWindows.begin();
    for ( ; wIter != trussWindows.end(); ++wIter ) {
        TrussUnitWindow* trussWindow = *wIter;
        QDomElement trussWindowElem = trussWindow->saveToXML( doc );
        uint order = 0;
        if ( layoutMap.contains(trussWindow) )
            order = layoutMap[trussWindow];

        // Set layout order
        trussWindowElem.setAttribute( "layoutOrder", order );

        prjElem.appendChild( trussWindowElem );
    }

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
    // Layout order
    typedef QMap<uint, TrussUnitWindow*> LayoutOrderMap;
    LayoutOrderMap layoutMap;   

    TrussUnitWindowManager& mng = getTrussUnitWindowManager();
    QDomNodeList trussWindows = prjElem.elementsByTagName( "TrussUnitWindow" );
    for ( int windsNum = 0; windsNum < trussWindows.count(); ++windsNum ) {
        QDomNode trussWindow = trussWindows.item( windsNum );
        if ( ! trussWindow.isElement() )
            throw LoadException();
        QDomElement windowElem = trussWindow.toElement();
        TrussUnitWindow& wnd = mng.createTrussUnitWindow( QString::null );
        wnd.loadFromXML( windowElem );

        if ( windowElem.hasAttribute( "layoutOrder" ) ) {
            bool ok;
            uint order = windowElem.attribute( "layoutOrder" ).toUInt( &ok );
            if ( !ok ) LoadException();
            layoutMap[order] = &wnd;
        }
    }

    // Restore window order
    LayoutOrderMap::Iterator itOrder = layoutMap.begin();
    for ( ; itOrder != layoutMap.end(); ++itOrder ) {
        TrussUnitWindow* trussWindow = *itOrder;
        designerWidget->focusOnWindow( *trussWindow );
    }
}

void FermaNextProject::activate ()
{
    if ( stackedWidget && stackedWidget->currentWidget() != 
         projectTab ) {
        stackedWidget->setCurrentWidget( projectTab );
        emit onActivate( *this );
    }
}

bool FermaNextProject::isActivated () const
{
    return (stackedWidget && stackedWidget->currentWidget() == 
            projectTab);
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

TrussUnitDesignerWidget& FermaNextProject::getDesignerWidget ()
{
    return *designerWidget;
}

CalcDataToolBar& FermaNextProject::getCalcDataToolBar ()
{
    return *calcDataToolBar;
}

FermaNextWorkspace& FermaNextProject::getWorkspace()
{
    return currentWorkspace;
}

/*****************************************************************************/
