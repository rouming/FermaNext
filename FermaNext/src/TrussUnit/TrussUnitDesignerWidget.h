
#ifndef TRUSSUNITDESIGNERWIDGET_H
#define TRUSSUNITDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitWindowManager.h"
#include "TrussUnitToolBar.h"

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
    virtual void clearAllCursorCoordFields ();

    // Save states to Undo/Redo stack
    virtual void saveNodeStateAfterDrag ( DoublePoint );
    virtual void saveNodeStateAfterCreate ( TrussNode& );
    virtual void saveNodeStateAfterRemove ( TrussNode& );
    virtual void savePivotStateAfterDrag ( DoublePoint first, 
                                           DoublePoint last );
    virtual void savePivotStateAfterCreate ( TrussNode& firstNode, 
                                             bool firstPivotWasCreated,
                                             TrussNode& lastNode, 
                                             TrussPivot& );
public:
    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );

signals:
    void pressSelectButton ();
    void pressNodeDrawButton ();
    void pressPivotDrawButton ();
    void pressFixDrawButton ();
    void pressForceDrawButton ();
    void pressEraseButton ();

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );    
    virtual bool removeTrussUnitWindow ( TrussUnitWindow& );
    virtual void changeBehaviourToSelect ();
    virtual void changeBehaviourToNodeDraw ();
    virtual void changeBehaviourToPivotDraw ();
    virtual void changeBehaviourToFixDraw ();
    virtual void changeBehaviourToForceDraw ();
    virtual void changeBehaviourToErase ();
    virtual void updateDesignerWidget ();

protected:
    virtual void initToolBar ();
    virtual void clearTrussUnitWindows ();

private:
    enum TrussWindowBehaviour { windowIdle = 0, onWindowDrag, 
                                onHorResize, onVerResize, 
                                onBDiagResize, onFDiagResize };
    enum TrussNodeBehaviour { nodeIdle = 0, onNodeSelect, onNodeDrag };
    enum TrussPivotBehaviour { pivotIdle = 0, onPivotSelect, onPivotDrag };
    enum DesignerBehaviour { onSelect = 0, onNodeDraw, onPivotFirstNodeDraw, 
                            onPivotLastNodeDraw, onFixDraw, onForceDraw, onErase };

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
    QPoint firstNodeClickDist, lastNodeClickDist, toolBarCenterPos;
    // Undo/Redo
    DoublePoint beforeDragNodePos;
    DoublePoint beforeDragFirstPos, beforeDragLastPos;

    TrussUnitToolBar* toolBar;

    // TODO: in future to remove
    int X, Y; //temp    
};

#endif //TRUSSUNITDESIGNERWIDGET_H
