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

	setGeometry(300, 150, 800, 600);//���ô��ڳ�ʼλ�úʹ�С
	


	//qglClearColor(Qt::black);
	glClearColor(0.0, 0.0, 0.0, 0);
	//glShadeModel(GL_FLAT);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0);//������Ȼ���
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);//������Ȳ�������
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//����͸��У��


}

void MyGL::resizeGL(int width, int height)
{
	GLfloat nrange = 400.0f;
	if (0 == height)
		height = 1;//��ֹһ����Ϊ0
	glViewport(0, 0, (GLint)width, (GLint)height);//���õ�ǰ�ӿڣ����������ô��ڵģ�ֻ���������ﱻQt����װ����
	glMatrixMode(GL_PROJECTION);//ѡ��ͶӰ����
	glLoadIdentity();//����ѡ��õ�ͶӰ����
	//�ü�����
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

	//����һ����������
	glGenTextures(1, &texture[0]);
	//���������ְ󶨵�Ŀ����
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	//��������
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

	gheight = tex.height();//ͼƬ�߶�
	gwidth = tex.width();//ͼƬ���

	//������ʾͼ��ʱ�ķŴ���С���˲���ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MyGL::paintGL()
{
	

	if (option == 1)//����ӳ��
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


	//����

	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(5);
	//�������е�
	for (int i = 0; i < contour_p.size(); i++)
	{
		glBegin(GL_POINTS);
		glVertex3f(contour_p[i].x, contour_p[i].y, contour_p[i].z);
		glEnd;
	}

	glPointSize(1);

	//ʹ���ܹ���ת
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


	//����

	glColor3f(1.0f, 1.0f, 1.0f);
	//�������е�
	
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

	//ʹ���ܹ���ת
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);


	//����
	if (!match_tex.empty())//������
	{

		//��������
		loadGLTexture();
		//��������
		glEnable(GL_TEXTURE_2D);
		//����ӳ��
		for (int i = 0; i < face.size(); i++)
		{
			int a, b, c;
			//�õ���Ӧvhandles������
			a = face[i].x ;
			b = face[i].y;
			c = face[i].z ;

			/* ����������������弸������ */
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


	//��
	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it){
		auto point = mesh.point(it.handle());
		coor t = { point.data()[0], point.data()[1], point.data()[2] };

		points.push_back(t);

	}

	//��
	face.clear();

	for (auto it = mesh.faces_begin(); it != mesh.faces_end(); ++it){
		OpenMesh::FaceHandle fh = it.handle();

		coor facet;//��ǰ������
		int count = 0;//��ǵڼ�����
		for (auto it1 = mesh.fv_begin(fh); it1 != mesh.fv_end(fh); ++it1){
			auto point = mesh.point(it1.handle());
			t = { point.data()[0], point.data()[1], point.data()[2] };

			//find
			vector<coor>::iterator itv;//��¼�õ��ӦPoints�е�λ��
			itv = find_if(points.begin(), points.end(), [](coor const& obj){
				return (fabs(obj.x - t.x) < 0.01) && (fabs(obj.y - t.y) < 0.01) && (fabs(obj.z - t.z) < 0.01);
			});

			if (itv != points.end())//д����facet
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

