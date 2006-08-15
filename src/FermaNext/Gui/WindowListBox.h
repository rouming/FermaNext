
#ifndef WINDOWLISTBOX_H
#define WINDOWLISTBOX_H

#include <QListWidget>
#include <QMenu>
#include <QBitArray>

#include "FermaNextProject.h"

class WindowListBox;
class TrussUnitWindowItem;
class Plugin;

/*****************************************************************************/

class TrussUnitWindowItem : public QObject, public QListWidgetItem
{
    Q_OBJECT
public:
    TrussUnitWindowItem ( FermaNextProject&, TrussUnitWindow&, 
                          WindowListBox&, const QIcon&,
                          const QIcon& disPix );

    virtual QVariant data ( int role ) const;

    virtual void fillPopup ( QMenu* );

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
    virtual void executePlugin ( Plugin* );
    virtual void remove ();
    virtual void showResults ();

protected slots:
    // Life-time changes
    virtual void trussWindowDesisted ();
    virtual void trussWindowRevived ();

signals:
    // Emits when item changes its visibility status
    void onWindowItemShow ( TrussUnitWindowItem& );
    void onWindowItemHide ( TrussUnitWindowItem& );
    void onShowTrussResults ( const TrussUnitWindow& );

private:
    WindowListBox& listBox;
    FermaNextProject& project;
    TrussUnitWindow& trussWindow;
    QIcon disabledIcon;
    bool selected;    
};

/*****************************************************************************/

class PluginActionReceiver : public QObject
{
    Q_OBJECT
public:
    PluginActionReceiver ( Plugin* plg, TrussUnitWindowItem* itm ) :
        plugin(plg), item(itm)
    {}

public slots:
    void execute ()
    {
        if ( item == 0 || plugin == 0 )
            return;
        item->executePlugin( plugin );
    }

private:
    Plugin* plugin;
    TrussUnitWindowItem* item;
};

/*****************************************************************************/

class WindowListBox : public QListWidget
{
    Q_OBJECT
public:
    WindowListBox( FermaNextProject&, QWidget* parent = 0 );

    void contextMenuEvent ( QContextMenuEvent* );

protected:
    TrussUnitWindowItem* findByTrussUnitWindow ( 
        const TrussUnitWindow& ) const;

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
    virtual void raiseSelectedWindowItem ();
    virtual void checkAfterDoubleClick ( QListWidgetItem* );
    virtual void solveTrussUnit ( const TrussUnitWindow& );

signals:
    void onShowTrussResults ( const TrussUnitWindow& );

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
