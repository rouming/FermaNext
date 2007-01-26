#include "arcprop.h"
#include "arc.h"
#include "spoint.h"

ArcProp::ArcProp(Arc *parentArc, QWidget * parent, Qt::WindowFlags f)
:QDialog(parent,f)
{
    file=new QFile((const QString &)"arcprop_out.txt");
    file->open(QIODevice::WriteOnly);
    out.setDevice(file);
    arc=parentArc;
    labelAngle=new QLabel(tr("Angle"));
    labelCenter=new QLabel(tr("Center point"));
    labelRad=new QLabel(tr("Radius"));
	errMsg=new QLabel(tr(""));
    validator=new QDoubleValidator(this);
    validator->setRange(0,360);
    editSpan=new QLineEdit();
    editSpan->setValidator(validator);
    editStart=new QLineEdit();
    editStart->setValidator(validator);
    editRad=new QLineEdit();
    editRad->setValidator(new QDoubleValidator(editRad));
    editcx=new QLineEdit();
    editcx->setValidator(new QDoubleValidator(editcx));
    editcy=new QLineEdit();
    editcy->setValidator(new QDoubleValidator(editcy));
	QRegExp rx("([\\d\\(\\)\\+\\-\\/\\*\\,\\.\\X\\Y]|(sin|cos|tg|ctg|ln|lg|exp|sqrt))+");
	QValidator *validat = new QRegExpValidator(rx, this);
	editBoundCond=new QLineEdit();
	editBoundCond->setValidator(validat);
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

    setWindowTitle(tr("Arc Properties"));
    setValues();
}

void ArcProp::setValues()
{
QPointF point;
    editSpan->setText(QString::number(arc->spanAngle()*(-1)/16.));
    editStart->setText(QString::number(-1*arc->startAngle()/16.));
    editRad->setText(QString::number(arc->rect().width()/2.));
    point=QPointF(arc->rect().x()+arc->rect().width()/2,arc->rect().y()+arc->rect().width()/2.);
    point=arc->mapToScene(point);
    editcx->setText(QString::number(point.x()));
    editcy->setText(QString::number(point.y()));
	span=QString::number(arc->spanAngle()*(-1)/16.);
	start=QString::number(-1*arc->startAngle()/16.);
	rad=QString::number(arc->rect().width()/2.);
	cx=QString::number(point.x());
	cy=QString::number(point.y());
	editBoundCond->setText(arc->boundaryCondition());
}

void ArcProp::createGroupBox()
{
    groupBox = new QGroupBox(tr("Arc condition"));
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *lh1 = new QHBoxLayout;
    QHBoxLayout *lh2 = new QHBoxLayout;
    QHBoxLayout *lh3 = new QHBoxLayout;
    QHBoxLayout *lh4 = new QHBoxLayout;
    QHBoxLayout *lh5 = new QHBoxLayout;
	QHBoxLayout *lh6 = new QHBoxLayout;
    lh1->addWidget(labelAngle);
    lh1->addWidget(editStart);
    lh1->addWidget(editSpan);
    lh2->addWidget(labelCenter);
    lh2->addWidget(editcx);
    lh2->addWidget(editcy);
    lh3->addWidget(labelRad);
    lh3->addWidget(editRad);
	lh4->addWidget(labelBoundCond);
	lh5->addWidget(editBoundCond);
	lh6->addWidget(errMsg);
    layout->addLayout(lh1);
    layout->addLayout(lh2);
    layout->addLayout(lh3);
    layout->addLayout(lh4);
    layout->addLayout(lh5);
	layout->addLayout(lh6);
    groupBox->setLayout(layout);
}

void ArcProp::accept()
{
	if(!(editSpan->text()==span&&editStart->text()==start&&editRad->text()==rad
		&&editcx->text()==cx&&editcy->text()==cy))
	{
    	if (!editSpan->text().isEmpty() && !editStart->text().isEmpty()  && !editRad->text().isEmpty()
        	&& !editcx->text().isEmpty() && !editcy->text().isEmpty()) 
    	{
        	arc->setArc(QPointF(editcx->text().toDouble(),editcy->text().toDouble()),
            	editStart->text().toDouble(),editSpan->text().toDouble(),
            	editRad->text().toDouble());
			out<<"change"<<endl;
    	}
	}
	if (checkBoundCond(editBoundCond->text()))
	{
		arc->setBoundaryCondition(editBoundCond->text());
		QDialog::accept();
	}
		else
	{
		errMsg->setText(tr("wrong boundary condition!"));
		return;
	}
}

void ArcProp::reject()
{
    QDialog::reject();
}

bool ArcProp::checkBoundCond(const QString &str)
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
