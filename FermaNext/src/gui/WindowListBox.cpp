
#include "WindowListBox.h"
#include "SubsidiaryConstants.h"
#include <qstring.h>
#include <qcursor.h>
#include <qpopupmenu.h>

/*****************************************************************************
 * Windows List Box
 *****************************************************************************/

WindowListBox::WindowListBox ( FermaNextProject& prj, QWidget* parent,
                                           const char* name, WFlags fl ) : 
    QListBox( parent, name, fl ),
    project(prj)
{
    activ.resize(0);
    shown.resize(0);
    connect( this, SIGNAL(selected(int)), SLOT(DoubleClick(int)) );
    connect( this, SIGNAL(highlighted(int)), SLOT(_highl(int)) );
}

void WindowListBox::addItem( const QString & text )
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

void WindowListBox::DoubleClick(int index)
{

}

int WindowListBox::_isSelected ( int index )
{
    if ( activ[index]==1 )
        return 1;    
    else        
        return 0;
}

int WindowListBox::_isShown ( int index )
{
    if ( shown[index]==1 ) 
        return 1;
    else
        return 0;
}


void WindowListBox::_setSelected ( int index )
{
    if ( _isSelected(index)==0 ) {
        changeItem( QPixmap::fromMimeSource(imagesPath + "/select.png"),
                    text(index), index );
        activ[index] = 1;
    }
}

void WindowListBox::_setUnSelected ( int index )
{
    if ( _isSelected(index) ) {
        changeItem( QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                    text(index),index);
        activ[index] = 0;
    }
}

void WindowListBox::_highl ( int index )
{
    now_highl=index;
}

void WindowListBox::contextMenuEvent ( QContextMenuEvent* )
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
                      "Unselect", this, SLOT(_unsel()) );
    }
    contextMenu->insertSeparator();
    contextMenu->insertItem(
                      QPixmap::fromMimeSource(imagesPath + "/select.png"),
                      "Select All", this, SLOT(_selall()) );
    contextMenu->insertItem(
                       QPixmap::fromMimeSource(imagesPath + "/unselect.png"),
                       "Unselect All", this, SLOT(_unselall()) );
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

void WindowListBox::_sel()
{                    
    _setSelected(now_highl);
}

void WindowListBox::_unsel()
{
    _setUnSelected(now_highl);
}

void WindowListBox::_selall()
{
    for ( uint i = 0; i < activ.size(); i++ )
        _setSelected(i);
}

void WindowListBox::_unselall()
{
    for ( uint i = 0; i < activ.size(); i++ )
        _setUnSelected(i);
}

void WindowListBox::_show()
{
}

void WindowListBox::_hide()
{
}

/*****************************************************************************/
