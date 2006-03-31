
#ifndef WINDOWLISTBOX_H
#define WINDOWLISTBOX_H

// Qt3 Support classes
#include <Q3ListBox>
#include <Q3PopupMenu>

#include <QBitArray>

#include "FermaNextProject.h"

class WindowListBox;

class TrussUnitWindowItem : public QObject, public Q3ListBoxPixmap
{
    Q_OBJECT
public:
    TrussUnitWindowItem ( FermaNextProject&, TrussUnitWindow&, 
                          WindowListBox&, const QPixmap&,
                          const QPixmap& disPix );

    virtual const QPixmap* pixmap () const;

    virtual void fillPopup ( Q3PopupMenu* ) const;

    virtual bool isSelectedInGroup () const;
    virtual bool isShown () const;
    virtual bool isAlive () const;

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
    // Life-time changes
    virtual void trussWindowDesisted ();
    virtual void trussWindowRevived ();

    virtual void update ();

signals:
    // Emits when item changes its visibility status
    void onWindowItemShow ( TrussUnitWindowItem& );
    void onWindowItemHide ( TrussUnitWindowItem& );

private:
    WindowListBox& listBox;
    FermaNextProject& project;
    TrussUnitWindow& trussWindow;
    QPixmap disabledPix;
    bool selected;    
};


class WindowListBox : public Q3ListBox
{
    Q_OBJECT
public:
    WindowListBox( FermaNextProject&, QWidget* parent = 0, 
                   const char* name = 0, Qt::WFlags fl = 0 );

    void contextMenuEvent ( QContextMenuEvent* );

protected:
    TrussUnitWindowItem* findByTrussUnitWindow ( const TrussUnitWindow& ) const;

public slots:
    virtual void addTrussUnitWindow ( TrussUnitWindow& );
    virtual void removeTrussUnitWindow ( TrussUnitWindow& );
    
    virtual void selectAllInGroup ();
    virtual void unselectAllFromGroup ();

    // Life-time changes
    virtual void trussWindowDesisted ( const TrussUnitWindow& );
    virtual void trussWindowRevived ( const TrussUnitWindow& );

    // Focus changes
    virtual void trussWindowReceivedFocus ( TrussUnitWindow& );
    virtual void trussWindowLostFocus ( TrussUnitWindow& );

protected slots:
    virtual void raiseWindowItem ( Q3ListBoxItem* );
    virtual void showWindowItem ( TrussUnitWindowItem& );
    virtual void hideWindowItem ( TrussUnitWindowItem& );

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
