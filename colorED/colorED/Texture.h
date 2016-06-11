#pragma once
#include <QPoint>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cv.h>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <direct.h>

#include "global.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	void set_center(QPointF p);//设定花心
	void set_keypoints(vector<Vec3i> keypoints);//设定顶点和凹点
	void set_contour(vector<Vec2i> points);//设定花瓣轮廓
	void set_div(int a);//设定分段数

	void Docaculate();//求解映射点

	vector<QPoint> get_match();//获得映射点


private:
	QPoint center;//花心
	QPoint tip;//顶点
	QPoint valley[2];//两个凹点，分别为左右凹点
	vector<Vec2i> contour;//第一个花瓣轮廓
	int div;//网格所分层数
	vector<QPoint> match;//映射点

};

