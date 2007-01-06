
#ifndef AGGBUTTON_H
#define AGGBUTTON_H

#include <QString>
#include <QPoint>
#include <QSize>
#include <QObject>

#include "AggSubsidiary.h"

class AggButton : public QObject
{
    Q_OBJECT
public:
    AggButton ( const QString& );
    AggButton ();
    virtual ~AggButton ();

    virtual QPoint getPosition () const;
    virtual void setPosition ( QPoint newPos );

    virtual int getWidth () const;
    virtual void setWidth ( int w );

    virtual int getHeight () const;
    virtual void setHeight ( int h );

    virtual void setText ( const QString& );
    virtual QString text () const;

    virtual const QString& getHint () const;    
    virtual void setHint ( const QString& );

    virtual const QString& getStatusTip () const;
    virtual void setStatusTip ( const QString& );

    virtual void setPressed ( bool );
    virtual bool isPressed () const;

    virtual void setEnabled ( bool );
    virtual bool isEnabled () const;

    virtual void setVisible ( bool );
    virtual bool isVisible () const;

    virtual bool isHighlighted () const;
    virtual void setHighlighted ( bool );

    virtual bool inButtonRect ( int x, int y ) const;

    virtual void paint ( ren_dynarow& baseRend ) const;

signals:
    void onButtonPress();
    void onButtonRelease();
    void onChangeButtonHint ();
    void onChangeButtonState();

protected slots:
    void pressButton ();

private:
    int width, height;
    bool pressed, enabled, visible, highlighted;
    // leftTopPos - button position in inner dynarow buffer coords
    QPoint leftTopPos;
    QString buttonText, hint, statusTip;
};

#endif //AGGBUTTON_H
