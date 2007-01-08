
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
                          TrussMaterialLibrary* mLib = 0 );

    const TrussMaterialLibrary& getMaterialLibrary () const;
    const TrussMaterial* getMaterialByIndex ( int ) const;
    const TrussMaterial* getCurrentMaterial () const;
    int getMaterialIndex ( const TrussMaterial& ) const;
    
    void setMaterialLibrary ( TrussMaterialLibrary& );
    void clearMaterialLibrary ();

protected:
    void setArgList ();

public slots:
    void addMaterialItem ( const TrussMaterial& );
    void removeMaterialItem ( const TrussMaterial& );
    void setCurrentMaterial ( int idx );

protected slots:
    void updateMaterialItemName ( const QString& );
    void updateCurrentMaterial ( const TrussMaterial& );   
    
private:
    TrussMaterialLibrary* materialLib;
};

/*****************************************************************************/

class AggTrussToolBar : public AggToolBar
{
    Q_OBJECT
public:
    AggTrussToolBar ( TrussDesignerWidget* );
    virtual ~AggTrussToolBar ();

    const TrussMaterial* getCurrentMaterial () const;
    
    void setMaterialLibrary ( TrussMaterialLibrary& );

protected:
    void initButtons ();

private:
    TrussDesignerWidget* designerWidget;
    AggMaterialComboBox* aggComboBox;
};


#endif //AGGTRUSSTOOLBAR_H
