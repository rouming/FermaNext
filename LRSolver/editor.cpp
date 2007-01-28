//#include <windows.h>
#include "editor.h"
#include "line.h"
#include "spoint.h"
#include "arc.h"
#include "walkprop.h"
#include "gridNode.h"
#include <QMenu>
#include <QContextMenuEvent>
#include <QGraphicsEllipseItem>
#include <QCursor>
#include <math.h>
#include <stdlib.h>
#include <QTime>
#include <time.h>

#define PI 3.14159265

Editor::Editor(QGraphicsScene * canvas, QWidget *parent) 
: QGraphicsView(canvas, parent)
{
	construction=true;
    setMouseTracking(TRUE);
    scale( 1, -1 );
    setRenderHints(QPainter::Antialiasing);
    //setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::NoAnchor);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    //debug output
    file=new QFile((const QString &)"editor_out.txt");
    file->open(QIODevice::WriteOnly);
    out.setDevice(file);
    createActions();
    unSetStatus();
    pendingItem=0;
    pendingLine=0;
    pendingArc=0;
    activeItem=0;
    lastPos = QPoint(0,0);
    selection=false;
	areaPath=0;
	grid=0;
	setWalkNumber(5);
	setStep(10);
	settingRootPoint=false;
	setDemoMode(false);
	result=0;
}

Editor::~Editor()
{
    file->close(); //debug output
    delete file;
    delete addLineAct;
    delete addArcAct;
    delete zoomInAct;
    delete zoomOutAct;
    delete delAct;
    delete propAct;
    delete selectAct;
	delete breakAreaAct;
}

void Editor::setActiveItem(QGraphicsItem *item)
{ 
	if (!construction)
		return;
    if (selection && item)
    {
        if (item->type()==QGraphicsItem::UserType+1)
        {
            Line *l;
            l=qgraphicsitem_cast<Line *>(item);
            l->setSelect(true);    
			list.append(item);
			scene()->update();
        }
        if (item->type()==QGraphicsItem::UserType+3)
        {
            Arch *a;
            a=qgraphicsitem_cast<Arch *>(item);
            a->setSelect(true);    
			list.append(item);
			scene()->update();
        }
    return;
    }
    if(item!=activeItem)
    {
        if(activeItem) 
        {
            if (activeItem->type()==QGraphicsItem::UserType+1)
            {
                Line *l;
                l=qgraphicsitem_cast<Line *>(activeItem);
                l->setActive(false);    
            }
            if (activeItem->type()==QGraphicsItem::UserType+3)
            {
                Arch *a;
                a=qgraphicsitem_cast<Arch *>(activeItem);
                a->setActive(false);    
            }
            if (activeItem->type()==QGraphicsItem::UserType+2)
            {
                SPoint *sp;
                sp=qgraphicsitem_cast<SPoint *>(activeItem);
                sp->setActive(false);   
            }   
        }
        activeItem=item;
        if(activeItem)
        {
            if (activeItem->type()==QGraphicsItem::UserType+1)
            {
                Line *l;
                l=qgraphicsitem_cast<Line *>(activeItem);
                l->setActive(true); 
            }
            if (activeItem->type()==QGraphicsItem::UserType+3)
            {
                Arch *a;
                a=qgraphicsitem_cast<Arch *>(activeItem);
                a->setActive(true); 
            }   
            if (activeItem->type()==QGraphicsItem::UserType+2)
            {
                SPoint *sp;
                sp=qgraphicsitem_cast<SPoint *>(activeItem);
                sp->setActive(true);    
            }   
        }
        scene()->update();
    }
}

void Editor::createActions()
{
    addLineAct=new QAction(tr("Add &Line"), this);
    connect(addLineAct,SIGNAL(triggered()),this,SLOT(addLine()));
    addArcAct=new QAction(tr("Add &Arc"), this);
    connect(addArcAct,SIGNAL(triggered()),this,SLOT(addArc()));
    zoomInAct=new QAction(tr("Zoom &In"),this);
    connect(zoomInAct,SIGNAL(triggered()),this,SLOT(zoomIn()));
    zoomOutAct=new QAction(tr("Zoom &Out"),this);
    connect(zoomOutAct,SIGNAL(triggered()),this,SLOT(zoomOut()));
    delAct=new QAction(tr("Delete Item"),this);
    delAct->setShortcut (Qt::Key_Delete);
    connect(delAct,SIGNAL(triggered()),this,SLOT(del()));
    propAct=new QAction(tr("Properties"),this);
    connect(propAct,SIGNAL(triggered()),this,SLOT(properties()));
    selectAct=new QAction(tr("Create Area"),this);
    connect(selectAct,SIGNAL(triggered()),this,SLOT(select()));
	breakAreaAct=new QAction(tr("Break area"),this);
	connect(breakAreaAct,SIGNAL(triggered()),this,SLOT(breakArea()));
}

void Editor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu contextMenu(this);   
    if (activeItem) 
    {
        if (activeItem->type()!=QGraphicsItem::UserType+2) 
        {        
            contextMenu.addAction(delAct);
            contextMenu.addAction(propAct);
            contextMenu.exec(event->globalPos());
        }
        else
        {
            contextMenu.addAction(addLineAct);
            contextMenu.addAction(addArcAct);
            contextMenu.addSeparator();
            contextMenu.addAction(zoomInAct);
            contextMenu.addAction(zoomOutAct);
            contextMenu.exec(event->globalPos());
        }
    }
    else
    {
        if (!selection && construction)
        {
            contextMenu.addAction(addLineAct);
            contextMenu.addAction(addArcAct);
            contextMenu.addSeparator();
            contextMenu.addAction(selectAct);
        }
		if (!construction)
			contextMenu.addAction(breakAreaAct);
        contextMenu.addAction(zoomInAct);
        contextMenu.addAction(zoomOutAct);
        contextMenu.exec(event->globalPos());
    }
}

void Editor::addLine()
{
    addItem(new Line());
    setStatus(QString("Specificate 1st point: "));
}

void Editor::addArc()
{
    addItem(new Arch());
    setStatus(QString("Specificate center point: "));
}

void Editor::addItem(QGraphicsItem *item)
{
    delete pendingItem;
    pendingLine=0;
    pendingArc=0;
    pendingItem=item;
    scene()->addItem(item);
    setCursor(Qt::CrossCursor);
}

void Editor::zoomIn()
{
    scale( 2.0, 2.0 );
    scene()->update();
}

void Editor::zoomOut()
{
    scale( 0.5, 0.5 );
    scene()->update();
}

void Editor::del()
{
    if(activeItem) 
    {
        if (activeItem->type()!=QGraphicsItem::UserType+2)
        {
            QGraphicsItem *item = activeItem;
            setActiveItem(0);
            delete item;
            scene()->update();
        }
    }
}

void Editor::properties()
{
    if(activeItem) 
    {
        if (activeItem->type()==QGraphicsItem::UserType+1)
        {
            Line *item = qgraphicsitem_cast<Line *>(activeItem);
            item->execProp();
        }
        if (activeItem->type()==QGraphicsItem::UserType+3)
        {
            Arch *item = qgraphicsitem_cast<Arch *>(activeItem);
            item->execProp();
        }
    }
}

void Editor::walkProperties()
{
    WalkProp dialog(this);
    dialog.exec();
}

void Editor::select()
{
    setStatus("Select item and Press <Enter> to create area: ");
    selection=true;
}

void Editor::breakArea()
{
	settingRootPoint=false;
	construction=true;
	clearSelectionList();
	scene()->removeItem(&pathTest);
	gridList.clear();
	emit constructionUnlocked();
	if(areaPath)
		delete areaPath;
	areaPath=0;
}

void Editor::showNewItem(QGraphicsItem *item)
{
    item->show();
    scene()->update();
}

void Editor::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton && pendingItem)
    {
    QPointF eventPosition;
        eventPosition=mapToScene(event->pos().x(), event->pos().y());
    QList<QGraphicsItem *> itemList;
    SPoint *sp;
        itemList=items(event->pos());
        if (!itemList.isEmpty())
        for (int i = 0; i < itemList.size(); ++i) {
            if (itemList.at(i)->type() == QGraphicsItem::UserType+2) {
                sp=qgraphicsitem_cast<SPoint *>(itemList.at(i));
                eventPosition=sp->posit();
            }
        }
            if (pendingLine)
            {
                pendingLine->setDest(eventPosition);
                showNewItem(pendingLine);
                pendingItem=0;
                pendingLine=0;
                setActiveItem(0);
                unsetCursor();
                lastPos=event->pos();
                unSetStatus();
                return;
            }
            if (pendingItem && pendingItem->type()==QGraphicsItem::UserType+1 && !pendingLine)
            {
                pendingLine=qgraphicsitem_cast<Line *>(pendingItem);
                SPoint *sourceNode_ = new SPoint(this,pendingLine);
                scene()->addItem(sourceNode_);
                SPoint *destNode_ = new SPoint(this,pendingLine);
                scene()->addItem(destNode_);
                pendingLine->setSourceNode(sourceNode_);
                pendingLine->setDestNode(destNode_);
                pendingLine->setSource(eventPosition);
                pendingLine->setDest(eventPosition);
                scene()->update();
                lastPos=event->pos();
                unSetStatus();
                setStatus(QString("Specificate 2nd point: "));
                setActiveItem(pendingItem);
                return;
            }
            if (pendingItem && pendingItem->type()==QGraphicsItem::UserType+3 && !pendingArc)
            {
                pendingArc=qgraphicsitem_cast<Arch *>(pendingItem);
                SPoint *centerNode_ = new SPoint(this,pendingArc);
                scene()->addItem(centerNode_);
                SPoint *startNode_ = new SPoint(this,pendingArc);
                scene()->addItem(startNode_);
                SPoint *spanNode_ = new SPoint(this,pendingArc);
                scene()->addItem(spanNode_);
                pendingArc->setCenterNode(centerNode_);
                pendingArc->setStartNode(startNode_);
                pendingArc->setSpanNode(spanNode_);
                pendingArc->setCenter(eventPosition);
                pendingArc->setSpan(eventPosition);
                pendingArc->setStart(eventPosition);
                lastPos=event->pos();
                unSetStatus();
                setStatus(QString("Specificate 1st point: "));
                setActiveItem(pendingItem);
                return;
            }
            if (pendingArc && !pendingArc->ready())
            {
                pendingArc->setStart(eventPosition);
                lastPos=event->pos();
                unSetStatus();
                setStatus(QString("Specificate angle: "));
                pendingArc->setReady();
                return;
            }
            if (pendingArc && pendingArc->ready())
            {
                pendingArc->setSpan(eventPosition);
                lastPos=event->pos();
                unSetStatus();
                showNewItem(pendingArc);
                pendingItem=0;
                pendingArc=0;
                setActiveItem(0);
                unsetCursor();
                lastPos=event->pos();
                return;
            }
    }
    if(!pendingItem && event->button()==Qt::LeftButton)
    {
	QPointF eventPosition;
        eventPosition=mapToScene(event->pos().x(), event->pos().y());
		if (settingRootPoint && areaPath->contains(eventPosition))
		{
			rootPoint=eventPosition;
			unSetStatus();
			settingRootPoint=false;
			breakAreaAct->setEnabled(false);
			out<<"root point set"<<endl;
			meshArea();
			return;
		}
        if (!itemAt(event->pos()))
            setActiveItem(0);
        else
        { 
            setActiveItem(itemAt(event->pos()));
        }
    }
    lastPos=event->pos();
}

void Editor::mouseMoveEvent(QMouseEvent *event)
{
QString posStr;
    pointF=mapToScene(event->pos().x(), event->pos().y());
    posStr=QString::number(pointF.x());
    posStr+=" ";
    posStr+=QString::number(pointF.y());
    emit statusChanged(status()+posStr);

QPointF eventPosition;
    eventPosition=mapToScene(event->pos().x(), event->pos().y());
QList<QGraphicsItem *> itemList;
SPoint *sp;
    itemList=items(event->pos());
    if (!itemList.isEmpty())
    for (int i = 0; i < itemList.size(); ++i) {
        if (itemList.at(i)->type() == QGraphicsItem::UserType+2) {
            sp=qgraphicsitem_cast<SPoint *>(itemList.at(i));
            eventPosition=sp->posit();
        }
    }
    if (pendingLine)
    {
        pendingLine->setDest(eventPosition);
        lastPos=event->pos();
        return;
    }
    if (pendingArc && pendingArc->ready())
    {
        pendingArc->setSpan(eventPosition);
        lastPos=event->pos();
        return;
    }
    if (!items(event->pos()).isEmpty()&&!pendingItem)
        setCursor(Qt::CrossCursor);
    else
    {   if(pendingItem) 
            setCursor(Qt::CrossCursor);
        else unsetCursor();
    }
    if (!items(event->pos()).isEmpty()&&!activeItem)
    {
        if (itemAt(event->pos())->type()==QGraphicsItem::UserType+2)
        {
            setActiveItem(itemAt(event->pos()));
        }
    }
    if (items(event->pos()).isEmpty()&&activeItem&&!(event->buttons() & Qt::LeftButton))
    {
        if (activeItem->type()==QGraphicsItem::UserType+2)
        {
            setActiveItem(0);
        }
    }
    if (event->buttons() & Qt::LeftButton) 
    {
    QPointF delta;
        delta=/*mapToScene(event->pos().x(),event->pos().y())*/eventPosition-mapToScene(lastPos.x(),lastPos.y());
        if (activeItem)
        {
            activeItem->moveBy(delta.x(),delta.y());
            scene()->update();
            if (activeItem->type()==QGraphicsItem::UserType+2 && !pendingLine && !pendingArc)
            {
                SPoint *sp;
                sp=qgraphicsitem_cast<SPoint *>(activeItem);
                sp->setPosition(eventPosition);
                sp->positChange();
            }
        }
    }   
    lastPos=mapFromScene(eventPosition);
}

void Editor::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
		if (settingRootPoint)
			return;
        setActiveItem(0);
        if (pendingItem) delete pendingItem;
        pendingItem=0;
        pendingLine=0;
        pendingArc=0;
		if (selection == true)
		{
			selection=false;
			clearSelectionList();
			scene()->update();
		}
        unsetCursor();
        unSetStatus();
        break;
    case Qt::Key_Delete:
        del();
        break;
    case Qt::Key_Return:
        if (selection)
        {
            compliteSelection();
        }
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void Editor::compliteSelection()
{
Line *l;
Arch *a;
	if (list.isEmpty())
	{
		return;
	}
QList <QGraphicsItem*> listTmp;
	for (int i=0; i<list.size();++i) {
		if (!listTmp.contains(list.at(i)))
			listTmp.append(list.at(i));
	}
	list.clear();
	for (int i=0;i<listTmp.size();i++) {
		list.append(listTmp.at(i));
	}
QList <QPointF> pointList;
	for (int i = 0; i < list.size(); ++i) {
    	if (list.at(i)->type()==QGraphicsItem::UserType+1)
		{
			l=qgraphicsitem_cast<Line *>(list.at(i));
			pointList.append(l->sourceNode()->posit());
			pointList.append(l->destNode()->posit());		
		}
    	if (list.at(i)->type()==QGraphicsItem::UserType+3)
		{
			a=qgraphicsitem_cast<Arch *>(list.at(i));
			pointList.append(a->startNode()->posit());
			pointList.append(a->spanNode()->posit());
		}
	}
	listTmp.clear();
bool b=false;
	for (int i=0; i<pointList.size(); ++i) {
		for (int j=0; j<pointList.size(); ++j) {
			if (i!=j && pointList.at(i)==pointList.at(j))
			{
				if (b==true) 
					b=false;
				else
					b=true;
			}
		}
		if (b==false)
		{
			QMessageBox msgBox;
			msgBox.setStandardButtons(QMessageBox::Ok);
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.setText("Wrong Area selected");
			msgBox.setWindowTitle("error");
			msgBox.exec();
			clearSelectionList();
			scene()->update();
			return;
		}
		b=false;
	}
	lockConstruction();
    unSetStatus();
    selection=false;
	scene()->update();
	createPath();
}

void Editor::clearSelectionList()
{
	for (int i=0;i<list.size(); ++i)
	{
        if (list.at(i)->type()==QGraphicsItem::UserType+1)
        {
            Line *l;
            l=qgraphicsitem_cast<Line *>(list.at(i));
            l->setSelect(false);    
        }
        if (list.at(i)->type()==QGraphicsItem::UserType+3)
        {
            Arch *a;
            a=qgraphicsitem_cast<Arch *>(list.at(i));
            a->setSelect(false);    
        }
    }
    list.clear();
    unSetStatus();
    selection=false;
	scene()->update();
}

void Editor::lockConstruction()
{
	construction=false;
	emit constructionLocked();
}

void Editor::createPath()
{
	if (areaPath) delete areaPath;
	areaPath=new QPainterPath();
Line *l;
Arch *a;
QGraphicsItem *current;
QGraphicsItem *newcurrent;
QPointF position;
	if (list.at(0)->type()==QGraphicsItem::UserType+1)
    {
        l=qgraphicsitem_cast<Line *>(list.at(0));
		areaPath->moveTo(l->sourceNode()->posit());
		position=l->sourceNode()->posit();
		current=l;
    }
    if (list.at(0)->type()==QGraphicsItem::UserType+3)
    {
        a=qgraphicsitem_cast<Arch *>(list.at(0));
		areaPath->moveTo(a->startNode()->posit());
		position=a->startNode()->posit();
		current=a;
    }	
	for (int i=0; i<list.size(); ++i)
	{
		if (current->type()==QGraphicsItem::UserType+1)
		{
	        l=qgraphicsitem_cast<Line *>(current);
			if (position==l->sourceNode()->posit())
			{
				areaPath->lineTo(l->destNode()->posit());
				position=l->destNode()->posit();
			}
			else
			{
				areaPath->lineTo(l->sourceNode()->posit());
				position=l->sourceNode()->posit();
			}
		}
		else
		{
		double delta;
		double ang;
		double radius;
			a=qgraphicsitem_cast<Arch *>(current);
			if (position==a->startNode()->posit())
			{
				delta=a->spanAngle()/(-1600.0);
				ang=((-1)*a->startAngle())/16;
				radius=sqrt((a->centerNode()->posit().x()-a->startNode()->posit().x())*
					(a->centerNode()->posit().x()-a->startNode()->posit().x())+
					(a->centerNode()->posit().y()-a->startNode()->posit().y())*
					(a->centerNode()->posit().y()-a->startNode()->posit().y()));
				for (int i=0; i<99; ++i)
				{
					ang=ang+delta;
					areaPath->lineTo(a->centerNode()->posit()+
						QPointF(radius*cos((ang)*PI/180),radius*sin((ang)*PI/180)));
				}
				areaPath->lineTo(a->spanNode()->posit());
				position=a->spanNode()->posit();
			}
			else
			{
				delta=a->spanAngle()/(1600.0);
				ang=((-1)*(a->startAngle()+a->spanAngle()))/16;
				radius=sqrt((a->centerNode()->posit().x()-a->startNode()->posit().x())*
					(a->centerNode()->posit().x()-a->startNode()->posit().x())+
					(a->centerNode()->posit().y()-a->startNode()->posit().y())*
					(a->centerNode()->posit().y()-a->startNode()->posit().y()));
				for (int i=0; i<99; ++i)
				{
					ang=ang+delta;
					areaPath->lineTo(a->centerNode()->posit()+
						QPointF(radius*cos((ang)*PI/180),radius*sin((ang)*PI/180)));
				}
				areaPath->lineTo(a->startNode()->posit());
				position=a->startNode()->posit();
			}
		}
		newcurrent=0;
		for (int j=0;j<list.size(); ++j)
		{
			if (list.at(j)!=current)
			{
				if (list.at(j)->type()==QGraphicsItem::UserType+1)
				{
					l=qgraphicsitem_cast<Line *>(list.at(j));
					if((l->sourceNode()->posit()==position)
						||(l->destNode()->posit()==position))
						newcurrent=list.at(j);
				}
				if (list.at(j)->type()==QGraphicsItem::UserType+3)
				{
					a=qgraphicsitem_cast<Arch *>(list.at(j));
					if((a->startNode()->posit()==position)
						||(a->spanNode()->posit()==position))
						newcurrent=list.at(j);
				}
			}
		}
		current=newcurrent;
	}
/*	pathTest.setPath(*areaPath);
	scene()->addItem(&pathTest);
	scene()->update();*/
	setRootPoint();
}

void Editor::setRootPoint()
{
	settingRootPoint=true;
    setStatus(QString("Specificate point for calculates: "));
}

void Editor::meshArea()
{
QList<GridNode> tmpList;
QList<QPointF> tmpPointList;
GridNode node(0,0);
	out<<"start meshing"<<endl;
	grid=new QPainterPath();
	tmpList.append(GridNode(rootPoint.x(),rootPoint.y()));
	grid->addEllipse(rootPoint.x()-1,rootPoint.y()-1,2,2);	
	tmpPointList.append(GridNode(rootPoint.x(),rootPoint.y()));
	out<<"check1"<<endl;
	while (!tmpList.isEmpty())
	{
		node=tmpList.first();
		if (areaPath->contains(QPointF(node.x()+_step,node.y())))
		{
			if (!tmpPointList.contains(QPointF(node.x()+_step,node.y())))
			{
				tmpPointList.append(QPointF(node.x()+_step,node.y()));
				tmpList.append(GridNode(node.x()+_step,node.y()));
				grid->addEllipse(node.x()+_step-1,node.y()-1,2,2);
			}
/*			grid->moveTo(QPointF(node.x(),node.y()));
			grid->lineTo(QPointF(node.x()+_step,node.y()));*/
		}
		out<<"check21"<<endl;
  		if (areaPath->contains(QPointF(node.x()-_step,node.y())))
		{
			if (!tmpPointList.contains(QPointF(node.x()-_step,node.y())))
			{
				tmpPointList.append(QPointF(node.x()-_step,node.y()));
				tmpList.append(GridNode(node.x()-_step,node.y()));
				grid->addEllipse(node.x()-_step-1,node.y()-1,2,2);				
			}
/*			grid->moveTo(QPointF(node.x(),node.y()));
			grid->lineTo(QPointF(node.x()-_step,node.y()));*/
		}
		out<<"check22"<<endl;
  		if (areaPath->contains(QPointF(node.x(),node.y()-_step)))
		{
			if(!tmpPointList.contains(QPointF(node.x(),node.y()-_step)))
			{
				tmpPointList.append(QPointF(node.x(),node.y()-_step));
				tmpList.append(GridNode(node.x(),node.y()-_step));
				grid->addEllipse(node.x()-1,node.y()-_step-1,2,2);
			}
/*			grid->moveTo(QPointF(node.x(),node.y()));
			grid->lineTo(QPointF(node.x(),node.y()-_step));*/
		}
		out<<"check23"<<endl;
   		if (areaPath->contains(QPointF(node.x(),node.y()+_step)))
		{
			if (!tmpPointList.contains(QPointF(node.x(),node.y()+_step)))
			{
				tmpPointList.append(QPointF(node.x(),node.y()+_step));
				tmpList.append(GridNode(node.x(),node.y()+_step));
				grid->addEllipse(node.x()-1,node.y()+_step-1,2,2);
			}
/*			grid->moveTo(QPointF(node.x(),node.y()));
			grid->lineTo(QPointF(node.x(),node.y()+_step));*/
		}
		out<<"check24"<<endl;
		if (!areaPath->contains(QPointF(node.x()+_step,node.y())) ||
			!areaPath->contains(QPointF(node.x()-_step,node.y())) ||
			!areaPath->contains(QPointF(node.x(),node.y()-_step)) ||
			!areaPath->contains(QPointF(node.x(),node.y()+_step)))
		{
			out<<"its a border node"<<endl;
			node.setBorder(true);
			out<<"border cond set"<<endl;
			setBorderValue(&node);
			out<<"check bord"<<endl;
		}
		out<<"trying append node"<<endl;
		gridList.append(node);
		out<<"node appended"<<endl;
		tmpList.erase(tmpList.begin());
		out<<"end"<<endl;
	}
	out<<"area meshed"<<endl;
	drawGrid();
	out<<"grid drawn"<<endl;
	emit unlockCalculation();
}

void Editor::drawGrid()
{
	if (!grid)
		out<<"grid is NULL!"<<endl;
	pathTest.setPath(*grid);
	scene()->addItem(&pathTest);
	scene()->update();
}

void Editor::setBorderValue(GridNode *node)
{
double incr=_step/20.;
int j=0;
double x;
double y;
	out<<"searching for nearest element"<<endl;
	if(!areaPath->contains(QPointF(node->x()+_step,node->y())))
	{
	bool b=false;
	int i=1;
		while (!b)
		{
			if(areaPath->contains(QPointF(node->x()+incr*i,node->y())))
				++i;
			else
				b=true;
		}
		if(j==0||i<j)
		{
			j=i;
			x=node->x()+incr*i;
			y=node->y();
		}
	}
	if(!areaPath->contains(QPointF(node->x()-_step,node->y())))
	{
	bool b=false;
	int i=1;
		while (!b)
		{
			if(areaPath->contains(QPointF(node->x()-incr*i,node->y())))
				++i;
			else
				b=true;
		}
		if(j==0||i<j)
		{
			j=i;
			x=node->x()-incr*i;
			y=node->y();
		} 
	}
	if(!areaPath->contains(QPointF(node->x(),node->y()-_step)))
	{
	bool b=false;
	int i=1;
		while (!b)
		{
			if(areaPath->contains(QPointF(node->x(),node->y()-incr*i)))
				++i;
			else
				b=true;
		}
		if(j==0||i<j)
		{
			j=i;
			x=node->x();
			y=node->y()-incr*i;
		} 
	}
	if(!areaPath->contains(QPointF(node->x(),node->y()+_step)))
	{
	bool b=false;
	int i=1;
		while (!b)
		{
			if(areaPath->contains(QPointF(node->x(),node->y()+incr*i)))
				++i;
			else
				b=true;
		}
		if(j==0||i<j)
		{
			j=i;
			x=node->x();
			y=node->y()+incr*i;
		} 
	}
	out<<"searchin' collisions"<<endl;
	QPainterPath p;
	p.moveTo(QPointF(node->x(),node->y()));
	p.lineTo(QPointF(x,y));
	QList<QGraphicsItem*> l=scene()->items(p);
	QGraphicsItem* it=0;
	for (int ii=0;ii<l.size();++ii)
	{
		if(l.at(ii)->type()==QGraphicsItem::UserType+1||l.at(ii)->type()==QGraphicsItem::UserType+3)
		{	
			if(!it)
				it=l.at(ii);
			if (l.at(ii)->type()==QGraphicsItem::UserType+1&&it->type()==QGraphicsItem::UserType+3)
				it=l.at(ii);
		}
	}
	Line *line=0;
	Arch *arc=0;
	QString boundCond;
	boundCond="0";
	if (it==0)
	{
		out<<"item not found"<<endl;
		node->setValue(0);
		return;
	}
	if(it->type()==QGraphicsItem::UserType+1)
	{
		line=qgraphicsitem_cast<Line *>(it);
		boundCond=line->boundaryCondition();
		
	}
	else if(it->type()==QGraphicsItem::UserType+3)
	{
		arc=qgraphicsitem_cast<Arch *>(it);
		boundCond=arc->boundaryCondition();
	}
	else
		out<<"shit"<<endl;
	QString sx;
	QString sy;
	sx.setNum(x);
	sy.setNum(y);
	out<<"starting calculation"<<endl;
	node->setValue(calculateBorderValue(boundCond,sx,sy));
}

double Editor::calculateBorderValue(QString s,QString x,QString y)
{
QStringList parsedList;
int i;
QRegExp rx("([\\d()+\\-/*,\\.XY]|(sin|cos|tg|ctg|ln|lg|exp|sqrt))");
	parsedList.clear();
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
QStringList num;
QStringList unop;
QStringList biop;
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
bool b=false;
	for(int j=0;j<parsedList.size()-1;++j)
	{
		if((num.contains(parsedList.at(j))||parsedList.at(j)=="."||parsedList.at(j)==","||b)
			&&(num.contains(parsedList.at(j+1))||parsedList.at(j+1)=="."||parsedList.at(j+1)==","))
		{
			parsedList[j].append(parsedList.at(j+1));
			parsedList.removeAt(j+1);
			--j;
			b=true;
		}
		else
			b=false;		
	}
	for(int j=0;j<parsedList.size();++j)
	{
		if(parsedList.at(j)=="X")
		{
			parsedList[j]=x;
		}
		if(parsedList.at(j)=="Y")
		{
			parsedList[j]=y;
		}
	}
	while(parsedList.size()!=1)
	{
	int p1=0;
	int p2=parsedList.size()-1;
	int m1=0;
	int m2=0;
	QStringList temp;
	    for(int d=0;d<parsedList.size();++d)
		{
			if(parsedList.at(d)=="(")
			{
				m2=m2+1;
				if(m2>=m1)
				{
					m1=m2;
					p1=d;
				}
			}
			if(parsedList.at(d)==")")
			{
				if(m2==m1)
					p2=d;
				m2=m2-1;
			}
		}
		temp.clear();
		for(int d=p1;d<p2+1;++d)
		{
			temp.append(parsedList.at(d));
		}
		for (int d=0;d<p2-p1;++d)
		{
			parsedList.removeAt(p1+1);
		}
		if(temp.at(0)=="(")
		{
			temp.removeAt(temp.size()-1);
			temp.removeAt(0);
		}

		while(temp.size()!=1)
		{
		QString str;
			if(temp.size()==2&&temp.at(0)=="-")
			{
				temp[0].append(temp.at(1));
				temp.removeAt(1);
			}
			for(int di=0;di<temp.size()-1;++di)
			{
				if(unop.contains(temp.at(di)))
				{
				double res1;
					if (temp.at(di)=="sin")
					{
						res1=sin(temp.at(di+1).toDouble()*PI/180.);
					}
					if (temp.at(di)=="cos")
					{
						res1=cos(temp.at(di+1).toDouble()*PI/180.);
					}
					if (temp.at(di)=="tg")
					{
						res1=tan(temp.at(di+1).toDouble()*PI/180.);
					}
					if (temp.at(di)=="ctg")
					{
						res1=1./tan(temp.at(di+1).toDouble()*PI/180.);
					}
					if (temp.at(di)=="exp")
					{
						res1=exp(temp.at(di+1).toDouble()*PI/180.);
					}
					if (temp.at(di)=="sqrt")
					{
						if(temp.at(di+1).toDouble()<0)
							return 0;
						res1=sqrt(temp.at(di+1).toDouble());
					}
					if (temp.at(di)=="ln")
					{
						if(temp.at(di+1).toDouble()<0)
							return 0;
						res1=log(temp.at(di+1).toDouble());
					}
					if (temp.at(di)=="lg")
					{
						if(temp.at(di+1).toDouble()<0)
							return 0;
						res1=log10(temp.at(di+1).toDouble());
					}
				temp[di].setNum(res1);
				temp.removeAt(di+1);
				}
			}
			for (int ki=1;ki<temp.size()-1;++ki)
			{
				if(temp.at(ki)=="*" || temp.at(ki)=="/")
				{
				int res2;
					if(temp.at(ki)=="*")
					{
						res2=temp.at(ki-1).toDouble()*temp.at(ki+1).toDouble();
					}
					if(temp.at(ki)=="/")
					{
						if(temp.at(ki+1).toDouble()!=0)
						{
							res2=temp.at(ki-1).toDouble()/temp.at(ki+1).toDouble();
						}
						else return 0;
					}
				temp[ki-1].setNum(res2);
				temp.removeAt(ki);
				temp.removeAt(ki);	
				ki--;
				}
			}
			for (int ki=1;ki<temp.size()-1;++ki)
			{
				if(temp.at(ki)=="+" || temp.at(ki)=="-")
				{
				int res2;
					if(temp.at(ki)=="+")
					{
						res2=temp.at(ki-1).toDouble()+temp.at(ki+1).toDouble();
					}
					if(temp.at(ki)=="-")
					{
						res2=temp.at(ki-1).toDouble()-temp.at(ki+1).toDouble();
					}
				temp[ki-1].setNum(res2);
				temp.removeAt(ki);
				temp.removeAt(ki);	
				ki--;
				}
			}
		}
		parsedList[p1]=temp.at(0);
	}
return parsedList.at(0).toDouble();
}

void Editor::calculate()
{
double x;
double y;
double rd;
int ri;
double dx;
double dy;
double s=0;
bool repeat;
QGraphicsEllipseItem demoPoint;
demoPoint.setPen(QPen(Qt::red));
demoPoint.setBrush(QBrush(Qt::red));
thread()->setPriority(QThread::HighestPriority);
for (int i=0;i<_walkNumber;++i)
{
	QTime midnight(0, 0, 0);
    srand(midnight.secsTo(QTime::currentTime()));
	x=rootPoint.x();
	y=rootPoint.y();
	if(demoMode())
	{
		demoPoint.setRect(x-2,y-2,4,4);
		if (i==0)
		{		
			scene()->addItem(&demoPoint);
			repaint();
			QApplication::processEvents();
		clock_t goal=500+clock();
    	while (goal > clock());
		}
	}
	repeat=true;
	while(repeat)
	{
		rd=((double)rand()/((double)(RAND_MAX)+(double)(1)));
		ri=(int)(rd*100);
		if(ri<25)
		{
			dx=_step;
			dy=0;
		}
		if(ri>24&&ri<50)
		{
			dx=-_step;
			dy=0;
		}
		if(ri>49&&ri<75)
		{
			dx=0;
			dy=_step;
		}
		if(ri>74)
		{
			dx=0;
			dy=-_step;
		}
		x=x+dx;
		y=y+dy;
		for(int j=0;j<gridList.size();++j)
		{
			if(QPointF(x,y)==(QPointF)gridList.at(j))
			{
				if(gridList[j].border())
				{
					s=s+gridList[j].value();
					repeat=false;
				}
			}
		}
		if(demoMode())
		{
			demoPoint.setRect(x-2,y-2,4,4);
			repaint();
			QApplication::processEvents();
			clock_t goal=500+clock();
		    while (goal > clock());
		}
	}
}
result=s/_walkNumber;
	QString msg;
	msg="result is"+QString::number(result);
	QMessageBox msgBox;
	msgBox.setText(msg);
	QPushButton *okButton = msgBox.addButton(QMessageBox::Ok);
	msgBox.exec();
	if (msgBox.clickedButton() == okButton) {
    	pathTest.setPath(QPainterPath());
		delete grid;
		gridList.clear();
		setRootPoint();
		breakAreaAct->setEnabled(true);
 	} 
}
