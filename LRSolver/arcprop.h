#ifndef ARCPROP_H
#define ARCPROP_H

#include <QtGui>
#include <QDialog>

class Arch;

class ArcProp : public QDialog
{
    Q_OBJECT

public:
    ArcProp(Arch *parentArc, QWidget * parent = 0, Qt::WindowFlags f = 0);
public slots:
    void accept();
    void reject();
private:
    void createGroupBox();
    void setValues();
    QGroupBox *groupBox;
    QLineEdit *editSpan;
    QLineEdit *editStart;
    QLineEdit *editRad;
    QLineEdit *editcx;
    QLineEdit *editcy;
    QLabel *labelAngle;
    QLabel *labelRad;
    QLabel *labelCenter;
	QLabel *errMsg;
	QLineEdit *editBoundCond;
	QLabel *labelBoundCond;
    QDialogButtonBox *buttonBox;
    Arch *arc;
    QDoubleValidator* validator;
	bool checkBoundCond(const QString &str);
	QString start;
	QString span;
	QString rad;
	QString cx;
	QString cy;
};

#endif
