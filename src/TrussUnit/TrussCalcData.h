
#ifndef TRUSSCALCDATA_H
#define TRUSSCALCDATA_H

#include <vector>

class TrussCalcData
{
public:
    int nodesNum;                       //nodes number
    int pivotsNum;                      //pivots number
    int loadsNum;                       //load cases number
    double v;                           //volume
    double stressLimit;                 //stress limit    
    double forceWeight;                 //truss force weight
    std::vector<int> pivotsFirstNodes;  //first nodes list of pivots
    std::vector<int> pivotsLastNodes;   //last nodes list of pivots
    std::vector<double> x;              //list of x nodes coord
    std::vector<double> y;              //list of y nodes coord
    std::vector<double> xTrans;         //list of nodes transference by x
    std::vector<double> yTrans;         //list of nodes transference by y
    std::vector<double> stress;         //list of pivots stresses
    std::vector<double> safetyFactor;   //list of pivots safety factor
    std::vector<double> pivotSquare;    //list of pivots square
    std::vector<double> pivotLength;    //list of pivots length

    TrussCalcData () :
        nodesNum(0),
        pivotsNum(0),
        loadsNum(0),
        v(0),
        stressLimit(0),
        forceWeight(0)
    {}
};

#endif //TRUSSCALCDATA_H
