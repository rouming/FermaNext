#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include "XMLSerializableObject.h"
#include "AggSubsidiary.h"

class TrussUnit;
class TrussNode;
class TrussPivot;

/*****************************************************************************/

typedef QList<DoublePoint> DoublePointArray;

/*****************************************************************************/

class PaintableTrussElement
{
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool h, bool v, bool e, bool r );
    virtual ~PaintableTrussElement ();

    virtual bool isVisible () const;
    virtual bool isHighlighted () const;
    virtual bool isEnabled () const;
    virtual bool isRendered () const;

public:
    virtual void setVisible ( bool );
    virtual void setHighlighted ( bool );
    virtual void setEnabled ( bool );

protected:
    virtual void rendered ( bool ) const;


// We should use this pretty hack to workaround Qt prohibition
// of using multiple inheritance. So we just declare methods as 
// pure virtual and emit them in the body of the this class.
// We get cheap possibility to have signals and multiple inheritance
// at the same time.
protected:
    virtual void onVisibleChange ( bool ) = 0;
    virtual void onHighlightChange ( bool ) = 0;
    virtual void onEnableChange ( bool ) = 0;

private:
    bool visible;
    bool highlighted;
    bool enabled;
    mutable bool renderedFlag;
};

/*****************************************************************************/

class TrussUnit : public Truss<TrussNode, TrussPivot>,                  
                  public PaintableTrussElement,
                  public XMLSerializableObject
{
    Q_OBJECT
public:        
    TrussUnit ( const QString& name, ObjectStateManager* mng );
    virtual ~TrussUnit ();

    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    const QString& getTrussName () const;

    virtual bool isCalculated () const;
    virtual void setCalculatedStatus ( bool );

    virtual void setFocusOnNode ( TrussNode* selectedNode );
    virtual void setFocusOnPivot ( TrussPivot* selectedPivot );
    virtual void removeNodesHighlight ( const TrussNode* node1 = 0, 
                                        const TrussNode* node2 = 0 );
    virtual void removePivotsHighlight ( const TrussPivot* pivot = 0 );

    virtual DoublePoint getTrussCoordFromWidgetPos ( int x, int y ) const;
    virtual DoublePoint getTrussCoordFromWidgetPos ( QPoint pos ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( double x, double y ) const;
    virtual QPoint getWidgetPosFromTrussCoord ( const DoublePoint& ) const;

    virtual DoubleSize getTrussSize () const;

    virtual double getNodePrecision ( bool inPix = true ) const;
    virtual double getPivotPrecision () const;

    virtual TrussNode* findNodeByWidgetPos ( const QPoint& pos, 
                                             double precision ) const;
    virtual TrussNode* findNodeByWidgetPos ( const QPoint& pos ) const;
    virtual TrussNode* findNodeByWidgetPos ( int x, int y, 
                                             double precision ) const;
    virtual TrussNode* findNodeByWidgetPos ( int x, int y ) const;

    virtual TrussPivot* findPivotByWidgetPos ( const QPoint& pos, 
                                               double precision ) const;
    virtual TrussPivot* findPivotByWidgetPos ( const QPoint& pos ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y, 
                                               double precision ) const;
    virtual TrussPivot* findPivotByWidgetPos ( int x, int y ) const;

    virtual void moveTrussNode ( int x, int y, TrussNode* node );

    virtual void moveTrussPivot ( int x, int y, TrussPivot* pivot, 
                                  QPoint firstNodeClickDist, 
                                  QPoint lastNodeClickDist );

    virtual void mergeNodes ( TrussNode* mergingNode, TrussNode* node );

    virtual void dividePivot ( TrussPivot& dividualPivot, 
                               TrussNode& dividingNode );

    virtual TrussPivot* findDividualPivot ( TrussNode& dividingNode,
                                            double precision ) const;
    virtual TrussPivot* findDividualPivot ( TrussNode& dividingNode ) const;

    virtual void updateAfterNodeManipulation ( TrussNode* selectedNode, 
                                              bool fixationCheck );

    virtual void updateAfterPivotManipulation ( TrussPivot* selectedPivot, 
                                               bool fixationCheck );

    void nodeToFront ( TrussNode& node );
    // Manually call. We should save states before pivot desist.
    void desistAdjoiningPivots ( const TrussNode& node );

    void paint ( ren_dynarow& baseRend ) const;

public slots:
    void setTrussName ( const QString& name );

protected slots:
    void trussUnitStateIsChanged ();
    void clearFrontNodePointer ( const Node& node );
    void clearFrontPivotPointer ( const Node& first, const Node& last );

signals:
    void onTrussNameChange ( const QString& );
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
    void onEnableChange ( bool );

protected:
    void pivotToFront ( TrussPivot& pivot );

    virtual void setTrussRenderedStatus ( bool ) const;

    virtual void setTrussPosition ( const QPoint& pos );
    virtual void setTrussAreaSizeInPix ( const QSize& size );    

    virtual DoublePoint getTrussScaleMultiplier () const;

    virtual TrussPivot* findPivotByCoord ( const DoublePoint& coord, 
                                           double precision ) const;
    virtual TrussPivot* findPivotByCoord ( const DoublePoint& coord ) const;

    virtual TrussNode* nodesMergingComparison ( TrussNode& comparableNode, 
                                                double precision, 
                                                bool fixationCheck );

    virtual DoublePointArray getPivotCrossPoints ( 
                                    const PivotList& nonCrossingPivots ) const;
    virtual TrussNode& createCrossNode ( const DoublePoint& crossPoint );
    virtual void createPivotCrossNodes ( const DoublePointArray& );

    virtual void updateNodePosition ( TrussNode* selectedNode, 
                                      bool fixationCheck );

    void drawLoad ( ren_dynarow& baseRend, const TrussLoad& load, 
                    const QPoint& tailPos ) const;

    virtual void drawNodeNumber ( TrussNode* node, 
                                  ren_dynarow& baseRend, 
                                  solidRenderer& solidRend, 
                                  scanline_rasterizer& ras, 
                                  agg::scanline_p8& sl ) const;

    virtual void drawPivotNumber ( TrussPivot* pivot, 
                                   ren_dynarow& baseRend, 
                                   solidRenderer& solidRend, 
                                   scanline_rasterizer& ras, 
                                   agg::scanline_p8& sl ) const;

    virtual void drawTrussElementsNumbers ( ren_dynarow& baseRend, 
                                            solidRenderer& solidRend, 
                                            scanline_rasterizer& ras, 
                                            agg::scanline_p8& sl ) const;

private:
    mutable bool trussRendered, calculated;
    QPoint leftTopPos;
    QSize pixAreaSize;
    QString trussName;
    TrussNode *frontNode, *firstFront, *lastFront;
};

/*****************************************************************************/

class TrussNode: public Node, 
                 public PaintableTrussElement,
                 public XMLSerializableObject
{    
    Q_OBJECT
public:
    TrussNode ( ObjectStateManager* );
    TrussNode ( double x, double y, ObjectStateManager* );
    TrussNode ( double x, double y, Fixation, ObjectStateManager* );

    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    void drawFixation ( scanline_rasterizer& ras, solidRenderer& solidRend, 
                        agg::scanline_p8& sl, const QPoint& nodePos,
                        int lineWidth, color_type color ) const;

    void paint ( ren_dynarow& baseRend,  const DoublePoint& scaleMult,
                 double trussAreaHeight ) const;

signals:
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
    void onEnableChange ( bool );

public slots:
    void removeNodeHighlight ();
};

/*****************************************************************************/

class TrussPivot : public Pivot<TrussNode>, 
                   public PaintableTrussElement,
                   public XMLSerializableObject
{
    Q_OBJECT
public:
    TrussPivot ( ObjectStateManager* );
    TrussPivot ( TrussNode&, TrussNode&, ObjectStateManager* );

    // XML serialization
    virtual void loadFromXML ( const QDomElement& ) throw (LoadException);
    virtual QDomElement saveToXML ( QDomDocument& );

    bool getDrawingStatus () const;
    void setDrawingStatus ( bool status );
    
    void setHighlighted ( bool h );

    void paint ( ren_dynarow& baseRend,  const DoublePoint& scaleMult,
                 double trussAreaHeight) const;

signals:
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
    void onEnableChange ( bool );
    void onDrawingStatusChange ( bool );

public slots:
    void removePivotHighlight ();

private:
    bool drawingStatus;
};

#endif //TRUSSUNIT_H
