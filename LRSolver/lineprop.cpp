#include "lineprop.h"
#include "line.h"
#include "spoint.h"

LineProp::LineProp(Line *parentLine, QWidget * parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
	line=parentLine;
	label1=new QLabel("1st point");
	label2=new QLabel("2nd point");
	errMsg=new QLabel("");
	edit1x=new QLineEdit();
	edit1x->setValidator(new QDoubleValidator(edit1x));
	edit2x=new QLineEdit();
	edit2x->setValidator(new QDoubleValidator(edit2x));
	edit1y=new QLineEdit();
	edit1y->setValidator(new QDoubleValidator(edit1y));
	edit2y=new QLineEdit();
	edit2y->setValidator(new QDoubleValidator(edit2y));
	QRegExp rx("([\\d\\(\\)\\+\\-\\/\\*\\,\\.\\X\\Y]|(sin|cos|tg|ctg|ln|lg|exp|sqrt))+");
	QValidator *validator = new QRegExpValidator(rx, this);
	editBoundCond=new QLineEdit();
	editBoundCond->setValidator(validator);
	labelBoundCond=new QLabel(tr("Boundary condition"));
    createGroupBox();
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Line Properties"));
	setValues();
}

void LineProp::setValues()
{
	edit1x->setText(QString::number(line->source().x()));
	edit1y->setText(QString::number(line->source().y()));
	edit2x->setText(QString::number(line->dest().x()));
	edit2y->setText(QString::number(line->dest().y()));
	x1=QString::number(line->source().x());
	y1=QString::number(line->source().y());
	x2=QString::number(line->dest().x());
	y2=QString::number(line->dest().y());
	editBoundCond->setText(line->boundaryCondition());
}

void LineProp::createGroupBox()
{
    groupBox = new QGroupBox(tr("Line condition"));
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *lh1 = new QHBoxLayout;
    QHBoxLayout *lh2 = new QHBoxLayout;
	QHBoxLayout *lh3 = new QHBoxLayout;
	QHBoxLayout *lh4 = new QHBoxLayout;
	QHBoxLayout *lh5 = new QHBoxLayout;
	lh1->addWidget(label1);
	lh1->addWidget(edit1x);
	lh1->addWidget(edit1y);
	lh2->addWidget(label2);
	lh2->addWidget(edit2x);
	lh2->addWidget(edit2y);
	lh3->addWidget(labelBoundCond);
	lh4->addWidget(editBoundCond);
	lh5->addWidget(errMsg);
    layout->addLayout(lh1);
	layout->addLayout(lh2);
	layout->addLayout(lh3);
	layout->addLayout(lh4);
	layout->addLayout(lh5);
    groupBox->setLayout(layout);
}

void LineProp::accept()
{
	if (!edit1x->text().isEmpty() && !edit1y->text().isEmpty() && (edit1x->text()!=x1||edit1y->text()!=y1))
	{
		line->setP1(QPointF(edit1x->text().toDouble(),edit1y->text().toDouble()));
		line->sourceNode()->setPosition(QPointF(edit1x->text().toDouble(),edit1y->text().toDouble()));
	}
	if (!edit2x->text().isEmpty() && !edit2y->text().isEmpty() && (edit2x->text()!=x2||edit2y->text()!=y2))
	{
		line->setP2(QPointF(edit2x->text().toDouble(),edit2y->text().toDouble()));
		line->destNode()->setPosition(QPointF(edit2x->text().toDouble(),edit2y->text().toDouble()));
	}
	if (checkBoundCond(editBoundCond->text()))
	{
		line->setBoundaryCondition(editBoundCond->text());
		QDialog::accept();
	}
	else
	{
		errMsg->setText(tr("wrong boundary condition!"));
		return;
	}
}

void LineProp::reject()
{
	QDialog::reject();
}

bool LineProp::checkBoundCond(const QString &str)
{
if(!str.length())
	return false;
QString s;
int bracketBalance=0;
int i;
	s=editBoundCond->text();
	for (i=0;i<s.length();++i)
	{
		if (s[i]=='(')
			bracketBalance+=1;
		if (s[i]==')')
			bracketBalance+=-1;
		if (bracketBalance<0)
			return false;
	}
	if(bracketBalance)
		return false;
QStringList parsedList;
	parsedList.clear();
	QRegExp rx("([\\d()+\\-/*,\\.XY]|(sin|cos|tg|ctg|ln|lg|exp|sqrt))");
	i=-1;
	while(s.length()!=0)
	{
		++i;
		if(rx.exactMatch(s.left(i+1)))
		{
			parsedList.append(s.left(i+1));
			s.remove(0,i+1);
			i=-1;
		}
	}
QStringList unop;
QStringList biop;
QStringList num;
	unop.clear();
	unop.append(QString("sin"));
	unop.append(QString("cos"));
	unop.append(QString("tg"));
	unop.append(QString("ctg"));
	unop.append(QString("ln"));
	unop.append(QString("lg"));
	unop.append(QString("exp"));
	unop.append(QString("sqrt"));
	biop.clear();
	biop.append(QString("-"));
	biop.append(QString("+"));
	biop.append(QString("/"));
	biop.append(QString("*"));
	num.clear();
	num.append(QString("0"));
	num.append(QString("1"));
	num.append(QString("2"));
	num.append(QString("3"));
	num.append(QString("4"));
	num.append(QString("5"));
	num.append(QString("6"));
	num.append(QString("7"));
	num.append(QString("8"));
	num.append(QString("9"));
	if(!(parsedList.at(0)=="-"||parsedList.at(0)=="("||parsedList.at(0)=="X"||parsedList.at(0)=="Y"
		||num.contains(parsedList.at(0))||unop.contains(parsedList.at(0))))
		return false;
	if(!(parsedList.at(parsedList.size()-1)==")"||parsedList.at(parsedList.size()-1)=="X"||
		parsedList.at(parsedList.size()-1)=="Y"||num.contains(parsedList.at(parsedList.size()-1))))
		return false;
	for (i=0;i<parsedList.size()-1;++i)
	{
		if(unop.contains(parsedList.at(i)))
		{
			if(parsedList.at(i+1)!="(")
				return false;
		}
		if(biop.contains(parsedList.at(i)))
		{
			if(!(parsedList.at(i+1)=="("||parsedList.at(i+1)=="X"||parsedList.at(i+1)=="Y"
				||num.contains(parsedList.at(i+1))))
				return false; 
		}
		if(parsedList.at(i)=="(")
		{
			if(!(parsedList.at(i+1)=="("||num.contains(parsedList.at(i+1))||parsedList.at(i+1)=="Y"
				||parsedList.at(i+1)=="X"||unop.contains(parsedList.at(i+1))))
				return false;
		}
		if(num.contains(parsedList.at(i)))
		{
			if(!(parsedList.at(i+1)=="."||parsedList.at(i+1)==","||parsedList.at(i+1)==")"
				||num.contains(parsedList.at(i+1))||biop.contains(parsedList.at(i+1))))
				return false;
		}
		if(parsedList.at(i)==","||parsedList.at(i)==".")
		{
			if(!num.contains(parsedList.at(i+1)))
				return false;
		}
		if(parsedList.at(i)==")")
		{
			if(!(parsedList.at(i)==")"||biop.contains(parsedList.at(i+1))))
				return false;
		} 
		if(parsedList.at(i)=="X"||parsedList.at(i)=="Y")
		{
			if(!(biop.contains(parsedList.at(i+1))||parsedList.at(i+1)==")"))
				return false;
		}
	}
return true;
}
