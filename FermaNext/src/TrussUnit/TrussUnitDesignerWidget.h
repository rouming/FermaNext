
#ifndef TRUSSUNITDESIGNERWIDGET_H
#define TRUSSUNITDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitWindowManager.h"
#include "TrussUnitToolBar.h"
#include <qpopupmenu.h>

class FixationPopupMenu : public QPopupMenu
{
    Q_OBJECT
public:
    FixationPopupMenu ( QWidget* parent = 0, const char*  name = 0 );
    void showFixationPopup ( QMouseEvent* pressEvent, TrussNode* );

protected slots:
    void popupHide ();

    void fixNodeByX ();
    void fixNodeByY ();
    void fixNodeByXY ();
    void unfixNode ();

private:
    QMouseEvent releaseEvent;
    TrussNode* node;
};

/*****************************************************************************/

class TrussUnitDesignerWidget : public AggQWidget
{
    Q_OBJECT
public:
    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual ~TrussUnitDesignerWidget ();

    // Manage window focus
    virtual void focusOnWindow ( TrussUnitWindow& );
    virtual TrussUnitWindow* getFocusedWindow () const;

protected:
    virtual void clearWindowFocus ();
    virtual TrussUnitWindow* findWindowByWidgetPos ( int x, int y );
    
    // remove highlight from nodes and pivots and renew their behaviour  
    virtual void removeTrussElemHighlight ();
    // remove highlight from all window buttons
    virtual void removeWindowButtonHighlight ();
    // set release status for all window buttons
    virtual void releaseWindowButtons ();
    // clear window cursor position field
    virtual void clearAllCursorCoordFields ();

    virtual bool nodeCanBeDrawn ( int x, int y );

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
    void pressLoadDrawButton ();
    void pressEraseButton ();

    // Popup list signals
    void onFixationSet ( TrussNode& );
    void onLoadSet ( TrussNode& );

    // Focus signals                          
    void onFocusReceive ( TrussUnitWindow& );
    void onFocusLose ( TrussUnitWindow& );

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );    
    virtual bool removeTrussUnitWindow ( TrussUnitWindow& );
    virtual void changeBehaviourToSelect ();
    virtual void changeBehaviourToNodeDraw ();
    virtual void changeBehaviourToPivotDraw ();
    virtual void changeBehaviourToFixDraw ();
    virtual void changeBehaviourToLoadDraw ();
    virtual void changeBehaviourToErase ();

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
                            onPivotLastNodeDraw, onFixDraw, onLoadDraw, onErase };

    // Windows to show
    WindowList trussWindows;
    // Node fixation popup menu
    FixationPopupMenu* fixationPopup;
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
    bool buttonPressed;
    int clickX, clickY;
    QPoint firstNodeClickDist, lastNodeClickDist;
    // Undo/Redo
    DoublePoint beforeDragNodePos;
    DoublePoint beforeDragFirstPos, beforeDragLastPos;

    TrussUnitToolBar* toolBar;

    // TODO: in future to remove
    int X, Y; //temp    
};

#endif //TRUSSUNITDESIGNERWIDGET_H
