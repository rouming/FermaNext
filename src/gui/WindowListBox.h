
#ifndef WINDOWLISTBOX_H
#define WINDOWLISTBOX_H

#include "FermaNextProject.h"
#include <qlistbox.h>
#include <qbitarray.h>

class WindowListBox;

class TrussUnitWindowItem : public QObject, public QListBoxPixmap
{
    Q_OBJECT
public:
    TrussUnitWindowItem ( FermaNextProject&, TrussUnitWindow&, 
                          WindowListBox&, const QPixmap&,
                          const QPixmap& disPix );

    virtual const QPixmap* pixmap () const;

    virtual void fillPopup ( QPopupMenu* ) const;

    virtual bool isSelectedInGroup () const;
    virtual bool isShown () const;
    virtual bool isAlive () const;

protected:
    virtual void saveVisibilityChange ( bool );

public slots:
    virtual void setText ( const QString& );

    virtual void raise ();
    virtual void show ();
    virtual void hide ();
    virtual void selectInGroup ();
    virtual void unselectFromGroup ();
    virtual void selectAllInGroup ();
    virtual void unselectAllFromGroup ();
    virtual void calculate ();
    virtual void remove ();

protected slots:
    virtual void trussWindowDesisted ();
    virtual void trussWindowRevived ();
    virtual void update ();

private:
    WindowListBox& listBox;
    FermaNextProject& project;
    TrussUnitWindow& trussWindow;
    QPixmap disabledPix;
    bool selected;    
};


class WindowListBox : public QListBox
{
    Q_OBJECT
public:
    WindowListBox( FermaNextProject&, QWidget* parent = 0, 
                   const char* name = 0, WFlags fl = 0 );

    void contextMenuEvent ( QContextMenuEvent* );

protected:
    TrussUnitWindowItem* findByTrussUnitWindow ( const TrussUnitWindow& ) const;

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );
    virtual void removeTrussUnitWindow ( TrussUnitWindow& );
    
    virtual void selectAllInGroup ();
    virtual void unselectAllFromGroup ();

    virtual void trussWindowDesisted ( const TrussUnitWindow& );
    virtual void trussWindowRevived ( const TrussUnitWindow& );

protected slots:
    virtual void raiseTrussUnitWindowItem ( QListBoxItem* );

private:
    struct IndexedItem 
    {
        TrussUnitWindowItem* item;
        int index;
    };

    typedef QMap< const TrussUnitWindow*, IndexedItem > WindowMap;
    typedef WindowMap::iterator WindowMapIter;

    FermaNextProject& project;
    WindowMap windowItems;
};

#endif // WINDOWLISTBOX_H
