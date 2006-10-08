
#ifndef TRUSSUNITCOPY_H
#define TRUSSUNITCOPY_H

#include "Geometry.h"
#include "TrussUnit.h"

class LoadCaseResults;

/*****************************************************************************/

class TrussCopyNode
{    
public:
    TrussCopyNode ( const TrussNode& );
    ~TrussCopyNode ();
    
    void setBaseCoord ( DoublePoint );
    const DoublePoint& getBaseCoord () const;

    void setCurrentCoord ( DoublePoint );
    const DoublePoint& getCurrentCoord () const;

    void setDisplacement ( DoublePoint );
    const DoublePoint& getDisplacement () const;

    int getNumber () const;
    Node::Fixation getFixation () const;

private:
    int number;
    DoublePoint baseCoord, currentCoord, displacement;
    Node::Fixation fix;
};

/*****************************************************************************/

class TrussCopyPivot
{
public:
    TrussCopyPivot ( TrussCopyNode&, TrussCopyNode&, double, int );
    ~TrussCopyPivot ();

    TrussCopyNode& getFirstNode () const;
    TrussCopyNode& getLastNode () const;

    void setStress ( double );
    double getStress () const;

    double getThickness () const;

    int getNumber () const;

private:
    TrussCopyNode &first, &last;
    double thickness, stress;
    int number;
};

/*****************************************************************************/

class TrussUnitCopy : public QObject
{
    Q_OBJECT
public:
    typedef QList<TrussCopyNode*> TrussCopyNodeList;
    typedef QList<TrussCopyPivot*> TrussCopyPivotList;
    typedef TrussCopyNodeList::iterator TrussCopyNodeListIter;
    typedef TrussCopyPivotList::iterator TrussCopyPivotListIter;

    TrussUnitCopy ();
    ~TrussUnitCopy ();
    
    void loadTrussUnitData ( const TrussUnit& );
    void loadResults ( const LoadCaseResults& );
    void displaceNodes ( double );
    void restoreNodesBasePosition ();
    DoubleSize getTrussAreaSize () const;
    TrussCopyNodeList getNodeList () const;
    TrussCopyPivotList getPivotList () const;
    int countNodes () const;
    int countPivots () const;
    int countLoadCases () const;
    int countMaterials () const;

protected:
    void clearNodes();
    void clearPivots();
    void loadDisplacements ( const LoadCaseResults& res );
    void loadStresses ( const LoadCaseResults& res );
    TrussCopyNode* findNodeByNumber ( int ) const;

signals:
    void displacementLoaded ( bool );
    void stressLoaded ( bool );
    void trussDataLoaded ();
    void stateIsChanged ();

private:
    TrussCopyNodeList nodes;
    TrussCopyPivotList pivots;
    DoubleSize trussAreaSize;
    double nodesDeformCoeff;
    int loadCaseNumb, materialNumb;
};

#endif // TRUSSUNITCOPY_H
