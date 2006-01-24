
#ifndef OPTIMIZATIONINTERFACE_H
#define OPTIMIZATIONINTERFACE_H

template <class N, class P> class Truss;
template <class N> class Pivot;
class Node;
typedef Truss< Node, Pivot<Node> > TrussTopology;

class TrussCalcData;

class OptimizationInterface
{
    virtual void optimize ( TrussTopology&, TrussCalcData& ) const = 0;
};

#endif //OPTIMIZATIONINTERFACE_H
