
#include "TrussSolutionResults.h"
#include "TrussUnitCopy.h"

/*****************************************************************************
 * Truss Copy Node
 *****************************************************************************/

TrussCopyNode::TrussCopyNode ( const TrussNode& node )
{
    baseCoord = node.getPoint();
    currentCoord = baseCoord;
    fix = node.getFixation();
    number = node.getNumber();
    displacement = DoublePoint( 0, 0 );
}

TrussCopyNode::~TrussCopyNode ()
{}

void TrussCopyNode::setBaseCoord ( DoublePoint coord )
{
    baseCoord = coord;
}

const DoublePoint& TrussCopyNode::getBaseCoord () const
{
    return baseCoord;
}

void TrussCopyNode::setCurrentCoord ( DoublePoint coord )
{
    currentCoord = coord;
}

const DoublePoint& TrussCopyNode::getCurrentCoord () const
{
    return currentCoord;
}

void TrussCopyNode::setDisplacement ( DoublePoint disp )
{
    displacement = disp;
}

const DoublePoint& TrussCopyNode::getDisplacement () const
{
    return displacement;
}

int TrussCopyNode::getNumber () const
{
    return number;
}

Node::Fixation TrussCopyNode::getFixation () const
{
    return fix;
}

/*****************************************************************************
 * Truss Copy Pivot
 *****************************************************************************/

TrussCopyPivot::TrussCopyPivot ( TrussCopyNode& first_,
                                 TrussCopyNode& last_,
                                 double thick,
                                 int numb ) :
    first( first_ ),
    last( last_ ),
    thickness( thick ),
    number( numb )
{}

TrussCopyPivot::~TrussCopyPivot ()
{}

TrussCopyNode& TrussCopyPivot::getFirstNode () const
{
    return first;
}

TrussCopyNode& TrussCopyPivot::getLastNode () const
{
    return last;
}

void TrussCopyPivot::setStress ( double s )
{
    stress = s;
}

double TrussCopyPivot::getStress () const
{
    return stress;
}

double TrussCopyPivot::getThickness () const
{
    return thickness;
}

int TrussCopyPivot::getNumber () const
{
    return number;
}

/*****************************************************************************
 * Truss Unit Copy
 *****************************************************************************/

TrussUnitCopy::TrussUnitCopy () :
    loadCaseNumb( 0 ),
    materialNumb( 0 )
{}

TrussUnitCopy::~TrussUnitCopy ()
{
    clearNodes();
    clearPivots();
}

void TrussUnitCopy::clearNodes ()
{
    TrussCopyNodeListIter iter = nodes.begin();
    for ( ; iter != nodes.end(); ++iter )
        delete *iter;
    nodes.clear();
}

void TrussUnitCopy::clearPivots ()
{
    TrussCopyPivotListIter iter = pivots.begin();
    for ( ; iter != pivots.end(); ++iter )
        delete *iter;
    pivots.clear();
}

DoubleSize TrussUnitCopy::getTrussAreaSize () const
{
    return trussAreaSize;
}

TrussUnitCopy::TrussCopyNodeList TrussUnitCopy::getNodeList () const
{
    return nodes;
}

TrussUnitCopy::TrussCopyPivotList TrussUnitCopy::getPivotList () const
{
    return pivots;
}

int TrussUnitCopy::countNodes () const
{
    return nodes.size();
}

int TrussUnitCopy::countPivots () const
{
    return pivots.size();
}

int TrussUnitCopy::countLoadCases () const
{
    return loadCaseNumb;
}

int TrussUnitCopy::countMaterials () const
{
    return materialNumb;
}

TrussCopyNode* TrussUnitCopy::findNodeByNumber ( int numb ) const
{
    TrussCopyNodeList::const_iterator iter = nodes.begin();
    for ( ; iter < nodes.end(); ++iter ) {
        TrussCopyNode* n = *iter;
        if ( n->getNumber() == numb )
            return n;
    }
    return 0;
}

void TrussUnitCopy::loadTrussUnitData ( const TrussUnit& truss )
{
    clearNodes();
    clearPivots();
    
    // load node data
    TrussUnit::NodeList trussNodeList = truss.getNodeList();
    TrussUnit::NodeListIter nodeIter = trussNodeList.begin();
    for ( ; nodeIter < trussNodeList.end(); ++nodeIter )
        nodes.push_back( new TrussCopyNode( **nodeIter ) );

    // load pivot data
    TrussUnit::PivotList trussPivotList = truss.getPivotList();
    TrussUnit::PivotListIter pivotIter = trussPivotList.begin();
    for ( ; pivotIter < trussPivotList.end(); ++pivotIter ) {
        TrussPivot& p = **pivotIter;
        TrussCopyNode* first = findNodeByNumber( p.getFirstNode().getNumber() );
        TrussCopyNode* last = findNodeByNumber( p.getLastNode().getNumber() );
        if ( ! first || ! last )
            continue;
        pivots.push_back( new TrussCopyPivot( *first, *last, p.getThickness(), 
                                              p.getNumber() ) );
    }
    trussAreaSize = truss.getTrussAreaSize();
    loadCaseNumb = truss.getLoadCases().countLoadCases();
    materialNumb = truss.countMaterials();
    
    emit trussDataLoaded();
    emit stateIsChanged();
}

void TrussUnitCopy::loadResults ( const LoadCaseResults& res )
{
    if ( res.countDisplacements() != 0 )
        emit displacementLoaded( true );
    else
        emit displacementLoaded( false );
    loadDisplacements( res ); 
    
    if ( res.countStresses() != 0 )
        emit stressLoaded( true );
    else
        emit stressLoaded( false );
    loadStresses( res );
    emit stateIsChanged();
}

void TrussUnitCopy::loadDisplacements ( const LoadCaseResults& res )
{
    TrussCopyNode* node = 0;
    TrussCopyNodeListIter nIter = nodes.begin();
    for ( ; nIter != nodes.end(); ++nIter ) {
        node = *nIter;
        bool valid;
        DoublePoint disp = res.getDisplacement( node->getNumber() - 1, 
                                                valid );
        if ( ! valid ) {
            node->setDisplacement( DoublePoint( 0, 0 ) );
            node->setCurrentCoord( node->getBaseCoord() );
            continue;
        }
        node->setDisplacement( disp );
    }
}

void TrussUnitCopy::loadStresses ( const LoadCaseResults& res )
{
    TrussCopyPivot* pivot = 0;
    TrussCopyPivotListIter pIter = pivots.begin();
    for ( ; pIter != pivots.end(); ++pIter ) {
        pivot = *pIter;
        bool valid;
        double stress = res.getStress( pivot->getNumber() - 1, valid );
        if ( ! valid ) {
            pivot->setStress( 0 );
            continue;
        }
        pivot->setStress( stress );
    }
}

void TrussUnitCopy::displaceNodes ( double koeff )
{
    double dx = 0, dy = 0;
    TrussCopyNode* node = 0;
    TrussCopyNodeListIter iter = nodes.begin();
    for ( ; iter != nodes.end(); ++iter ) {
        node = *iter;
        dx = node->getDisplacement().x() * koeff;
        dy = node->getDisplacement().y() * koeff;
        node->setCurrentCoord( node->getBaseCoord() + DoublePoint( dx, dy ) );       
    }
    emit stateIsChanged();
}

void TrussUnitCopy::restoreNodesBasePosition ()
{
    TrussCopyNodeListIter iter = nodes.begin();
    for ( ; iter != nodes.end(); ++iter ) {
        TrussCopyNode* node = *iter;
        node->setCurrentCoord( node->getBaseCoord() );
    }
    emit stateIsChanged();
}

/*****************************************************************************/
