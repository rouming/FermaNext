
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

signals:
    // Truss signals
    void onStateChange ();

    // Nodes signals
    void beforeNodeCreation ();
    void afterNodeCreation ( const Node& );
    void beforeNodeRemoval ( const Node& );
    void afterNodeRemoval ();

    // Pivots signals
    void beforePivotCreation ();
    void afterPivotCreation ( const Node&, const Node& );
    void beforePivotRemoval ( const Node&, const Node& );
    void afterPivotRemoval ();
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

    typedef std::vector<N*>  NodeList;
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
        NodeListConstIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
        {
            N* node = *iter;
            if ( !node->isAlive() || !node->isEnabled() )
                continue;
            QPoint pos = node->getPoint();
            if ( ( (point.x() - pos.x()) * (point.x() - pos.x()) + 
                   (point.y() - pos.y()) * (point.y() - pos.y()) ) < 
                   4 * nodesRadius * nodesRadius )
                return node;
        }
        return 0;
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

    virtual bool reviveNode ( N& node ) 
    {
        NodeListIter iter = std::find( nodes.begin(), nodes.end(), &node );
        if ( iter == nodes.end() )
            return false;
        if ( node.isAlive() )
            return false;

        emit beforeNodeCreation();
        node.revive();
        emit afterNodeCreation(node);
        emit onStateChange();
        return true;
    }

    virtual bool revivePivot ( P& pivot )
    {
        PivotListIter iter = std::find( pivots.begin(), pivots.end(), &pivot );
        if ( iter == pivots.end() )
            return false;
        if ( pivot.isAlive() )
            return false;

        emit beforePivotCreation();
        pivot.revive();
        emit afterPivotCreation( pivot.getFirstNode(), pivot.getLastNode() );
        emit onStateChange();
        return true;
    }

    virtual N& createNode ( int x, int y )
    {
        // Clean earlier desisted nodes/pivots
        suspendedClean();

        emit beforeNodeCreation();
        N* node = new N(getStateManager());
        node->setPoint( x, y );
        nodes.push_back(node);

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
        pivots.push_back(pivot);
        emit afterPivotCreation(pivot->getFirstNode(), 
                                pivot->getLastNode());
        emit onStateChange();
        return *pivot;    
    }

    virtual bool removePivot ( const P& pivot )
    {
        PivotListIter iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) 
            if ( (*iter) == &pivot ) {  
                removePivot(iter);
                return true;
            }
        return false;    
    }

    virtual bool removeNode ( const N& node )
    {
        NodeListIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
            if ( (*iter) == &node ) {
                removeNode(iter);
                return true;
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
    // Physically removes nodes and pivots
    virtual void suspendedClean () 
    {
        // Clean desisted nodes without states
        NodeListIter nIter = nodes.begin();
        for ( ; nIter != nodes.end(); ) {
            N* node = *nIter;
            if ( !node->isAlive() && node->countEnabledStates() == 0 ) {
                nodes.erase(nIter);
                delete node;
            }
            else
                ++nIter;
        }

        // Clean desisted pivots without states
        PivotListIter pIter = pivots.begin();
        for ( ; pIter != pivots.end(); ) {
            P* pivot = *pIter;
            if ( !pivot->isAlive() && pivot->countEnabledStates() == 0 ) {
                pivots.erase(pIter);
                delete pivot;
            }
            else
                ++pIter;
        }
    }

    // Doesn't physically remove node
    virtual void removeNode ( NodeListIter& iter )
    {
        if ( iter == nodes.end() )
            return;
        // Clean earlier desisted nodes/pivots
        suspendedClean();
        Node* n = *iter;
        emit beforeNodeRemoval(*n);
        n->desist();
        emit afterNodeRemoval();
        emit onStateChange();
    }
    // Doesn't physically remove pivot
    virtual void removePivot ( PivotListIter& iter )
    {
        if ( iter == pivots.end() )
            return;
        // Clean earlier desisted nodes/pivots
        suspendedClean();
        P* p = *iter;
        emit beforePivotRemoval(p->getFirstNode(), p->getLastNode());
        p->desist();
        emit afterPivotRemoval();
        emit onStateChange();
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
