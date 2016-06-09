#include "MyPointItem.h"


MyPointItem::MyPointItem()
{
	
}


MyPointItem::~MyPointItem()
{
}

void MyPointItem::setOption(int op)
{
	PaintOption = op;
	return;
}


void MyPointItem::setTippoints(vector<QPointF> points)
{
	tippoints.clear();

	for (int i = 0; i < points.size(); i++)
	{
		tippoints.push_back(points[i]);
	}
	return;
}

void MyPointItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF a = event->pos();

	if (event->button() == Qt::LeftButton)
	{
		if (PaintOption == CENTER)
			f_o = a;
		prepareGeometryChange();
		update();
	}
	return;
}


QRectF MyPointItem::boundingRect()const
{
	QImage *image = new QImage();
	int height, width;
	if (image->load(g_filedst))
	{
		height = image->height();
		width = image->width();
	}
	return QRectF(0, 0, width, height);
}

void MyPointItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPen PenCenter;//花心画笔
	PenCenter.setWidth(15);
	PenCenter.setColor(QColor(0,0,255));


	QPen PenTippoints;//顶点画笔
	PenTippoints.setWidth(15);
	PenTippoints.setColor(QColor(255, 0, 0));


	switch (PaintOption)
	{
	case 0:
	case TIPPOINT://绘制花心和顶点
		painter->setPen(PenTippoints);
		painter->drawPoints(tippoints);
	case CENTER://点击绘制花心
		painter->setPen(PenCenter);
		painter->drawPoint(f_o);
		break;
	case DRAWCENTER:// 绘制已有花心
		painter->setPen(PenCenter);
		painter->drawPoint(f_o);
		break;
	default:
		break;
	}

	

	return;
}

QPointF MyPointItem::f_o=QPointF(0,0);
