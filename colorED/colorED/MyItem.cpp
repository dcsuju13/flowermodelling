#include "MyItem.h"


MyItem::MyItem()
{

	QImage *image = new QImage();
	if (image->load(g_filedst))
	{
		height = image->height();
		width = image->width();
	}
		
}


MyItem::~MyItem()
{
}

QRectF MyItem::boundingRect()const
{
	return QRectF(0,0,width,height);
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	

	QPen pen;
	pen.setStyle(Qt::SolidLine);
	pen.setColor(QColor(255, 255, 255,120));
	
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);

	QBrush brush(QColor(255, 255, 255,120), Qt::SolidPattern); // 画刷
	painter->setBrush(brush);
	
	
	for (int i = 0; i < edgeArea.size(); i++)//已经确定的边缘区域
	{
		pen.setWidth(2 * edgeBrushsize[i]);
		painter->setPen(pen);
		painter->drawLines(edgeArea[i]);
	}
		
	pen.setWidth(2 * brushsize);
	painter->setPen(pen);
	painter->drawLines(thisArea);//这一笔的区域
	

	QPainter painter1;
	painter1.setBrush(brush);
	if (Acoption == PAINT || Acoption == MOVE)
		painter1.drawEllipse(now.x() - brushsize, now.y() - brushsize, 2*brushsize, 2*brushsize);//绘制笔刷
	
	return;
}

void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	if (event->button() == Qt::LeftButton)
	{
		Acoption = PAINT;
		now = event->pos();
		thisArea.push_back(now);
		prepareGeometryChange();
		update();

	}
	
	return;
}

void MyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		Acoption = MOVE;
		now = event->pos();
		
		thisArea.push_back(now);
		prepareGeometryChange();
		update();
		
	}
	QGraphicsItem::mouseMoveEvent(event);
	return;
}

void MyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{	
	now = event->pos();
	edgeArea.push_back(thisArea);
	edgeBrushsize.push_back(brushsize);
	
	prepareGeometryChange();
	update();
	Acoption = RELEASE;

	//保存一张mask图
	//得到提取边缘的区域
	QVector<QPointF> ea = this->getEDArea();
	Mat maskArea = Mat(height,width, CV_8UC1, Scalar(0));

	for (int i = 0; i < ea.size(); i++)
	{
		int x = ea[i].x();
		int y = ea[i].y();
		if (x >= width || y >= height|| x <= 0 || y <= 0)
			continue;
		if (maskArea.at<uchar>(y, x) == 0)
			maskArea.at<uchar>(y, x) = 255;
	}

	
	string num;
	stringstream ss;
	ss << countImage;
	ss >> num;
	string maskfile = "edgearea"+num+".jpg";

	imwrite(maskfile, maskArea);
	countImage++;

	QGraphicsItem::mouseReleaseEvent(event);
	return;
}


QVector<QPointF> MyItem::getEDArea()
{

	//返回最终区域，对EDArea赋值
	EDArea.clear();
	
	for (int j = 0; j < thisArea.size(); j++)
	{
		for (int k = thisArea[j].x() - brushsize; k <= thisArea[j].x() + brushsize; k++)
		{
			int low = thisArea[j].y() - sqrtf(brushsize*brushsize - (k - thisArea[j].x())*(k - thisArea[j].x()));
			int high = thisArea[j].y() + sqrtf(brushsize*brushsize - (k - thisArea[j].x())*(k - thisArea[j].x()));

			for (int a = low; a <= high; a++)
			{
				QPoint t1(k, a);	
				EDArea.push_back(t1);

			}
		}
	}
		

	thisArea.clear();

	return EDArea;
}

void MyItem::ClearedgeArea()
{
	edgeArea.clear();
	thisArea.clear();
	EDArea.clear();
	update();
	return;
}

int MyItem::getMaskCount()
{
	return countImage;
}