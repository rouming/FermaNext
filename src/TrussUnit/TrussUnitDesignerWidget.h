
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitWindowManager.h"

class TrussUnitDesignerWidget : public AggQWidget
{
    Q_OBJECT
public:
    bool init; //temp    
    int X1, Y1; //temp       
    int X2, Y2; //temp        

    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    virtual ~TrussUnitDesignerWidget ();

protected:
    virtual void trussWindowToFront ( TrussUnitWindow& );    
    virtual TrussUnitWindow* findTrussUnitWindowByCoord ( int x, int y );    
    virtual void removeAllHighlight ();
	virtual void onDraw();
	virtual void initTrussUnitWindow(); //temp

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
    enum WindowBehaviour { windowIdle = 0, onDrag, onHorResize, onVerResize, 
                           onBDiagResize, onFDiagResize };
    enum TrussElementBehaviour { trussElementIdle = 0, onNodeSelect, onNodeDrag,
                                  onPivotSelect, onPivotDrag };
    WindowList trussWindows;
    TrussUnitWindow* trussWindow;
    TrussNode* trussNode;
    WindowBehaviour winBehaviour;
    TrussElementBehaviour trussElemBehaviour;
    int clickX, clickY;
};

#endif //TRUSSDESIGNERWIDGET_H
