
#ifndef TRUSS_H
#define TRUSS_H

#include <vector>
#include <algorithm>

#include <QObject>
#include <QMap>
#include <QPoint>
#include <QSize>
#include <QMetaType>

#include "Geometry.h"
#include "StatefulObject.h"
#include "TrussLoad.h"
#include "TrussMaterial.h"

// Basic classes for truss unit construction.
template <class N, class P> class Truss;
template <class N> class Pivot;
class Node;
typedef Truss< Node, Pivot<Node> > TrussTopology;

/*****************************************************************************
 * Truss Dimension
 *****************************************************************************/

class TrussDimension
{
public:
    // Exceptions
    class WrongArgsException {};

    enum LengthMeasure { mm = 0, sm, m };  //millimeter, centimeter, meter
    enum ForceMeasure  { newton = 0, kg };

    TrussDimension ();
    TrussDimension ( LengthMeasure, ForceMeasure );
    // Try to parse arguments
    TrussDimension ( const QString& lengthMeasure,
                     const QString& forceMeasure )
        /*throw (WrongArgsException)*/;

    TrussDimension& operator= ( const TrussDimension& );

    LengthMeasure getLengthMeasure () const;
    ForceMeasure getForceMeasure () const;

    // String representation of measures
    QString getLengthMeasureStr () const;
    QString getForceMeasureStr () const;

    void setLengthMeasure ( LengthMeasure );
    void setForceMeasure ( ForceMeasure );
private:
    LengthMeasure length;
    ForceMeasure force;
};

/*****************************************************************************
 * Truss Emitter
 *****************************************************************************/

// Truss emitter. All signals logic was moved to this
// classe because of Qt rejection to moc templates.

class TrussEmitter : public StatefulObject
{
    Q_OBJECT
public:
    TrussEmitter ( ObjectStateManager* mng );

public slots:
    virtual void setTrussAreaSize ( const DoubleSize& ) = 0;

protected slots:
    void stateIsChanged ();

    virtual void nodeBeforeRevive ( StatefulObject& ) = 0;
    virtual void nodeAfterRevive ( StatefulObject& ) = 0;
    virtual void nodeBeforeDesist ( StatefulObject& ) = 0;
    virtual void nodeAfterDesist ( StatefulObject& ) = 0;

    virtual void pivotBeforeRevive ( StatefulObject& ) = 0;
    virtual void pivotAfterRevive ( StatefulObject& ) = 0;
    virtual void pivotBeforeDesist ( StatefulObject& ) = 0;
    virtual void pivotAfterDesist ( StatefulObject& ) = 0;

    virtual void topologyBeforeRevive ( StatefulObject& ) = 0;
    virtual void topologyAfterRevive ( StatefulObject& ) = 0;
    virtual void topologyBeforeDesist ( StatefulObject& ) = 0;
    virtual void topologyAfterDesist ( StatefulObject& ) = 0;

    virtual void removePivotMaterial ( const TrussMaterial& mat ) = 0;

signals:
    // Truss signals
    void onStateChange ();
    void onAreaChange ( const DoubleSize& );
    void onSwitchLoadCase ();

    // Nodes create/remove signals
    void beforeNodeCreation ();
    void afterNodeCreation ( const Node& );
    void beforeNodeRemoval ( const Node& );
    void afterNodeRemoval ();

    // Nodes revive/desist signals
    void beforeNodeRevive ( const Node& );
    void afterNodeRevive ( const Node& );
    void beforeNodeDesist ( const Node& );
    void afterNodeDesist ( const Node& );

    // Pivots create/remove signals
    void beforePivotCreation ();
    void afterPivotCreation ( const Node&, const Node& );
    void beforePivotRemoval ( const Node&, const Node& );
    void afterPivotRemoval ();

    // Pivots revive/desist signals
    void beforePivotRevive ( const Node&, const Node& );
    void afterPivotRevive ( const Node&, const Node& );
    void beforePivotDesist ( const Node&, const Node& );
    void afterPivotDesist ( const Node&, const Node& );

    // Topologies create/remove signals
    void beforeTopologyCreation ();
    void afterTopologyCreation ( const TrussTopology& );
    void beforeTopologyRemoval ( const TrussTopology& );
    void afterTopologyRemoval ();

    // Topologies revive/desist signals
    void beforeTopologyRevive ( const TrussTopology& );
    void afterTopologyRevive ( const TrussTopology& );
    void beforeTopologyDesist ( const TrussTopology& );
    void afterTopologyDesist ( const TrussTopology& );
};

/*****************************************************************************
 * Truss
 *****************************************************************************/

template <class N, class P>
class Truss : public TrussEmitter
{
public:
    // Truss exceptions
    class NodeIndexOutOfBoundException {};

    // Basic typedefs
    typedef std::vector<N*> NodeList;
    typedef std::vector<P*> PivotList;
    typedef std::vector<TrussTopology*> TopologyList;
    typedef TrussLoadCaseArray<N> LoadCases;
    typedef TrussLoadCase<N> LoadCase;
    // Node iterators
    typedef typename NodeList::iterator NodeListIter;
    typedef typename NodeList::const_iterator NodeListConstIter;
    // Pivot iterators
    typedef typename PivotList::iterator PivotListIter;
    typedef typename PivotList::const_iterator PivotListConstIter;
    // Topology iterators
    typedef typename TopologyList::iterator TopologyListIter;
    typedef typename TopologyList::const_iterator TopologyListConstIter;

    Truss ( ObjectStateManager* mng, const TrussMaterialLibrary& mLib ) :
        TrussEmitter(mng),
        materialLibrary(mLib)
    {
        // Signal connects to catch life time changing and deligate them
        // to common 'onStateChange' signal
        QObject::connect( this, SIGNAL(onAfterRevive(StatefulObject&)),
                                 SLOT(stateIsChanged()) );
        QObject::connect( this, SIGNAL(onAfterDesist(StatefulObject&)),
                                 SLOT(stateIsChanged()) );

        QObject::connect( &loadCases, SIGNAL(afterLoadCaseCreation(int)),
                                      SIGNAL(onStateChange()) );
        QObject::connect( &loadCases, SIGNAL(afterLoadCaseRemoval()),
                                      SIGNAL(onStateChange()) );
        QObject::connect( &loadCases, SIGNAL(currentLoadCaseChanged(int)),
                                      SIGNAL(onSwitchLoadCase()) );

        QObject::connect( &loadCases, SIGNAL(onTrussLoadCreate(const Node&)),
                                      SIGNAL(onStateChange()) );
        QObject::connect( &loadCases, SIGNAL(onTrussLoadRemove(const Node&)),
                                      SIGNAL(onStateChange()) );
        QObject::connect( &loadCases, SIGNAL(onTrussLoadChange(const Node&)),
                                      SIGNAL(onStateChange()) );

        loadCases.createLoadCase();
    }

    virtual void clear ()
    {
        NodeListIter itNode;
        PivotListIter itPivot;
        TopologyListIter itTopology;
        for ( itNode = nodes.begin();
              itNode != nodes.end(); )
            itNode = removeNode( itNode );
        for ( itPivot = pivots.begin();
              itPivot != pivots.end(); )
            itPivot = removePivot( itPivot );
        for ( itTopology = topologies.begin();
              itTopology != topologies.end(); )
            itTopology = removeTopology( itTopology );
    }

    virtual ~Truss ()
    {
        clear();
    }

    virtual N* findNodeByCoord ( const DoublePoint& point ) const
    {
        NodeListConstIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
        {
            N* node = *iter;
            if ( !node->isAlive() )
                continue;
            if ( node->getPoint() == point )
                return node;
        }
        return 0;
    }

    virtual N* findNodeByCoord ( const DoublePoint& point,
                                 double precision ) const
    {
        NodeListConstIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
        {
            N* node = *iter;
            if ( !node->isAlive() )
                continue;
            const DoublePoint& pos = node->getPoint();
            if ( ( (point.x() - pos.x()) * (point.x() - pos.x()) +
                   (point.y() - pos.y()) * (point.y() - pos.y()) ) <
                 sqrt( precision ) )
                return node;
        }
        return 0;
    }

    virtual N* findNodeByNumber ( uint num ) const
    {
        NodeListConstIter iter = nodes.begin();
        for ( uint i = 1; iter != nodes.end(); ++iter )
        {
            if ( (*iter)->isAlive() && i++ == num )
                return *iter;
        }
        return 0;
    }

    virtual P* findPivotByNumber ( uint num ) const
    {
        PivotListConstIter iter = pivots.begin();
        for ( uint i = 1; iter != pivots.end(); ++iter )
        {
            if ( (*iter)->isAlive() && i++ == num )
                return *iter;
        }
        return 0;
    }

    virtual P* findPivotByNodes ( const N& node1, const N& node2 ) const
    {
        PivotListConstIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter )
        {
            P* pivot = (*iter);
            if ( ! pivot->isAlive() )
                continue;
            if ( (&(pivot->getFirstNode()) == &node1 &&
                  &(pivot->getLastNode()) == &node2) ||
                 (&(pivot->getFirstNode()) == &node2 &&
                  &(pivot->getLastNode()) == &node1) ) {
                return pivot;
            }
        }
        return 0;
    }

    // Returns only alive pivots
    virtual PivotList findAdjoiningPivots ( const N& node ) const
    {
        return findAdjoiningPivots( node, true );
    }

    virtual N& createNode ( const DoublePoint& p )
    {
        return createNode( p.x(), p.y() );
    }

    virtual N& createNode ( double x, double y )
    {
        // Clean earlier desisted truss objects
        suspendedClean();

        emit beforeNodeCreation();
        N* node = new N(getStateManager());
        node->setPoint( x, y );
        nodes.push_back(node);

        // Signal connects to be the first and to catch life time changing
        QObject::connect( node, SIGNAL(onBeforeRevive(StatefulObject&)),
                                 SLOT(nodeBeforeRevive(StatefulObject&)) );
        QObject::connect( node, SIGNAL(onAfterRevive(StatefulObject&)),
                                 SLOT(nodeAfterRevive(StatefulObject&)) );
        QObject::connect( node, SIGNAL(onBeforeDesist(StatefulObject&)),
                                 SLOT(nodeBeforeDesist(StatefulObject&)) );
        QObject::connect( node, SIGNAL(onAfterDesist(StatefulObject&)),
                                 SLOT(nodeAfterDesist(StatefulObject&)) );
        // Just subsidiary catches
        QObject::connect( node, SIGNAL(onFixationChange ( Fixation )),
                                SLOT(stateIsChanged()) );
        QObject::connect( node, SIGNAL(onPositionChange(double, double)),
                                SLOT(stateIsChanged()) );

        // Force reindex of elements numbers
        reindexNodesPivotsNumbers();

        emit afterNodeCreation(*node);
        emit onStateChange();
        return *node;
    }

    virtual P& createPivot ( uint firstNodeIndex, uint lastNodeIndex )
        /*throw (NodeIndexOutOfBoundException)*/
    {
        if ( firstNodeIndex >= nodes.size() || lastNodeIndex >= nodes.size() )
            throw NodeIndexOutOfBoundException();

        N* first = nodes.at(firstNodeIndex);
        N* last  = nodes.at(lastNodeIndex);

        return createPivot( *first, *last );
    }

    virtual P& createPivot ( N& first, N& last )
    {
        // Clean earlier desisted truss objects
        suspendedClean();

        emit beforePivotCreation();
        P* pivot = new P( first, last, getStateManager(), materialLibrary );

        // Signal connects to be the first and to catch life time changing
        QObject::connect( pivot, SIGNAL(onBeforeRevive(StatefulObject&)),
                                 SLOT(pivotBeforeRevive(StatefulObject&)) );
        QObject::connect( pivot, SIGNAL(onAfterRevive(StatefulObject&)),
                                 SLOT(pivotAfterRevive(StatefulObject&)) );
        QObject::connect( pivot, SIGNAL(onBeforeDesist(StatefulObject&)),
                                 SLOT(pivotBeforeDesist(StatefulObject&)) );
        QObject::connect( pivot, SIGNAL(onAfterDesist(StatefulObject&)),
                                 SLOT(pivotAfterDesist(StatefulObject&)) );
        // Just subsidiary catches
        QObject::connect( pivot, SIGNAL(onThicknessChange(double)),
                                 SLOT(stateIsChanged()) );
        QObject::connect( pivot, SIGNAL(onMaterialChange()),
                                 SLOT(stateIsChanged()) );
        QObject::connect( pivot, SIGNAL(onFirstNodeChange()),
                                 SLOT(stateIsChanged()) );
        QObject::connect( pivot, SIGNAL(onLastNodeChange()),
                                 SLOT(stateIsChanged()) );

        pivots.push_back(pivot);

        // Force reindex of elements numbers
        reindexNodesPivotsNumbers();

        emit afterPivotCreation(pivot->getFirstNode(),
                                pivot->getLastNode());
        emit onStateChange();

        return *pivot;
    }

    // Momentary removing of an object. Nothing can revive it.
    virtual bool removePivot ( const P& pivot )
    {
        // Clean earlier desisted truss objects
        suspendedClean();
        PivotListIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter )
            if ( (*iter) == &pivot ) {
                // Desist first
                (*iter)->desist();
                iter = removePivot(iter);
                if ( iter == pivots.end() )
                    return false;
                return true;
            }
        return false;
    }

    // Momentary removing of an object. Nothing can revive it.
    virtual bool removeNode ( const N& node )
    {
        // Clean earlier desisted truss objects
        suspendedClean();
        NodeListIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
            if ( (*iter) == &node ) {
                // Desist first
                (*iter)->desist();
                iter = removeNode(iter);
                if ( iter == nodes.end() )
                    return false;
                return true;
            }
        return false;
    }

    // Momentary removing of an object. Nothing can revive it.
    virtual bool removeTopology ( const TrussTopology& topology )
    {
        // Clean earlier desisted truss objects
        suspendedClean();
        TopologyListIter iter = topologies.begin();
        for ( ; iter != topologies.end(); ++iter )
            if ( (*iter) == &topology ) {
                // Desist first
                (*iter)->desist();
                iter = removeTopology(iter);
                if ( iter == topologies.end() )
                    return false;
                return true;
            }
        return false;
    }

    virtual uint countNodes () const
    {
        uint nodesNum = 0;
        NodeListConstIter nIter = nodes.begin();
        for ( ; nIter != nodes.end(); ++nIter ) {
            N* node = *nIter;
            if ( node->isAlive() )
                ++nodesNum;
        }
        return nodesNum;
    }

    virtual uint countPivots () const
    {
        uint pivotsNum = 0;
        PivotListConstIter pIter = pivots.begin();
        for ( ; pIter != pivots.end(); ++pIter ) {
            P* pivot = *pIter;
            if ( pivot->isAlive() )
                ++pivotsNum;
        }
        return pivotsNum;
    }

    virtual uint countTopologies () const
    {
        uint topologiesNum = 0;
        TopologyListConstIter tIter = topologies.begin();
        for ( ; tIter != topologies.end(); ++tIter ) {
            TrussTopology* topology = *tIter;
            if ( topology->isAlive() )
                ++topologiesNum;
        }
        return topologiesNum;
    }

    virtual uint countMaterials () const
    {
        QList<const TrussMaterial*> materials;
        const TrussMaterial* material = 0;
        PivotListConstIter pIter = pivots.begin();
        for ( ; pIter != pivots.end(); ++pIter ) {
            P* pivot = *pIter;
            material = pivot->getMaterial();
            if ( ! material )
                continue;

            QList<const TrussMaterial*>::const_iterator mIter =
                std::find( materials.begin(), materials.end(),
                                              material );
            if ( mIter == materials.end() )
                materials.push_back( material );
        }
        return materials.size();
    }

    // Returns alive nodes
    virtual NodeList getNodeList () const
    {
        NodeList aliveNodes;
        NodeListConstIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter ) {
            N* node = *iter;
            if ( node->isAlive() ) {
                aliveNodes.push_back( node );
            }
        }
        return aliveNodes;
    }

    // Returns alive pivots
    virtual PivotList getPivotList () const
    {
        PivotList alivePivots;
        PivotListConstIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) {
            P* pivot = *iter;
            if ( pivot->isAlive() ) {
                alivePivots.push_back( pivot );
            }
        }
        return alivePivots;
    }

    // Returns alive topologies
    virtual TopologyList getTopologyList () const
    {
        TopologyList aliveTopologies;
        TopologyListConstIter iter = topologies.begin();
        for ( ; iter != topologies.end(); ++iter ) {
            TrussTopology* topology = *iter;
            if ( topology->isAlive() ) {
                aliveTopologies.push_back( topology );
            }
        }
        return aliveTopologies;
    }

    virtual const DoubleSize& getTrussAreaSize () const
    {
        return trussAreaSize;
    }

    virtual void setTrussAreaSize ( const DoubleSize& area )
    {
        trussAreaSize = area;
        emit onAreaChange( trussAreaSize );
        emit onStateChange();
    }

    virtual const LoadCases& getLoadCases () const
    {
        return loadCases;
    }

    virtual LoadCases& getLoadCases ()
    {
        return loadCases;
    }

    virtual const TrussDimension& getDimension () const
    {
        return dimension;
    }

    virtual TrussDimension& getDimension ()
    {
        return dimension;
    }

    virtual void setDimension ( const TrussDimension& dim )
    {
        dimension = dim;
    }

protected:
/****************************** nodes ****************************************/

    virtual void nodeBeforeRevive ( StatefulObject& st )
    {
        // Safe conversion
        try {
            N& node = dynamic_cast<N&>(st);
            emit beforeNodeRevive(node);
        }
        catch ( ... ) { return; }
    }

    virtual void nodeAfterRevive ( StatefulObject& st )
    {
        // Safe conversion
        try {
            N& node = dynamic_cast<N&>(st);
            // Force reindex of elements numbers
            reindexNodesPivotsNumbers();
            emit afterNodeRevive(node);
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

    // Desist all adjoining pivots
    virtual void nodeBeforeDesist ( StatefulObject& st )
    {
        // Safe conversion
        try {
            N& node = dynamic_cast<N&>(st);
            PivotList pivotsToDesist = findAdjoiningPivots( node, false );
            PivotListIter iter = pivotsToDesist.begin();
            for ( ; iter != pivotsToDesist.end(); ++iter ) {
                P* pivot = *iter;
                if ( pivot->isAlive() )
                    (*iter)->desist();
            }
            emit beforeNodeDesist(node);
        }
        catch ( ... ) { return; }
    }

    virtual void nodeAfterDesist ( StatefulObject& st )
    {
        // Safe conversion
        try {
            N& node = dynamic_cast<N&>(st);
            // Force reindex of elements numbers
            reindexNodesPivotsNumbers();
            emit afterNodeDesist(node);
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

/****************************** pivots ***************************************/

    virtual void pivotBeforeRevive ( StatefulObject& st )
    {
        // Safe conversion
        try {
            P& pivot = dynamic_cast<P&>(st);
            emit beforePivotRevive( pivot.getFirstNode(), pivot.getLastNode());
        }
        catch ( ... ) { return; }
    }

    virtual void pivotAfterRevive ( StatefulObject& st )
    {
        // Safe conversion
        try {
            P& pivot = dynamic_cast<P&>(st);
            // Force reindex of elements numbers
            reindexNodesPivotsNumbers();
            emit afterPivotRevive( pivot.getFirstNode(), pivot.getLastNode());
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

    virtual void pivotBeforeDesist ( StatefulObject& st )
    {
        // Safe conversion
        try {
            P& pivot = dynamic_cast<P&>(st);
            emit beforePivotDesist( pivot.getFirstNode(), pivot.getLastNode());
        }
        catch ( ... ) { return; }
    }

    virtual void pivotAfterDesist ( StatefulObject& st )
    {
        // Safe conversion
        try {
            P& pivot = dynamic_cast<P&>(st);
            // Force reindex of elements numbers
            reindexNodesPivotsNumbers();
            emit afterPivotDesist( pivot.getFirstNode(), pivot.getLastNode() );
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

    virtual void removePivotMaterial ( const TrussMaterial& mat )
    {
        PivotList pivots = getPivotList();
        for ( PivotListIter iter = pivots.begin();
              iter != pivots.end(); ++iter ) {
            P* pivot = *iter;
            if ( pivot->getMaterial() == &mat )
                pivot->setMaterial( 0 );
        }
    }

/****************************** toplogies ************************************/

    virtual void topologyBeforeRevive ( StatefulObject& st )
    {
        // Safe conversion
        try {
            TrussTopology& topology = dynamic_cast<TrussTopology&>(st);
            emit beforeTopologyRevive( topology );
        }
        catch ( ... ) { return; }
    }

    virtual void topologyAfterRevive ( StatefulObject& st )
    {
        // Safe conversion
        try {
            TrussTopology& topology = dynamic_cast<TrussTopology&>(st);
            emit afterTopologyRevive( topology );
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

    virtual void topologyBeforeDesist ( StatefulObject& st )
    {
        // Safe conversion
        try {
            TrussTopology& topology = dynamic_cast<TrussTopology&>(st);
            emit beforeTopologyDesist( topology );
        }
        catch ( ... ) { return; }
    }

    virtual void topologyAfterDesist ( StatefulObject& st )
    {
        // Safe conversion
        try {
            TrussTopology& topology = dynamic_cast<TrussTopology&>(st);
            emit afterTopologyDesist( topology );
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

/************************** basic operations *********************************/

    // Force reindex all numbers of nodes and pivots
    virtual void reindexNodesPivotsNumbers ()
    {
        NodeListIter nIter = nodes.begin();
        int i = 1;
        for ( i = 1; nIter != nodes.end(); ++nIter ) {
            N* node = *nIter;
            if ( node->isAlive() )
                node->setNumber( i++ );
        }

        PivotListIter pIter = pivots.begin();
        for ( i = 1; pIter != pivots.end(); ++pIter ) {
            P* pivot = *pIter;
            if ( pivot->isAlive() )
                pivot->setNumber( i++ );
        }
    }

    // Physically removes nodes and pivots
    virtual void suspendedClean ()
    {
        // Clean desisted pivots without states
        PivotListIter pIter = pivots.begin();
        for ( ; pIter != pivots.end(); ) {
            P* pivot = *pIter;
            if ( !pivot->isAlive() && pivot->countEnabledStates() == 0 ) {
                pIter = removePivot(pIter);
            }
            else
                ++pIter;
        }

        // Clean desisted nodes without states
        NodeListIter nIter = nodes.begin();
        for ( ; nIter != nodes.end(); ) {
            N* node = *nIter;
            if ( !node->isAlive() && node->countEnabledStates() == 0 ) {
                nIter = removeNode(nIter);
            }
            else
                ++nIter;
        }

        // Clean desisted topologies without states
        TopologyListIter tIter = topologies.begin();
        for ( ; tIter != topologies.end(); ) {
            TrussTopology* topology = *tIter;
            if ( !topology->isAlive() &&
                 topology->countEnabledStates() == 0 ) {
                tIter = removeTopology(tIter);
            }
            else
                ++tIter;
        }
    }

    // Momentary removing of desisted object. Nothing can revive it.
    // Of course, adjoining pivots should be removed too.
    // Returns next iter after erase.
    virtual NodeListIter removeNode ( NodeListIter iter )
    {
        if ( iter == nodes.end() )
            return nodes.end();
        N* n = *iter;
        if ( n->isAlive() )
            return nodes.end();
        // We should remove adjoining pivots first
        removeAdjoiningPivots(*n);
        emit beforeNodeRemoval(*n);
        iter = nodes.erase(iter);
        delete n;

        // Force reindex of elements numbers
        reindexNodesPivotsNumbers();

        emit afterNodeRemoval();
        emit onStateChange();
        return iter;
    }

    // If flag is true returns only alive pivots
    virtual PivotList findAdjoiningPivots ( const N& node,
                                            bool selectAlive ) const
    {
        PivotList resPivots;
        if ( selectAlive && ! node.isAlive() )
            return resPivots;
        PivotListConstIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) {
            P* pivot = (*iter);
            if ( selectAlive &&
                 ( !pivot->isAlive() ) )
                continue;
            if ( &(pivot->getFirstNode()) == &node ||
                 &(pivot->getLastNode()) == &node )
                 resPivots.push_back( pivot );
        }
        return resPivots;
    }

    // Physically remove all adjoining pivots
    virtual void removeAdjoiningPivots ( N& node )
    {
        PivotList pivotsToRemove = findAdjoiningPivots( node, false );
        PivotListIter iter = pivotsToRemove.begin();
        for ( ; iter != pivotsToRemove.end(); ++iter ) {
            // Desist first
            (*iter)->desist();
            // Find not temp iter.
            // (As you know, we create temp vector for adjoining elements)
            PivotListIter realIter = std::find( pivots.begin(), pivots.end(),
                                                *iter );
            // We can omit saving of the returning iter, bacause we are
            // working with temp vector
            removePivot(realIter);
        }
    }

    // Momentary removing of desisted object. Nothing can revive it.
    // Returns next iter after erase.
    virtual PivotListIter removePivot ( PivotListIter iter )
    {
        if ( iter == pivots.end() )
            return pivots.end();
        P* p = *iter;
        if ( p->isAlive() )
            return pivots.end();
        emit beforePivotRemoval(p->getFirstNode(), p->getLastNode());
        iter = pivots.erase(iter);
        delete p;

        // Force reindex of elements numbers
        reindexNodesPivotsNumbers();

        emit afterPivotRemoval();
        emit onStateChange();
        return iter;
    }

    // Momentary removing of desisted object. Nothing can revive it.
    // Returns next iter after erase.
    virtual TopologyListIter removeTopology ( TopologyListIter iter )
    {
        if ( iter == topologies.end() )
            return topologies.end();
        TrussTopology* t = *iter;
        if ( t->isAlive() )
            return topologies.end();
        emit beforeTopologyRemoval(*t);
        iter = topologies.erase(iter);
        delete t;
        emit afterTopologyRemoval();
        emit onStateChange();
        return iter;
    }

private:
    NodeList  nodes;
    PivotList pivots;
    TopologyList topologies;
    DoubleSize trussAreaSize;
    LoadCases loadCases;
    TrussDimension dimension;
    const TrussMaterialLibrary& materialLibrary;
};

/*****************************************************************************
 * Pivot Emitter
 *****************************************************************************/

// Pivot emitter. All signals logic was moved to this
// class because of Qt rejection to moc templates.

class PivotEmitter : public StatefulObject
{
    Q_OBJECT
public:
    PivotEmitter ( ObjectStateManager* mng );

signals:
    void onThicknessChange ( double );
    void onFirstNodeChange ();
    void onLastNodeChange ();
    void onMaterialChange ();
};

/*****************************************************************************
 * Pivot
 *****************************************************************************/

template <class N>
class Pivot : public PivotEmitter
{
public:
    Pivot ( ObjectStateManager* mng, const TrussMaterialLibrary& mLib ) :
        PivotEmitter(mng),
        materialLib(mLib),
        first(0), last(0),
        material(0),
        thickness(0.1),
        number(0)
    {}
    Pivot ( N& first_, N& last_, ObjectStateManager* mng,
            const TrussMaterialLibrary& mLib ) :
        PivotEmitter(mng),
        materialLib(mLib),
        first(&first_),
        last(&last_),
        material(0),
        thickness(0.1),
        number(0)
    {}
    virtual ~Pivot ()
    {}

    virtual N& getFirstNode () const
    { return *first; }
    virtual N& getLastNode () const
    { return *last; }
    virtual void setFirstNode ( N* first_ )
    { first = first_;
      emit onFirstNodeChange(); }

    virtual void setLastNode ( N* last_ )
    { last = last_;
      emit onLastNodeChange(); }

    virtual const TrussMaterial* getMaterial () const
    { return material; }
    virtual void setMaterial ( const TrussMaterial* mat )
    {
        if ( material == mat )
            return;
        material = mat;
        emit onMaterialChange();
    }

    virtual double getThickness () const
    { return thickness; }
    virtual void setThickness ( double t_ )
    { thickness = t_;
      emit onThicknessChange(thickness); }

    virtual int getNumber () const
    { return number; }
    virtual void setNumber ( int num )
    { number = num; }

protected:
    virtual const TrussMaterialLibrary& materialLibrary() const
    { return materialLib; }

private:
    const TrussMaterialLibrary& materialLib;
    N *first, *last;
    const TrussMaterial* material;
    double thickness;
    int number;
};

/*****************************************************************************
 * Node
 *****************************************************************************/

class Node : public StatefulObject
{
    Q_OBJECT
public:
    typedef enum { Unfixed = 0,
                   FixationByX,
                   FixationByY,
                   FixationByXY } Fixation;

signals:
    void onFixationChange ( Fixation );
    void onPositionChange ( double, double );

public:
    Node ( ObjectStateManager* mng );
    Node ( double x, double y, ObjectStateManager* mng );
    Node ( double x, double y, Fixation, ObjectStateManager* mng );

    virtual void setFixation ( Fixation );
    virtual Fixation getFixation () const;

    virtual void setPoint ( DoublePoint );
    virtual void setPoint ( double x, double y );

    virtual const DoublePoint& getPoint () const;
    virtual double getX () const;
    virtual double getY () const;

    virtual int getNumber () const;
    virtual void setNumber ( int );

private:
    DoublePoint pos;
    Fixation fix;
    int number;
};

/************************* Declare meta types ********************************/

Q_DECLARE_METATYPE (Node::Fixation);

#endif //TRUSS_H
