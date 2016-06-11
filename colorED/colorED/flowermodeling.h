#ifndef flowermodeling_H
#define flowermodeling_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsView>  
#include <QGraphicsScene> 
#include <QGraphicsSceneMouseEvent>  
#include <QImage>  
#include <QMessageBox>
#include <QTextCodec>
#include <vector>
#include "global.h"
#include "MyGL.h"
#include <QFileDialog> 
#include "MyItem.h"
#include "MyPointItem.h"
#include "EdgeDetect.h"
#include "PetalTemplate.h"
#include "MeshFitting.h"
#include "MultLayer.h"
#include "Texture.h"
#include "ui_flowermodeling.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

class flowermodeling : public QMainWindow
{
	Q_OBJECT

public:
	flowermodeling(QWidget *parent = 0);
	~flowermodeling();

private slots:
	void on_Button_scan_clicked();
	void on_Button_center_clicked();
	void on_Button_paint_clicked();
	void on_Button_done_clicked();
	void SetBrushsize();//���ñ�ˢ��С
	//void on_Button_keypoint_clicked();
	void on_Button_template_clicked();
	void on_Button_surface_clicked();
	void on_Button_mesh_clicked();
	void on_Button_meshfitting_clicked();
	void on_Button_mesh1_clicked();
	void on_Button_mesh2_clicked();
	void on_Button_mult_clicked();
	void on_Button_Occ_clicked();
	void on_Button_Texture_clicked();

private:
	Ui::flowermodelingClass ui;
	QImage *image;//ͼƬ
	QString ImageName;//ͼƬ����
	string mesh1name;
	string mesh2name;
	QGraphicsScene *scene;
	Mat edgeArea;
	MyItem *brush;
	QPointF f_o;//����

	EdgeDetect edgedetect;//edgedetect����ʵ��
	PetalTemplate template_p;//comefitting����ʵ��
	MeshFitting meshfit;//meshfitting����ʵ��
	MultLayer mult;//�ϲ���㲿��ʵ��
	Texture tex;//������ʵ��

	MyMesh mesh;
};

#endif // flowermodeling_H
