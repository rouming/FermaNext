
#ifndef AGGQWIDGET_H
#define AGGQWIDGET_H

#include <vector>
#include <qwidget.h>
#include <qimage.h>
#include <agg_basics.h>
#include <agg_rendering_buffer.h>
#include <util/agg_color_conv_rgb8.h>
#include <ctrl/agg_ctrl.h>

class AggCtrlContainer
{
    typedef std::vector<agg::ctrl*> CtrlList;
private:
    CtrlList ctrls;
    agg::ctrl* current;

public:
    AggCtrlContainer ();

    void addCtrl ( agg::ctrl& );    
    bool removeCtrl ( agg::ctrl& );    
    bool inRect ( double x, double y );
    const agg::ctrl* whoIsInRect ( double x, double y );
    bool currentCtrl ( double x, double y );
    const agg::ctrl& currentCtrl ();

    // Events
    const agg::ctrl* mousePressEvent ( double x, double y );
    const agg::ctrl* mouseReleaseEvent ( double x, double y );
    const agg::ctrl* mouseMoveEvent ( double x, double y, bool button_flag );
    const agg::ctrl* arrowKeysPressEvent ( bool left, bool right,
                                           bool down, bool up );
};

class AggQWidget : public QWidget
{
protected:
    typedef agg::rendering_buffer          rendering_buffer;

private:
    rendering_buffer      aggBuffer;    // store the ptr to reattach to m_image on resize 
    QImage*               mainQImage;   // this is attached to the main rendering buffer
    AggCtrlContainer      ctrlContainer;
    bool                  aggFlipY;     // flip the y axis?
    int                   initialWidth;
    int                   initialHeight;

public:
    AggQWidget ( QWidget* parent = 0, bool flip_y = false );
    virtual ~AggQWidget ();

    // Just prints widget
    virtual void print ();

protected:
    virtual rendering_buffer& getAggRenderingBuffer ();
    virtual QImage& getMainImage ();

// Hide QT events, override AGG events instead.
private:
    virtual void paintEvent ( QPaintEvent* );
    virtual void resizeEvent ( QResizeEvent* );
    virtual void keyPressEvent ( QKeyEvent* );
    virtual void mouseMoveEvent ( QMouseEvent* );
    virtual void mouseReleaseEvent ( QMouseEvent* );
    virtual void mousePressEvent ( QMouseEvent* );
    

// Shared AGG events
protected:
    virtual void aggPaintEvent ( QPaintEvent* );
    virtual void aggResizeEvent ( QResizeEvent* );
    virtual void aggKeyPressEvent ( QKeyEvent* );
    virtual void aggMouseMoveEvent ( QMouseEvent* );
    virtual void aggMouseReleaseEvent ( QMouseEvent* );
    virtual void aggMousePressEvent ( QMouseEvent* );
    virtual void aggCtrlChangedEvent ( const agg::ctrl* );
    
};

#endif //AGGQWIDGET_H
