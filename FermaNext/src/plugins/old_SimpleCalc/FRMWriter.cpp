
#include "FRMWriter.h"

#include <qfile.h>

/*****************************************************************************
 * FRM Writer
 *****************************************************************************/

FRMWriter::FRMWriter ( const TrussUnit& truss_ ) :
    truss(truss_)
{}

void FRMWriter::write ( const QString& name )
{
    QFile file( name );    
    if ( ! file.open( IO_WriteOnly ) ) {
        return;
    } 

    QTextStream out( &file );
    out.setEncoding( QTextStream::Latin1 );

    TrussUnit::NodeList nodes = truss.getNodeList();
    TrussUnit::PivotList pivots = truss.getPivotList();
    
    TrussUnit::NodeListIter nIter;
    TrussUnit::PivotListIter pIter;

    const TrussMaterial& mat = truss.getMaterial();
    const TrussUnitLoadCases& loadCases = truss.getLoadCases();

    uint fixNum = 0;
    for ( nIter = nodes.begin(); nIter != nodes.end(); ++nIter )
        if ( (*nIter)->getFixation() != Node::Unfixed )
            ++fixNum;

    out << pivots.size() << "\n";
    out << nodes.size() << "\n";
    out << fixNum  << "\n";
    out << QString::number(mat.getElasticityModule(), 'E', 14) << "\n";
    out << loadCases.countLoadCases() << "\n";
    out << QString::number(mat.getWorkingStress(), 'E', 14)  << "\n";

    for ( pIter = pivots.begin(); pIter != pivots.end(); ++pIter ) {
        uint firstInd = 1, lastInd = 1;
        TrussPivot* p = *pIter;
        TrussNode& firstNode = p->getFirstNode();
        TrussNode& lastNode = p->getLastNode();
        for ( nIter = nodes.begin(); nIter != nodes.end();
              ++nIter, ++firstInd ) {
            if ( *nIter == &firstNode ) {
                out << firstInd << "\n";
                break;
            }            
        }
        for ( nIter = nodes.begin(); nIter != nodes.end(); 
              ++nIter, ++lastInd ) {
            if ( *nIter == &lastNode ) {
                out << lastInd << "\n";
                break;
            }            
        }
    }

    for ( nIter = nodes.begin(); nIter != nodes.end(); ++nIter ) {
        TrussNode* n = *nIter;
        out << QString::number(n->getX(), 'E', 14) << "\n" << 
               QString::number(n->getY(), 'E', 14) << "\n";
    }

    for ( pIter = pivots.begin(); pIter != pivots.end(); ++pIter ) {
        TrussPivot* p = *pIter;
        out << QString::number(p->getThickness(),'E', 14) << "\n";
    }

    for ( nIter = nodes.begin(); nIter != nodes.end(); ++nIter ) {
        TrussNode* n = *nIter;
        Node::Fixation fix = n->getFixation();
        if ( fix == Node::FixationByX )
            out << 0 << "\n" << 1 << "\n";
        else if ( fix == Node::FixationByY )
            out << 1 << "\n" << 0 << "\n";
        else if ( fix == Node::FixationByXY )
            out << 0 << "\n" << 0 << "\n";
        else
            out << 1 << "\n" << 1 << "\n";
    }

    for ( uint loadInd = 1; loadInd <= loadCases.countLoadCases(); 
          ++loadInd ) {
        TrussUnitLoadCase* loadCase = loadCases.findLoadCase( loadInd );
        if ( loadCase == 0 )
            continue;
        for ( nIter = nodes.begin(); nIter != nodes.end(); ++nIter ) {
            TrussNode* n = *nIter;
            TrussLoad* l = loadCase->findLoad( *n );
            if ( l )
                out << QString::number(l->getXForce(), 'E', 14)<< "\n"
                    << QString::number(l->getYForce(), 'E', 14)<< "\n";
            else
                out << QString::number((double)0, 'E', 14)  << "\n"
                    << QString::number((double)0, 'E', 14)  << "\n";
        }
    }

    const TrussDimension& dim = truss.getDimension();

    QString length;
    if ( dim.getLengthMeasure() == TrussDimension::mm )
        length = "לל";
    else if ( dim.getLengthMeasure() == TrussDimension::sm )
        length = "סל";
    else 
        length = "Ì";

    QString force;
    if ( dim.getForceMeasure() == TrussDimension::newton )
        force = "Í";
    else 
        force = "ךÃ";

    const DoubleSize& ar = truss.getTrussAreaSize();
    out << length << "\n"
        << force << "\n"
        << QString::number(ar.width(), 'E', 14) << "\n"
        << QString::number(ar.height(), 'E', 14) << "\n";
}

/*****************************************************************************/
