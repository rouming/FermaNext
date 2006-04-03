
#ifndef AGGBUTTON_H
#define AGGBUTTON_H

#include <QString>
#include <QPoint>
#include <QSize>
#include <QObject>

class AggButton : public QObject
{
    Q_OBJECT
public:
    AggButton ( const QString&, const QPoint& leftTopPos, 
                int width, int height );
    AggButton ( QPoint leftTopPos, int width, int height );
    virtual ~AggButton ();

    virtual const QPoint& getPosition () const;
    virtual void setPosition ( const QPoint& newPos );

    virtual int getWidth () const;
    virtual void setWidth ( int w );

    virtual int getHeight () const;
    virtual void setHeight ( int h );

    virtual const QString& getLabel () const;
    virtual void setLabel ( const QString& );

    virtual const QString& getHint () const;    
    virtual void setHint ( const QString& );

    virtual void setPressed ( bool );
    virtual bool isPressed () const;

    virtual bool isHighlighted () const;
    virtual void setHighlighted ( bool );

    virtual bool inButtonRect ( int x, int y ) const;

signals:
    void onButtonPress();
    void onButtonRelease();
    void onChangeButtonHint ();
    void onChangeButtonState();

protected slots:
    void pressButton ();

private:
    int width, height;
    bool pressed, highlighted;
    // leftTopPos - button position in inner dynarow buffer coords
    QPoint leftTopPos;
    QString label, hint;
};

#endif //AGGBUTTON_H
