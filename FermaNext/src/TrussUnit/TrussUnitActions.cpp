
#include "TrussUnitActions.h"

/*****************************************************************************
 * Truss Node Actions
 *****************************************************************************/

TrussNodeAction::TrussNodeAction ( TrussUnit& t_, TrussNode& n_ ) :
    truss(t_), node(n_)
{}

TrussUnit& TrussNodeAction::getTruss ()
{ 
    return truss; 
}

TrussNode& TrussNodeAction::getNode ()
{ 
    return node; 
}

/**** Truss Node Create Action ***/

TrussNodeCreateAction::TrussNodeCreateAction ( TrussUnit& t_, TrussNode& n_ ) :
    TrussNodeAction( t_, n_ )
{}

void TrussNodeCreateAction::execute ()
{        
    getTruss().reviveNode( getNode() );
    emit actionExecuted(*this);
}

void TrussNodeCreateAction::unexecute ()
{
    getTruss().removeNode( getNode() );
    emit actionUnexecuted(*this);
}

/**** Truss Node Create Action ***/

TrussNodeRemoveAction::TrussNodeRemoveAction ( TrussUnit& t_, TrussNode& n_ ) :
    TrussNodeAction( t_, n_ )
{}

void TrussNodeRemoveAction::execute ()
{        
    getTruss().removeNode( getNode() );
    emit actionExecuted(*this);
}

void TrussNodeRemoveAction::unexecute ()
{
    getTruss().reviveNode( getNode() );
    emit actionUnexecuted(*this);
}

/*****************************************************************************
 * Truss Pivot Actions
 *****************************************************************************/

TrussPivotAction::TrussPivotAction ( TrussUnit& t_, TrussPivot& p_ ) :
    truss(t_), pivot(p_)
{}

TrussUnit& TrussPivotAction::getTruss ()
{ 
    return truss; 
}

TrussPivot& TrussPivotAction::getPivot ()
{ 
    return pivot;
}

/**** Truss Pivot Create Action ***/

TrussPivotCreateAction::TrussPivotCreateAction ( TrussUnit& t_, 
                                                 TrussPivot& p_ ) :
    TrussPivotAction( t_, p_ )
{}

void TrussPivotCreateAction::execute ()
{        
    getTruss().revivePivot( getPivot() );
    emit actionExecuted(*this);
}

void TrussPivotCreateAction::unexecute ()
{
    getTruss().removePivot( getPivot() );
    emit actionUnexecuted(*this);
}

/**** Truss Pivot Create Action ***/

TrussPivotRemoveAction::TrussPivotRemoveAction ( TrussUnit& t_, 
                                                 TrussPivot& p_ ) :
    TrussPivotAction( t_, p_ )
{}

void TrussPivotRemoveAction::execute ()
{        
    getTruss().removePivot( getPivot() );
    emit actionExecuted(*this);
}

void TrussPivotRemoveAction::unexecute ()
{
    getTruss().revivePivot( getPivot() );
    emit actionUnexecuted(*this);
}

/*****************************************************************************
 * Truss Unit Actions
 *****************************************************************************/

TrussUnitWindowAction::TrussUnitWindowAction ( TrussUnitWindowManager& mng_, 
                                               TrussUnitWindow& t_ ) :
    mng(mng_), truss(t_)
{}

TrussUnitWindow& TrussUnitWindowAction::getTruss ()
{ 
    return truss; 
}

TrussUnitWindowManager& TrussUnitWindowAction::getTrussManager ()
{ 
    return mng;
}

/**** Truss Unit Create Action ***/

TrussUnitWindowCreateAction::TrussUnitWindowCreateAction ( 
                                               TrussUnitWindowManager& mng_, 
                                               TrussUnitWindow& t_ ) :
    TrussUnitWindowAction( mng_, t_ )
{}

void TrussUnitWindowCreateAction::execute ()
{        
    getTrussManager().reviveTrussUnitWindow( getTruss() );
    emit actionExecuted(*this);
}

void TrussUnitWindowCreateAction::unexecute ()
{
    getTrussManager().removeTrussUnitWindow( getTruss() );
    emit actionUnexecuted(*this);
}

/**** Truss Unit Create Action ***/

TrussUnitWindowRemoveAction::TrussUnitWindowRemoveAction ( 
                                             TrussUnitWindowManager& mng_,
                                             TrussUnitWindow& t_ ) :
    TrussUnitWindowAction( mng_, t_ )
{}

void TrussUnitWindowRemoveAction::execute ()
{        
    getTrussManager().removeTrussUnitWindow( getTruss() );
    emit actionExecuted(*this);
}

void TrussUnitWindowRemoveAction::unexecute ()
{
    getTrussManager().reviveTrussUnitWindow( getTruss() );
    emit actionUnexecuted(*this);
}

/****************************************************************************/
