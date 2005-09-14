#ifndef TRUSSUNIT_H
#define TRUSSUNIT_H

#include "Truss.h"
#include "AggSubsidiary.h"

class TrussNode;
class TrussPivot;

/*****************************************************************************/

typedef TrussLoadCaseArray<TrussNode> TrussUnitLoadCases;
typedef TrussLoadCase<TrussNode> TrussUnitLoadCase;

/*****************************************************************************/

class PaintableTrussElement
{
public:
    PaintableTrussElement ();
    PaintableTrussElement ( bool h, bool v, bool e, bool r );

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
                  public PaintableTrussElement
{
    Q_OBJECT
public:        
    TrussUnit ( const QString& name, ObjectStateManager* mng );
    virtual ~TrussUnit ();

    const QString& getTrussName () const;
    void nodeToFront ( TrussNode& node );
    // Manually call. We should save states before pivot desist.
    void desistAdjoiningPivots ( const TrussNode& node );

    void paintLoad ( TrussLoad& load, QPoint tailPos, ren_dynarow& baseRend ) const;
    void paint ( ren_dynarow& baseRend, double scaleMultX, 
                 double scaleMultY ) const;

public slots:
    void setTrussName ( const QString& name );

protected slots:
    void trussUnitStateIsChanged ();
    void clearFrontNodePointer ( Node& node );

signals:
    void onTrussNameChange ( const QString& );
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
    void onEnableChange ( bool );

private:
    static const QString UNNAMED;
    QString trussName;
    TrussNode* frontNode;
};

/*****************************************************************************/

class TrussNode: public Node, public PaintableTrussElement
{    
    Q_OBJECT
public:
    TrussNode ( ObjectStateManager* );
    TrussNode ( int x, int y, ObjectStateManager* );
    TrussNode ( int x, int y, Fixation, ObjectStateManager* );

    void drawFixation ( scanline_rasterizer& ras, solidRenderer& solidRend, 
                        agg::scanline_p8& sl, int trussAreaHeight,
                        double scaleMultX, double scaleMultY,
                        int lineWidth, color_type color ) const;

    void drawGradientEllipse ( scanline_rasterizer& ras, ren_dynarow& baseRend, 
                               agg::scanline_p8& sl, int x, int y, int radius, 
                               color_type begin, color_type middle, color_type end ) const;

    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight ) const;

signals:
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
    void onEnableChange ( bool );

public slots:
    void removeNodeHighlight ();
};

/*****************************************************************************/

class TrussPivot : public Pivot<TrussNode>, public PaintableTrussElement
{
    Q_OBJECT
public:
    TrussPivot ( ObjectStateManager* );
    TrussPivot ( TrussNode&, TrussNode&, ObjectStateManager* );

    bool getDrawingStatus () const;
    void setDrawingStatus ( bool status );
    
    void setHighlighted ( bool h );

    void paint ( ren_dynarow& baseRend, double scaleMultX, double scaleMultY,
                int trussAreaHeight) const;

signals:
// Paintable signals
    void onVisibleChange ( bool );
    void onHighlightChange ( bool );
    void onEnableChange ( bool );

public slots:
    void removePivotHighlight ();

private:
    bool drawingStatus;
};

#endif //TRUSSUNIT_H
