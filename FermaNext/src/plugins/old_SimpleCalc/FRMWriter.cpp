
#include "FRMWriter.h"

#include <fstream>
#include <iostream>


/*****************************************************************************
 * FRM Writer
 *****************************************************************************/

FRMWriter::FRMWriter ( const TrussUnit& truss_ ) :
    truss(truss_)
{}

void FRMWriter::write ( const QString& name )
{
    std::fstream file( name, std::ios::out );
    if ( ! file.is_open() ) {
        exit(1);  
    } 

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

    file << pivots.size() << "\n";
    file << nodes.size() << "\n";
    file << fixNum  << "\n";
    file << ' ' << QString::number(mat.getElasticityModule(), 'E', 14) << "\n";
    file << loadCases.countLoadCases() << "\n";
    file << ' ' << QString::number(mat.getWorkingStress(), 'E', 14)  << "\n";

    for ( pIter = pivots.begin(); pIter != pivots.end(); ++pIter ) {
        uint firstInd = 1, lastInd = 1;
        TrussPivot* p = *pIter;
        TrussNode& firstNode = p->getFirstNode();
        TrussNode& lastNode = p->getLastNode();
        for ( nIter = nodes.begin(); nIter != nodes.end();
              ++nIter, ++firstInd ) {
            if ( *nIter == &firstNode ) {
                file << firstInd << "\n";
                break;
            }            
        }
        for ( nIter = nodes.begin(); nIter != nodes.end(); 
              ++nIter, ++lastInd ) {
            if ( *nIter == &lastNode ) {
                file << lastInd << "\n";
                break;
            }            
        }
    }

    for ( nIter = nodes.begin(); nIter != nodes.end(); ++nIter ) {
        TrussNode* n = *nIter;
        file << ' ' << QString::number(n->getX(), 'E', 14) << "\n" << 
                ' ' << QString::number(n->getY(), 'E', 14) << "\n";
    }

    for ( pIter = pivots.begin(); pIter != pivots.end(); ++pIter ) {
        TrussPivot* p = *pIter;
        file << ' ' << QString::number(p->getThickness(),'E', 14) << "\n";
    }

    for ( nIter = nodes.begin(); nIter != nodes.end(); ++nIter ) {
        TrussNode* n = *nIter;
        Node::Fixation fix = n->getFixation();
        if ( fix == Node::FixationByX )
            file << 0 << "\n" << 1 << "\n";
        else if ( fix == Node::FixationByY )
            file << 1 << "\n" << 0 << "\n";
        else if ( fix == Node::FixationByXY )
            file << 0 << "\n" << 0 << "\n";
        else
            file << 1 << "\n" << 1 << "\n";
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
                file << ' ' << QString::number(l->getXForce(), 'E', 14)<<  "\n"
                     << ' ' << QString::number(l->getYForce(), 'E', 14)<< "\n";
            else
                file << ' ' << QString::number((double)0, 'E', 14)  <<  "\n"
                     << ' ' << QString::number((double)0, 'E', 14)  <<  "\n";
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
    file << length.ascii() << "\n"
         << force.ascii() << "\n"
         << ' ' << QString::number(ar.width(), 'E', 14) << "\n"
         << ' ' << QString::number(ar.height(), 'E', 14) << "\n";
}

/*****************************************************************************/
