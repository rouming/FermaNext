
#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include <vector>
#include <agg_rendering_buffer.h>

class TrussNode;
class TrussPivot;

class PaintableTrussElement 
{
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool e, bool v );
    virtual void setVisible ( bool );
    virtual void setEnabled ( bool );
    virtual bool isVisible () const;
    virtual bool isEnabled () const;
    virtual void paint ( agg::rendering_buffer& ) const = 0;
private:
    bool visible;
    bool enabled;    
};

class TrussUnit : public Truss<TrussNode, TrussPivot>, 
                  public PaintableTrussElement
{
public:
    TrussUnit ();
    ~TrussUnit ();
    void paint ( agg::rendering_buffer& ) const;
};

class TrussNode: public Node, public PaintableTrussElement
{    
public:
    void paint ( agg::rendering_buffer& ) const;
};

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
public:
    void paint ( agg::rendering_buffer& ) const;
};


#endif //TRUSSUNIT_H
