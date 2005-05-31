
#ifndef PSEUDOWINDOWLISTBOX_H
#define PSEUDOWINDOWLISTBOX_H

#include "FermaNextProject.h"
#include <qlistbox.h>
#include <qbitarray.h>

class PseudoWindowListBox : public QListBox
{
    Q_OBJECT
public:
    PseudoWindowListBox( FermaNextProject&, QWidget* parent = 0, 
                         const char* name = 0, WFlags fl = 0 );    

    void addItem( const QString & text );
    void _setSelected( int index );
    void _setUnSelected ( int index );
    int _isSelected ( int index );
    int _isShown ( int index );

    void contextMenuEvent ( QContextMenuEvent * );

protected slots:/*
    virtual void highlight ( int );
    virtual void windowToFront ( int );
    virtual void _sel ();
    virtual void _unsel ();
    virtual void _selall ();
    virtual void _unselall ();
    virtual void _show ();
    virtual void _hide ();
*/

    virtual void _highl ( int );
    virtual void DoubleClick ( int );
    virtual void _sel ();
    virtual void _unsel ();
    virtual void _selall ();
    virtual void _unselall ();
    virtual void _show ();
    virtual void _hide ();

private:
    FermaNextProject& project;
    QBitArray activ;
    QBitArray shown;
    int now_highl;
};

#endif // PSEUDOWINDOWLISTBOX_H
