#ifndef AGGPOPUPMENU_H
#define AGGPOPUPMENU_H

#include <QObject>
#include <QPoint>

#include "AggSubsidiary.h"
#include "Global.h"

/*****************************************************************************/

class AggMenuAction : public QObject
{
    Q_OBJECT

public:
    AggMenuAction ( const QString& text );
    virtual ~AggMenuAction ();

    void highlight ( bool );
    bool isHighlighted () const;

    void setText ( const QString& ); 
    const QString& text () const;

public slots:
    void trigger ();

signals:
    void triggered ();
    void stateChanged();

private:
    QString actionText;
    bool highlighted;
};

/*****************************************************************************/

class AggPopupMenu : public QObject
{
    Q_OBJECT

public:
    AggPopupMenu ();

    virtual ~AggPopupMenu ();

    void addAction ( const QString& );
    void removeAction ( AggMenuAction* );
    void removeAction ( int idx );
    AggMenuAction* getAction ( int idx ) const;

    int width () const;
    int height () const;

    textFont getActionFont () const;

    QPoint getPosition () const;
    void setPosition ( QPoint leftTop );

    void resize ( int w, int h );

    virtual void mouseMove ( int xGlobal, int yGlobal );
    virtual void mousePress ( int xGlobal, int yGlobal );
    virtual void mouseRelease ( int xGlobal, int yGlobal );

    bool isVisible () const;

    bool isRendered () const;

    void clear ();

    virtual void paint ( base_renderer& ) const;

protected:
    virtual void updateGeometry ();
    
    QPoint getSelectionLeftTop ( const AggMenuAction& action ) const;
    QPoint fromGlobalPos (int x, int y ) const;
    bool inActionRect ( int, int, const AggMenuAction& action ) const;
    bool inMenuRect ( int, int ) const;
    
    void setVisible ( bool );

    void drawAction ( ren_dynarow&, const AggMenuAction& action ) const;
    
public slots:
    void clearRenderedFlag ();
    void show ();
    void hide ();

protected slots:
    void onActionTriggered ();

signals:
    void actionTriggered ( int );
    void renderedFlagCleared ();

private:
    typedef QList<AggMenuAction*> ActionList;

    ActionList actionList;

    QPoint leftTopPos;
    textFont actionFont;

    int widthLimit;
    int selectionHeight, selectionWidth;
    int selectionVerIndent, selectionHorIndent;

    int separatorHeight, topIndent, bottomIndent, leftIndent, rightIndent;

    mutable bool rendered;

    bool visible;

    rbuf_dynarow* menuBuf;
};

#endif //AGGPOPUPMENU_H
