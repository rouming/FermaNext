#ifndef LINEPROP_H
#define LINEPROP_H

#include <QtGui>
#include <QDialog>

class Line;

class LineProp : public QDialog
{
    Q_OBJECT

public:
    LineProp(Line *parentLine, QWidget * parent = 0, Qt::WindowFlags f = 0);
public slots:
	void accept();
	void reject();
private:
    void createGroupBox();
	void setValues();
    QGroupBox *groupBox;
    QLineEdit *edit1x;
    QLineEdit *edit2x;
	QLineEdit *edit1y;
	QLineEdit *edit2y;
    QLabel *label1;
    QLabel *label2;
	QLabel *errMsg;
	QLineEdit *editBoundCond;
	QLabel *labelBoundCond;
    QDialogButtonBox *buttonBox;
	Line *line;
	bool checkBoundCond(const QString &str);
	QString x1;
	QString y1;
	QString x2;
	QString y2;
};

#endif
