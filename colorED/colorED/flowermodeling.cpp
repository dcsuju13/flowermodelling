#include "flowermodeling.h"

flowermodeling::flowermodeling(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->image = new QImage();
	this->template_p = PetalTemplate();
	//this->scene = new QGraphicsScene();
	

	ui.Slider_brush->setMinimum(0);
	ui.Slider_brush->setMaximum(100);
	ui.Slider_brush->setValue(30);
	brushsize = 30;
	QString str = QString("%1").arg(brushsize);
	ui.Edit_brush->setText(str);

	connect(ui.Slider_brush, SIGNAL(valueChanged(int)), this, SLOT(SetBrushsize()));

}

flowermodeling::~flowermodeling()
{

}

void flowermodeling::on_Button_scan_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "open image file",
		".",
		"Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
	ImageName = fileName;
	if (fileName != "")
	{
		if (image->load(fileName))
		{
			g_filedst = fileName;
			ui.Edit_image->setText(fileName);
			QGraphicsScene *scene1 = new QGraphicsScene();
			scene1->addPixmap(QPixmap::fromImage(*image));
			scene = scene1;
			ui.graphics_image->setScene(scene);
			ui.graphics_image->show();	
		}
		else
		{
			QTextCodec *codec = QTextCodec::codecForName("GBK");
			QMessageBox::information(this,
				codec->toUnicode("打开图像失败"),
				codec->toUnicode("打开图像失败"));
			delete image;
			return;

		}
	}

	return;
}

void  flowermodeling::on_Button_center_clicked()
{
	QGraphicsScene *scene1 = new QGraphicsScene();
	scene1->addPixmap(QPixmap::fromImage(*image));
	MyPointItem *item = new MyPointItem();
	item->setOption(1);//花心选项
	
	scene1->addItem(item);
	scene = scene1;
	ui.graphics_image->setScene(scene);
	ui.graphics_image->show();
	
	return;
}

void flowermodeling::on_Button_paint_clicked()
{

	MyItem *item = new MyItem();
	item->ClearedgeArea();
	brush = NULL;
	QGraphicsScene *scene1 = new QGraphicsScene();
	//MyPointItem *item1 = new MyPointItem();
	//item1->setOption(2);
	scene1->addPixmap(QPixmap::fromImage(*image));
	//scene1->addItem(item1);//.....................
	scene1->addItem(item);
	
	scene = scene1;
	ui.graphics_image->setScene(scene);
	ui.graphics_image->show();

	brush = item;
	return;
}

void flowermodeling::on_Button_done_clicked()
{
	//得到提取边缘的区域
	QVector<QPointF> ea = brush->getEDArea();
	
	edgeArea = Mat(image->height(), image->width(), CV_8UC1, Scalar(0));

	for (int i = 0; i < ea.size(); i++)
	{	
		int x = ea[i].x();
		int y = ea[i].y();
		if (x >= image->width() || y >= image->height()||x<=0||y<=0)
			continue;
		if (edgeArea.at<uchar>(y, x)==0)
		    edgeArea.at<uchar>(y, x) = 255;
	}
	
	imwrite("edgearea.jpg", edgeArea);
	

	//keypoint
	f_o = MyPointItem::f_o;

	edgedetect = EdgeDetect(f_o);
	edgedetect.caculateEdge();//计算边缘

	//显示边缘图
	image->load("EdgeL.jpg");
	QGraphicsScene *scene1 = new QGraphicsScene();
	scene1->addPixmap(QPixmap::fromImage(*image));

	//显示顶点、花心
	MyPointItem *item = new MyPointItem();
	item->setTippoints(edgedetect.getKeypic());//设置顶点
	item->setOption(3);
	scene1->addItem(item);
	scene = scene1;
	ui.graphics_image->setScene(scene);
	ui.graphics_image->show();
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QMessageBox::information(this,
		codec->toUnicode("完成"),
		codec->toUnicode("边缘提取完成！"));

	return;
}

void flowermodeling::SetBrushsize()
{
	brushsize = ui.Slider_brush->value();
	QString str = QString("%1").arg(brushsize);
	ui.Edit_brush->setText(str);
	return;
}


void flowermodeling::on_Button_template_clicked()
{
	template_p.setF_o(f_o);
	template_p.setO(edgedetect.getO());
	template_p.setContour(edgedetect.getContour());
	template_p.setAngle(edgedetect.getAngle());
	template_p.setHeight(edgedetect.getHeight());
	template_p.setRadius(edgedetect.getRadius());
	template_p.setKeypoints(edgedetect.getKeypoints());

	template_p.coneTemplate();
	template_p.replaceTemplate();
	vector<coor> petaltemplate = template_p.getPetalTemplate();
	vector<vector<coor>> alltemplate = template_p.getAllTemplate();

	vector<coor> p;
	for (int i = 0; i < alltemplate.size(); i++)
		p.insert(p.end(), alltemplate[i].begin(), alltemplate[i].end());
	MyGL *gl = new MyGL(p);
	gl->setoption(1);
	ui.scrollArea_cone->setWidget(gl);
	return;
}

void flowermodeling::on_Button_surface_clicked()
{
	template_p.coneFitting();
	vector<vector<coor>> alltemplate = template_p.getAllTemplate();

	vector<coor> p;
	//p = template_p.getContour();
	for (int i = 0; i < alltemplate.size(); i++)
		p.insert(p.end(), alltemplate[i].begin(), alltemplate[i].end());
	MyGL *gl = new MyGL(p);
	gl->setoption(1);
	ui.scrollArea_cone->setWidget(gl);
	return;
}

void flowermodeling::on_Button_mesh_clicked()
{
	vector<vector<coor>> alltemplate = template_p.getAllTemplate();
	vector<petal> contourTag = template_p.getContourTag();
	this->meshfit = MeshFitting(alltemplate,contourTag);

	MyGL *gl = new MyGL();
	MyMesh mesh = meshfit.getAllmesh();
	gl->setMesh(mesh);
	gl->setoption(2);
	ui.scrollArea_mesh->setWidget(gl);

	return;
}

void flowermodeling::on_Button_meshfitting_clicked()
{
	double h = template_p.getMinh();
	meshfit.setMinh(h);
	MyMesh mesh = meshfit.getMvmesh();
	MyGL *gl = new MyGL();
	gl->setMesh(mesh);
	gl->setoption(2);
	ui.scrollArea_mesh->setWidget(gl);
	QString str = "mesh fitting finished!\nsaved as all.ply";
	ui.label_state->setText(str);
	return;
}

void flowermodeling::on_Button_Occ_clicked()
{
	meshfit.setKeypoints(edgedetect.getKeypoints());
	MyMesh mesh = meshfit.getOccmesh();
	MyGL *gl = new MyGL();
	gl->setMesh(mesh);
	gl->setoption(2);
	ui.scrollArea_mesh->setWidget(gl);
	QString str = "mesh fitting finished!\nsaved as all.ply";
	ui.label_state->setText(str);

	return;
}


void flowermodeling::on_Button_mesh1_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "open mesh file",
		".",
		"Mesh files (*.ply *.off );;All files (*.*)");

	if (fileName != "")
	{
		mesh1name = fileName.toStdString();
		ui.Edit_mesh1->setText(fileName);

	}
	else
	{
		QTextCodec *codec = QTextCodec::codecForName("GBK");
		QMessageBox::information(this,
			codec->toUnicode("选择文件失败"),
			codec->toUnicode("选择文件失败"));
		return;

	}
	return;
}

void flowermodeling::on_Button_mesh2_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(
		this, "open mesh file",
		".",
		"Mesh files (*.ply *.off );;All files (*.*)");

	if (fileName != "")
	{
		mesh2name = fileName.toStdString();
		ui.Edit_mesh2->setText(fileName);

	}
	else
	{
		QTextCodec *codec = QTextCodec::codecForName("GBK");
		QMessageBox::information(this,
			codec->toUnicode("选择文件失败"),
			codec->toUnicode("选择文件失败"));
		return;

	}
	return;
}


void flowermodeling::on_Button_mult_clicked()
{

	if (mesh1name.empty() || mesh2name.empty())
	{
		QTextCodec *codec = QTextCodec::codecForName("GBK");
		QMessageBox::information(this,
			codec->toUnicode("未选择网格文件"),
			codec->toUnicode("未选择网格文件"));
		return;

	}

	MyMesh mesh1,mesh2;
	bool result;
	result = OpenMesh::IO::read_mesh(mesh1, mesh1name);
	if (!result)
		return;
	result = OpenMesh::IO::read_mesh(mesh2, mesh2name);
	if (!result)
		return;
	mult = MultLayer(mesh1,mesh2);
	mesh1 = mult.getMesh1();

	MyGL *gl = new MyGL();
	gl->setMesh(mesh1);
	gl->setoption(2);
	ui.scrollArea_mult->setWidget(gl);
	QString str = "mesh combine finished! saved as mult.ply";
	ui.label_mult->setText(str);





	return;
}