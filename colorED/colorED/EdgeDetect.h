#pragma once
#include <QPoint>
#include <vector>
#include <cmath>
#include <cv.h>
#include "global.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

#define PI 3.1415926

class EdgeDetect
{
public:
	EdgeDetect();
	EdgeDetect(QPointF fcenter);
	~EdgeDetect();

	void caculateEdge();//计算边缘
	QPointF getF_o();
	QPointF getO();
	vector<coor> getContour();
	vector<coorTag> getKeypoints();
	vector<QPointF> getKeypic();
	int getRadius();
	int getHeight();
	double getAngle();
private:
	Mat GradientCaculate(Mat GaussL, Mat ResultL, Mat Mask, int a);
	vector<Vec2i> Walk(Vec3i StartP, int direct, Mat ResultL, Mat EdgeL, vector<Vec2i> theedge);//走出边缘的函数，其中，direct数字对应小键盘方向 
	Mat WalkEnd(Mat EdgeL, Mat ResultL);
	vector<Vec2i> ConnectNode(int type, vector<Vec2i> TargrtChain, vector<Vec2i> EdgeChain, Mat EdgeL);
	vector<Vec2i> CEDContours(Mat ResultL);
	vector<Vec3i> GetKeyPoint(vector<Vec2i> EdgeChain, Vec2i CircleCentre); //分析花瓣顶点和底部的函数
	vector<Vec3i> govalley(Mat EdgeL, vector<Vec3i> KeyChain, vector<Vec2i> EdgeChain, Vec2i CircleCentre);
	Vec3f PointRotate(Vec3f RotateZ, Vec3f RotateP, float angle);

	

	QPointF f_o;
	QPointF o;
	vector<coor> contour;//轮廓点在圆锥坐标
	vector<coorTag> keypoints;//顶点+凹点在圆锥坐标
	vector<QPointF> keypicture;//顶点凹点在图上坐标

	//圆锥半径和高度
	int radius;
	int height;
	double angle;
};

