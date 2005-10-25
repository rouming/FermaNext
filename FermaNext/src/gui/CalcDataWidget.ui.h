/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#ifndef __TR2_H__
#define __TR2_H__

#if QT_VERSION >= 0x30000
#include <qtextcodec.h>
#define fromUnicode(s) 	(QTextCodec::codecForName("CP1251"))->fromUnicode(s)
#define toUnicode(s)   	(QTextCodec::codecForName("CP1251"))->toUnicode(s)
#define tr2(s) 		QString::fromLocal8Bit(s)
#else
#define fromUnicode(s) 	s
#define toUnicode(s)   	s
#define tr2(s) 		s
#endif

#endif


void CalcDataWidget::initCalc ( TrussCalcData data )
{
    Res = data;
    comboBox->clear();

    radio1->setChecked ( TRUE );
    table1->resize(387,table1->height());
    for ( int i = 1; i <= Res.loadsNum; i++ )
    {
	comboBox->insertItem( QString::number( i ) );
    }
    lineEdit1->setText(QString::number(Res.stressLimit,'e',3));
    lineEdit2->setText(QString::number(Res.v,'e',3));
    lineEdit3->setText(QString::number(Res.forceWeight,'e',3));
    textLabel7->setText(toUnicode("из ") + QString::number(Res.loadsNum));

    connect ( pushButton1, SIGNAL(clicked()), SLOT(close()) );
    connect ( radio1, SIGNAL(clicked()), SLOT( radio1_clicked()) );
    connect ( radio2, SIGNAL(clicked()), SLOT( radio2_clicked()) ); 
    connect ( comboBox, SIGNAL(activated(int)), SLOT (combo_activated()) );

    radio1_clicked();
}

void CalcDataWidget::init()
{}

void CalcDataWidget::radio1_clicked()
{
int i;
int k;
    groupBox1->setTitle( toUnicode( "Перемещения узлов [см]" ) );
    table1->setColumnLabels ( QStringList::split( "\t", toUnicode( "Перемещения по X\tПеремещения по Y" ) ) );
    table1->setNumRows(Res.nodesNum);
    k = (comboBox->currentText()).toInt() - 1;
    for (i = 0; i < Res.nodesNum; i++) 
    {	
	 table1->setText(i,0,QString::number(Res.xTrans[Res.nodesNum*k+i],'e',3));
    }
    for (i = 0; i < Res.nodesNum; i++) 
    {
	table1->setText( i,1,QString::number(Res.yTrans[Res.nodesNum*k+i],'e',3));
    }
}


void CalcDataWidget::radio2_clicked()
{
int i;
int k;
    groupBox1->setTitle( toUnicode( "Напряжение в стержнях" ) );
    table1->setColumnLabels ( QStringList::split( "\t", toUnicode( "Напряжение [Н/см^2]\tКоэффицент запаса прочности" ) ) );
    table1->setNumRows(Res.pivotsNum);
    k = comboBox->currentText().toInt() - 1;
    for (i = 0; i < Res.pivotsNum; i++) 
    {	
	 table1->setText(i,0,QString::number(Res.stress[Res.pivotsNum*k+i],'e',3));
    }
    for (i = 0; i < Res.pivotsNum; i++) 
    {
	table1->setText( i,1,QString::number(Res.safetyFactor[Res.pivotsNum*k+i],'e',3));
    }
}


void CalcDataWidget::combo_activated()
{
    if ( radio2->isChecked() )
	radio2_clicked();
    else
	radio1_clicked();

}




void CalcDataWidget::resizeEvent( QResizeEvent * )
{
    table1->setColumnWidth( 0, ( table1->width() - table1->verticalHeader()->width() -4 ) / 2  );
    table1->setColumnWidth( 1, ( table1->width() - table1->verticalHeader()->width() -4 ) / 2  );
}
