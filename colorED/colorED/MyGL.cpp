#include "MyGL.h"


MyGL::MyGL()
{
	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;
	translationX = 0;
	translationY = 0;
	translationZ = 0;
	match_tex = {};
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
	match_tex = {};
	
}


MyGL::~MyGL()
{
}


void MyGL::setoption(int op)
{
	option = op;
}

void MyGL::setMatchtex(vector<QPoint> v)
{
	match_tex = v;
	return;
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

void MyGL::loadGLTexture()
{
	QImage tex, buf;
	if (!buf.load(g_filedst)){
		QImage dummy(128, 128, QImage::Format_RGB32);
		dummy.fill(QColor(Qt::green).rgb());
		buf = dummy;
	}

	tex = QGLWidget::convertToGLFormat(buf);

	//创建一个纹理名字
	glGenTextures(1, &texture[0]);
	//将纹理名字绑定到目标上
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//创建纹理
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

	gheight = tex.height();//图片高度
	gwidth = tex.width();//图片宽度

	//设置显示图像时的放大、缩小的滤波方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
	glColor3f(1.0f, 1.0f, 1.0f);


	//纹理
	if (!match_tex.empty())//有纹理
	{

		//载入纹理
		loadGLTexture();
		//启用纹理
		glEnable(GL_TEXTURE_2D);
		//纹理映射
		for (int i = 0; i < face.size(); i++)
		{
			int a, b, c;
			//得到对应vhandles的索引
			a = face[i].x ;
			b = face[i].y;
			c = face[i].z ;

			/* 设置纹理坐标和物体几何坐标 */
			glBegin(GL_TRIANGLES);
			double t[2] = { match_tex[a%match_tex.size()].y() / (gwidth*1.0), (gheight - match_tex[a%match_tex.size()].x()) / (gheight*1.0) };
			glTexCoord2f(match_tex[a%match_tex.size()].y() / (gwidth*1.0), (gheight - match_tex[a%match_tex.size()].x()) / (gheight*1.0));
			glVertex3f(points[a].x, points[a].y, points[a].z);

			double t1[2] = { match_tex[b%match_tex.size()].y() / (gwidth*1.0), (gheight - match_tex[b%match_tex.size()].x()) / (gheight*1.0) };
			glTexCoord2f(match_tex[b%match_tex.size()].y() / (gwidth*1.0), (gheight - match_tex[b%match_tex.size()].x()) / (gheight*1.0));
			glVertex3f(points[b].x, points[b].y, points[b].z);

			double t2[2] = { match_tex[c%match_tex.size()].y() / (gwidth*1.0), (gheight - match_tex[c%match_tex.size()].x()) / (gheight*1.0) };
			glTexCoord2f(match_tex[c%match_tex.size()].y() / (gwidth*1.0), (gheight - match_tex[c%match_tex.size()].x()) / (gheight*1.0));
			glVertex3f(points[c].x, points[c].y, points[c].z);


			glEnd();


		}
	}

	return;
}

void MyGL::setMesh(MyMesh mesh)
{
	
	petal_mesh = mesh;

	points.clear();


	//点
	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it){
		auto point = mesh.point(it.handle());
		coor t = { point.data()[0], point.data()[1], point.data()[2] };

		points.push_back(t);

	}

	//面
	face.clear();

	for (auto it = mesh.faces_begin(); it != mesh.faces_end(); ++it){
		OpenMesh::FaceHandle fh = it.handle();

		coor facet;//当前面索引
		int count = 0;//标记第几个点
		for (auto it1 = mesh.fv_begin(fh); it1 != mesh.fv_end(fh); ++it1){
			auto point = mesh.point(it1.handle());
			t = { point.data()[0], point.data()[1], point.data()[2] };

			//find
			vector<coor>::iterator itv;//记录该点对应Points中的位置
			itv = find_if(points.begin(), points.end(), [](coor const& obj){
				return (fabs(obj.x - t.x) < 0.01) && (fabs(obj.y - t.y) < 0.01) && (fabs(obj.z - t.z) < 0.01);
			});

			if (itv != points.end())//写入面facet
			{
				if (count == 0){
					facet.x = itv - points.begin();
				}
				else if (count == 1){
					facet.y = itv - points.begin();
				}
				else
					facet.z = itv - points.begin();
			}
			else
				break;
			count++;


		}
		face.push_back(facet);

	}

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
	if (e->orientation() == Qt::Horizontal)
	{

	}
	else{
		GLfloat zValue = e->delta() / 8;
		translationZ -= zValue * 5;
		if (translationZ>1.0)
		{
			translationZ -= zValue * 5;

		}

	}
	updateGL();
	return;
}

