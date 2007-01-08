
#include <QTabWidget>
#include <QStackedWidget>
#include <QTextStream>
#include <QVBoxLayout>
#include <QFrame>

#include "AggTrussToolBar.h"
#include "FermaNextProject.h"
#include "FermaNextWorkspace.h"
#include "Global.h"
#include "TrussResultsManager.h"

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
    projectWidget( new QFrame(stackedWidget) ),
    designerWidget( new TrussDesignerWidget ),
    materialLibrary( new TrussMaterialLibrary ),
    trussWindowManager( new TrussUnitWindowManager( *materialLibrary ) ),
    trussResultsManager( new TrussResultsManager( wsp.pluginManager(), 
                                                  *trussWindowManager ) ),
    isModified(false),
    silenceMode(true)
{
    connect( materialLibrary, 
             SIGNAL(onBeforeMaterialRemoval(const TrussMaterial&)), 
             trussWindowManager, 
             SIGNAL(beforeMaterialRemove(const TrussMaterial&)) );

    QVBoxLayout* designerLayout = new QVBoxLayout( projectWidget );
    designerLayout->addWidget( designerWidget );
    designerLayout->setMargin(0);

    // Should be hidden while creating other widgets
    // Only for aesthetic purposes 
    projectWidget->setVisible(false);
    designerWidget->setVisible(true);

    projectWidget->setFrameStyle( QFrame::Raised | QFrame::Panel );
    stackedWidget->addWidget(projectWidget);

    // Catch trusses creation or deletion.
    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             designerWidget, SLOT(addTrussUnitWindow(TrussUnitWindow&)) );
    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             designerWidget, SLOT(removeTrussUnitWindow(TrussUnitWindow&)) );

    designerWidget->getToolBar().setMaterialLibrary( *materialLibrary );

    // Catch modified signals
    connect( designerWidget, SIGNAL(modified()),
                               SLOT(projectModified()) );
    
    connect( materialLibrary, SIGNAL(libraryModified()),
                                SLOT(projectModified()) );

    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowCreate(TrussUnitWindow&)), 
             SLOT(projectModified()) );

    connect( trussWindowManager, 
             SIGNAL(onTrussUnitWindowRemove(TrussUnitWindow&)), 
             SLOT(projectModified()) );

    connect( trussResultsManager, 
             SIGNAL(onAfterCreateSolutionResults(TrussSolutionResults&)), 
             SLOT(projectModified()) );

    connect( trussResultsManager, 
             SIGNAL(onAfterRemoveSolutionResults()), 
             SLOT(projectModified()) );

    // Create default materials
    TrussMaterial& m = materialLibrary->createMaterial( 
                    30000, 7000000, 0.028, tr( "Aluminum Alloy" ) );
    materialLibrary->createMaterial( 70000, 20000000, 0.078, tr( "Steel" ) );
    materialLibrary->selectMaterial( m );

    silenceMode = false;
}

FermaNextProject::~FermaNextProject ()
{
    if ( stackedWidget )
        stackedWidget->removeWidget( projectWidget );
    delete trussWindowManager;
    delete projectWidget;
    delete materialLibrary;
}

void FermaNextProject::loadFromFile ( const QString& fileName )
    /*throw (IOException, WrongXMLDocException, LoadException)*/
{
    silenceMode = true;

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

    silenceMode = false;
}

void FermaNextProject::saveToFile () /*throw (FileNameIsNotDefinedException, 
                                              IOException)*/
{
    if ( ! isFileNameDefined() )
        throw FileNameIsNotDefinedException();
    saveToFile( getProjectFileName() );
}

void FermaNextProject::saveToFile ( const QString& fileName )
    /*throw (IOException)*/
{
    QFile xmlFile( fileName );
    if ( ! xmlFile.open( QIODevice::WriteOnly ) )
        throw IOException();

    setProjectFileName( fileName );

    QTextStream stream( &xmlFile );

    QDomDocument doc;
    QDomNode xmlInstr = doc.createProcessingInstruction(
                        "xml", QString("version=\"1.0\" encoding=\"UTF8\"") );
    doc.insertBefore( xmlInstr, doc.firstChild() );
    
    QDomElement prjElement = saveToXML( doc );
    doc.appendChild( prjElement );
    doc.save( stream, 4 );

    projectSaved();
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

    /** 
     * Save truss material library
     ******************************/
    QDomElement materialLibElem = doc.createElement( "TrussMaterialLibrary" );
    prjElem.appendChild( materialLibrary->saveToXML( doc ) );

    /**
     * Save truss solution results
     ******************************/
    TrussResultsList resultsList = 
        getTrussResultsManager().getResultsList();

    TrussSolutionResults* results = 0;
    foreach ( results, resultsList ) {
        WindowList windows = 
            getTrussUnitWindowManager().getTrussUnitWindowList();
        // save only actual results
        foreach ( TrussUnitWindow* w, windows )
            if ( w->getUUID() == results->getTrussUnitUUID() ) {
                if ( ! w->isCalculated() )
                    continue;
                else {
                    QDomElement resultsElem = results->saveToXML( doc );
                    prjElem.appendChild( resultsElem );
                }
            }
    }
    return prjElem;
}

void FermaNextProject::loadFromXML ( const QDomElement& prjElem ) 
    /*throw (LoadException)*/
{
    XMLSerializableObject::loadFromXML( prjElem );

    /**
     * Set project name
     ********************/
    if ( ! prjElem.hasAttribute( "name" ) )
        throw LoadException();
    setName( prjElem.attribute( "name" ) );

    /**
     * Create truss material library
     *********************************/
    QDomNodeList materialLib = 
        prjElem.elementsByTagName( "TrussMaterialLibrary" );
    if ( materialLib.count() != 1 )
        throw LoadException();
    if ( ! materialLib.item(0).isElement() )
        throw LoadException();

    QDomElement materialLibElem = materialLib.item(0).toElement();
    materialLibrary->loadFromXML( materialLibElem );
    
    /**
     * Create truss unit windows
     *****************************/
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

    /**
     * Create truss solution results
     *********************************/
    TrussResultsManager& resMng = getTrussResultsManager();
    QDomNodeList trussResultsList = 
        prjElem.elementsByTagName( "TrussSolutionResults" );

    for ( int i = 0; i < trussResultsList.count(); ++i ) {
        QDomNode trussResultsNode = trussResultsList.item( i );
        if ( ! trussResultsNode.isElement() )
            throw LoadException();
        QDomElement trussResultsElem = trussResultsNode.toElement();
        TrussSolutionResults& res = resMng.createSolutionResults();
        res.loadFromXML( trussResultsElem );
    }
}

void FermaNextProject::activate ()
{
    if ( stackedWidget && 
         stackedWidget->currentWidget() != projectWidget ) {
        stackedWidget->setCurrentWidget( projectWidget );
        emit onActivate( *this );
    }
}

bool FermaNextProject::isActivated () const
{
    return (stackedWidget && stackedWidget->currentWidget() == 
            projectWidget);
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

bool FermaNextProject::isProjectModified () const
{
    return isModified;
}

void FermaNextProject::projectModified ()
{
    if ( isModified || silenceMode )
        return;

    isModified = true;
    emit modified();
}

void FermaNextProject::projectSaved ()
{
    if ( ! isModified )
        return;

    isModified = false;
    emit saved();
}

TrussUnitWindowManager& FermaNextProject::getTrussUnitWindowManager ()
{
    return *trussWindowManager;
}

TrussResultsManager& FermaNextProject::getTrussResultsManager ()
{
    return *trussResultsManager;
}

TrussDesignerWidget& FermaNextProject::getDesignerWidget ()
{
    return *designerWidget;
}

FermaNextWorkspace& FermaNextProject::getWorkspace()
{
    return currentWorkspace;
}

TrussMaterialLibrary& FermaNextProject::getMaterialLibrary () const
{
    return *materialLibrary;
}

/*****************************************************************************/
