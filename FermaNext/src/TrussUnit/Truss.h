
#ifndef TRUSS_H
#define TRUSS_H

#include <vector>
#include <agg_rendering_buffer.h>

class Truss
{
public:        
    class Node;
    class Pivot;

    typedef std::vector<Node*>  NodesList;
    typedef std::vector<Pivot*> PivotsList;

    Truss ();
    virtual ~Truss ();

    Node& createNode ();
    Pivot& createPivot ();
    Pivot& createPivot ( const Node& first, const Node& last );
    
    bool removeNode ( const Node& );
    bool removePivot ( const Pivot& );

private:
    NodesList  nodes;
    PivotsList pivots;
};

class Truss::Node
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
    void setFixation ( Fixation );
    Fixation getFixation () const;

private:
    int x, y;
    Fixation fix;
};

class Truss::Pivot
{
    friend class Truss;
protected:
    Pivot ();
    Pivot ( const Node& first, const Node& last );        

public:
    void setFirstNode ( const Node& );
    const Node& getFirstNode () const;

    void setLastNode ( const Node& );
    const Node& getLastNode () const;

private:
    const Node *first, *last;
};

#endif //TRUSS_H
