
#ifndef GUISUBSIDIARY_H
#define GUISUBSIDIARY_H

#include <QComboBox>
#include <QDockWidget>
#include <QStringList>
#include <QValidator>

#include "Global.h"

class QString;
class TrussMaterial;
class TrussMaterialLibrary;

/*****************************************************************************/

class MaterialComboBox : public QComboBox
{
    Q_OBJECT
public:
    MaterialComboBox ( QWidget* parent = 0, 
                       const TrussMaterialLibrary* mLib = 0 );
    const TrussMaterialLibrary& getMaterialLibrary () const;
    const TrussMaterial* getCurrentMaterial () const;
    void clearMaterialLibrary ();

public slots:
    void setMaterialLibrary ( const TrussMaterialLibrary& );
    void setCurrentMaterial ( const TrussMaterial& );
    void setCurrentMaterial ( const QString& );

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

#endif //GUISUBSIDIARY_H
