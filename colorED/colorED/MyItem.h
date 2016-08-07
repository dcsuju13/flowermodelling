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

	void ClearedgeArea();//��ձ�ˢ����
	QVector<QPointF> getEDArea();
	int getMaskCount();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
	QRectF boundingRect()const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	QVector<QPointF> thisArea;//��һ�ʵĻ�������
	QVector<QVector<QPointF>> edgeArea;//ǰ���ʵĻ�������
	QVector<int> edgeBrushsize;//�洢����·����Ӧ�ĵı�ˢ��С
	QVector<QPointF> EDArea;//���յ���ȡ����
	int count_num=0;
	int height, width;
	int Acoption=0;
	int countImage = 0;//��¼�洢������Maskͼ
	
	QPointF now;//��ǰλ��
	
};

