
#include "TrussUnitActions.h"

/*****************************************************************************
 * Truss Node Actions
 *****************************************************************************/

TrussNodeAction::TrussNodeAction ( TrussNode& n_ ) :
    node(n_)
{}

TrussNode& TrussNodeAction::getNode ()
{ 
    return node; 
}

/**** Truss Node Create Action ***/

TrussNodeCreateAction::TrussNodeCreateAction ( TrussNode& n_ ) :
    TrussNodeAction( n_ )
{}

void TrussNodeCreateAction::execute ()
{   
    getNode().revive();
    emit actionExecuted(*this);
}

void TrussNodeCreateAction::unexecute ()
{
    getNode().desist();
    emit actionUnexecuted(*this);
}

/**** Truss Node Remove Action ***/

TrussNodeRemoveAction::TrussNodeRemoveAction ( TrussNode& n_ ) :
    TrussNodeAction( n_ )
{}

void TrussNodeRemoveAction::execute ()
{        
    getNode().desist();
    emit actionExecuted(*this);
}

void TrussNodeRemoveAction::unexecute ()
{
    getNode().setHighlighted( false );
    getNode().revive();
    emit actionUnexecuted(*this);
}

/*****************************************************************************
 * Truss Pivot Actions
 *****************************************************************************/

TrussPivotAction::TrussPivotAction ( TrussPivot& p_ ) :
    pivot(p_)
{}

TrussPivot& TrussPivotAction::getPivot ()
{ 
    return pivot;
}

/**** Truss Pivot Create Action ***/

TrussPivotCreateAction::TrussPivotCreateAction ( TrussPivot& p_ ) :
    TrussPivotAction( p_ )
{}

void TrussPivotCreateAction::execute ()
{        
    getPivot().revive();
    emit actionExecuted(*this);
}

void TrussPivotCreateAction::unexecute ()
{
    getPivot().desist();
    emit actionUnexecuted(*this);
}

/**** Truss Pivot Remove Action ***/

TrussPivotRemoveAction::TrussPivotRemoveAction ( TrussPivot& p_ ) :
    TrussPivotAction( p_ )
{}

void TrussPivotRemoveAction::execute ()
{        
    getPivot().desist();
    emit actionExecuted(*this);
}

void TrussPivotRemoveAction::unexecute ()
{
    getPivot().setHighlighted( false );
    getPivot().revive();
    emit actionUnexecuted(*this);
}

/**** Truss Pivot Change Node Action ***/

TrussPivotChangeNodeAction::TrussPivotChangeNodeAction ( TrussPivot& p_,
                                                         TrussNode& o_,
                                                         TrussNode& n_,
                                                         bool changeMode ) :
    pivot(p_), 
    oldNode(o_), 
    newNode(n_),
    firstNodeChange(changeMode)
{}

void TrussPivotChangeNodeAction::execute ()
{
    if ( firstNodeChange )
        pivot.setFirstNode( &newNode );
    else 
        pivot.setLastNode( &newNode );
    emit actionExecuted(*this);
}

void TrussPivotChangeNodeAction::unexecute ()
{
    if ( firstNodeChange )
        pivot.setFirstNode( &oldNode );
    else 
        pivot.setLastNode( &oldNode );
    newNode.setHighlighted( false );
    pivot.setHighlighted( false );
    emit actionUnexecuted(*this);
}

/*****************************************************************************
 * Truss Unit Window Actions
 *****************************************************************************/

TrussUnitWindowAction::TrussUnitWindowAction ( TrussUnitWindow& t_ ) :
    truss(t_)
{}

TrussUnitWindow& TrussUnitWindowAction::getTruss ()
{ 
    return truss; 
}

/**** Truss Unit Window Create Action ***/

TrussUnitWindowCreateAction::TrussUnitWindowCreateAction (
                                               TrussUnitWindow& t_ ) :
    TrussUnitWindowAction( t_ )
{}

void TrussUnitWindowCreateAction::execute ()
{        
    getTruss().revive();
    emit actionExecuted(*this);
}

void TrussUnitWindowCreateAction::unexecute ()
{
    getTruss().desist();
    emit actionUnexecuted(*this);
}

/**** Truss Unit Window Remove Action ***/

TrussUnitWindowRemoveAction::TrussUnitWindowRemoveAction (
                                             TrussUnitWindow& t_ ) :
    TrussUnitWindowAction( t_ )
{}

void TrussUnitWindowRemoveAction::execute ()
{        
    getTruss().desist();
    emit actionExecuted(*this);
}

void TrussUnitWindowRemoveAction::unexecute ()
{
    getTruss().revive();
    emit actionUnexecuted(*this);
}

/**** Truss Unit Window Visibility Change  ***/

TrussUnitWindowVisibilityChange::TrussUnitWindowVisibilityChange ( 
                                              TrussUnitWindow& t_, bool v_ ) :
    TrussUnitWindowAction( t_ ), 
    visible( v_ )
{}

void TrussUnitWindowVisibilityChange::execute ()
{        
    getTruss().setVisible(visible);
    emit actionExecuted(*this);
}

void TrussUnitWindowVisibilityChange::unexecute ()
{
    getTruss().setVisible(!visible);
    emit actionUnexecuted(*this);
}

/****************************************************************************/
