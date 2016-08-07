#pragma once
#include <QtWidgets\qgraphicssceneevent.h>
#include <QGraphicsItem> 
#include <QPainter>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include "global.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define PAINT 1
#define RELEASE 2
#define MOVE 3



class MyItem :
	public QGraphicsItem
{
public:
	MyItem();
	~MyItem();

	void ClearedgeArea();//清空笔刷区域
	QVector<QPointF> getEDArea();
	int getMaskCount();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	QRectF boundingRect()const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QVector<QPointF> thisArea;//这一笔的画线区域
	QVector<QVector<QPointF>> edgeArea;//前几笔的画线区域
	QVector<int> edgeBrushsize;//存储已有路径对应的的笔刷大小
	QVector<QPointF> EDArea;//最终的提取区域
	int count_num=0;
	int height, width;
	int Acoption=0;
	int countImage = 0;//记录存储多少张Mask图
	
	QPointF now;//当前位置
	
};

