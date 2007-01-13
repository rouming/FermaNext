
#ifndef PROJECTTOOLBOX_H
#define PROJECTTOOLBOX_H

#include <QToolBox>
#include <QMap>

#include "FermaNextWorkspace.h"
#include "FermaNextProject.h"

class WindowListBox;
class QDoubleSpinBox;
class QLineEdit;
class QPushButton;

/*****************************************************************************/

class CreateTrussDialog : public QDialog
{
    Q_OBJECT

public:
    CreateTrussDialog ( TrussUnitWindowManager&, QWidget* parent = 0 );

protected slots:
    void checkDialogState ();
    void onOk ();
    void onCancel ();

private:
    QLineEdit *nameEdit;
    QDoubleSpinBox *xSizeEdit, *ySizeEdit;
    QPushButton *okButton;
    TrussUnitWindowManager& wndMng;
};

/*****************************************************************************/

class ProjectToolBoxPage : public QWidget
{
    Q_OBJECT

public:
    ProjectToolBoxPage ( FermaNextProject& prj, QWidget* parent = 0, 
                         Qt::WFlags f = 0 );

    FermaNextProject& project () const;

    void setTrussNumber ( uint );
    uint getTrussNumber () const;

    void setHiddenTrussNumber ( uint );
    uint getHiddenTrussNumber () const;

    void setTOK ( double );
    double getTOK () const;

    WindowListBox& getWindowListBox () const;

protected:
    void init ();

protected slots:
    void importIsPressed ();
    void newTrussIsPressed ();
    void calculateAllIsPressed ();
    void afterTrussCountChange ( TrussUnitWindow& );
    void afterTrussVisibilityChange ();

private:
    FermaNextProject& pageProject;
    WindowListBox* listBox;

    // project info labels
    QLabel *tokLabel, *trussNumberLabel, *trussHiddenLabel;
};

/*****************************************************************************/

class ProjectToolBox : public QToolBox
{
    Q_OBJECT

public:
    ProjectToolBox ( FermaNextWorkspace&,
                     QWidget* parent = 0, 
                     Qt::WFlags f = 0 );

    virtual FermaNextProject* currentProject () const;

    virtual ProjectToolBoxPage* getPageForProject ( 
                                        const FermaNextProject& ) const;

    virtual bool eventFilter( QObject*, QEvent* );

protected slots:
    virtual int addProject ( FermaNextProject& );
    virtual int removeProject ( FermaNextProject& );
    virtual void projectRename ( const QString& );
    virtual void projectIsActivated ( FermaNextProject& );
    virtual void activateSelected ( int index );

public slots:
    virtual void clear ();

signals:
    void onShowTrussResults ( const TrussUnitWindow& );
    void calculateTrussUnit ( const TrussUnitWindow& );

private:
    FermaNextWorkspace& workspace;
};

#endif //PROJECTTOOLBOX_H
