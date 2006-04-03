
#ifndef GEOMETRYTABWIDGET_H
#define GEOMETRYTABWIDGET_H

#include <QAbstractButton>
#include <QItemDelegate>
#include <QSpinBox>
#include <QTabWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QValidator>

#include "TrussUnitWindow.h"

class QCheckBox;
class QColorGroup;
class QLabel;
class QLineEdit;
class QPainter;
class QSpacerItem;

/*****************************************************************************/

class RangeValidator : public QDoubleValidator
{
public:
    RangeValidator ( QObject* parent );
    RangeValidator ( double bottom, double top, int decimals, 
                     QObject* parent );
    virtual QValidator::State validate ( QString& input, int& ) const;
};

/*****************************************************************************/

class NodeTableDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    NodeTableDelegate ( QObject* parent = 0 );

    QRect getCheckBoxDrawArea ( const QStyleOptionViewItem &option,
                                bool forFirstCheckBox = true ) const;

    void getCheckStates ( bool& checkState1, bool& checkState2, 
                          const QModelIndex& index ) const;

    Node::Fixation getFixationByCheckStates ( bool, bool ) const;

    QWidget* createEditor ( QWidget *parent, const QStyleOptionViewItem&,
                            const QModelIndex& ) const;

    void paint ( QPainter* painter, const QStyleOptionViewItem& option,
                 const QModelIndex& index ) const;

    void setEditorData ( QWidget* editor, const QModelIndex& index ) const;

    void setModelData ( QWidget* editor, QAbstractItemModel* model, 
                        const QModelIndex& index ) const;

    QSize sizeHint( const QStyleOptionViewItem &option,
                    const QModelIndex &index ) const;

    bool editorEvent ( QEvent* event, QAbstractItemModel* model,
                       const QStyleOptionViewItem& option,
                       const QModelIndex& index );

    void updateEditorGeometry ( QWidget *editor,
                                const QStyleOptionViewItem& option, 
                                const QModelIndex& ) const;

private:
    DoubleSize areaSize;

signals:
    void cellWasChanged ( int, int );

protected slots:
    void updateTrussAreaSize ( const DoubleSize& );
};

/*****************************************************************************/

class FixationItem : public QTableWidgetItem
{
public:
    FixationItem ( Node::Fixation, int type = Type );
    FixationItem ( int type = Type );
    virtual void setFixation ( Node::Fixation );
    virtual Node::Fixation getFixation () const;
};

/*****************************************************************************/

class NodeTable : public QTableWidget
{
    Q_OBJECT
public:
    NodeTable ( QWidget* parent = 0 );
    virtual void setCoord ( int row, int col, double coord );
    virtual double getCoord ( int row, int col ) const;
    virtual int getFixedNodesNumber () const;
    virtual void setFixationItem ( int row, Node::Fixation fix );
    virtual FixationItem* getFixationItem ( int row ) const;
    virtual void addNode ( const Node& );
    virtual void updateMaximumHeight ();
};

/*****************************************************************************/
/*
class ComboItem : public QTableWidgetItem
{
public:
    ComboItem( QTable* table, int currentNumb, int adjNumb );
    virtual void setAdjoiningNodeNumber ( int );
    virtual QWidget* createEditor() const;
    virtual void setContentFromEditor( QWidget* );
    virtual void setText( const QString& );

protected:
    virtual QStringList getComboArgList () const;

private:
    QComboBox *comboBox;
    int currentValue, adjNodeValue;
};
*/
/*****************************************************************************/
/*
class PivotTable : public QTableWidget
{
public:
    PivotTable ( QWidget* parent = 0, const char* name = 0  );
    ComboItem* getComboItem ( int row, int col ) const;
    virtual void setNodeNumber ( int row, int col, int numb, int adjNumb );
    virtual void setThickness ( int row, double thick );
    virtual double getThickness ( int row ) const;
    virtual void setNodesTotalNumber ( int );
    virtual int getNodesNumber () const;
    virtual void addPivot ( const TrussPivot&, int row = -1 );

protected:
    virtual QWidget* createEditor ( int row, int col, 
                                    bool initFromCell ) const;

private:
    int nodesNumb;
};
*/
/*****************************************************************************/

class GeometryTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    GeometryTabWidget ( QWidget* parent = 0 );
    virtual ~GeometryTabWidget ();
    virtual void changeFocusWindow ( TrussUnitWindow* focusWindow );

protected:
    virtual void init ();
    virtual void initNodesTab ();
    virtual void initPivotsTab ();
    virtual void initAreaTab ();
    virtual void fillNodeTable ();
//    virtual void fillPivotTable ();
    virtual void saveNodeStateAfterMoving ( TrussNode& node,
                                            const DoublePoint& pos );

protected slots:
    virtual void trussUnitWindowWasCreated ( TrussUnitWindow& );

    virtual void addNodeToTable ( const Node& );
    virtual void showNodeTableRow ( bool );
    virtual void removeNodeFromTable ( const Node& );
    virtual void updateNodeTableCoords ();
    virtual void updateNodeTableFixation ();
    virtual void updateNodeState ( int, int );
/*
    virtual void addPivotToTable ( const Node&, const Node& );
    virtual void removePivotFromTable ( const Node&, const Node& );
    virtual void updateNodesNumbers ( const Node& node );
    virtual void showPivotTableRow ( bool );
    virtual void updatePivotTableFirstNode ();
    virtual void updatePivotTableLastNode ();
    virtual void updatePivotTableThickness ();
    virtual void updatePivotState ( int row, int col );
*/
    virtual void changeTrussAreaSize ( const QString& );

private:
    NodeTable *nodeTable;
//    PivotTable *pivotTable;
    QLabel *nodesNumbLabel, *fixedNodesLabel, *pivotsNumbLabel;
    TrussUnitWindow *focusWindow;
    QSpacerItem *nodesSpacer, *pivotsSpacer;
    // Undo/Redo
    DoublePoint beforeMovingNodePos;
};

#endif //GEOMETRYTABWIDGET_H
