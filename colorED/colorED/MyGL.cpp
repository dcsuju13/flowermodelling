#include "MyGL.h"


MyGL::MyGL()
{
	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	translationX = 0;
	translationY = 0;
	translationZ = 0;

}

MyGL::MyGL(vector<coor> contour)
{
	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	translationX = 0;
	translationY = 0;
	translationZ = 0;

	contour_p = contour;
	
}


MyGL::~MyGL()
{
}


void MyGL::setoption(int op)
{
	option = op;
}

void MyGL::initializeGL()
{

	setGeometry(300, 150, 800, 600);//设置窗口初始位置和大小

	//qglClearColor(Qt::black);
	glClearColor(0.0, 0.0, 0.0, 0);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);//设置深度缓存
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);//设置深度测试类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//进行透视校正


}

void MyGL::resizeGL(int width, int height)
{
	GLfloat nrange = 400.0f;
	if (0 == height)
		height = 1;//防止一条边为0
	glViewport(0, 0, (GLint)width, (GLint)height);//重置当前视口，本身不是重置窗口的，只不过是这里被Qt给封装好了
	glMatrixMode(GL_PROJECTION);//选择投影矩阵
	glLoadIdentity();//重置选择好的投影矩阵
	//裁剪区域
	/*if (width <= height)
	glOrtho(-nrange, nrange, -nrange*height / width, nrange*height / width, -nrange, nrange);
	else
	glOrtho(-nrange*width / height, nrange*width / height, -nrange, nrange, -nrange, nrange);
	*/

	gluPerspective(70.0f, (GLfloat)width / (GLfloat)height, 0, 2 * nrange);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MyGL::paintGL()
{
	

	if (option == 1)//花瓣映射
	{
		drawPetal();
	}
	else if (option == 2)
	{
		drawMesh();
	}
	

	glFlush();
}

void MyGL::drawPetal()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -600);
	glTranslatef(translationX, translationY, translationZ);

	glRotatef(rotationX, 1.0, 0.0, 0.0);
	glRotatef(rotationY, 0.0, 1.0, 0.0);
	glRotatef(rotationZ, 0.0, 0.0, 1.0);


	//画点

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5);
	//打上所有点
	for (int i = 0; i < contour_p.size(); i++)
	{
		glBegin(GL_POINTS);
		glVertex3f(contour_p[i].x, contour_p[i].y, contour_p[i].z);
		glEnd;
	}

	glPointSize(1);

	//使得能够旋转
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();

	return;
}

void MyGL::drawMesh()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0, 0, -600);
	glTranslatef(translationX, translationY, translationZ);

	glRotatef(rotationX, 1.0, 0.0, 0.0);
	glRotatef(rotationY, 0.0, 1.0, 0.0);
	glRotatef(rotationZ, 0.0, 0.0, 1.0);


	//画点

	glColor3f(1.0f, 1.0f, 1.0f);
	//打上所有点
	/*
	for (int i = 0; i < trip.size(); i++)
	{
		glBegin(GL_LINES);
		glVertex3f(meshp.at(trip.at(i).x).x, meshp.at(trip.at(i).x).y, meshp.at(trip.at(i).x).z);
		glVertex3f(meshp.at(trip.at(i).y).x, meshp.at(trip.at(i).y).y, meshp.at(trip.at(i).y).z);
		glEnd;

		glBegin(GL_LINES);
		glVertex3f(meshp.at(trip.at(i).x).x, meshp.at(trip.at(i).x).y, meshp.at(trip.at(i).x).z);
		glVertex3f(meshp.at(trip.at(i).z).x, meshp.at(trip.at(i).z).y, meshp.at(trip.at(i).z).z);
		glEnd;

		glBegin(GL_LINES);
		glVertex3f(meshp.at(trip.at(i).y).x, meshp.at(trip.at(i).y).y, meshp.at(trip.at(i).y).z);
		glVertex3f(meshp.at(trip.at(i).z).x, meshp.at(trip.at(i).z).y, meshp.at(trip.at(i).z).z);
		glEnd;
	}
	*/
	for (auto it = petal_mesh.halfedges_begin(); it != petal_mesh.halfedges_end(); ++it)
	{
		auto fromv = petal_mesh.from_vertex_handle(it.handle());

		auto tov = petal_mesh.to_vertex_handle(it.handle());

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3f((GLfloat)petal_mesh.point(fromv).data()[0], (GLfloat)petal_mesh.point(fromv).data()[1], (GLfloat)petal_mesh.point(fromv).data()[2]);
		glVertex3f((GLfloat)petal_mesh.point(tov).data()[0], (GLfloat)petal_mesh.point(tov).data()[1], (GLfloat)petal_mesh.point(tov).data()[2]);
		glEnd();

	}

	glPointSize(1);

	//使得能够旋转
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();

	return;
}

void MyGL::setMesh(MyMesh mesh)
{
	
	petal_mesh = mesh;

	return;
}


void MyGL::mousePressEvent(QMouseEvent *event)
{

	lastPos = event->pos();
}


void MyGL::mouseMoveEvent(QMouseEvent *event)
{
	GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
	GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
	if (event->buttons() & Qt::LeftButton){
		rotationX += 180 * dy;
		rotationY += 180 * dx;
		updateGL();
	}
	else if (event->buttons() & Qt::RightButton){
		rotationX += 180 * dy;
		rotationZ += 180 * dx;
		updateGL();
	}
	lastPos = event->pos();

}

void MyGL::wheelEvent(QWheelEvent *e)
{
	if (e->buttons()&Qt::MiddleButton)
	{
		int numDegrees = e->delta()/8;

		if (numDegrees>0)
		{
			translationZ -= 5*numDegrees;
			updateGL();
		}
		else
		{
			translationZ += 5*numDegrees;
			updateGL();
		}
	}
	return;
}

