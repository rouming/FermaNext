
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitWindowManager.h"

class TrussUnitDesignerWidget : public AggQWidget
{
    Q_OBJECT
public:
    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual ~TrussUnitDesignerWidget ();

protected:
    // Manage window focus
    virtual void focusOnWindow ( TrussUnitWindow& );
    virtual void clearWindowFocus ();

    virtual TrussUnitWindow* findTrussUnitWindowByCoord ( int x, int y );    
    virtual void removeAllHighlight ();
    virtual void moveTrussNode ( int x, int y, TrussUnitWindow* window, 
                                TrussNode* node );
    virtual void moveTrussPivot ( int dx, int dy, TrussUnitWindow* window, 
                                 TrussPivot* pivot );

	virtual void initTrussUnitWindow(); //temp

    // Save states to Undo/Redo stack after drag
    virtual void saveNodeStateAfterDrag ( QPoint );
    virtual void savePivotStateAfterDrag ( QPoint first, QPoint last );

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
    virtual bool removeTrussUnitWindow ( const TrussUnitWindow& );

protected:
    virtual void clearTrussUnitWindows ();

private:
    enum TrussWindowBehaviour { windowIdle = 0, onWindowDrag, 
                                onHorResize, onVerResize, 
                                onBDiagResize, onFDiagResize };
    enum TrussNodeBehaviour { nodeIdle = 0, onNodeSelect, onNodeDrag };
    enum TrussPivotBehaviour { pivotIdle = 0, onPivotSelect, onPivotDrag };

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
    // Subsidiary vars
    int clickX, clickY, dxFirst, dxLast, dyFirst, dyLast;
    // Undo/Redo
    QPoint beforeDragNodePos;
    QPoint beforeDragFirstPos, beforeDragLastPos;

    // TODO: in future to remove
    bool init; //temp
    int X, Y; //temp    
};

#endif //TRUSSDESIGNERWIDGET_H
