
#ifndef AGGPAINTTHREAD_H
#define AGGPAINTTHREAD_H

#include <qthread.h>
#include <qobject.h>
#include <qwidget.h>

class AggPaintThread : public QObject, public QThread
{
    Q_OBJECT
public:
    AggPaintThread ( QWidget& p, int frames, int rate, int delay );
    AggPaintThread ( QWidget& p );
    virtual ~AggPaintThread ();

    virtual void setFramesNumber ( int frames );
    virtual void setFrameRate ( int rate );
    virtual void setFrameDelayMsec ( int delay );
    virtual void setAlphaChannelCoeff ( double* alpha, 
                                        double alphaChange );
    virtual void run ();

signals:
    void onAnimationRun ();

private:
    QWidget& widgetToRepaint;
    int framesNumb, frameRate, frameDelay;
    double* alphaCoeff;
    double alphaChange;
};

#endif //AGGPAINTTHREAD_H
