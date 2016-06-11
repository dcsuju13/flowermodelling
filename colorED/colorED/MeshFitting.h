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
	MyMesh getAllmesh();//获得原始完整花网格
	MyMesh getMvmesh();//获得变形后完整花轮廓
	MyMesh getOccmesh();//获得遮挡变形后完整花轮廓
	int getDiv();//获得采样点分的层数
private:
	void triMeshAll();//生成索引
	void buildMesh(vector<coor> sample,vector<coor> tri);//构造网格

	//网格变形
	int meshFit();//网格变形
	vector<vector<coor>> calnewsample(vector<int> &H,vector<coor> &u);//求解新的采样点

	vector<coor> RotateMeshPoint(vector<coor> AllSample, vector<coorTag> KeyPoint);//处理遮挡
	coor PointRotate(Vec3f RotateZ, Vec3f RotateP, float angle);//旋转点

	vector<vector<coor>> all_sample;//模板网格采样点
	vector<coor> allsamplepoints;//所有采样点顺序存储
	vector<coor> triall;//所有花瓣网格的索引
	vector<vector<coor>> all_template;//模板轮廓
	vector<petal> contourTag;//存储各个原始花瓣轮廓，顺序和凹点一致

	vector<coorTag> keypoints;
	MyMesh all_mesh;//完整花网格

	double minvallayh;//最低凹点高度
};

