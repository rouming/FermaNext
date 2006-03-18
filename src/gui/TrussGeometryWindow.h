
#ifndef TRUSSGEOMETRYWINDOW_H
#define TRUSSGEOMETRYWINDOW_H

#include "TrussUnitWindow.h"

#include <QButton>
#include <QTable>
#include <QTabWidget>
#include <QValidator>

class QColorGroup;
class QLabel;
class QLineEdit;
class QPainter;
class QSpacerItem;

/*****************************************************************************/

class RangeValidator : public QDoubleValidator
{
public:
    RangeValidator ( QObject* parent, const char* name = 0 );
    RangeValidator ( double bottom, double top, int decimals, 
                      QObject* parent, const char* name = 0 );
    virtual QValidator::State validate ( QString& input, int& ) const;
};

/*****************************************************************************/

class DoubleCheckBox : public QButton
{
    Q_OBJECT
public:
    DoubleCheckBox ( const QString& label1, const QString& label2,
                     QWidget* parent = 0, const char* name = 0, WFlags f = 0 );
    DoubleCheckBox ( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
    virtual void init ();
    virtual bool isFirstChecked () const;
    virtual bool isSecondChecked () const;
    virtual void setFirstLabel ( QString& label );
    virtual void setSecondLabel ( QString& label );

protected:
    virtual void mousePressEvent ( QMouseEvent* me );

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

class FixationItem : public QObject, public QTableItem
{
    Q_OBJECT
public:
    FixationItem ( QTable*, Node::Fixation );
    FixationItem ( QTable* );
    virtual void setFixation ( Node::Fixation );
    virtual void setFixation ( bool, bool );
    virtual Node::Fixation getFixation () const;

protected:
    virtual QWidget* createEditor () const;
    virtual void setContentFromEditor( QWidget *widget );
    virtual void paint ( QPainter* p, const QColorGroup& cg,
				         const QRect& cr, bool selected );
protected slots:
    void setValueChanged ();

signals:
    void onTableFixValueChange ( int, int );

private:
    DoubleCheckBox* fixCheckBox;
    Node::Fixation fixType;
    bool xChecked, yChecked;
};

/*****************************************************************************/

class NodesTable : public QTable
{
    Q_OBJECT
public:
    NodesTable ( QWidget* parent = 0, const char* name = 0 );
    virtual void setCoord ( int row, int col, double coord );
    virtual double getCoord ( int row, int col ) const;
    virtual int getFixedNodesNumber () const;
    virtual void setFixationItem ( int row, Node::Fixation fix );
    virtual FixationItem* getFixationItem ( int row ) const;
    virtual void addNode ( const Node& );
    virtual void updateMaximumHeight ();

protected slots:
    void updateTrussAreaSize ( const DoubleSize& );

protected:
    virtual QWidget* createEditor ( int row, int col, bool initFromCell ) const;

private:
    DoubleSize areaSize;
};

/*****************************************************************************/

class ComboItem : public QTableItem
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

/*****************************************************************************/

class PivotsTable : public QTable
{
public:
    PivotsTable ( QWidget* parent = 0, const char* name = 0  );
    ComboItem* getComboItem ( int row, int col ) const;
    virtual void setNodeNumber ( int row, int col, int numb, int adjNumb );
    virtual void setThickness ( int row, double thick );
    virtual double getThickness ( int row ) const;
    virtual void setNodesTotalNumber ( int );
    virtual int getNodesNumber () const;
    virtual void addPivot ( const TrussPivot& );

protected:
    virtual QWidget* createEditor ( int row, int col, bool initFromCell ) const;

private:
    int nodesNumb;
};

/*****************************************************************************/

class TrussGeometryWindow : public QTabWidget
{
    Q_OBJECT
public:
    TrussGeometryWindow ( QWidget* parent = 0, Qt::WFlags f = 0 );
    virtual ~TrussGeometryWindow ();
    virtual void changeFocusWindow ( TrussUnitWindow* focusWindow );

protected:
    virtual void init ();
    virtual void initNodesTab ();
    virtual void initPivotsTab ();
    virtual void fillNodesTable ();
    virtual void fillPivotsTable ();
    // Undo/Redo
    virtual void saveNodeStateAfterMoving ( TrussNode& node,
                                            const DoublePoint& pos );
    virtual void updatePivotTableNode ( int col, const TrussPivot& pivot );
    virtual void closeEvent( QCloseEvent* );

protected slots:
    // connect new truss unit window with geometry widget
    virtual void trussUnitWindowWasCreated ( TrussUnitWindow& );
    // add new node to the respective table
    virtual void addNodeToTable ( const Node& );
    virtual void setNodeTableRowVisible ( bool );
    // remove node from the respective table
    virtual void removeNodeFromTable ( const Node& );
    // update table fields according to new values of node coordinates
    virtual void updateNodeTableCoords ();
    // update table field according to new value of node fixation
    virtual void updateNodeTableFixation ();
    // update node state after table field has been chaged
    virtual void updateNodeState ( int, int );

    virtual void addPivotToTable ( const Node&, const Node& );
    virtual void removePivotFromTable ( const Node&, const Node& );
    virtual void setPivotTableRowVisible ( bool );
    virtual void updatePivotTableFirstNode ();
    virtual void updatePivotTableLastNode ();
    virtual void updatePivotTableThickness ();
    virtual void updatePivotState ( int row, int col );
signals:
    void onGeometryWindowClose();

private:
    NodesTable *nodesTable;
    PivotsTable *pivotsTable;
    QLabel *nodesNumbLabel, *fixedNodesLabel, *pivotsNumbLabel;
    TrussUnitWindow *focusWindow;
    QSpacerItem *nodesSpacer, *pivotsSpacer;
    // Undo/Redo
    DoublePoint beforeMovingNodePos;
};

#endif //TRUSSGEOMETRYWINDOW_H
