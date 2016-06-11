#pragma once
#include <QPoint>
#include <vector>
#include "MymeshFit.h"
#include "matrix.h"
#include "global.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;


// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
// ----------------------------------------------------------------------------
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

using namespace std;

class MeshFitting
{
public:
	MeshFitting();
	MeshFitting(vector<vector<coor>> all, vector<petal> contour);
	~MeshFitting();

	void setMinh(double a);
	void setKeypoints(vector<coorTag> k);
	MyMesh getAllmesh();//���ԭʼ����������
	MyMesh getMvmesh();//��ñ��κ�����������
	MyMesh getOccmesh();//����ڵ����κ�����������
	int getDiv();//��ò�����ֵĲ���
private:
	void triMeshAll();//��������
	void buildMesh(vector<coor> sample,vector<coor> tri);//��������

	//�������
	int meshFit();//�������
	vector<vector<coor>> calnewsample(vector<int> &H,vector<coor> &u);//����µĲ�����

	vector<coor> RotateMeshPoint(vector<coor> AllSample, vector<coorTag> KeyPoint);//�����ڵ�
	coor PointRotate(Vec3f RotateZ, Vec3f RotateP, float angle);//��ת��

	vector<vector<coor>> all_sample;//ģ�����������
	vector<coor> allsamplepoints;//���в�����˳��洢
	vector<coor> triall;//���л������������
	vector<vector<coor>> all_template;//ģ������
	vector<petal> contourTag;//�洢����ԭʼ����������˳��Ͱ���һ��

	vector<coorTag> keypoints;
	MyMesh all_mesh;//����������

	double minvallayh;//��Ͱ���߶�
};

