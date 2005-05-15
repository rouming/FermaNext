
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitPseudoWindow.h"

class TrussUnitDesignerWidget : public AggQWidget
{
    Q_OBJECT
public:
    bool init; //temp    
    int X1, Y1; //temp       
    int X2, Y2; //temp        

    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual ~TrussUnitDesignerWidget ();
    
    virtual TrussUnitPseudoWindow* findPseudoWindowByCoord ( int x, int y );
    virtual void pseudoWindowToFront ( TrussUnitPseudoWindow* window );
	virtual void onDraw();
	virtual void initPseudoWindow(); //temp

    // Handlers on events
    void aggPaintEvent ( QPaintEvent* );
    void aggResizeEvent ( QResizeEvent* );
    void aggKeyPressEvent ( QKeyEvent* );
    void aggMouseMoveEvent ( QMouseEvent* );
    void aggMouseReleaseEvent ( QMouseEvent* );
    void aggMousePressEvent ( QMouseEvent* );
    void aggCtrlChangedEvent ( const agg::ctrl* );

public slots:
    virtual void addTrussUnit ( TrussUnit& );
    virtual bool removeTrussUnit ( const TrussUnit& );
    virtual bool removeTrussUnitPseudoWindow ( const TrussUnitPseudoWindow& );

protected:
    virtual void clearPseudoWindows ();

private:
    typedef std::vector<TrussUnitPseudoWindow*> PseudoWindowList;
    typedef std::vector<TrussUnitPseudoWindow*>::iterator PseudoWindowListIter;

    enum WindowBehaviour { windowIdle = 0, onDrag, onHorResize, onVerResize, 
                           onBDiagResize, onFDiagResize };
    enum TrussElementBehaviour { trussElementIdle = 0, onNodeSelect, onNodeDrag,
                                  onPivotSelect, onPivotDrag };
    PseudoWindowList pseudoWindows;
    TrussUnitPseudoWindow* pseudoWindow;
    TrussNode* trussNode;
    WindowBehaviour winBehaviour;
    TrussElementBehaviour trussElemBehaviour;
    int clickX, clickY;
};

#endif //TRUSSDESIGNERWIDGET_H
