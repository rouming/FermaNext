
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitWindowManager.h"
#include "agg_svg_parser.h"

class TrussUnitDesignerWidget : public AggQWidget
{
    Q_OBJECT
public:
    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual ~TrussUnitDesignerWidget ();

    // Manage window focus
    virtual void focusOnWindow ( TrussUnitWindow& );

protected:
    virtual void clearWindowFocus ();
    virtual TrussUnitWindow* findWindowByWidgetPos ( int x, int y );    
    virtual void removeAllHighlight ();
    virtual bool nodeCanBeDrawn ( int x, int y );

    // Save states to Undo/Redo stack
    virtual void saveNodeStateAfterDrag ( QPoint );
    virtual void saveNodeStateAfterCreate ( TrussUnit&, TrussNode& );
    virtual void saveNodeStateAfterRemove ( TrussUnit&, TrussNode& );
    virtual void savePivotStateAfterDrag ( QPoint first, QPoint last );    
    virtual void savePivotStateAfterCreate ( TrussUnit&, TrussNode& firstNode,
                                             TrussNode& lastNode, TrussPivot& );

    // SVG viewer methods
    virtual void parseSvg ( const char* fname );
    virtual void drawSvg ( base_renderer&, solid_renderer&, int x, int y );

public:
    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );    
    virtual bool removeTrussUnitWindow ( TrussUnitWindow& );

protected:
    virtual void clearTrussUnitWindows ();

private:
    enum TrussWindowBehaviour { windowIdle = 0, onWindowDrag, 
                                onHorResize, onVerResize, 
                                onBDiagResize, onFDiagResize };
    enum TrussNodeBehaviour { nodeIdle = 0, onNodeSelect, onNodeDrag };
    enum TrussPivotBehaviour { pivotIdle = 0, onPivotSelect, onPivotDrag };
    enum DesignerBehaviour { onSelect = 0, onNodeDraw, onPivotFirstNodeDraw, 
                            onPivotLastNodeDraw, onErase, onFixSet };

    // Windows to show
    WindowList trussWindows;
    // Focused truss unit window
    TrussUnitWindow* focusedWindow;
    // Current selected truss elements
    TrussUnitWindow* selectedWindow;
    TrussNode* selectedNode;
    TrussPivot* selectedPivot;
    // Different behaviours
    TrussWindowBehaviour winBehaviour;
    TrussNodeBehaviour nodeBehaviour;
    TrussPivotBehaviour pivotBehaviour;
    DesignerBehaviour designerBehaviour;
    // Subsidiary vars
    int clickX, clickY;
    QPoint firstNodeClickDist, lastNodeClickDist;
    // Undo/Redo
    QPoint beforeDragNodePos;
    QPoint beforeDragFirstPos, beforeDragLastPos;

    agg::svg::path_renderer pathRend;

    // TODO: in future to remove
    int X, Y; //temp    
};

#endif //TRUSSDESIGNERWIDGET_H
