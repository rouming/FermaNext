#ifndef RESULTFORM_H
#define RESULTFORM_H

#include <QtGui>
#include <QDialog>

class ResultForm : public QDialog
{
    Q_OBJECT

public:
    ResultForm(QList<double> dl,double wn,QWidget * parent = 0, Qt::WindowFlags f = 0);
public slots:
    void accept();
    void reject();
private:
    void createGroupBox();
    void setValues();
    QGroupBox *groupBox;
	QLabel resultLabel;
	QLineEdit resultEdit;
	QGraphicsView *view;
    QGraphicsScene *scene;
};

#endif
