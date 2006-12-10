
#ifndef TRUSSMATERIALEDITOR_H
#define TRUSSMATERIALEDITOR_H

#include <QDialog>
#include <QDomDocument>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QLabel>

class FermaNextProject;
class TrussMaterial;
class TrussMaterialLibrary;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QStackedWidget;
class QTreeWidget;
class QtColorComboBox;

/*****************************************************************************/

class MaterialTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT

public:
    MaterialTreeWidgetItem ( const TrussMaterial&, int type = Type );
    const TrussMaterial& getItemMaterial () const;

protected slots:
    void updateName ();

private:
    const TrussMaterial& material;
};

/*****************************************************************************/

class ProjectTreeWidgetItem : public QTreeWidgetItem
{
public:
    ProjectTreeWidgetItem ( const QString& projName,
                            TrussMaterialLibrary& mLib,
                            bool current = false, 
                            int type = Type );
    TrussMaterialLibrary& getMaterialLibrary () const;

protected:
    void setCurrent ( bool );

private:
    TrussMaterialLibrary& lib;
    bool isCurrent;
};

/*****************************************************************************/

class MaterialTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    MaterialTreeWidget ( QWidget* parent = 0 );
        
    ProjectTreeWidgetItem* getSelectedProjectItem () const;
    MaterialTreeWidgetItem* getSelectedMaterialItem () const;
    MaterialTreeWidgetItem* getMaterialItem ( const TrussMaterial&,
                                        const ProjectTreeWidgetItem& ) const;

    void addMaterialItem ( const TrussMaterial& );
    void removeMaterialItem ( const TrussMaterial& );
    void removeItemWithMaterial ( QTreeWidgetItem& );

protected:
    QTreeWidgetItem* getSelectedItem ( bool& materialSelected ) const;

    void mousePressEvent ( QMouseEvent* );
    void mouseMoveEvent ( QMouseEvent* );
    void dragEnterEvent( QDragEnterEvent* event );
    void dragMoveEvent( QDragMoveEvent *event );
    bool dropMimeData ( QTreeWidgetItem* parent, int index, 
                        const QMimeData* data, Qt::DropAction action );
    QStringList mimeTypes () const;
    Qt::DropActions supportedDropActions () const;
    void startDrag ();

private:
    QPoint dragPos;
    bool acceptDrag;;
};

/*****************************************************************************/

class MaterialEditWidget : public QWidget
{
    Q_OBJECT
public:
    MaterialEditWidget ( QWidget* parent = 0, Qt::WFlags f = 0 );
    void setMaterial ( TrussMaterial& m );
    void clearPage ();
    
protected:
    void init ();
    
protected slots:
    void updateName ();
    void updateWorkingStress ();
    void updateElasticityModule ();
    void updateDensity ();

private:
    QLineEdit* nameLineEdit;
    QDoubleSpinBox *elasticitySpinBox, *stressSpinBox, *densitySpinBox;    
    TrussMaterial* material;
    QtColorComboBox* materialColorCmb;
    QStackedWidget* widgetStack;
};

/*****************************************************************************/

class TrussMaterialEditor : public QDialog
{
    Q_OBJECT
public:
    TrussMaterialEditor ( QString& startPage, QWidget* parent = 0, 
                          Qt::WFlags f = 0 );
    ~TrussMaterialEditor ();

    void addProject ( FermaNextProject& project, bool current = false );

protected:
    void init ();
    
    void copyLib ( TrussMaterialLibrary& libFrom, TrussMaterialLibrary& libTo );
    void clearCopyLibs ();

public slots:
    void exec ();
    
protected slots:
    void addMaterial ();
    void removeSelectedMaterial ();

    void applyChanges ();
    void cancelChanges ();

    void fillEditorFields ();

private:
    typedef QMap<FermaNextProject*, TrussMaterialLibrary*> MaterialLibMap;

    MaterialLibMap originalMaterialLibs;
    MaterialEditWidget* editWidget;
    MaterialTreeWidget* materialLibTreeList;
    QPushButton *addButton, *removeButton;
    QAction *actAddMaterial, *actRemoveMaterial;
    QString& startPageUUID;
};

#endif //TRUSSMATERIALEDITOR_H
