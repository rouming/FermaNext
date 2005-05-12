
#ifndef TRUSSDESIGNERWIDGET_H
#define TRUSSDESIGNERWIDGET_H

#include "AggQWidget.h"
#include "TrussUnitPseudoWindow.h"

typedef std::vector<TrussUnitPseudoWindow*>  PseudoWindowList;
typedef std::vector<TrussUnitPseudoWindow*>::iterator  PseudoWindowListIter;

class TrussUnitDesignerWidget : public AggQWidget
{
    Q_OBJECT
public:
    bool init; //temp    

    TrussUnitDesignerWidget ( QWidget* parent = 0 );
    
    virtual bool isHorResize ( int x, int y );
    virtual bool isVerResize ( int x, int y );
    virtual bool isBDiagResize ( int x, int y );
    virtual bool isFDiagResize ( int x, int y );
    virtual TrussUnitPseudoWindow* whoIsInRect ( int x, int y );
	virtual void initPseudoWindow(); //temp
	virtual void onDraw();

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
    virtual bool removeTrussUnitPseudoWindow ( const TrussUnitPseudoWindow& );

private:
    PseudoWindowList pseudoWindows;
    TrussUnitPseudoWindow* selectedPseudoWindow;
    enum winBehaviours { nothing = 0, onDrag, onHorResize, onVerResize, 
        onBDiagResize, onFDiagResize };
    int behaviour;
    int clickX, clickY;
};

#endif //TRUSSDESIGNERWIDGET_H
