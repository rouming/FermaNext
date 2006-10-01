
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
public:
    typedef QList<HtmlTableCell> HtmlTableCellList;

    HtmlTable ( const QString& title, QWidget* parent = 0 );
    virtual ~HtmlTable ();
    const QDomDocument& getDomDocument () const;
    virtual void clearData ();
    virtual QSize sizeHint ();    
protected:
    virtual void init ( const QString& );
    virtual void fillCell ( QDomElement& elem, const QString& text, 
                            const QString& color, const QString& align, 
                            int width, int height, int colspan );
    virtual void addRow ( const HtmlTableCellList& );
    virtual void fillTable () = 0;
    virtual void resizeRootTable ( int width, int height = -1 );
    virtual void wheelEvent( QWheelEvent * );

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
public:
    HtmlStressTable ( const QString& title, QWidget* parent = 0 );
    void updateTable ( const PluginResults&, 
                       TrussUnitCopy::TrussCopyPivotList );
    
protected:
    void fillTable ();

private:
    const PluginResults* pluginResults;
    TrussUnitCopy::TrussCopyPivotList pivots;
};

/*****************************************************************************/

class HtmlDisplacementTable : public HtmlTable
{
public:
    HtmlDisplacementTable ( const QString& title, QWidget* parent = 0 );
    void updateTable ( const PluginResults&, 
                       TrussUnitCopy::TrussCopyNodeList );
    
protected:
    void fillTable ();

private:
    const PluginResults* pluginResults;
    TrussUnitCopy::TrussCopyNodeList nodes;
};

#endif // HTMLTABLE_H_
