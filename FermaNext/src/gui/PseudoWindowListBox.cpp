
#include "PseudoWindowListBox.h"
#include "SubsidiaryConstants.h"
#include <qstring.h>
#include <qcursor.h>
#include <qpopupmenu.h>

/*****************************************************************************
 * Pseudo Windows List Box
 *****************************************************************************/

PseudoWindowListBox::PseudoWindowListBox ( QWidget* parent, const char* name, 
                                           WFlags fl ) : 
    QListBox( parent, name, fl )
{
    activ.resize(0);
    shown.resize(0);
    connect( this, SIGNAL(selected(int)), SLOT(DoubleClick(int)) );
    connect( this, SIGNAL(highlighted (int)), SLOT(_highl(int)) );
}

void PseudoWindowListBox::addItem( const QString & text )
{
    int i;
    insertItem( QPixmap::fromMimeSource( imagesPath + "/unselect.png" ), 
                tr( text ) );
    i =activ.size();
    i++;
    activ.resize(i);
    activ[i-1]=0;
    shown.resize(i);
    shown[i-1]=1;
}

void PseudoWindowListBox::DoubleClick(int index)
{
}

int PseudoWindowListBox::_isSelected ( int index )
{
    if ( activ[index]==1 )
        return 1;    
    else        
        return 0;
}

int PseudoWindowListBox::_isShown ( int index )
{
    if ( shown[index]==1 ) 
        return 1;
    else
        return 0;
}


void PseudoWindowListBox::_setSelected ( int index )
{
    if ( _isSelected(index)==0 ) {
        changeItem( QPixmap::fromMimeSource(imagesPath + "/select.png"),
                    text(index), index );
        activ[index] = 1;
    }
}

void PseudoWindowListBox::_setUnSelected ( int index )
{
    if ( _isSelected(index) ) {
        changeItem( QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                    text(index),index);
        activ[index] = 0;
    }
}

void PseudoWindowListBox::_highl ( int index )
{
    now_highl=index;
}

void PseudoWindowListBox::contextMenuEvent ( QContextMenuEvent* )
{
    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    if ( _isSelected(now_highl)==0 ) {
        contextMenu->insertItem( 
                  QPixmap::fromMimeSource(imagesPath + "/select.png"),
                  "Select", this, SLOT(_sel()) );
    }
    if ( _isSelected(now_highl) ) {
        contextMenu->insertItem(
                      QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                      "UnSelect", this, SLOT(_unsel()) );
    }
    contextMenu->insertSeparator();
    contextMenu->insertItem(
                      QPixmap::fromMimeSource(imagesPath + "/select.png"),
                      "SelectAll", this, SLOT(_selall()) );
    contextMenu->insertItem(
                       QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                       "UnselectAll", this, SLOT(_unselall()) );
    contextMenu->insertSeparator();
    contextMenu->insertItem(
                       QPixmap::fromMimeSource(imagesPath + "/calculate.png"),
                       "Calculate", this, SLOT(_none()) );
    contextMenu->insertSeparator();
    contextMenu->insertItem(
                        QPixmap::fromMimeSource(imagesPath + "/delete.png"),
                        "Remove", this, SLOT(_none()) );
    contextMenu->insertSeparator();

    if ( _isShown(now_highl)==0 ) {
        contextMenu->insertItem( 
                        QPixmap::fromMimeSource( imagesPath + "/show.png"),
                        "Show",this,SLOT(_show()) );
    }
    if ( _isShown(now_highl) )  {
        contextMenu->insertItem(
                        QPixmap::fromMimeSource(imagesPath + "/hide.png"),
                        "Hide", this, SLOT(_hide()) );
    }
    contextMenu->exec( QCursor::pos() );
    delete contextMenu;
}

void PseudoWindowListBox::_sel()
{                    
    _setSelected(now_highl);
}

void PseudoWindowListBox::_unsel()
{
    _setUnSelected(now_highl);
}

void PseudoWindowListBox::_selall()
{
    for ( uint i = 0; i < activ.size(); i++ )
        _setSelected(i);
}

void PseudoWindowListBox::_unselall()
{
    for ( uint i = 0; i < activ.size(); i++ )
        _setUnSelected(i);
}

void PseudoWindowListBox::_show()
{
}

void PseudoWindowListBox::_hide()
{
}

/*****************************************************************************/
