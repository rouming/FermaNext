
#ifndef HTMLTABLE_H_
#define HTMLTABLE_H_

#include <QDomDocument>
#include <QTextBrowser>

#include "Global.h"
#include "TrussUnitCopy.h"

class PluginResults;
class QWheelEvent;

/*****************************************************************************/

struct HtmlTableCell 
{
    HtmlTableCell ( QString text_ = "", 
                    QString bgColor_ = "",
                    QString align_ = "center",
                    int colspan_ = 1,
                    int width_ = Global::htmlColWidth, 
                    int height_ = Global::htmlRowHeight ) :
        text( text_ ),
        bgColor( bgColor_ ),
        align( align_ ),
        colspan( colspan_ ),
        width( width_ ),
        height( height_ )
    {}

    QString text, bgColor, align;
    int colspan, width, height;
};

/*****************************************************************************/

class HtmlTable : public QTextBrowser
{
    Q_OBJECT

public:
    typedef QList<HtmlTableCell> HtmlTableCellList;

    HtmlTable ( const QString& title, QWidget* parent = 0 );
    virtual ~HtmlTable ();
    const QDomDocument& getDomDocument () const;
    int getHeight () const;
    
protected:
    virtual void init ( const QString& );
    virtual void fillCell ( QDomElement& elem, const QString& text, 
                            const QString& color, const QString& align, 
                            int width, int height, int colspan );
    virtual void addRow ( const HtmlTableCellList& );
    virtual void fillTable () = 0;
    virtual void resizeRootTable ( int width, int height = -1 );
    virtual QDomElement getRowElement ( int row ) const;
    virtual QDomElement getColumnElement ( int col, 
                                           const QDomElement& rowElem ) const;
    virtual void wheelEvent( QWheelEvent * );

public slots:
    virtual void clearData ();
    virtual void updateHtml();
    virtual void setCellHighlight( int row, int col, const QString& color ); 
    virtual void setColumnHighlight( int col, const QString& color );
    virtual void removeHighlight ();

private:
    QDomDocument htmlDoc;
    QDomElement rootTable, childTable, dataBody;
};

/*****************************************************************************/

class HtmlPropertyTable : public HtmlTable
{
public:
    HtmlPropertyTable ( const QString& title, QWidget* parent = 0 );

protected:
    void addProperty ( const QString&, int );
    void fillTable ();
};

/*****************************************************************************/

class HtmlStressTable : public HtmlTable
{
    Q_OBJECT

public:
    HtmlStressTable ( const QString& title, QWidget* parent = 0 );
    void updateTable ( const PluginResults&, 
                       TrussUnitCopy::TrussCopyPivotList );
    
protected:
    void fillTable ();

protected slots:
    void changeLoadCaseHighlight ( int );

private:
    const PluginResults* pluginResults;
    TrussUnitCopy::TrussCopyPivotList pivots;
    int loadCaseNumb;
};

/*****************************************************************************/

class HtmlDisplacementTable : public HtmlTable
{
    Q_OBJECT
        
public:
    HtmlDisplacementTable ( const QString& title, QWidget* parent = 0 );
    void updateTable ( const PluginResults&, 
                       TrussUnitCopy::TrussCopyNodeList );
    
protected:
    void fillTable ();
    
protected slots:
    void changeLoadCaseHighlight ( int );

private:
    const PluginResults* pluginResults;
    TrussUnitCopy::TrussCopyNodeList nodes;
    int loadCaseNumb;
};

#endif // HTMLTABLE_H_
