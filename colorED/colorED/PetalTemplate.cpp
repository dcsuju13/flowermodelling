#include "PetalTemplate.h"


PetalTemplate::PetalTemplate()
{
}


PetalTemplate::~PetalTemplate()
{
}


void PetalTemplate::setF_o(QPointF f)
{
	f_o = f;
	return;
}

void PetalTemplate::setO(QPointF p)
{
	o = p;
	return;
}

void PetalTemplate::setContour(vector<coor> v)
{
	contour = v;
	return;
}

void PetalTemplate::setKeypoints(vector<coorTag> v)
{
	keypoints = v;
	tippoints.clear();
	vallypoints.clear();
	for (int i = 0; i < keypoints.size(); i++)
	{
		if (keypoints[i].tag != 1)//凹点
		{
			vallypoints.push_back(keypoints[i].co);
			if (i == 0)
				flag_keypoint = 1;
		}
		else//==1顶点
		{
			tippoints.push_back(keypoints[i].co);
		}

	}

	height = 0;
	for (int i = 0; i < tippoints.size(); i++)//更新height
		if (height < tippoints[i].z)
			height = tippoints[i].z;

	//求解半径集合
	rset.clear();
	for (int i = 0; i <= height; i++)
	{
		rset.push_back(radius*i / (1.0*height));
	}
	lastrset = rset;
	return;
}

void PetalTemplate::setRadius(int r)
{
	radius = r;
	return;
}

void PetalTemplate::setHeight(int h)
{
	height = h;
	return;
}

void PetalTemplate::setAngle(double a)
{
	angle = a;
	return;
}

//--------------------------test-----------------------------------
void PetalTemplate::loadData()
{
	//轮廓点
	contour.clear();

	ifstream fin("rotatekeypoint.txt", std::ios::in);
	
	
	char line[1024] = { 0 };
	string x = "";
	string y = "";
	string z = "";
	//string n = "";
	while (fin.getline(line, sizeof(line)))//读取采样点坐标
	{
		stringstream word(line);
		word >> x;
		word >> y;
		word >> z;

		coor t;
		t.x = atof(x.c_str());
		t.y = atof(y.c_str());
		t.z = atof(z.c_str());

		contour.push_back(t);

	}
	fin.clear();
	fin.close();


	//顶点凹点
	tippoints.clear();
	vallypoints.clear();

	ifstream fin1("rotatetippoint.txt", std::ios::in);

	string flag;
	int count = 0;
	while (fin1.getline(line, sizeof(line)))//读取采样点坐标
	{
		count++;
		stringstream word(line);
		word >> x;
		word >> y;
		word >> z;
		word >> flag;//最后一个是标记顶点凹点,凹点是1，顶点是0
		int flag1 = atoi(flag.c_str());

		coor t;
		t.x = atof(x.c_str());
		t.y = atof(y.c_str());
		t.z = atof(z.c_str());

		if (flag1 == 1)//凹点
		{
			vallypoints.push_back(t);
			if (count == 1)
				flag_keypoint = 1;//第一个点为凹点

		}
		else
			tippoints.push_back(t);

	}
	fin.clear();
	fin.close();

	f_o = QPointF(330,240);
	o = QPointF(343,197);
	angle = 1.0999;

	radius = 153;
	height = 0;
	for (int i = 0; i < tippoints.size();i++)
		if (height < tippoints[i].z)
			height = tippoints[i].z; 

	rset.clear();
	for (int i = 0; i <= height; i++)
	{
		rset.push_back(radius*i / (1.0*height));
	}
	lastrset = rset;
	return;
}

void PetalTemplate::coneTemplate()
{
	petal_template.clear();
	//将花瓣轮廓分开标记存储
	int j = 0;
	int index_tip;//对应的顶点下标
	for (int i = 0; i < vallypoints.size(); i++)//凹点作为分隔
	{
		coor tip;
		if (flag_keypoint == 1&&i==0)//第一个顶点为凹点,则最后一个顶点为第一个花瓣顶点
		{
			tip = tippoints.back();
			index_tip = 0;//下一个花瓣顶点小标为0
		}
		else if (flag_keypoint == 0 && i == 0)
		{
			tip = tippoints.at(0);
			index_tip = 1;//下一个花瓣顶点小标为1
		}
		else
		{
			tip = tippoints.at(index_tip);
			index_tip++;
		}
			

		vector<coor> nowpetal;
		for (; j < contour.size(); j++)
		{
			nowpetal.push_back(contour[j]);
			if (fabs(contour[j].x - vallypoints[i].x) < 0.001&&fabs(contour[j].y - vallypoints[i].y) < 0.001
				&&fabs(contour[j].z - vallypoints[i].z) < 0.001)//凹点跳出，此时完成一个花瓣轮廓
			{
				j++;
				break;

			}
				
		}
		petal p = { tip, nowpetal };
		contourTag.push_back(p);//存储一个花瓣信息
	}
	//最后一个凹点之后的点仍为第一个花瓣的点，插入最前面
	vector<coor> nowpetal;
	for (; j < contour.size(); j++)
		nowpetal.push_back(contour[j]);
	contourTag[0].petal_contour.insert(contourTag[0].petal_contour.begin(),nowpetal.begin(),nowpetal.end());


	//对第一个花瓣拟合模板
	//pre：将该花瓣轮廓旋转至中轴和y轴正向重合
	double a[2] = {contourTag[0].tip.x,contourTag[0].tip.y};
	double b[2] = { 0, 1 };
	double angle = calangle(a,b);

	coor direction = {0,0,1};
	coor nowtip = rotate(contourTag[0].tip, angle, direction);//旋转顶点

	//vector<coor> nowpetal;
	nowpetal.clear();
	for (int i = 0; i < contourTag[0].petal_contour.size(); i++)
	{
		coor t = rotate(contourTag[0].petal_contour[i],angle,direction);
		nowpetal.push_back(t);
	}
	//1、分成左右轮廓（x,y）
	vector<coor> contour_l;
	vector<coor> contour_r;
	bool meettip = false;
	for (int i = 0; i < nowpetal.size(); i++)
	{
		if (fabs(nowpetal[i].x - nowtip.x) < 0.001&&fabs(nowpetal[i].y - nowtip.y) < 0.001
			&&fabs(nowpetal[i].z - nowtip.z) < 0.001)
		{
			meettip = true;
			contour_l.push_back(nowpetal[i]);
			contour_r.push_back(nowpetal[i]);
			continue;
		}
			
		if (meettip)//已经过了顶点
		{
			contour_r.push_back(nowpetal[i]);
		}
		else
			contour_l.push_back(nowpetal[i]); 

	}
	//下方没有轮廓部分生成
	//左边
	coor nearest;
	if (fabs(contour_l.back().x - nowtip.x) < 0.001&&fabs(contour_l.back().y - nowtip.y) < 0.001
		&&fabs(contour_l.back().z - nowtip.z) < 0.001)//最后一个点是否为顶点
		nearest = contour_l[0];
	else
		nearest = contour_l.back();
	float k = nearest.x / nearest.y;//斜率，根据y求解x
	for (int i = nearest.y - 1; i >= 0; i--)
	{
		coor t = {k*i,i,0};
		contour_l.push_back(t);
	}
	//右边
	if (fabs(contour_r.back().x - nowtip.x) < 0.001&&fabs(contour_r.back().y - nowtip.y) < 0.001
		&&fabs(contour_r.back().z - nowtip.z) < 0.001)//最后一个点是否为顶点
		nearest = contour_r[0];
	else
		nearest = contour_r.back();
	k = nearest.x / nearest.y;//斜率，根据y求解x
	for (int i = nearest.y - 1; i >= 0; i--)
	{
		coor t = { k*i, i, 0 };
		contour_r.push_back(t);
	}

	coor o = {0,0,0};
	contour_l.push_back(o);
	contour_r.push_back(o);
	//2、左右轮廓分别拟合曲线，重新生成轮廓

	//左轮廓拟合曲线
	FittingCurve fcl = FittingCurve(contour_l);
	fcl.CurveFit();
	double* indexcl = new double[3];
	indexcl = fcl.getP();//左边曲线方程的系数
	//右轮廓拟合曲线
	FittingCurve fcr = FittingCurve(contour_r);
	fcr.CurveFit();
	double* indexcr = new double[3];
	indexcr = fcr.getP();//右边曲线方程的系数
	
	petal_template.clear();//模板轮廓
	petal_template.push_back(nowtip);
	for (int h = nowtip.z - 1; h > 0; h--)//从y高到低生成轮廓
	{
		int y = h*radius / height;//高度降低1对应的半径 
		coor t;
		//左边
		t.x = indexcl[0] * y*y + indexcl[1] * y+indexcl[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
		//右边
		t.x = indexcr[0] * y*y + indexcr[1] * y+indexcr[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
	}
	coor t = { 0, 0, 0 };
	petal_template.push_back(t);


	return;
}

double PetalTemplate::calangle(double a[2], double b[2])
{
	double mult = sqrtf(a[0] * a[0] + a[1] * a[1])*sqrtf(b[0] * b[0] + b[1] * b[1]);
	mult = (a[0] * b[0] + a[1] * b[1]) / (1.0*mult);

	double rangle;
	if (fabs(mult - 1) <= 0.0001)
		rangle = 0;
	else if (fabs(mult + 1) <= 0.0001)
		rangle = PI;
	else
	{
		rangle = acos(mult);//逆时针旋转夹角弧度
		double cross = a[0] * b[1] - b[0] * a[1];
		if (cross < 0)//.....
			rangle = 2 * PI - rangle;
	}

	return rangle;

}

coor PetalTemplate::rotate(coor src, double angle, coor direction)
{
	coor dst = {0,0,0};

	float t[3] = { src.x, src.y, src.z };
	Mat vsrc = Mat(1, 3, CV_32FC1, t);

	int ax = direction.x;
	int ay = direction.y;
	int az = direction.z;

	float A[3][3] = { ax*ax, ax*ay, ax*az,
		ay*ax, ay*ay, ay*az,
		az*ax, az*ay, az*az
	};
	float B[3][3] = { 0, -az, ay,
		az, 0, -ax,
		-ay, ax, 0
	};
	Mat mA = Mat(3, 3, CV_32FC1, A);
	Mat mB = Mat(3, 3, CV_32FC1, B);
	Mat mI = Mat::eye(3, 3, CV_32FC1);

	Mat IA = mI - mA;
	Mat M = mA + cos(angle)*IA + sin(angle)*mB;
	Mat Mt = M.t();
	Mat vdst = vsrc*Mt;

	dst.x = vdst.at<float>(0, 0);
	dst.y = vdst.at<float>(0, 1);
	dst.z = vdst.at<float>(0, 2);

	return dst;
}

void PetalTemplate::replaceTemplate()
{
	all_template.clear();
	//将模板花瓣轮廓旋转至Tippoints中各个位置
	coor direction = { 0, 0, 1 };

	for (int i = 0; i < contourTag.size(); i++)
	{
		vector<coor> nowpetal;
		//1、求解每个花瓣顶点和模板花瓣顶点旋转角
		double a[2] = { 0, 1 };
		double b[2] = { contourTag[i].tip.x, contourTag[i].tip.y };
		double angle = calangle(a, b);
		//2、将模板花瓣旋转至该顶点对应花瓣处
		for (int j = 0; j < petal_template.size(); j++)
		{
			coor t = rotate(petal_template[j],angle,direction);
			nowpetal.push_back(t);
		}
		all_template.push_back(nowpetal);

	}

	return;
}

void PetalTemplate::coneFitting()
{
	for (int i = 0; i < 5; i++)
	{
		surfaceFit1();
		updateTemplate();
	}
	
	return;
}

void PetalTemplate::surfaceFit1()
{
	double V[3] = {o.x()-f_o.x(),o.y()-f_o.y(),0};
	double Z[3] = {0,0,1};
	
	coor derection = { V[1] / sqrtf(V[1] * V[1] + V[0] * V[0]), -V[0] / sqrtf(V[1] * V[1] + V[0] * V[0]), 0 };
	
	//只变化最低凹点以上的部分
	lastrset = rset;
	int minh = height;
	for (int i = 0; i < vallypoints.size(); i++)
	if (vallypoints[i].z < minh)
		minh = vallypoints[i].z;
	//找到每个高度环上的点，和圆心连线找到和对应原始花瓣的交点
	vector<double> changedr;
	vector<int> tag;


	minvallyh = minh;
	for (int i = minh; i < rset.size()-10; i++)
	{
		vector<double> thisr;
		for (int j = 0; j < all_template.size(); j++)//第j个花瓣
		{
			vector<coor> insect;//存储该花瓣在对应高度的交点
			for (int k = 0; k < all_template[j].size(); k++)
			{
				if (fabs(all_template[j][k].z - i) < 0.001)//高度一致
					insect.push_back(all_template[j][k]);
			}
			//和原点连线
			for (int k = 0; k < insect.size(); k++)
			{
				//insect[k].y = -insect[k].y;
				//coor newinsect = rotate(insect[k],-(PI/2-angle),derection);
				coor newinsect = insect[k];
				//insect[k].y = -insect[k].y;
				double crossl = newinsect.y / newinsect.x;//斜率
				//遍历该花瓣的原始轮廓，找到斜率相同的点对应的半径
				vector<coor> thispetal = contourTag[j].petal_contour;
				double mindist = radius;//记录距离和原来点最近的距离
				double selectr = 0;
				
				for (int index = 0; index < thispetal.size(); index++)
				{
					//thispetal[index].y = -thispetal[index].y;
					//coor newthispetal = rotate(thispetal[index],-(PI/2-angle),derection);
					coor newthispetal = thispetal[index];
					//thispetal[index].y = -thispetal[index].y;
					double t = newthispetal.y / newthispetal.x;
					if (fabs(crossl - t) < 3)//斜率一致,找到和原来点最近的点
					{
						int b[2] = { fabs(thispetal[index].x - insect[k].x), fabs(thispetal[index].y - insect[k].y) };//和交点的向量
						int a[2] = {newthispetal.x,newthispetal.y};
						double dist = sqrtf(a[0]*a[0]+a[1]*a[1]);	
						double distb = sqrtf(b[0] * b[0] + b[1] * b[1]);
						//if (dist < mindist)
						if (dist<rset[i])
						{
							if (distb < mindist)
							{
								mindist = distb;

								selectr = sqrtf(thispetal[index].x*thispetal[index].x + thispetal[index].y*thispetal[index].y);
								
							}
							   
						}
							
					}

				}
				if (selectr>0)
				    thisr.push_back(selectr);

			}
		}

		//求解新的r
		
		double newr = 0;
		int count = 0;
		for (int j = 0; j < thisr.size(); j++)
		{
			if (fabs(thisr[j] - rset[i]) < 40)
			{
				newr += thisr[j];
				count++;
			}	
		}
		if (count>0)
		{
			rset[i] = newr / count;
			changedr.push_back(rset[i]);
			tag.push_back(i);
		}
			
		
	}

	//将半径平滑曲线拟合
	vector<coor> rtemp;
	coor t = { 0, 0, 0 };
	rtemp.push_back(t);
	for (int i = 0; i < rset.size(); i++)
	//for (int i = 0; i < changedr.size(); i++)
	{
		coor t = { rset[i], i, 0 };
		//coor t = { changedr[i], tag[i], 0 };
		rtemp.push_back(t);
	}
	
	
	FittingCurve fc = FittingCurve(rtemp);
	fc.CurveFit();
	double* indexr = new double[3];
	indexr = fc.getP();//
	for (int i = 0; i < rset.size(); i++)
		rset[i] = indexr[0] * i*i + indexr[1] * i;
	
	

	return;
}

void PetalTemplate::surfaceFit2()
{
	//只变化最低凹点以上的部分
	lastrset = rset;
	int minh = height;
	for (int i = 0; i < vallypoints.size(); i++)
	if (vallypoints[i].z < minh)
		minh = vallypoints[i].z;
	
	vector<double> changedr;
	vector<int> tag;
	for (int i = minh; i < rset.size(); i++)
	{
		vector<double> thisr;
		
		for (int j = 0; j < all_template.size(); j++)//第j个花瓣
		{
			vector<coor> insect;//存储该模板花瓣在对应高度的交点,并且计算法向量
			vector<coor> normalv;//法向量
			for (int k = 0; k < all_template[j].size(); k++)
			{
				if ((k>=2&&k<all_template[j].size()-2)&&(fabs(all_template[j][k].z - i) < 0.001))//高度一致	
				{
					insect.push_back(all_template[j][k]);
					//求出对应的法向量
					coor last, next;
					last = all_template[j][k - 2];
					next = all_template[j][k + 2];
					//单位化
					double norm;
					norm = sqrtf(last.x*last.x + last.y*last.y+ last.z*last.z);
					last.x /= norm;
					last.y /= norm;
					last.z /= norm;
					norm = sqrtf(next.x*next.x + next.y*next.y + next.z*next.z);
					next.x /= norm;
					next.y /= norm;
					next.z /= norm;
					coor t = {last.x+next.x,last.y+next.y,last.z+next.z};
					normalv.push_back(t);
				}
					
			}

			//求解insect法向量和原始花瓣轮廓交点
			vector<coor> thispetal = contourTag[j].petal_contour;
			for (int k = 0; k < thispetal.size(); k++)
			{
				if (!insect.empty())
				{
					for (int index = 0; index < normalv.size(); index++)
					{
						double t = normalv[index].y / normalv[index].x;
						if (fabs(t - thispetal[k].y / thispetal[k].x) < 0.001)
						{
							double selectr = sqrtf(thispetal[k].x*thispetal[k].x + thispetal[k].y*thispetal[k].y);
							thisr.push_back(selectr);
						}

					}
				}
			}
			
		}

		//求解新的r

		double newr = 0;
		int count = 0;
		for (int j = 0; j < thisr.size(); j++)
		{
			if (fabs(thisr[j] - rset[i]) < 40)
			{
				newr += thisr[j];
				count++;
			}
		}
		if (count>0)
		{
			rset[i] = newr / count;
			changedr.push_back(rset[i]);
			tag.push_back(i);
		}


	}

	//将半径平滑曲线拟合
	vector<coor> rtemp;
	coor t = { 0, 0, 0 };
	rtemp.push_back(t);
	//for (int i = 0; i < rset.size(); i++)
	for (int i = 0; i < changedr.size(); i++)
	{
		//coor t = {rset[i],i,0};
		coor t = { changedr[i], tag[i], 0 };
		rtemp.push_back(t);
	}

	FittingCurve fc = FittingCurve(rtemp);
	fc.CurveFit();
	double* indexr = new double[3];
	indexr = fc.getP();//
	for (int i = 0; i < rset.size(); i++)
		rset[i] = indexr[0] * i*i + indexr[1]*i+indexr[2];

	return;	
}

void PetalTemplate::updateTemplate()
{
	//更新模板
	
	for (int i = 0; i < petal_template.size(); i++)
	{
			int h = petal_template[i].z;
			double scale = rset[h] / (lastrset[h]);
			petal_template[i].x *= scale;
			petal_template[i].y *= scale;
	}
	
	//模板重新曲线拟合
	vector<coor> contour_l, contour_r;
	contour_l.push_back(petal_template[0]);
	contour_r.push_back(petal_template[0]);
	for (int i = 1; i < petal_template.size()-1; i=i+2)
	{
		contour_l.push_back(petal_template[i]);
		contour_r.push_back(petal_template[i+1]);
	}
	coor o = { 0, 0, 0 };
	contour_l.push_back(o);
	contour_r.push_back(o);

	//左轮廓拟合曲线
	FittingCurve fcl = FittingCurve(contour_l);
	fcl.CurveFit();
	double* indexcl = new double[3];
	indexcl = fcl.getP();//左边曲线方程的系数
	//右轮廓拟合曲线
	FittingCurve fcr = FittingCurve(contour_r);
	fcr.CurveFit();
	double* indexcr = new double[3];
	indexcr = fcr.getP();//右边曲线方程的系数

	petal_template.clear();//模板轮廓
	contour_l[0].y = rset[contour_l[0].z];
	petal_template.push_back(contour_l[0]);
	for (int h = contour_l[0].z - 1; h > 0; h--)//从y高到低生成轮廓
	{
		int y = rset[h];//高度降低1对应的半径 
		coor t;
		//左边
		t.x = indexcl[0] * y*y + indexcl[1] * y + indexcl[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
		//右边
		t.x = indexcr[0] * y*y + indexcr[1] * y + indexcr[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
	}
	coor t = { 0, 0, 0 };
	petal_template.push_back(t);

	replaceTemplate();//模板替换

	return;
}

vector<coor> PetalTemplate::getPetalTemplate()
{
	
	return petal_template;
}

vector<vector<coor>> PetalTemplate::getAllTemplate()
{
	return all_template;
}


//test
vector<coor> PetalTemplate::getContour()
{
	return contour;
}

vector<petal> PetalTemplate::getContourTag()
{
	return contourTag;
}

double PetalTemplate::getMinh()
{
	return minvallyh;
}

