
#ifndef AGGPAINTTHREAD_H
#define AGGPAINTTHREAD_H

#include <QThread>
#include <QObject>
#include <QWidget>

class AggPaintThread : public QThread
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
