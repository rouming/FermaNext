
#ifndef TRUSSUNITACTIONS_H
#define TRUSSUNITACTIONS_H

#include "AbstractObjectAction.h"
#include "TrussUnit.h"
#include "TrussUnitWindowManager.h"

/*****************************************************************************
 * Truss Node Actions
 *****************************************************************************/

class TrussNodeAction : public AbstractObjectAction
{
public:
    TrussNodeAction ( TrussUnit&, TrussNode& );

    TrussUnit& getTruss ();
    TrussNode& getNode ();

private:
    TrussUnit& truss;
    TrussNode& node;
};

class TrussNodeCreateAction : public TrussNodeAction
{
public:
    TrussNodeCreateAction ( TrussUnit&, TrussNode& );

    void execute ();
    void unexecute ();
};

class TrussNodeRemoveAction : public TrussNodeAction
{
public:
    TrussNodeRemoveAction ( TrussUnit&, TrussNode& );

    void execute ();
    void unexecute ();
};

/*****************************************************************************
 * Truss Pivot Actions
 *****************************************************************************/

class TrussPivotAction : public AbstractObjectAction
{
public:
    TrussPivotAction ( TrussUnit&, TrussPivot& );

    TrussUnit& getTruss ();
    TrussPivot& getPivot ();

private:
    TrussUnit& truss;
    TrussPivot& pivot;
};

class TrussPivotCreateAction : public TrussPivotAction
{
public:
    TrussPivotCreateAction ( TrussUnit&, TrussPivot& );

    void execute ();
    void unexecute ();
};

class TrussPivotRemoveAction : public TrussPivotAction
{
public:
    TrussPivotRemoveAction ( TrussUnit&, TrussPivot& );

    void execute ();
    void unexecute ();
};

class TrussPivotChangeNodeAction : public AbstractObjectAction
{
public:
    TrussPivotChangeNodeAction ( TrussPivot&, TrussNode& oldNode, 
                                 TrussNode& newNode, bool changeMode = true );

    void execute ();
    void unexecute ();
private:
    TrussPivot& pivot;
    TrussNode& oldNode;
    TrussNode& newNode;
    bool firstNodeChange;
};

/*****************************************************************************
 * Truss Unit Window Actions
 *****************************************************************************/

class TrussUnitWindowAction : public AbstractObjectAction
{
public:
    TrussUnitWindowAction ( TrussUnitWindowManager&, TrussUnitWindow& );

    TrussUnitWindow& getTruss ();
    TrussUnitWindowManager& getTrussManager ();

private:
    TrussUnitWindowManager& mng;
    TrussUnitWindow& truss;
};

class TrussUnitWindowCreateAction : public TrussUnitWindowAction
{
public:
    TrussUnitWindowCreateAction ( TrussUnitWindowManager&, TrussUnitWindow& );

    void execute ();
    void unexecute ();
};

class TrussUnitWindowRemoveAction : public TrussUnitWindowAction
{
public:
    TrussUnitWindowRemoveAction ( TrussUnitWindowManager&, TrussUnitWindow& );

    void execute ();
    void unexecute ();
};

#endif //TRUSSUNITACTIONS_H
