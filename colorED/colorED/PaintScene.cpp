#include "PaintScene.h"


PaintScene::PaintScene()
{
}


PaintScene::~PaintScene()
{
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QPointF t = event->pos();
		MyItem *item = new MyItem();
		addItem(item);

	}
	QGraphicsScene::mousePressEvent(event);
	return;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	return;
}
void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	return;
}
