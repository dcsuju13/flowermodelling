#pragma once
#include <QtWidgets\qgraphicssceneevent.h>
#include <QGraphicsItem> 
#include <QPainter>
#include "qgraphicsitem.h"
#include "global.h"

#define CENTER 1
#define DRAWCENTER 2
#define TIPPOINT 3
class MyPointItem :
	public QGraphicsItem
{
public:
	MyPointItem();
	~MyPointItem();

	static QPointF f_o;
	void setOption(int op);
	void setTippoints(vector<QPointF> points);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	
private:
	QRectF boundingRect()const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	
	QVector<QPointF> tippoints;
	int	PaintOption=0;
};

