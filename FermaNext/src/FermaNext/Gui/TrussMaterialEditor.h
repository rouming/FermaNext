
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

/*****************************************************************************/

class MaterialTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    MaterialTreeWidgetItem ( const TrussMaterial&, int type = Type );
    const TrussMaterial& getItemMaterial () const;

protected slots:
    void updateName ( const QString& );
    void updateWorkingStress ( double );
    void updateElasticityModule ( double );
    void updateDensity ( double );

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

class TrussMaterialEditDialog : public QDialog
{
    Q_OBJECT
public:
    TrussMaterialEditDialog ( QWidget* parent = 0, Qt::WFlags f = 0 );
    void setMaterial ( TrussMaterial& m, bool newMaterial );

protected:
    void init ();

protected slots:
    void applyChanges ();
    void cancelChanges ();

signals:
    void onMaterialCreationApply ( const TrussMaterial& );
    void onMaterialCreationCancel ( TrussMaterial& );

private:
    QLineEdit* nameLineEdit;
    QDoubleSpinBox *elasticitySpinBox, *stressSpinBox, *densitySpinBox;    
    TrussMaterial* material;
    bool onMaterialCreate;
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
    void editMaterial ();
    void removeMaterial ();

    void applyMaterialCreation ( const TrussMaterial& );
    void cancelMaterialCreation ( TrussMaterial& );

    void checkButtons ();

private:
    TrussMaterialEditDialog* editDialog;
    QTreeWidget* materialLibTreeList;
    QPushButton *addButton, *editButton, *removeButton;
};

#endif //TRUSSMATERIALEDITOR_H
