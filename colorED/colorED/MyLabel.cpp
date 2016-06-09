#include "MyLabel.h"


MyLabel::MyLabel(QWidget *parent)
{
}


MyLabel::~MyLabel()
{
}


void MyLabel::paintEvent(QPaintEvent *event)
{
	QLabel::paintEvent(event);//�ȵ��ø����paintEventΪ����ʾ'����'!!!
	QPainter painter(this);

	static vector<dPoint> flag;

	QBrush brush(QColor(255, 255, 255, 150), Qt::SolidPattern); // ��ˢ
	painter.setBrush(brush);
	painter.setPen(QPen(QColor(255, 255, 255, 150)));

	if (option == PAINT)
	{
		painter.drawEllipse(a.x() - 20, a.y() - 20, 40, 40);//���Ʊ�ˢ
	}

	if (option == PAINT || option == RELEASE)
	{
		//��ˢ������ĵ�д��edgeArea
		dPoint t;
		t.x = a.x();
		t.y = a.y();
		if (find_if(flag.begin(), flag.end(), std::bind2nd(PointFindByCoord(), t)) == flag.end())//�����ظ������
		{
			edgeArea.push_back(a);
			flag.push_back(t);
		}

		
		for (int i = a.x() - 20; i <= a.x() + 20; i++)
		{
			int low = a.y() - sqrtf(400 - (i - a.x())*(i - a.x()));
			int high = a.y() + sqrtf(400 - (i - a.x())*(i - a.x()));
			
			for (int j = low; j <= high; j++)
			{
				QPoint t1(i, j);
				dPoint t;
				t.x = t1.x();
				t.y = t1.y();
				if (find_if(flag.begin(), flag.end(), std::bind2nd(PointFindByCoord(), t)) == flag.end())
				{
					edgeArea.push_back(t1);
					flag.push_back(t);
				}

			}
		}
		
		
		//for (int i = 0; i < edgeArea.size();i++)
		//painter.drawEllipse(edgeArea[i].x() - 20, edgeArea[i].y() - 20, 40.0, 40.0);
		//painter.drawEllipse(a.x()-20,a.y()-20,40,40);//���Ʊ�ˢ
		painter.drawPoints(edgeArea);//��������
	}
		

	
	     
	
	return;
}


void MyLabel::mousePressEvent(QMouseEvent *event)
{
	if (canPaint > 0)
	{
		a = event->pos();
		option = PAINT;
		update();
		//repaint();
	}
	
	
	
	return;
}

void MyLabel::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		a = event->pos();
		update();
		//repaint();
	}
	return;
}

void MyLabel::mouseReleaseEvent(QMouseEvent *event)
{
	if ((event->button() == Qt::LeftButton) && (option == PAINT))
	{
		a = event->pos();
		option = RELEASE;
		update();
	}
		
	
	return;
}
