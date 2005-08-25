
#ifndef TRUSS_H
#define TRUSS_H

#include <vector>
#include <algorithm>
#include <qobject.h>
#include <qpoint.h>

#include "StatefulObject.h"

// Basic classes for truss unit construction. 
template <class N, class P> class Truss;
template <class N> class Pivot;
class Node;

/*****************************************************************************
 * Truss Emitter
 *****************************************************************************/

// Emitters for truss unit basic classes. 
// All signals logic was moved to this classes 
// because of Qt rejection to moc templates.

class TrussEmitter : public StatefulObject
{
    Q_OBJECT
public:
    TrussEmitter ( ObjectStateManager* mng );

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

signals:
    // Truss signals
    void onStateChange ();

    // Nodes create/remove signals
    void beforeNodeCreation ();
    void afterNodeCreation ( const Node& );
    void beforeNodeRemoval ( const Node& );
    void afterNodeRemoval ();

    // Nodes revive/desist signals
    void beforeNodeRevive ( Node& );
    void afterNodeRevive ( Node& );
    void beforeNodeDesist ( Node& );
    void afterNodeDesist ( Node& );

    // Pivots create/remove signals
    void beforePivotCreation ();
    void afterPivotCreation ( const Node&, const Node& );
    void beforePivotRemoval ( const Node&, const Node& );
    void afterPivotRemoval ();

    // Pivots revive/desist signals
    void beforePivotRevive ( Node&, Node& );
    void afterPivotRevive ( Node&, Node& );
    void beforePivotDesist ( Node&, Node& );
    void afterPivotDesist ( Node&, Node& );
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

    typedef std::vector<N*> NodeList;
    typedef std::vector<P*> PivotList;
    typedef typename NodeList::iterator NodeListIter;
    typedef typename NodeList::const_iterator NodeListConstIter;
    typedef typename PivotList::iterator PivotListIter;
    typedef typename NodeList::const_iterator NodeListConstIter;
    typedef typename PivotList::const_iterator PivotListConstIter;

    Truss ( ObjectStateManager* mng ) :
        TrussEmitter(mng)
    {}

    virtual void clear () 
    {
        NodeListIter itNodes;
        PivotListIter itPivots;
        for ( itNodes = nodes.begin(); 
              itNodes != nodes.end(); 
              ++itNodes )
            delete *itNodes;
        for ( itPivots = pivots.begin(); 
              itPivots != pivots.end();  
              ++itPivots )
            delete *itPivots;
        nodes.clear();
        pivots.clear();
    }

    virtual ~Truss ()
    {
        clear();
    }

    virtual N* findNodeByCoord ( QPoint point ) const
    {   
        return findNodeByCoord( point, (4 * nodesRadius * nodesRadius) );
    } 
    
    virtual N* findNodeByCoord ( QPoint point, int precision ) const
    {        
        NodeListConstIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
        {
            N* node = *iter;
            if ( !node->isAlive() || !node->isEnabled() )
                continue;
            QPoint pos = node->getPoint();
            if ( ( (point.x() - pos.x()) * (point.x() - pos.x()) + 
                   (point.y() - pos.y()) * (point.y() - pos.y()) ) < precision )
                return node;
        }
        return 0;
    }   

    virtual N* findNodeByNumber ( int num ) const
    {
        NodeListConstIter iter = nodes.begin();
        for ( int i = 1; iter != nodes.end(); ++iter ) 
        {
            if ( (*iter)->isAlive() && (*iter)->isEnabled() && i++ == num )
                return *iter;
        }
        return 0;
    }

    virtual P* findPivotByCoord ( double x, double y, double precision ) const
    {
        QPoint firstCoord, lastCoord, coord( (int)x, (int)y );
        double Y;
        PivotListConstIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter )
        {
            P* pivot = *iter;
            if ( !pivot->isEnabled() )
                continue;

            N& firstNode = pivot->getFirstNode ();
            N& lastNode = pivot->getLastNode ();
            firstCoord = firstNode.getPoint ();
            lastCoord = lastNode.getPoint ();

            if ( findNodeByCoord( coord ) != &firstNode && 
                 findNodeByCoord( coord ) != &lastNode )
            {               
                // equation of line
                Y = fabs( ( x - double( firstCoord.x() ) ) * 
                          double( firstCoord.y() - lastCoord.y() ) - 
                          ( y - double( firstCoord.y() ) ) * 
                          double( firstCoord.x() - lastCoord.x() ) );

                if ( ( x >= firstCoord.x() && x <= lastCoord.x() || 
                       x >= lastCoord.x() && x <= firstCoord.x() ) && 
                     ( y >= firstCoord.y() && y <= lastCoord.y() || 
                       y >= lastCoord.y() && y <= firstCoord.y() ) && Y <= precision )
                    return pivot;
            }
        }
        return 0;
    }

    virtual P* findPivotByNodes ( const N& node1, const N& node2 ) const
    {
        PivotListConstIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter )
        {
            P* pivot = (*iter);
            if ( ! pivot->isAlive() || !pivot->isEnabled() )
                continue;
            if ( &(pivot->getFirstNode()) == &node1 && 
                 &(pivot->getLastNode()) == &node2 ||
                 &(pivot->getFirstNode()) == &node2 && 
                 &(pivot->getLastNode()) == &node1 ) {
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

    virtual N& createNode ( int x, int y )
    {
        // Clean earlier desisted nodes/pivots
        suspendedClean();

        emit beforeNodeCreation();
        N* node = new N(getStateManager());
        node->setPoint( x, y );
        nodes.push_back(node);

        // Signal connects to catch life time changing
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
        QObject::connect( node, SIGNAL(onPositionChange ( int, int )),
                                SLOT(stateIsChanged()) );

        emit afterNodeCreation(*node);
        emit onStateChange();
        return *node;
    }

    virtual P& createPivot ( QPoint p1 , QPoint p2 )
    {
        N* first = findNodeByCoord ( p1 );
        N* last = findNodeByCoord ( p2 );
        if ( first == 0 )  
            first = &createNode( p1.x(), p1.y() );            
        
        if ( last == 0 )        
            last = &createNode( p2.x(), p2.y() );

        return createPivot( *first, *last );
    }

    virtual P& createPivot ( uint firstNodeIndex, uint lastNodeIndex ) 
                                                        throw (NodeIndexOutOfBoundException) 
    {
        if ( lastNodeIndex >= nodes.size() || lastNodeIndex >= nodes.size() )
            throw NodeIndexOutOfBoundException();
        
        N* first = nodes.at(firstNodeIndex);
        N* last  = nodes.at(lastNodeIndex);

        return createPivot( *first, *last );
    }

    virtual P& createPivot ( N& first, N& last )
    {
        // Clean earlier desisted nodes/pivots
        suspendedClean();

        emit beforePivotCreation();
        P* pivot = new P( first, last, getStateManager() );
        // Signal connects to catch life time changing
        QObject::connect( pivot, SIGNAL(onBeforeRevive(StatefulObject&)),
                                 SLOT(pivotBeforeRevive(StatefulObject&)) );
        QObject::connect( pivot, SIGNAL(onAfterRevive(StatefulObject&)),
                                 SLOT(pivotAfterRevive(StatefulObject&)) );
        QObject::connect( pivot, SIGNAL(onBeforeDesist(StatefulObject&)),
                                 SLOT(pivotBeforeDesist(StatefulObject&)) );
        QObject::connect( pivot, SIGNAL(onAfterDesist(StatefulObject&)),
                                 SLOT(pivotAfterDesist(StatefulObject&)) );
        pivots.push_back(pivot);
        emit afterPivotCreation(pivot->getFirstNode(), 
                                pivot->getLastNode());
        emit onStateChange();
        return *pivot;    
    }

    // Momentary removing of an object. Nothing can revive it.
    virtual bool removePivot ( const P& pivot )
    {
        // Clean earlier desisted nodes/pivots
        suspendedClean();
        PivotListIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) 
            if ( (*iter) == &pivot ) {
                // Desist first
                (*iter)->desist();
                return removePivot(iter);
            }
        return false;    
    }

    // Momentary removing of an object. Nothing can revive it.
    virtual bool removeNode ( const N& node )
    {
        // Clean earlier desisted nodes/pivots
        suspendedClean();
        NodeListIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
            if ( (*iter) == &node ) {                
                // Desist first
                (*iter)->desist();
                return removeNode(iter);
            }            
        return false;    
    }

    // Returns alive nodes
    virtual NodeList getNodeList () const
    {
        NodeList aliveNodes;
        NodeListConstIter iter = nodes.begin();
        for ( int i = 1; iter != nodes.end(); ++iter ) {
            N* node = *iter;
            if ( node->isAlive() ) {
                node->setNumber( i++ );
                aliveNodes.push_back( node );
            }
        }
        return aliveNodes;
    }

    // Returns alive nodes
    virtual PivotList getPivotList () const
    {
        PivotList alivePivots;
        PivotListConstIter iter = pivots.begin();
        for ( int i = 1; iter != pivots.end(); ++iter ) {
            P* pivot = *iter;
            if ( pivot->isAlive() ) {
                pivot->setNumber( i++ );
                alivePivots.push_back( pivot );
            }
        }
        return alivePivots;
    }

    virtual void nodeToFront ( N& selectedNode )
    {    
        if ( nodes.back() == &selectedNode )
            return;
        NodeListIter newSelectedIter = std::find ( nodes.begin(), nodes.end(), 
                                                  &selectedNode );
        if ( newSelectedIter == nodes.end() )
            return;

        if ( *newSelectedIter != &selectedNode )
            return;
        // push selected node to front
        nodes.erase ( newSelectedIter );
        nodes.push_back ( &selectedNode );
    }

protected:
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
            emit afterNodeRevive(node);
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

    // Desist all adjoining pivots
    virtual void nodeBeforeDesist ( StatefulObject& st )
    {
        // Safe conversion
        N* node = 0;
        try { node = dynamic_cast<N*>(&st); }
        catch ( ... ) { return; }

        PivotList pivotsToDesist = findAdjoiningPivots( *node, false );
        PivotListIter iter = pivotsToDesist.begin();
        for ( ; iter != pivotsToDesist.end(); ++iter ) {
            P* pivot = *iter;
            if ( pivot->isAlive() )
                (*iter)->desist();
        }
        emit beforeNodeDesist(*node);
    }

    virtual void nodeAfterDesist ( StatefulObject& st )
    {
        // Safe conversion
        try { 
            N& node = dynamic_cast<N&>(st);
            emit afterNodeDesist(node);
            emit onStateChange();
        }
        catch ( ... ) { return; }        
    }

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
            emit afterPivotDesist( pivot.getFirstNode(), pivot.getLastNode() );
            emit onStateChange();
        }
        catch ( ... ) { return; }
    }

    // Physically removes nodes and pivots
    virtual void suspendedClean () 
    {
        // Clean desisted pivots without states
        PivotListIter pIter = pivots.begin();
        for ( ; pIter != pivots.end(); ) {
            P* pivot = *pIter;
            if ( !pivot->isAlive() && pivot->countEnabledStates() == 0 ) {
                removePivot(pIter);
            }
            else
                ++pIter;
        }

        // Clean desisted nodes without states
        NodeListIter nIter = nodes.begin();
        for ( ; nIter != nodes.end(); ) {
            N* node = *nIter;
            if ( !node->isAlive() && node->countEnabledStates() == 0 ) {
                removeNode(nIter);
            }
            else
                ++nIter;
        }
    }

    // Momentary removing of desisted object. Nothing can revive it.
    // Of course, adjoining pivots should be removed too.
    virtual bool removeNode ( NodeListIter& iter )
    {
        if ( iter == nodes.end() )
            return false;
        N* n = *iter;
        if ( n->isAlive() )
            return false;
        // We should remove adjoining pivots first
        removeAdjoiningPivots(*n);
        emit beforeNodeRemoval(*n);
        nodes.erase(iter);
        delete n;
        emit afterNodeRemoval();
        emit onStateChange();
        return true;
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
                 ( !pivot->isAlive() || !pivot->isEnabled() ) )
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
            removePivot(iter);
        }
    }

    // Momentary removing of desisted object. Nothing can revive it.
    virtual bool removePivot ( PivotListIter& iter )
    {
        if ( iter == pivots.end() )
            return false;
        P* p = *iter;
        if ( p->isAlive() )
            return false;
        emit beforePivotRemoval(p->getFirstNode(), p->getLastNode());
        pivots.erase(iter);
        delete p;
        emit afterPivotRemoval();
        emit onStateChange();
        return true;
    }

private:
    NodeList  nodes;
    PivotList pivots;
};

/*****************************************************************************
 * Pivot
 *****************************************************************************/

template <class N> 
class Pivot : public StatefulObject
{
    friend class Truss<class N_, class P_>;

protected:
    Pivot ( ObjectStateManager* mng ) : 
        StatefulObject(mng),
        first(0), last(0),
        number(0)
    {}
    Pivot ( N& first_, N& last_, ObjectStateManager* mng ) :
        StatefulObject(mng),
        first(&first_),
        last(&last_),
        number(0)
    {}
    virtual ~Pivot ()
    {}

public:
    virtual N& getFirstNode () const
    { return *first; }
    virtual N& getLastNode () const
    { return *last; }
    virtual void setFirstNode ( N* first_ )
    { first = first_; }
    virtual void setLastNode ( N* last_ )
    { last = last_; }
    virtual int getNumber () const
    { return number; }
    virtual void setNumber ( int num )
    { number = num; }

private:    
    N *first, *last;
    int number;
};

/*****************************************************************************
 * Node
 *****************************************************************************/

class Node : public StatefulObject
{
    Q_OBJECT
    friend class Truss<Node, class P>;

public:
    typedef enum { Unfixed = 0,
                   FixationByX, 
                   FixationByY, 
                   FixationByXY } Fixation;

signals:
    void onFixationChange ( Fixation );
    void onPositionChange ( int, int );

protected:
    Node ( ObjectStateManager* mng );
    Node ( int x, int y, ObjectStateManager* mng );
    Node ( int x, int y, Fixation, ObjectStateManager* mng );

public:
    virtual void setFixation ( Fixation );
    virtual Fixation getFixation () const;
    
    virtual void setPoint ( QPoint );
    virtual void setPoint ( int x, int y );
        
    virtual QPoint getPoint () const;
    virtual int getX () const;
    virtual int getY () const;

    virtual int getNumber () const;
    virtual void setNumber ( int );

private:
    int x, y;
    Fixation fix;
    int number;
};

#endif //TRUSS_H
