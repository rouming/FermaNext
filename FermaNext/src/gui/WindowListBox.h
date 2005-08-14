
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
                          WindowListBox*, const QPixmap&,
                          const QPixmap& disPix );

    virtual const QPixmap* pixmap () const;

    virtual void fillPopup ( QPopupMenu* ) const;

    virtual bool isSelectedInGroup () const;
    virtual bool isShown () const;

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

private:
    WindowListBox* listBox;
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

protected slots:
    virtual void raiseTrussUnitWindowItem ( QListBoxItem* );

private:
    typedef QMap< const TrussUnitWindow*, TrussUnitWindowItem* > WindowMap;
    typedef WindowMap::iterator WindowMapIter;

    FermaNextProject& project;
    WindowMap windowItems;
};

#endif // WINDOWLISTBOX_H
