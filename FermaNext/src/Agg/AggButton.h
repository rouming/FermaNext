
#ifndef AGGBUTTON_H
#define AGGBUTTON_H

#include "AggSubsidiary.h"
#include "SubsidiaryConstants.h"
#include "qstring.h"
#include "qpoint.h"
#include "qsize.h"
#include <qobject.h>

class AggButton : public QObject
{
    Q_OBJECT
public:
    // leftTopPos - button position in inner dynarow buffer coords
    AggButton ( QString label_, QPoint leftTopPos, int width, int height );
    AggButton ( QPoint leftTopPos, int width, int height );
    virtual ~AggButton ();
    
    virtual void setHint ( QString hint_ );
    virtual QString getHint () const;

    virtual void setLabel ( QString label_ );
    virtual QString getLabel () const;

    virtual void setPosition ( QPoint newPos );
    virtual QPoint getPosition () const;

    virtual void setWidth ( int w );
    virtual int getWidth () const;

    virtual void setHeight ( int h );
    virtual int getHeight () const;

    virtual bool isHighlighted () const;
    virtual void setHighlighted ( bool );

    virtual bool inButtonRect ( int x, int y ) const;

    virtual bool isPressed () const;
    virtual void setPressed ( bool );

signals:
    void onButtonHighlightChange ();
    void onButtonPress();
    void onButtonRelease();
    void onChangeButtonState();

protected slots:
    void pressButton ();

private:
    int width, height;
    bool highlighted, pressed;
    QPoint leftTopPos;
    QString label, hint;
};

#endif //AGGBUTTON_H
