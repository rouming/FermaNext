
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

class DoubleCheckBox : public QWidget
{
    Q_OBJECT
public:
    DoubleCheckBox ( const QString& label1, const QString& label2,
                     QWidget* parent = 0 );
    DoubleCheckBox ( QWidget* parent = 0 );
    virtual void init ();
    virtual bool isFirstChecked () const;
    virtual bool isSecondChecked () const;
    virtual void setFirstLabel ( QString& label );
    virtual void setSecondLabel ( QString& label );

protected:
    virtual void mousePressEvent ( QMouseEvent* );

public slots:
    void setFirstChecked ( bool check );
    void setSecondChecked ( bool check );

signals:
    void onValueChange ();

private:
    QCheckBox *checkBox1, *checkBox2;
    QString firstLabel, secondLabel;
};

/*****************************************************************************/

class NodeTableDelegate : public QItemDelegate
{
public:
    NodeTableDelegate ( QObject* parent = 0 );

    QWidget* createEditor ( QWidget *parent, const QStyleOptionViewItem&,
                            const QModelIndex& ) const;
/*
    void paint ( QPainter* painter, const QStyleOptionViewItem& option,
                 const QModelIndex& index ) const;
*/
    void setEditorData ( QWidget* editor, const QModelIndex& index ) const;

    void setModelData ( QWidget* editor, QAbstractItemModel* model, 
                        const QModelIndex& index ) const;
/*
    bool editorEvent ( QEvent* event, QAbstractItemModel* model,
                       const QStyleOptionViewItem& option,
                       const QModelIndex& index );
*/
    void updateEditorGeometry ( QWidget *editor,
                                const QStyleOptionViewItem& option, 
                                const QModelIndex& ) const;
};

/*****************************************************************************/

class FixationItem : public QTableWidgetItem
{
public:
    FixationItem ( Node::Fixation, int type = Type );
    FixationItem ( int type = Type );
    virtual void setFixation ( Node::Fixation );
    virtual void setFixation ( bool, bool );
    virtual Node::Fixation getFixation () const;

private:
    Node::Fixation fixType;
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

protected slots:
    void updateTrussAreaSize ( const DoubleSize& );

private:
    DoubleSize areaSize;
};

/*****************************************************************************

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

/*****************************************************************************

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
    virtual QWidget* createEditor ( int row, int col, bool initFromCell ) const;

private:
    int nodesNumb;
};

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
    virtual void closeEvent( QCloseEvent* );

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
signals:
    void onGeometryWindowClose();

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
