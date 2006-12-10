
#ifndef AGGTRUSSTOOLBAR_H
#define AGGTRUSSTOOLBAR_H

#include "AggToolBar.h"
#include "AggComboBox.h"

class TrussDesignerWidget;

/*****************************************************************************/

class AggMaterialComboBox : public AggComboBox 
{
    Q_OBJECT
public:
    AggMaterialComboBox ( QWidget& parentWidget,
                          const TrussMaterialLibrary* mLib = 0 );

    const TrussMaterialLibrary& getMaterialLibrary () const;
    const TrussMaterial* getMaterialByIndex ( int ) const;
    void setCurrentMaterial ( const TrussMaterial& );
    const TrussMaterial* getCurrentMaterial () const;
    int getMaterialIndex ( const TrussMaterial& ) const;
    
    void setMaterialLibrary ( const TrussMaterialLibrary& );
    void clearMaterialLibrary ();

protected:
    void setArgList ();

protected slots:
    void addMaterialItem ( const TrussMaterial& );
    void removeMaterialItem ( const TrussMaterial& );
    void updateMaterialItemName ( const QString& );

private:
    const TrussMaterialLibrary* materialLib;
};

/*****************************************************************************/

class AggTrussToolBar : public AggToolBar
{
    Q_OBJECT
public:
    AggTrussToolBar ( TrussDesignerWidget* );
    virtual ~AggTrussToolBar ();

    void setMaterialLibrary ( const TrussMaterialLibrary& );

protected:
    void initButtons ();

private:
    TrussDesignerWidget* designerWidget;
    AggMaterialComboBox* aggComboBox;
};


#endif //AGGTRUSSTOOLBAR_H
