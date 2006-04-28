
#ifndef GUISUBSIDIARY_H
#define GUISUBSIDIARY_H

#include <QComboBox>
#include <QStringList>
#include <QValidator>

class QString;
class TrussMaterial;
class TrussMaterialLibrary;

/*****************************************************************************/

class MaterialComboBox : public QComboBox
{
public:
    MaterialComboBox ( QWidget* parent = 0, 
                       const TrussMaterialLibrary* mLib = 0 );
    const TrussMaterialLibrary& getMaterialLibrary () const;
    const TrussMaterial* getCurrentMaterial () const;

protected:
    void setArgList ();

public slots:
    void setMaterialLibrary ( const TrussMaterialLibrary& );
    void setCurrentMaterial ( const TrussMaterial& );

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

#endif //GUISUBSIDIARY_H
