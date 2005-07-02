
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
            QPoint pos = node->getPoint();
            if ( ( (point.x() - pos.x()) * (point.x() - pos.x()) + 
                   (point.y() - pos.y()) * (point.y() - pos.y()) ) <  
                 4 * nodesRadius * nodesRadius )
                return node;
        }
        return 0;
    }   

    virtual N& createNode ( int x, int y )
    {
        emit beforeNodeCreation();
        N* node = new N(getStateManager());
        node->setPoint( x, y );
        nodes.push_back(node);

        QObject::connect( node, SIGNAL(onFixationChange ( Fixation )),
                                SLOT(stateIsChanged()) );
        QObject::connect( node, SIGNAL(onPositionChange ( int, int )),
                                SLOT(stateIsChanged()) );
        QObject::connect( node, SIGNAL(onNumberChange ( int )),
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

    virtual const NodeList& getNodeList () const
    {
        return nodes;
    }

    virtual const PivotList& getPivotList () const
    {
        return pivots;
    }

protected:
    virtual void removeNode ( NodeListIter& iter )
    {
        if ( iter == nodes.end() )
            return;
        Node* n = *iter;
        emit beforeNodeRemoval(*n);
        delete n;
        nodes.erase(iter);
        emit afterNodeRemoval();
        emit onStateChange();
    }

    virtual void removePivot ( PivotListIter& iter )
    {
        if ( iter == pivots.end() )
            return;
        P* p = *iter;
        emit beforePivotRemoval(p->getFirstNode(), p->getLastNode());
        delete p;
        pivots.erase(iter);
        emit afterPivotRemoval();
        emit onStateChange();
    }

private:
    NodeList  nodes;
    PivotList pivots;
};



template <class N> 
class Pivot : public StatefulObject
{
    friend class Truss<class N_, class P_>;

protected:
    Pivot ( ObjectStateManager* mng ) : 
        StatefulObject(mng),
        first(0), last(0) 
    {}
    Pivot ( N& first_, N& last_, ObjectStateManager* mng ) :
        StatefulObject(mng),
        first(&first_),
        last(&last_) 
    {}
    virtual ~Pivot ()
    {}

public:
    virtual N& getFirstNode () const
    { return *first; }
    virtual N& getLastNode () const
    { return *last; }

private:    
    N *first, *last;
};



class Node : public StatefulObject
{
    Q_OBJECT
    friend class Truss<class N, class P>;

public:
    typedef enum { Unfixed = 0,
                   FixationByX, 
                   FixationByY, 
                   FixationByXY } Fixation;

signals:
    void onFixationChange ( Fixation );
    void onPositionChange ( int, int );
    void onNumberChange ( int );

protected:
    Node ( ObjectStateManager* mng );
    Node ( int x, int y, ObjectStateManager* mng );
    Node ( int x, int y, Fixation, ObjectStateManager* mng );
    Node ( int x, int y, Fixation, int, ObjectStateManager* mng );

public:
    virtual void setFixation ( Fixation );
    virtual Fixation getFixation () const;
    
    virtual void setPoint ( QPoint );
    virtual void setPoint ( int x, int y );
        
    virtual QPoint getPoint () const;
    virtual int getX () const;
    virtual int getY () const;

    virtual void setNumber ( int );
    virtual int getNumber () const;

private:
    int x, y;
    int number;
    Fixation fix;
};

#endif //TRUSS_H
