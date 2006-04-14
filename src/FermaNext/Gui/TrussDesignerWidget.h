
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QValidator>
#include <QMouseEvent>

#include "AggWidget.h"
#include "TrussUnitWindowManager.h"

class AggPopupHint;
class AggTrussToolBar;

/*****************************************************************************/

class FixationPopupMenu : public QMenu
{
    Q_OBJECT
public:
    FixationPopupMenu ( QWidget* parent = 0 );
    void showFixationPopup ( QMouseEvent* pressEvent, TrussNode* );

protected slots:
    void fixNodeByX ();
    void fixNodeByY ();
    void fixNodeByXY ();
    void unfixNode ();

private:
    TrussNode* node;
};

/*****************************************************************************/

class LoadPopupMenu : public QMenu
{
    Q_OBJECT
public:
    LoadPopupMenu ( QWidget* parent=0 );
    void showLoadPopup ( QMouseEvent* pressEvent, TrussNode*, 
                         TrussUnitWindow* );

protected slots:
    void okClicked();

private:
    TrussNode* node;
    TrussUnitWindow* selectedWindow;
    // x box and layout
    QWidget* xBox;
    QHBoxLayout* xLayout;
    // y box and layout
    QWidget* yBox;
    QHBoxLayout* yLayout;
    // buttons box and layout
    QWidget* buttonBox;
    QHBoxLayout* buttonLayout;
    // main layout
    QVBoxLayout* vLayout;
    // buttons
    QPushButton* ok;
    QPushButton* cancel;
    // edits and labels
    QLabel* loadXLabel;
    QLabel* loadYLabel;
    QLineEdit* loadXLine;
    QLineEdit* loadYLine;
};

/*****************************************************************************/

class TrussDesignerWidget : public AggWidget
{
    Q_OBJECT
public:
    TrussDesignerWidget ( QWidget* parent = 0 );
    virtual ~TrussDesignerWidget ();

    // Manage window focus
    virtual void focusOnWindow ( TrussUnitWindow& );
    virtual void focusOnPrevWindow ( TrussUnitWindow& );
    virtual void sendWindowToBack ( TrussUnitWindow& );
    virtual TrussUnitWindow* getFocusedWindow () const;

    // Returns window stack.
    virtual WindowList getTrussUnitWindowList () const;

protected:
    virtual void clearTrussUnitWindows ();
    virtual void initToolBar ();    

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
    virtual void clearHintsFromNonSelectedWindows ( TrussUnitWindow* );

    virtual bool nodeCanBeDrawn ( int x, int y );

    // Save states to Undo/Redo stack
    virtual void saveNodeStateAfterDrag ( TrussNode&, const DoublePoint& );
    virtual void saveNodeStateAfterCreate ( TrussNode& );
    virtual void saveNodeStateAfterRemove ( TrussNode& );
    virtual void savePivotStateAfterDrag ( TrussPivot& pivot,
                                           const DoublePoint& first, 
                                           const DoublePoint& last );
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

protected slots:
    // Life-time and visibility changes
    virtual void trussWindowChangedVisibility ( bool );
    virtual void trussWindowDesisted ( StatefulObject& );

    virtual void disableLoadDrawBehaviour ( bool );

private:
    enum TrussWindowBehaviour { windowIdle = 0, onWindowDrag, 
                                onHorResize, onVerResize, 
                                onBDiagResize, onFDiagResize };

    enum TrussNodeBehaviour { nodeIdle = 0, onNodeSelect, onNodeDrag };

    enum TrussPivotBehaviour { pivotIdle = 0, onPivotSelect, onPivotDrag };

    enum DesignerBehaviour { onSelect = 0, onNodeDraw, 
                             onPivotFirstNodeDraw, onPivotLastNodeDraw, 
                             onFixDraw, onLoadDraw, onErase };
    // Windows to show
    WindowList trussWindows;
    // Node fixation popup menu
    FixationPopupMenu* fixationPopup;
    // Node load popup menu
    LoadPopupMenu* loadPopup;

    // Top-level truss unit window.
    // Also, we get state manager from this window to collect 
    // states of this window between press and release mouse events. 
    // So, press mouse event have to start state block and release 
    // have close it.
    TrussUnitWindow* focusedWindow;

    ObjectStateManager* stateMngWithStartedBlock;
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
    bool buttonPressed, noLoadBehaviour;
    int xPos, yPos;
    QPoint firstNodeClickDist, lastNodeClickDist, mouseOffset;
    // Undo/Redo
    DoublePoint beforeDragNodePos;
    DoublePoint beforeDragFirstPos, beforeDragLastPos;

    AggTrussToolBar* toolBar;
    AggPopupHint* aggHint;

    // TODO: in future to remove
    int X, Y; //temp    
};

#endif //TRUSSDESIGNERWIDGET_H
