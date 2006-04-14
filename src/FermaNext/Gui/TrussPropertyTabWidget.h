
#ifndef TRUSSPROPERTYWINDOW_H
#define TRUSSPROPERTYWINDOW_H

#include <QAbstractButton>
#include <QItemDelegate>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QValidator>

#include "TrussLoad.h"
#include "TrussUnitWindow.h"

class TrussLoad;
class QCheckBox;
class QColorGroup;
class QComboBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPainter;
class QPushButton;
class QSpacerItem;

/*****************************************************************************/

class LoadTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    LoadTableDelegate ( QObject* parent = 0 );

    QWidget* createEditor ( QWidget *parent, const QStyleOptionViewItem&,
                            const QModelIndex& ) const;

    void setEditorData ( QWidget* editor, const QModelIndex& index ) const;

    void setModelData ( QWidget* editor, QAbstractItemModel* model, 
                        const QModelIndex& index ) const;

    void updateEditorGeometry ( QWidget *editor,
                                const QStyleOptionViewItem& option, 
                                const QModelIndex& ) const;

signals:
    void cellWasChanged ( int, int );
};

/*****************************************************************************/

class LoadTable : public QTableWidget
{
    Q_OBJECT
public:
    LoadTable ( QWidget* parent = 0 );
    virtual void setLoad ( int row, const TrussLoad& );
    virtual TrussLoad getLoad ( int row ) const;
    virtual int getLoadedNodesNumber () const;
};

/*****************************************************************************

class PivotPropertyTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PivotPropertyTableDelegate( QWidget* parent = 0 );
    
    QWidget* createEditor ( QWidget *parent, const QStyleOptionViewItem&,
                            const QModelIndex& ) const;

    void setEditorData ( QWidget* editor, const QModelIndex& index ) const;

    void setModelData ( QWidget* editor, QAbstractItemModel* model, 
                        const QModelIndex& index ) const;

    void updateEditorGeometry ( QWidget *editor,
                                const QStyleOptionViewItem& option, 
                                const QModelIndex& index ) const;

signals:
    void cellWasChanged ( int, int );
};

/*****************************************************************************

class PivotPropertyTable : public QTableWidget
{
public:
    PivotPropertyTable ( QWidget* parent = 0 );
    virtual void setMaterial ( int row, int col, const TrussMaterial& );
    virtual TrussMaterial& getMaterial ( int row ) const;
    virtual void setThickness ( int row, double thick );
    virtual double getThickness ( int row ) const;
    virtual void addPivot ( const TrussPivot&, int row = -1 );
};

/*****************************************************************************/

class TrussPropertyTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TrussPropertyTabWidget ( QWidget* parent = 0 );
    virtual ~TrussPropertyTabWidget ();
    virtual void changeFocusWindow ( TrussUnitWindow* focusWindow );

protected:
    virtual void init ();
    virtual void initLoadTab ();
    virtual void initPivotPropertyTab ();
    virtual void fillLoadTab ();
    virtual void fillLoadTable ( const TrussUnit::LoadCase* loadCase );
    //virtual void fillPivotPropertyTable ();

protected slots:
    virtual void trussUnitWindowWasCreated ( TrussUnitWindow& );
    
    virtual void fillLoadCaseComboBox ();

    virtual void addLoadTableRow ( const Node& );
    virtual void showLoadTableRow ( bool );
    virtual void removeLoadTableRow ( const Node& );

    virtual void changeTabCurrentLoadCase ( int );
    
    virtual void addLoadCase ();
    virtual void removeLoadCase ();
    virtual void setCurrentLoadCase ( int );

    virtual void updateTableLoad ( const Node& );
    virtual void updateTrussLoad ( int, int );

    virtual void setLoadTableNonEditable ( bool );

/*
    virtual void addPivotToTable ( const Node&, const Node& );
    virtual void removePivotFromTable ( const Node&, const Node& );
    virtual void showPivotTableRow ( bool );
    virtual void updatePivotTableMaterial ();
    virtual void updatePivotTableThickness ();
    virtual void updatePivotState ( int row, int col );
*/
private:
    TrussUnitWindow *focusWindow;
    LoadTable *loadTable;
    //PivotPropertyTable *pivotPropTable;
    QLabel *nodesNumbLabel, *loadedNodesLabel, *pivotsNumbLabel;
    QPushButton *createLoadCaseBtn, *removeLoadCaseBtn;
    QComboBox *loadCaseComboBox;
    QGroupBox *loadCaseGroupBox;
    QSpacerItem *loadSpacer, *pivotPropSpacer;
};

#endif //TRUSSPROPERTYWINDOW_H
