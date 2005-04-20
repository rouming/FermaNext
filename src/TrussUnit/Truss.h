
#ifndef TRUSS_H
#define TRUSS_H

#include <vector>
#include <algorithm>

template <class Node, class Pivot> class Truss
{
public:        
    typedef std::vector<Node*>  NodeList;
    typedef std::vector<Pivot*> PivotList;

    inline Truss () {}
    inline ~Truss ()
    {
        NodeList::iterator itNodes;
        PivotList::iterator itPivots;
        for ( itNodes = nodes.begin(); itNodes != nodes.end();  ++itNodes )
            delete *itNodes;
        for ( itPivots = pivots.begin(); itPivots != pivots.end();  ++itPivots )
            delete *itPivots;
    }

    inline Node& createNode ()
    {
        Node* node = new Node;
        nodes.push_back(node);
        return *node;
    }

    inline Pivot& createPivot ()
    {
        Pivot* pivot = new Pivot;
        pivots.push_back(pivot);
        return *pivot;
    }

    inline Pivot& createPivot ( const Node& first, const Node& last )
    {
        Pivot* pivot = new Pivot( first, last );
        pivots.push_back(pivot);
        return *pivot;    
    }

    inline bool removePivot ( const Pivot& pivot )
    {
        PivotList::iterator iter = pivots.begin();
        for ( ; iter != pivots.end(); ++iter ) 
            if ( (*iter) == &pivot ) {
                delete *iter;
                pivots.erase(iter);
                return true;
            }
        return false;    
    }

    inline bool removeNode ( const Node& node )
    {
        NodeList::iterator iter = nodes.begin();
        for ( ; iter != nodes.end(); ++iter )
            if ( (*iter) == &node ) {
                delete *iter;
                nodes.erase(iter);
                return true;
            }            
        return false;    
    }


    /*
    Truss ();
    virtual ~Truss ();

    virtual Node& createNode ();
    virtual Pivot& createPivot ();
    virtual Pivot& createPivot ( const Node& first, const Node& last );
    
    virtual bool removeNode ( const Node& );
    virtual bool removePivot ( const Pivot& );
    */
    

private:
    NodeList  nodes;
    PivotList pivots;
};



template <class Node> class Pivot
{
    friend class Truss;
protected:
    Pivot () : first(NULL), last(NULL) {}
    Pivot ( const Node& first, const Node& last ) :  
       first(&first_),
       last(&last_) {}

public:
    virtual void setFirstNode ( const Node& first_ ) 
    { first = &first_; }
    virtual const Node& getFirstNode () const
    { return *first; }
    virtual void setLastNode ( const Node& last_ )
    { last = &last_; }
    virtual const Node& getLastNode () const
    { return *last; }

private:
    const Node *first, *last;
};



class Node
{    
    friend class Truss;
public:
    typedef enum { FixationLack = 0,
                   XFixation, 
                   YFixation, 
                   XYFixation } Fixation;
protected:
    Node ();
    Node ( int x, int y );
    Node ( int x, int y, Fixation );

public:
    virtual void setFixation ( Fixation );
    virtual Fixation getFixation () const;

private:
    int x, y;
    Fixation fix;
};

#endif //TRUSS_H
