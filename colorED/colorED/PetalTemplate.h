#pragma once
#include <QPoint>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include "global.h"
#include "FittingCurve.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

using namespace std;

#define PI 3.1415926




class PetalTemplate
{
public:
	PetalTemplate();
	~PetalTemplate();
	void coneTemplate();
	void replaceTemplate();
	void coneFitting();
	
	vector<coor> getPetalTemplate();
	vector<vector<coor>> getAllTemplate();

	//test
	void setF_o(QPointF f);
	void setO(QPointF p);
	void setContour(vector<coor> v);
	void setKeypoints(vector<coorTag> v);
	void setRadius(int r);
	void setHeight(int h);
	void setAngle(double a);

	vector<coor> getContour();
	vector<petal> getContourTag();
	double getMinh();
private:
	//test,从文件读取顶点信息
	void loadData();

	void surfaceFit1();
	void surfaceFit2();
	void updateTemplate();
	

	double calangle(double a[2],double b[2]);//计算聪a旋转到b的角度
	coor rotate(coor src, double angle,coor direction);//src绕direction旋转angle

	vector<coor> contour;//完整花轮廓
	vector<petal> contourTag;//存储各个原始花瓣轮廓，顺序和凹点一致
	vector<coorTag> keypoints;//顶点加凹点
	vector<coor> tippoints;//顶点
	vector<coor> vallypoints;//凹点
	vector<coor> petal_template;//模板轮廓
	vector<vector<coor>> all_template;//完整花的模板轮廓

	//圆锥半径和高度
	int radius;
	int height;
	QPointF f_o;
	QPointF o;
	double angle;//圆锥倾斜角

	double minvallyh;//最低凹点高度

	vector<double> rset;//半径组
	vector<double> lastrset;

	int flag_keypoint = 0;//标记第一个keypoint是凹点还是顶点
};

