#include "walkprop.h"
#include "editor.h"

WalkProp::WalkProp(Editor *v, QWidget * parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
	editor=v;
    labelDelta=new QLabel(tr("Walk Incrementation"));
    labelWalkNumber=new QLabel(tr("Number of random walks"));
    editDelta=new QLineEdit();
	editDelta->setValidator(new QDoubleValidator(editDelta));
	editWalkNumber=new QLineEdit();
    editWalkNumber->setValidator(new QIntValidator(editWalkNumber));
	demoMode=new QCheckBox(tr("Demonstration mode"));
    createGroupBox();
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Random Walk Properties"));
    setValues();       
}

void WalkProp::setValues()
{
	editDelta->setText(QString::number(editor->step()));
	editWalkNumber->setText(QString::number(editor->walkNumber()));
	if(editor->demoMode()) 
		demoMode->setCheckState(Qt::Checked);
	else
		demoMode->setCheckState(Qt::Unchecked);
}

void WalkProp::createGroupBox()
{
    groupBox = new QGroupBox(tr("Properties"));
    QVBoxLayout *layout=new QVBoxLayout;
    QHBoxLayout *lh1=new QHBoxLayout;
    QHBoxLayout *lh2=new QHBoxLayout;
	QHBoxLayout *lh3=new QHBoxLayout;
    lh1->addWidget(labelDelta);
    lh1->addWidget(editDelta);
    lh2->addWidget(labelWalkNumber);
    lh2->addWidget(editWalkNumber);
	lh3->addWidget(demoMode);
    layout->addLayout(lh1);
    layout->addLayout(lh2);
	layout->addLayout(lh3);
    groupBox->setLayout(layout);
}

void WalkProp::accept()
{
double delta=editDelta->text().toDouble();
int walkNum=editWalkNumber->text().toInt();
	if (delta>0)
		editor->setStep(delta);
	if (walkNum>0)
		editor->setWalkNumber(walkNum);
	editor->setDemoMode(demoMode->checkState() && Qt::Checked);
    QDialog::accept();
}

void WalkProp::reject()
{
    QDialog::reject();
}
