
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
    TrussNodeAction ( TrussNode& );

    TrussNode& getNode ();

private:
    TrussNode& node;
};

class TrussNodeCreateAction : public TrussNodeAction
{
public:
    TrussNodeCreateAction ( TrussNode& );

    void execute ();
    void unexecute ();
};

class TrussNodeRemoveAction : public TrussNodeAction
{
public:
    TrussNodeRemoveAction ( TrussNode& );

    void execute ();
    void unexecute ();
};

/*****************************************************************************
 * Truss Pivot Actions
 *****************************************************************************/

class TrussPivotAction : public AbstractObjectAction
{
public:
    TrussPivotAction ( TrussPivot& );

    TrussPivot& getPivot ();

private:
    TrussPivot& pivot;
};

class TrussPivotCreateAction : public TrussPivotAction
{
public:
    TrussPivotCreateAction ( TrussPivot& );

    void execute ();
    void unexecute ();
};

class TrussPivotRemoveAction : public TrussPivotAction
{
public:
    TrussPivotRemoveAction ( TrussPivot& );

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
    TrussUnitWindowAction ( TrussUnitWindow& );

    TrussUnitWindow& getTruss ();

private:
    TrussUnitWindow& truss;
};

class TrussUnitWindowCreateAction : public TrussUnitWindowAction
{
public:
    TrussUnitWindowCreateAction ( TrussUnitWindow& );

    void execute ();
    void unexecute ();
};

class TrussUnitWindowRemoveAction : public TrussUnitWindowAction
{
public:
    TrussUnitWindowRemoveAction ( TrussUnitWindow& );

    void execute ();
    void unexecute ();
};

class TrussUnitWindowVisibilityChange : public TrussUnitWindowAction
{
public:
    TrussUnitWindowVisibilityChange ( TrussUnitWindow&, bool );

    void execute ();
    void unexecute ();

private:
    bool visible;
};

#endif //TRUSSUNITACTIONS_H
