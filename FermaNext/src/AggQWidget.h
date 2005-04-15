
#ifndef AGGQWIDGET_H
#define AGGQWIDGET_H

#include <qwidget.h>
#include <qimage.h>

#include <agg_basics.h>
#include <agg_rendering_buffer.h>
#include <agg_trans_affine.h>
#include <util/agg_color_conv_rgb8.h>
#include <ctrl/agg_ctrl.h>
#include <vector>

class AggCtrlContainer
{
    typedef std::vector<agg::ctrl*> CtrlList;
private:
    CtrlList ctrls;
    agg::ctrl* current;

public:
    AggCtrlContainer ();

    virtual void addCtrl ( agg::ctrl& );    
    virtual bool removeCtrl ( agg::ctrl& );    
    virtual bool inRect ( double x, double y );
    virtual bool currentCtrl ( double x, double y );
    virtual const agg::ctrl& currentCtrl ();

    // Events
    virtual bool mousePressEvent ( double x, double y );
    virtual bool mouseReleaseEvent ( double x, double y );
    virtual bool mouseMoveEvent ( double x, double y, bool button_flag );
    virtual bool arrowKeysPressEvent ( bool left, bool right, bool down, bool up );
};

class AggQWidget : public QWidget
{
private:    
    agg::rendering_buffer aggBuffer;    // store the ptr to reattach to m_image on resize 
    bool                  aggFlipY;     // flip the y axis?
    agg::trans_affine     aggResizeMtx;   
    QImage*               mainQImage;   // this is attached to the main rendering buffer
    int                   initialWidth;
    int                   initialHeight;

    AggCtrlContainer      ctrlContainer;

public:
    AggQWidget ( QWidget* parent = 0, bool flip_y = false );
    ~AggQWidget ();

//protected:
public:
    virtual agg::rendering_buffer& getAggRenderingBuffer ();
    virtual agg::trans_affine  getAggResizeMatrix ();
    virtual QImage& getMainImage ();

    virtual bool flipY ();
    virtual void flipY ( bool );

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
    
};

#endif //AGGQWIDGET_H
