#include "resultform.h"

ResultForm::ResultForm(QWidget * parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
	redPen.setColor(QColor(Qt::red));
	resList.clear();
	rootLabel=new QLabel(tr("Point coordinates:"));
	rootXEdit=new QLineEdit();
	rootYEdit=new QLineEdit();
	resultLabel=new QLabel(tr("Result:"));
	resultEdit=new QLineEdit();
	resultEdit->setReadOnly(true);
    scene=new QGraphicsScene();
    view=new QGraphicsView(scene,this);
    view->scale( 1, -1 );
	view->setRenderHints(QPainter::Antialiasing);
	hLine=scene->addLine(QLineF(10,10,20,10));
	QStringList header;
	header<<tr("X")<<tr("Y")<<tr("Value");
	table=new QTableWidget(0,3,this);
	table->setHorizontalHeaderLabels(header);
	table->setGridStyle(Qt::SolidLine);
    createGroupBox();
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle(tr("Result"));
}

void ResultForm::appendResult(double x,double y, double r)
{
int len;
	resList.append(r);
	len=resList.size()-1;
	table->insertRow(table->rowCount());
	table->setItem(table->rowCount()-1,0,new QTableWidgetItem(QString::number(x)));
	table->setItem(table->rowCount()-1,1,new QTableWidgetItem(QString::number(y)));
	table->setItem(table->rowCount()-1,2,new QTableWidgetItem(QString::number(r)));
double s=0;
	for (int i=0;i<len+1;++i)
	{
		s=s+resList.at(i);
	}
	s=s/(len+1);
	if (len==0)
	{
		lastPos=QPointF((len+1)*30,(int)s+10);
	}
	else
	{
		scene->addLine(QLineF(lastPos,QPointF((len+1)*30,(int)s+10)),redPen);
		lastPos=QPointF((len+1)*30,(int)s+10);
	}
	hLine->setLine(QLineF(10,10,(len+1)*30,10));
	scene->addLine(QLineF((len+1)*30,5,(len+1)*30,15));
	text=scene->addText(QString::number(s));
	if(len%2)
		text->setPos(QPointF((len+1)*30,(int)s+20));
	else
		text->setPos(QPointF((len+1)*30,(int)s-20));
	text->scale(1,-1);
	resultEdit->setText(QString::number(s));
}

void ResultForm::setRoot(double x,double y)
{
	rootXEdit->setText(QString::number(x));
	rootYEdit->setText(QString::number(y));
}

void ResultForm::createGroupBox()
{
    groupBox = new QGroupBox(tr("Result"));
    QVBoxLayout *layout=new QVBoxLayout;
    QHBoxLayout *lh1=new QHBoxLayout;
    QHBoxLayout *lh2=new QHBoxLayout;
	QHBoxLayout *lh4=new QHBoxLayout;
    QSplitter *splitter = new QSplitter;
    splitter->addWidget(view);
    splitter->addWidget(table);
	splitter->setOrientation(Qt::Vertical);
    splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 0);
    lh1->addWidget(resultLabel);
    lh1->addWidget(resultEdit);
	lh2->addWidget(splitter);
	lh4->addWidget(rootLabel);
	lh4->addWidget(rootXEdit);
	lh4->addWidget(rootYEdit);
    layout->addLayout(lh1);
	layout->addLayout(lh4);
    layout->addLayout(lh2);
    groupBox->setLayout(layout);
}

void ResultForm::reject()
{
    QDialog::reject();
}
