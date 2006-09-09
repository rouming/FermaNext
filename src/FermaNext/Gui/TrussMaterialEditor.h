
#ifndef TRUSSMATERIALEDITOR_H
#define TRUSSMATERIALEDITOR_H

#include <QDialog>
#include <QMap>
#include <QStringList>
#include <QTreeWidgetItem>

class FermaNextProject;
class TrussMaterial;
class TrussMaterialLibrary;
class QDoubleSpinBox;
class QLineEdit;
class QTreeWidget;

class MaterialProperties;

/*****************************************************************************/

class MaterialTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    MaterialTreeWidgetItem ( const TrussMaterial&, int type = Type );
    const TrussMaterial& getItemMaterial () const;
    const MaterialProperties& getMaterialProperties () const;

protected slots:
    // Update temporary values
    void updateName ( const QString& );
    void updateWorkingStress ( double );
    void updateElasticityModule ( double );
    void updateDensity ( double );

    // Set temporary properties to current material properties
    void clearMaterialProperties (); 

private:
    const TrussMaterial& material;
    MaterialProperties& tempProps;
};

/*****************************************************************************/

class ProjectTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    ProjectTreeWidgetItem ( const FermaNextProject&, int type = Type );
    TrussMaterialLibrary& getMaterialLibrary () const;
    const FermaNextProject& getItemProject () const;
    void setCurrent ( bool );

protected slots:
    void updateName ( const QString& );

private:
    const FermaNextProject& project;
    TrussMaterialLibrary* lib;
    bool isCurrent;
};

/*****************************************************************************/

class MaterialEditWidget : public QWidget
{
    Q_OBJECT
public:
    MaterialEditWidget ( QWidget* parent = 0, Qt::WFlags f = 0 );
    void setMaterial ( const MaterialProperties& props, 
                       bool clearEditors = false );
    void setMaterial ( const TrussMaterial& m, 
                       bool clearEditors = false );
    
protected:
    void init ();

protected slots:
    void verifyChanges ();

signals:
    void onNameChange ( const QString& );
    void onWorkingStressChange ( double );
    void onElasticityModuleChange ( double );
    void onDensityChange ( double );
    
private:
    QLineEdit* nameLineEdit;
    QDoubleSpinBox *elasticitySpinBox, *stressSpinBox, *densitySpinBox;    
    const TrussMaterial* material;
};

/*****************************************************************************/

class MaterialCreationDialog : public QDialog
{
    Q_OBJECT
public:
    MaterialCreationDialog ( QWidget* parent = 0, Qt::WFlags f = 0 );
    void setMaterial ( TrussMaterial& m );

protected:
    void init ();

protected slots:
    void applyChanges ();
    void cancelChanges ();
    
signals:
    void onMaterialCreationApply ( const TrussMaterial& );
    void onMaterialCreationCancel ( TrussMaterial& );

private:
    MaterialEditWidget* materialEditWidget;
    TrussMaterial* material;
};

/*****************************************************************************/

class TrussMaterialEditor : public QDialog
{
    Q_OBJECT
public:
    TrussMaterialEditor ( QWidget* parent = 0, Qt::WFlags f = 0 );

protected:
    void init ();
    
    QTreeWidgetItem* getSelectedItem ( bool& materialSelected ) const;
    ProjectTreeWidgetItem* getSelectedProjectItem () const;
    MaterialTreeWidgetItem* getSelectedMaterialItem () const;
    TrussMaterial* getSelectedMaterial () const;
    MaterialTreeWidgetItem* getMaterialItem ( const TrussMaterial&,
                                        const ProjectTreeWidgetItem& ) const;
    ProjectTreeWidgetItem* getProjectItem ( const FermaNextProject& ) const;
    
public slots:
    void setCurrentProjectItem ( FermaNextProject& );

protected slots:
    void addMaterialItem ( const TrussMaterial& );
    void removeMaterialItem ( const TrussMaterial& );

    void addProjectItem ( FermaNextProject& );
    void removeProjectItem ( FermaNextProject& );
    
    void addMaterial ();
    void removeMaterial ();

    void applyMaterialCreation ( const TrussMaterial& );
    void cancelMaterialCreation ( TrussMaterial& );
    
    void initEditorFields ( QTreeWidgetItem*, int );

    void checkButtons ();

private:
    MaterialCreationDialog* createDialog;
    MaterialEditWidget* editWidget;
    QTreeWidget* materialLibTreeList;
    QPushButton *addButton, *removeButton, *applyButton;
};

#endif //TRUSSMATERIALEDITOR_H
