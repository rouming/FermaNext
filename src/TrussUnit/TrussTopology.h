
#ifndef TRUSSTOPOLOGY_H
#define TRUSSTOPOLOGY_H

// Created to provide functionality of low-level truss class.
// Just wrappers for templates.

#include "Truss.h"

class TrussTopologyNode;

/*****************************************************************************/

typedef TrussLoadCaseArray<TrussTopologyNode> TrussTopologyLoadCases;
typedef TrussLoadCase<TrussTopologyNode> TrussTopologyLoadCase;

/*****************************************************************************/

class TrussTopologyNode : public Node
{
public:
    TrussTopologyNode ( ObjectStateManager* mng ) :
        Node(mng)
    {}
    bool isEnabled ()
    { return true; }
};

class TrussTopologyPivot : public Pivot<TrussTopologyNode>
{
public:
    TrussTopologyPivot ( ObjectStateManager* mng ) :
        Pivot<TrussTopologyNode>(mng)
    {}
    TrussTopologyPivot ( TrussTopologyNode& node1,
                         TrussTopologyNode& node2, 
                         ObjectStateManager* mng ) :
        Pivot<TrussTopologyNode>( node1, node2, mng )
    {}
    bool isEnabled ()
    { return true; }
};

class TrussTopology : public Truss<TrussTopologyNode, TrussTopologyPivot>
{
public:
    TrussTopology ( ObjectStateManager* mng = 0 ) :
        Truss<TrussTopologyNode, TrussTopologyPivot>(mng)
    {}
};

#endif //TRUSSTOPOLOGY_H
