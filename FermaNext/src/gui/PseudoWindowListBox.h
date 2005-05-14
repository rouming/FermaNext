
#ifndef PSEUDOWINDOWLISTBOX_H
#define PSEUDOWINDOWLISTBOX_H

#include <qlistbox.h>
#include <qbitarray.h>

class PseudoWindowListBox : public QListBox
{
    Q_OBJECT
public:
    PseudoWindowListBox( QWidget* parent = 0, 
                         const char* name = 0, WFlags fl = 0 );    

    void addItem( const QString & text );
    void _setSelected( int index );
    void _setUnSelected ( int index );
    int _isSelected ( int index );
    int _isShown ( int index );

    void contextMenuEvent ( QContextMenuEvent * );

protected slots:
    virtual void _highl ( int index );
    virtual void DoubleClick (int index);
    virtual void _sel ();
    virtual void _unsel ();
    virtual void _selall ();
    virtual void _unselall ();
    virtual void _show ();
    virtual void _hide ();

private:
    QBitArray activ;
    QBitArray shown;
    int now_highl;
};

#endif // PSEUDOWINDOWLISTBOX_H
