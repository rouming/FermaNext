
#include <QFile>
#include <QRegExp>
#include <QMessageBox>

#include "NativePluginFrontEnd.h"
#include "TrussUnit.h"
#include "TrussCalcData.h"
#include "FRMWriter.h"
#include "VYVReader.h"

#include "old_SimpleCalc.h"

#include "simple_truss_calc.h"

/*****************************************************************************
 * Old Simple Calculation plugin (main export routines)
 *****************************************************************************/

FERMA_NEXT_PLUGIN(SimpleCalcPlugin)

/*****************************************************************************/

SimpleCalcPlugin::SimpleCalcPlugin ( PluginManager& mng, 
                                     const QString& path ) :
    NativePlugin( mng, path )
{ 
    createTempFile(); 
}

SimpleCalcPlugin::~SimpleCalcPlugin ()
{ 
    destroyTempFile(); 
}

Plugin::Status SimpleCalcPlugin::pluginStatusCode () const
{
    return OkStatus;
}

QString SimpleCalcPlugin::pluginStatusMsg () const
{
    return QString();
}

void SimpleCalcPlugin::tryToAcceptParams ( 
    const PluginExecutionParams& ) const
{ /* nothing */ }

void SimpleCalcPlugin::startCalculation (
	const QString& fileNameIn,
	const QString& fileNameOut ) const
{
	simple_truss_calc( fileNameIn.toAscii(),
					   fileNameOut.toAscii() );
}

const PluginInfo& SimpleCalcPlugin::pluginInfo () const
{ 
    static PluginInfo inf( "SimpleOldCalcPlugin",
                           "Just calculation",
                           "calculation.simple.native" );
    return inf; 
}

Plugin::ExecutionResult SimpleCalcPlugin::specificExecute ( 
    const PluginExecutionParams& params,
    const QList<UUIDObject*>& argsList,
    const QHash< QString, QList<Plugin*> >& deps  )
    /*throw (Plugin::WrongExecutionArgsException)*/
{
    Q_UNUSED(params);
    Q_UNUSED(deps);

    if ( argsList.size() != 1 )
        throw WrongExecutionArgsException(getUUID());
    
    TrussUnit* truss = dynamic_cast<TrussUnit*>(argsList[0]);
    if ( truss == 0 )
        throw WrongExecutionArgsException(getUUID());

    //---------------- Check if truss has no nodes

    if ( ! truss->countNodes() ) {
        QString errStr = tr("Can't solve empty truss");
        return ExecutionResult( InternalErrStatus , errStr ); 
    }
    

    //---------------- Check node/pivot limits

    if ( truss->countNodes() > 9 ) {
        QString errStr = tr( "Node number exceeds permissible limit.\n"
                             "Maximum number of nodes = 9");
        return ExecutionResult( InternalErrStatus , errStr ); 
    }

    else if ( truss->countNodes() > 15 ) {
        QString errStr = tr( "Pivot number exceeds permissible limit.\n"
                             "Maximum number of pivots = 15");
        return ExecutionResult( InternalErrStatus , errStr ); 
    }

    
    //---------------- Check free nodes 

    TrussNode* node = 0;
    TrussUnit::NodeList nodeList = truss->getNodeList();
    foreach ( node, nodeList ) {
        TrussUnit::PivotList adjPivots = truss->findAdjoiningPivots( *node );
        if ( adjPivots.empty() ) {
            QString errStr = tr("Truss contains free nodes");
            return ExecutionResult( InternalErrStatus , errStr );
        }
    }


    //---------------- Check if truss is a mechanism

    // k - number of ties
    int k = 0; 
    foreach ( node, nodeList ) {
        if ( node->getFixation() == Node::FixationByX ||
             node->getFixation() == Node::FixationByY )
            ++k;
        else if ( node->getFixation() == Node::FixationByXY )
            k += 2;
    }

    int w = 2 * truss->countNodes() - truss->countPivots() - k;
    
    if ( w > 0 ) {
        // the line below causes 28 unresolved externals =) 
        //QString trussName( truss->getTrussName() );

        QString errStr = tr("Construction is a mechanism");
        return ExecutionResult( InternalErrStatus , errStr );
    }

    //---------------- Check materials
    
    TrussPivot* pivot = 0;
    const TrussMaterial* cmpMaterial = 0;
    TrussUnit::PivotList pivotList = truss->getPivotList();
    foreach ( pivot, pivotList ) {
        const TrussMaterial* material = pivot->getMaterial();
        if ( ! material ){
            QString errStr( tr("Material was'n set for pivot %1").arg( 
                                pivot->getNumber() ) );
            return ExecutionResult( InternalErrStatus , errStr );
        } 
        else if ( cmpMaterial && cmpMaterial != material ) {
            QString errStr( tr("Plugin doesn't calculate trusses with the different types of materials") );
            return ExecutionResult( InternalErrStatus , errStr );
        }
        else
            cmpMaterial = material;
    }


    /*
    TODO: find out what's the reason of wrong countMaterials() result
    
    if ( truss->countMaterials() != 1 ) {
        QString errStr(  
         tr("Plugin doesn't calculate trusses with the different types of materials") );
        return ExecutionResult( InternalErrStatus , errStr );
    }*/
        
    QString frmFile( tempFileName() );    
    QString vyvFile( frmFile + fermaResExt );

    // To be sure
    QFile::remove( frmFile );
    QFile::remove( vyvFile );

    FRMWriter frm(truss);
    frm.write( frmFile );
    startCalculation( frmFile, vyvFile );
    VYVReader vyv;
    vyv.read( vyvFile );

    // Last clean
    QFile::remove( frmFile );
    QFile::remove( vyvFile );

    QString resultsStr = vyv.toXMLString();
    QDomDocument doc;
    if ( ! doc.setContent( resultsStr ) ) {
        QString errStr( tr("Wrong results format") );
        return ExecutionResult( InternalErrStatus , errStr );
    }

    QDomElement resultsElem = doc.firstChild().toElement();
    resultsElem.setAttribute( "trussUUID", truss->getUUID() );
    resultsElem.setAttribute( "pluginName", pluginInfo().name );

    if ( resultsElem.hasAttribute("materialVolume") ) {
        QString valueStr = resultsElem.attribute( "materialVolume" );
        const TrussMaterial* material = pivotList[0]->getMaterial();
        double mass = valueStr.toDouble() * material->getDensity();
        resultsElem.setAttribute( "trussMass", mass );
    }
    else 
        resultsElem.setAttribute( "trussMass", "unknown" );

    return ExecutionResult( OkStatus, doc.toString() );
}

void SimpleCalcPlugin::createTempFile ()
{  
    tempFile = tmpnam(0);    
    // Remove possible trailing extension
    tempFile.remove( QRegExp("\\..*$") );
}

void SimpleCalcPlugin::destroyTempFile () 
{ 
    QFile::remove( tempFile ); 
}

const QString& SimpleCalcPlugin::tempFileName () const
{ 
    return tempFile; 
}

/*****************************************************************************/
