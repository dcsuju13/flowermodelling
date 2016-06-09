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
	//test,���ļ���ȡ������Ϣ
	void loadData();

	void surfaceFit1();
	void surfaceFit2();
	void updateTemplate();
	

	double calangle(double a[2],double b[2]);//�����a��ת��b�ĽǶ�
	coor rotate(coor src, double angle,coor direction);//src��direction��תangle

	vector<coor> contour;//����������
	vector<petal> contourTag;//�洢����ԭʼ����������˳��Ͱ���һ��
	vector<coorTag> keypoints;//����Ӱ���
	vector<coor> tippoints;//����
	vector<coor> vallypoints;//����
	vector<coor> petal_template;//ģ������
	vector<vector<coor>> all_template;//��������ģ������

	//Բ׶�뾶�͸߶�
	int radius;
	int height;
	QPointF f_o;
	QPointF o;
	double angle;//Բ׶��б��

	double minvallyh;//��Ͱ���߶�

	vector<double> rset;//�뾶��
	vector<double> lastrset;

	int flag_keypoint = 0;//��ǵ�һ��keypoint�ǰ��㻹�Ƕ���
};

