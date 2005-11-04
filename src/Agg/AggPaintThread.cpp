
#include "AggPaintThread.h"
#include <qdatetime.h>

/*****************************************************************************
 * Agg Paint Thread
 *****************************************************************************/

AggPaintThread::AggPaintThread( QWidget& p, int frames, 
                                int rate, int delay ) :
    widgetToRepaint( p ),
    framesNumb( frames ),
    frameRate( rate ),
    frameDelay( delay ),
    alphaCoeff( 0 ),
    alphaChange( 0 )
{}

AggPaintThread::AggPaintThread( QWidget& p ) :
    widgetToRepaint( p ),
    framesNumb( 0 ),
    frameRate( 0 ),
    frameDelay( 0 ),
    alphaCoeff( 0 ),
    alphaChange( 0 )
{}

AggPaintThread::~AggPaintThread ()
{}

void AggPaintThread::setFramesNumber ( int frames )
{
    framesNumb = frames;
}

void AggPaintThread::setFrameRate ( int rate )
{
    frameRate = rate;
}

void AggPaintThread::setFrameDelayMsec ( int delay )
{
    frameDelay = delay;
}

void AggPaintThread::setAlphaChannelCoeff ( double* alpha, double change )
{
    alphaCoeff = alpha;
    alphaChange = change;
}

void AggPaintThread::run ()
{
    int frameCount = 0;
    QTime time = QTime::currentTime();
    while ( frameCount < framesNumb ) 
    {
        if ( alphaCoeff )
        {
            *alphaCoeff+=alphaChange;
            if ( *alphaCoeff < 0 )
            {
                *alphaCoeff = 0;
                alphaChange = 0;
            }
            if ( *alphaCoeff > 1.0 )
            {
                *alphaCoeff = 1.0;
                alphaChange = 0;
            }
        }
        widgetToRepaint.update();
        emit onAnimationRun();
        time.addMSecs ( frameDelay );
        
        //int delay = time.msec() - QTime::currentTime().msec();
        //if ( delay > 0 )
        //    msleep( delay );
        
        msleep( frameDelay );
        frameCount+=frameRate;
    }
}

/*****************************************************************************/
