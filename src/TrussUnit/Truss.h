
#ifndef TRUSS_H
#define TRUSS_H

#include <vector>
#include <algorithm>
#include <qobject.h>
#include <qpoint.h>

// Basic classes for truss unit construction. 
template <class N, class P> class Truss;
template <class N> class Pivot;
class Node;


// Emitters for truss unit basic classes. 
// All signals logic was moved to this classes 
// because of Qt rejection to moc templates.

class TrussEmitter : public QObject
{
    Q_OBJECT
signals: 
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

class PivotEmitter : public QObject
{
    Q_OBJECT
signals: 
    void onFirstNodeChange ( const Node& );
    void onLastNodeChange ( const Node& );
};


template <class N, class P> class Truss : public TrussEmitter
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

    Truss () {}

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

    virtual N* findNode ( QPoint point ) const
    {        
        NodeListConstIter iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
        {
            N* node = *iter;
            int x1 = node->getX ();
            int y1 = node->getY ();
            if ( ( (point.x() - x1) * (point.x() - x1) + 
                   (point.y() - y1) * (point.y() - y1) ) <  
                 4 * nodesRadius * nodesRadius )
                return node;
        }
        return 0;
    }   

    virtual N& createNode ( int x, int y )
    {
        emit beforeNodeCreation();
        N* node = new N;
        node->setX ( x );
        node->setY ( y );
        nodes.push_back(node);
        emit afterNodeCreation(*node);
        return *node;
    }

    virtual P& createPivot ( QPoint p1 , QPoint p2 )
    {
        N* first = findNode( p1 );
        N* last = findNode( p2 );
        if ( first == 0 )  
            first = &createNode( p1.x(), p1.y() );            
        
        if ( last == 0 )        
            last = &createNode( p2.x(), p2.y() );

        emit beforePivotCreation();
        P* pivot = new P( *first, *last );        
        pivots.push_back(pivot);
        emit afterPivotCreation(pivot->getFirstNode(), 
                                pivot->getLastNode());                               
        return *pivot;
    }

    virtual P& createPivot ( uint firstNodeIndex, uint lastNodeIndex ) 
                                                        throw (NodeIndexOutOfBoundException) 
    {
        if ( lastNodeIndex >= nodes.size() || lastNodeIndex >= nodes.size() )
            throw NodeIndexOutOfBoundException();

        emit beforePivotCreation();
        N* firstNode = nodes.at(firstNodeIndex);
        N* lastNode = nodes.at(lastNodeIndex);

        P* pivot = new P( *firstNode, *lastNode );
        pivots.push_back(pivot);
        emit afterPivotCreation(pivot->getFirstNode(), 
                                pivot->getLastNode());
        return *pivot;
    }

    virtual P& createPivot ( N& first, N& last )
    {
        emit beforePivotCreation();
        P* pivot = new P( first, last );        
        pivots.push_back(pivot);
        emit afterPivotCreation(pivot->getFirstNode(), 
                                pivot->getLastNode());
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
    }

private:
    NodeList  nodes;
    PivotList pivots;
};



template <class N> class Pivot : public PivotEmitter
{
    friend class Truss<class N_, class P_>;

protected:
    Pivot () : first(0), last(0) {}
    Pivot ( N& first_, N& last_ ) :         
        first(&first_),
        last(&last_) 
    {}
    virtual ~Pivot ()
    {}

public:
    virtual const N& getFirstNode () const
    { return *first; }
    virtual const N& getLastNode () const
    { return *last; }

private:    
    N *first, *last;
};



class Node : public QObject
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

protected:
    Node ();
    Node ( int x, int y );
    Node ( int x, int y, Fixation );
    Node ( const Node& );

public:
    virtual void setFixation ( Fixation );
    virtual Fixation getFixation () const;
    
    virtual void setPoint ( QPoint );
    virtual void setX ( int newX );
    virtual void setY ( int newY );

    virtual QPoint getPoint () const;
    virtual int getX () const;
    virtual int getY () const;

private:
    int x, y;
    Fixation fix;
};

#endif //TRUSS_H
