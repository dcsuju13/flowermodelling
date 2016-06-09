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

	void caculateEdge();//�����Ե
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
	vector<Vec2i> Walk(Vec3i StartP, int direct, Mat ResultL, Mat EdgeL, vector<Vec2i> theedge);//�߳���Ե�ĺ��������У�direct���ֶ�ӦС���̷��� 
	Mat WalkEnd(Mat EdgeL, Mat ResultL);
	vector<Vec2i> ConnectNode(int type, vector<Vec2i> TargrtChain, vector<Vec2i> EdgeChain, Mat EdgeL);
	vector<Vec2i> CEDContours(Mat ResultL);
	vector<Vec3i> GetKeyPoint(vector<Vec2i> EdgeChain, Vec2i CircleCentre); //�������궥��͵ײ��ĺ���
	vector<Vec3i> govalley(Mat EdgeL, vector<Vec3i> KeyChain, vector<Vec2i> EdgeChain, Vec2i CircleCentre);
	Vec3f PointRotate(Vec3f RotateZ, Vec3f RotateP, float angle);

	

	QPointF f_o;
	QPointF o;
	vector<coor> contour;//��������Բ׶����
	vector<coorTag> keypoints;//����+������Բ׶����
	vector<QPointF> keypicture;//���㰼����ͼ������

	//Բ׶�뾶�͸߶�
	int radius;
	int height;
	double angle;
};
