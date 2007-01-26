#include "resultform.h"

ResultForm::ResultForm(QList<double> dl,double wn, QWidget * parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
	resultLabel=new QLabel(tr("Result:"));
	resultEdit=new QLineEdit();
	resultEdit->setReadOnly(true);
    scene=new QGraphicsScene();
    view=new QGraphicsView(scene,this);
    createGroupBox();
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);

    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Result"));
    setValues();       
}

void ResultForm::setValues()
{
double d;
	d=0.;
	for (int i=0;i<dl.sizw();++i)
	{
		d=d+dl.at(i);
	}
	d=d/wn;
	resultEdit->setText(QString::number(d));
}

void WalkProp::createGroupBox()
{
    groupBox = new QGroupBox(tr("Result"));
    QVBoxLayout *layout=new QVBoxLayout;
    QHBoxLayout *lh1=new QHBoxLayout;
    QHBoxLayout *lh2=new QHBoxLayout;
    lh1->addWidget(resultLabel);
    lh1->addWidget(resultEdit);
	lh2->addWidget(view);
    layout->addLayout(lh1);
    layout->addLayout(lh2);
    groupBox->setLayout(layout);
}

void ResultForm::reject()
{
    QDialog::reject();
}
