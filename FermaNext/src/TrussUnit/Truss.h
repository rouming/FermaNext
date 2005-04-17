
#ifndef TRUSS_H
#define TRUSS_H

#include <vector>
#include <agg_rendering_buffer.h>

class Truss
{
public:        
    class Node;
    class Pivot;

    typedef std::vector<Node*>  NodeList;
    typedef std::vector<Pivot*> PivotList;

    Truss ();
    virtual ~Truss ();

    virtual Node& createNode ();
    virtual Pivot& createPivot ();
    virtual Pivot& createPivot ( const Node& first, const Node& last );
    
    virtual bool removeNode ( const Node& );
    virtual bool removePivot ( const Pivot& );

private:
    NodeList  nodes;
    PivotList pivots;
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
    virtual void setFixation ( Fixation );
    virtual Fixation getFixation () const;

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
    virtual void setFirstNode ( const Node& );
    virtual const Node& getFirstNode () const;

    virtual void setLastNode ( const Node& );
    virtual const Node& getLastNode () const;

private:
    const Node *first, *last;
};

#endif //TRUSS_H
