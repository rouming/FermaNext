
#ifndef TRUSSMATERIALEDITOR_H
#define TRUSSMATERIALEDITOR_H

#include <QDialog>
#include <QDomDocument>
#include <QMap>
#include <QStringList>
#include <QTreeWidgetItem>

class FermaNextProject;
class TrussMaterial;
class TrussMaterialLibrary;
class QDoubleSpinBox;
class QLineEdit;
class QTreeWidget;

/*****************************************************************************/

class MaterialTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    MaterialTreeWidgetItem ( const TrussMaterial&, int type = Type );
    const TrussMaterial& getItemMaterial () const;

protected slots:
    void updateName ( const QString& );

private:
    const TrussMaterial& material;
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
    void setMaterial ( TrussMaterial& m, 
                       bool clearEditors = false );
    void showEditors ( bool );
    
protected:
    void init ();
    bool eventFilter ( QObject*, QEvent* );

protected slots:
    void updateName ();
    void updateWorkingStress ();
    void updateElasticityModule ();
    void updateDensity ();
    
private:
    QLineEdit* nameLineEdit;
    QDoubleSpinBox *elasticitySpinBox, *stressSpinBox, *densitySpinBox;    
    TrussMaterial* material;
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

    void saveMaterialProperties ();
    void loadMaterialProperties ();
    
public slots:
    void setCurrentProjectItem ( FermaNextProject& );
    void fillEditorFields ( QTreeWidgetItem*, int );
    void exec ();
    void cancelChanges ();
    void applyChanges ();

protected slots:
    void addMaterialItem ( const TrussMaterial& );
    void removeMaterialItem ( const TrussMaterial& );

    void addProjectItem ( FermaNextProject& );
    void removeProjectItem ( FermaNextProject& );
    
    void addMaterial ();
    void removeMaterial ();

    void applyMaterialCreation ( const TrussMaterial& );
    void cancelMaterialCreation ( TrussMaterial& );

    void checkButtons ();

private:
    typedef QMap<const FermaNextProject*, QDomElement> MaterialPropMap;

    MaterialPropMap materialProperties;
    MaterialCreationDialog* createDialog;
    MaterialEditWidget* editWidget;
    QTreeWidget* materialLibTreeList;
    QPushButton *addButton, *removeButton, *applyButton;
    QDomDocument materialPropDoc;
};

#endif //TRUSSMATERIALEDITOR_H
