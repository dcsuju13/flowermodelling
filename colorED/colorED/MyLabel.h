#pragma once
#include "qlabel.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <vector>
#include <algorithm>
#include <functional>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

struct dPoint
{
	int x;
	int y;
};

struct PointFindByCoord : public std::binary_function<dPoint, dPoint, bool>
{
	bool operator () (const dPoint &obj1, const dPoint &obj2) const
	{
		return obj1.x == obj2.x && obj1.y == obj2.y;
	}
};

#define PAINT 1
#define RELEASE 2

class MyLabel :
	public QLabel
{
public:
	MyLabel(QWidget *parent);
	~MyLabel();

	int canPaint = 0;
	

private:
	void paintEvent(QPaintEvent *event);//绘图操作
	void mousePressEvent(QMouseEvent *event);//鼠标按下
	void mouseMoveEvent(QMouseEvent *event);//鼠标移动
	void mouseReleaseEvent(QMouseEvent *event);//鼠标释放

	

	QPoint a;//当前鼠标点
	QVector<QPoint> edgeArea;//提取边缘区域
	int option = 0;

};

