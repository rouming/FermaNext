
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include <vector>
#include <agg_rendering_buffer.h>

class PaintableTrussElement 
{
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool e, bool v );
    virtual void setVisible ( bool );
    virtual void setEnabled ( bool );
    virtual bool isVisible ();
    virtual bool isEnabled ();
    virtual void paint ( agg::rendering_buffer& ) const = 0;
private:
    bool visible;
    bool enabled;    
};

class TrussUnit : public PaintableTrussElement
{
public:        
    class Node;
    class Pivot;

    typedef std::vector<Node*>  NodesList;
    typedef std::vector<Pivot*> PivotsList;

    TrussUnit ();
    virtual ~TrussUnit ();

    Node& createNode ();
    Pivot& createPivot ();
    Pivot& createPivot ( const Node& first, const Node& last );
    
    bool removeNode ( const Node& );
    bool removePivot ( const Pivot& );

    void paint ( agg::rendering_buffer& ) const;
    
private:
    NodesList  nodes;
    PivotsList pivots;
};

class TrussUnit::Node : public PaintableTrussElement
{    
    friend class TrussUnit;
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

    void paint ( agg::rendering_buffer& ) const;
private:
    int x, y;
    Fixation fix;
};

class TrussUnit::Pivot : public PaintableTrussElement
{
    friend class TrussUnit;
protected:
    Pivot ();
    Pivot ( const Node& first, const Node& last );        

public:
    void setFirstNode ( const Node& );
    const Node& getFirstNode () const;

    void setLastNode ( const Node& );
    const Node& getLastNode () const;

    void paint ( agg::rendering_buffer& ) const;
private:
    const Node *first, *last;
};


#endif //TRUSSUNIT_H
