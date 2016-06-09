#pragma once
#include <QGLWidget> 
#include <QVector>
#include <QPoint>


#include <iostream>
#include <string>
#include <vector>
#include "MyGL.h"
#include <QMouseEvent>
#include<QWheelEvent>  
#include <QColorDialog>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "global.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
// ----------------------------------------------------------------------------
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;


using namespace cv;
using namespace std;


class MyGL :
	public QGLWidget
{
public:
	MyGL();
	MyGL(vector<coor> contour);
	~MyGL();

	void setMesh(MyMesh mesh);//设置网格信息
	void setoption(int op);
private:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *e);

	void drawPetal();//绘制花瓣轮廓
	void drawMesh();//绘制花瓣网格

	vector<coor> contour_p;//花瓣轮廓
	
	MyMesh petal_mesh;//网格

	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	GLfloat translationX;
	GLfloat translationY;
	GLfloat translationZ;
	QPoint lastPos;

	int option = 0;//操作选项

	


};

