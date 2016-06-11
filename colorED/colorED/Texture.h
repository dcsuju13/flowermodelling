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

	void set_center(QPointF p);//�趨����
	void set_keypoints(vector<Vec3i> keypoints);//�趨����Ͱ���
	void set_contour(vector<Vec2i> points);//�趨��������
	void set_div(int a);//�趨�ֶ���

	void Docaculate();//���ӳ���

	vector<QPoint> get_match();//���ӳ���


private:
	QPoint center;//����
	QPoint tip;//����
	QPoint valley[2];//�������㣬�ֱ�Ϊ���Ұ���
	vector<Vec2i> contour;//��һ����������
	int div;//�������ֲ���
	vector<QPoint> match;//ӳ���

};

