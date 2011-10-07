#ifndef RESULTFORM_H
#define RESULTFORM_H

#include <QtGui>
#include <QDialog>

class ResultForm : public QDialog
{
    Q_OBJECT

public:
    ResultForm(QWidget * parent = 0, Qt::WindowFlags f = 0);
	void appendResult(double x, double y, double r);
	void setRoot(double x,double y);
public slots:
    void reject();
private:
    void createGroupBox();
    QGroupBox *groupBox;
	QLabel *resultLabel;
	QLineEdit *resultEdit;
	QLabel *rootLabel;
	QLineEdit *rootXEdit;
	QLineEdit *rootYEdit;
	QGraphicsView *view;
    QGraphicsScene *scene;
	QTableWidget *table;
	QList<double> resList;
    QDialogButtonBox *buttonBox;
	QGraphicsLineItem *hLine;
	QGraphicsTextItem *text;
	QPointF lastPos;
	QPen redPen;
};

#endif
