
#ifndef AGGPOPUPHINT_H
#define AGGPOPUPHINT_H

#include "AggSubsidiary.h"
#include "qobject.h"
#include "qpoint.h"
#include "qsize.h"
#include "qstring.h"

class QWidget;
class AggPaintThread;

class AggPopupHint : public QObject
{
    Q_OBJECT
public:
    AggPopupHint ( QWidget& widget );
    virtual ~AggPopupHint ();

    virtual void renewWidgetSize ( const QSize& );

    virtual void paint ( base_renderer& baseRenderer ) const;

protected slots:
    void show ( const QString&, const QPoint, bool );
    void hide ( bool );

protected:
    virtual void renewHintGeometry ();
    virtual QPoint getHintPosition () const;

private:
    enum SmoothType { noSmooth = 0, smoothShow, smoothHide };

    QString text_;
    textFont font_;
    QPoint cursorPos_;
    QSize size_, widgetSize_;
    double alphaCoeff; // alpha coefficient for smooth changing 
                       // of alpha channel
    SmoothType smooth_;
    mutable bool rendered_;
    rbuf_dynarow* buf_; // hint rendering buffer
    AggPaintThread* thread_;
};

#endif //AGGPOPUPHINT_H
