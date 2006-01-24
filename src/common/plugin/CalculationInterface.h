
#ifndef CALCULATIONINTERFACE_H
#define CALCULATIONINTERFACE_H

template <class N, class P> class Truss;
template <class N> class Pivot;
class Node;
typedef Truss< Node, Pivot<Node> > TrussTopology;

class TrussCalcData;

#define CALCULATION_TYPE "calculation.*"

class CalculationInterface
{
public:
    virtual void calculate ( TrussTopology&, TrussCalcData& ) const = 0;
};

#endif //CALCULATIONINTERFACE_H
