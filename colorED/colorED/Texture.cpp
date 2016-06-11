#include "Texture.h"


Texture::Texture()
{
}


Texture::~Texture()
{
}

void Texture::set_center(QPointF p)
{
	center = {(int)p.y(),(int)p.x()};
	return;
}

void Texture::set_keypoints(vector<Vec3i> keypoints)
{
	//将顶点和凹点分离
	if (keypoints[0](2) == 1)//第一个为顶点
	{
		tip = QPoint(keypoints[0](0),keypoints[0](1));//顶点
		valley[0] = QPoint(keypoints.back()(0),keypoints.back()(1));//最后一个点为左凹点
		valley[1] = QPoint(keypoints[1](0), keypoints[1](1));//第二点为右凹点
	}
	else//第一个点为凹点
	{
		tip = QPoint(keypoints.back()(0), keypoints.back()(1));//最后一个点为顶点
		valley[0] = QPoint(keypoints[keypoints.size() - 2](0), keypoints[keypoints.size() - 2](1));//最后第二个点为左凹点
		valley[1] = QPoint(keypoints[0](0), keypoints[0](1));//第一个点为右凹点
	}

	return;
}

void Texture::set_contour(vector<Vec2i> points)
{
	contour.clear();
	//第一个花瓣左边部分
	vector<Vec2i>  left;
	for (int i = points.size() - 1; i >= 0; i--)
	{
		left.push_back(points[i]);
		if ((points[i](0) == valley[0].x()) && (points[i](1) == valley[0].y()))//到达左边凹点
			break;
	}
	reverse(left.begin(),left.end());//翻转
	//第一个花瓣右边部分
	vector<Vec2i> right;
	for (int i = 0; i < points.size(); i++)
	{
		right.push_back(points[i]);
		if ((points[i](0) == valley[1].x()) && (points[i](1) == valley[1].y()))//到达右边凹点
			break;
	}
	contour.insert(contour.begin(),left.begin(),left.end());
	contour.insert(contour.end(), right.begin(), right.end());

	return;
}

void Texture::set_div(int a)
{
	div = a;
	return;
}


void Texture::Docaculate()
{
	match.clear();
	match.push_back(tip);//顶点记下

	//在轮廓中找到顶点位置
	int i_tip = -1;;
	for (int i = 0; i < contour.size();i++)
		if (contour[i](0) == tip.x()&&contour[i](1) == tip.y())
		{
			i_tip = i;
			break;
		}

	if (i_tip < 0)  return;//错误

	int lastleft = i_tip;
	int lastright = i_tip;

	int len = sqrtf(pow(tip.x()-center.x(),2)+pow(tip.y()-center.y(),2));//顶点和花心距离
	//求解每个梯度对应的轮廓点
	for (int i = 0; i < div; i++)
	{
		QPoint leftp,midp,rightp;
		int dist = len*(( div - i) / (div+1.0));

		//左边
		int j;
		for (j = lastleft; j >= 0; j--)
		{
			int t = sqrtf(pow(contour[j](0) - center.x(), 2) + pow(contour[j](1) - center.y(), 2));//点和花心距离
			if ((t-dist) < 5||(t<dist))
			{
				leftp = QPoint(contour[j](0),contour[j](1));
				lastleft = j;
				break;
			}
		}
		if (j < 0)//左边点bu在轮廓范围内
		{
			Vec2i v = {valley[0].x()-center.x(),valley[0].y()-center.y()};//花心到左边凹点向量
			int distv = sqrtf(pow(v(0), 2) + pow(v(1), 2));//向量长度
			float ratio = (1.0*dist) / distv;//应该缩短比例
			v = {(int)(v(0)*ratio),(int)(v(1)*ratio)};
			leftp = QPoint(center.x()+v(0),center.y()+v(1));//左边点
			lastleft = 0;
		}

		//右边
		for (j = lastright; j <contour.size(); j++)
		{
			int t = sqrtf(pow(contour[j](0) - center.x(), 2) + pow(contour[j](1) - center.y(), 2));//点和花心距离
			if ((t - dist) < 5 || (t<dist))
			{
				rightp = QPoint(contour[j](0), contour[j](1));
				lastright = j;
				break;
			}
		}
		if (j >=contour.size())//右边点bu在轮廓范围内
		{
			Vec2i v = { valley[1].x() - center.x(), valley[1].y() - center.y() };//花心到右边凹点向量
			int distv = sqrtf(pow(v(0), 2) + pow(v(1), 2));//向量长度
			float ratio = (1.0*dist) / distv;//应该缩短比例
			v = { (int)(v(0)*ratio), (int)(v(1)*ratio) };
			rightp = QPoint(center.x() + v(0), center.y() + v(1));//右边点
			lastright = contour.size();
		}

		//中间点
		midp = QPoint((leftp.x()+rightp.x())/2,(leftp.y()+rightp.y())/2);

		match.push_back(leftp);
		match.push_back(midp);
		match.push_back(rightp);
		
	}

	match.push_back(center);//花心

	//映射坐标写入文件
	ofstream file("file\\texture.txt");
	for (int k = 0; k<match.size(); k++)
	{
		file << match[k].x() << " " << match[k].y()<< endl;
	}
	file.close();

	string cwd = getcwd(NULL,0);

	//string s = "cp " + g_filedst.toStdString()+" "+cwd+"\file\tex.jpg ";
	//system(s.c_str());
	
	

	return;
}

vector<QPoint> Texture::get_match()
{
	return match;
}