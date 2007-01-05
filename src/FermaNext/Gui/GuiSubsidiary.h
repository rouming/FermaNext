
#ifndef GUISUBSIDIARY_H
#define GUISUBSIDIARY_H

#include <QComboBox>
#include <QDockWidget>
#include <QHeaderView>
#include <QStringList>
#include <QValidator>
#include <QCommonStyle>

#include "Global.h"

class QString;
class QStyleOption;
class QPainter;
class TrussMaterial;
class TrussMaterialLibrary;

/*****************************************************************************/

class MaterialDataId
{
public:
	enum MaterialItemData
	{
		Name =		        Qt::DisplayRole,
		Elasticity =		Qt::UserRole,
		WorkingStress =	    Qt::UserRole + 1,
		Density =		    Qt::UserRole + 2,
		Uuid =		        Qt::UserRole + 3,
	};
};

/*****************************************************************************/

class MaterialComboBox : public QComboBox
{
    Q_OBJECT
public:
    MaterialComboBox ( QWidget* parent = 0, 
                       const TrussMaterialLibrary* mLib = 0 );
    const TrussMaterialLibrary& getMaterialLibrary () const;
    const TrussMaterial* getMaterialByIndex ( int ) const;
    const TrussMaterial* getCurrentMaterial () const;
    int getMaterialIndex ( const TrussMaterial& ) const;
    void clearMaterialLibrary ();

public slots:
    void setMaterialLibrary ( const TrussMaterialLibrary& );
    void setCurrentMaterial ( const TrussMaterial& );

protected:
    void setArgList ();

protected slots:
    void addMaterialItem ( const TrussMaterial& );
    void removeMaterialItem ( const TrussMaterial& );
    void updateMaterialItemName ( const QString& );
    
signals:
    void comboBoxIsEmpty ( bool );

private:
    const TrussMaterialLibrary* materialLib;
};

/*****************************************************************************/

class MaterialModel : public QAbstractTableModel
{
public:
	MaterialModel ( const TrussMaterialLibrary& );	
 
	int rowCount ( const QModelIndex& ) const;
	int columnCount ( const QModelIndex& ) const;
	QVariant data ( const class QModelIndex &, int ) const;
	void synchronize ();
	void reset ();
	
protected:
	const TrussMaterialLibrary* materialLib;
};

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

class DockedWidget: public QDockWidget 
{ 
    Q_OBJECT 
public: 
    DockedWidget ( const QString& title, QWidget* parent = 0, Qt::WFlags flags = 0); 
    QSize sizeHint() const; 
};

/*****************************************************************************/

class TableHeader : public QHeaderView
{
public:
    TableHeader ( Qt::Orientation orientation, QWidget * parent = 0 );
    QSize sizeHint () const;
};

/*****************************************************************************/

class FlatStyle : public QCommonStyle
{
public:
	void drawPrimitive ( PrimitiveElement pe, const QStyleOption *opt, 
                         QPainter* p, const QWidget* w = 0 ) const;
};


#endif //GUISUBSIDIARY_H
