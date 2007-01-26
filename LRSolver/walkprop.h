#ifndef WALKPROP_H
#define WALKPROP_H

#include <QtGui>
#include <QDialog>

class Editor;

class WalkProp : public QDialog
{
    Q_OBJECT

public:
    WalkProp(Editor *v ,QWidget * parent = 0, Qt::WindowFlags f = 0);
public slots:
    void accept();
    void reject();
private:
    void createGroupBox();
    void setValues();
    QGroupBox *groupBox;
    QLineEdit *editDelta;
    QLineEdit *editWalkNumber;
    QLabel *labelDelta;
    QLabel *labelWalkNumber;
    QDialogButtonBox *buttonBox;
    QDoubleValidator* validator;
	QCheckBox *demoMode;
	Editor *editor;
};

#endif
