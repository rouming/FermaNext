
#ifndef AGGCOMBOBOX
#define AGGCOMBOBOX

#include <QMap>
#include <QVariant>

#include "AggButton.h"
#include "TrussMaterial.h"

class AggPopupMenu;
class AggMenuAction;

/*****************************************************************************/

class AggComboBoxButton : public AggButton
{
    Q_OBJECT

public:
    AggComboBoxButton ();
    virtual ~AggComboBoxButton ();

    void resize ( int w, int h );

    bool isRendered () const;

    virtual void mouseMove ( int xGlobal, int yGlobal );
    virtual void mousePress ( int xGlobal, int yGlobal );
    virtual void mouseRelease ( int xGlobal, int yGlobal );

    virtual void paint ( base_renderer& ) const;

protected:
    QPoint fromGlobalPos (int x, int y ) const;

public slots:
    void clearRenderedFlag();

private:
    rbuf_dynarow* buttonBuf;
    mutable bool rendered;
    QString buttonText;
};

/*****************************************************************************/

class AggComboBox : public QObject
{
    Q_OBJECT

public:
    AggComboBox ( QWidget& parentWidget );
    virtual ~AggComboBox ();

    QPoint getPosition () const;
    void setPosition ( QPoint );

    void addItem ( const QString& );
    void removeItem ( int idx );
    void setItemData ( int idx, QVariant data );

    QString currentText () const;
    int currentIndex () const;
    QVariant itemData ( int ) const;

    void clear ();

    virtual void mouseMove ( int xGlobal, int yGlobal );
    virtual void mousePress ( int xGlobal, int yGlobal );
    virtual void mouseRelease ( int xGlobal, int yGlobal );

    virtual void paint ( base_renderer& ) const;

protected:
    virtual void updateMenuShowPosition ();

public slots:
    void setCurrentIndex ( int idx );

signals:
    void currentIndexChanged ( int );

private:
    QMap<AggMenuAction*, QVariant> dataMap;
    AggPopupMenu* menu;
    AggComboBoxButton* button;
    QWidget& designerWidget;
    QPoint leftTopPos;
    int currIdx;
};

#endif //AGGCOMBOBOX