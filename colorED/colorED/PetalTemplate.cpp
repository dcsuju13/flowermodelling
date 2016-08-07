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
		if (keypoints[i].tag != 1)//����
		{
			vallypoints.push_back(keypoints[i].co);
			if (i == 0)
				flag_keypoint = 1;
		}
		else//==1����
		{
			tippoints.push_back(keypoints[i].co);
		}

	}

	height = 0;
	for (int i = 0; i < tippoints.size(); i++)//����height
		if (height < tippoints[i].z)
			height = tippoints[i].z;

	//���뾶����
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
	//������
	contour.clear();

	ifstream fin("rotatekeypoint.txt", std::ios::in);
	
	
	char line[1024] = { 0 };
	string x = "";
	string y = "";
	string z = "";
	//string n = "";
	while (fin.getline(line, sizeof(line)))//��ȡ����������
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


	//���㰼��
	tippoints.clear();
	vallypoints.clear();

	ifstream fin1("rotatetippoint.txt", std::ios::in);

	string flag;
	int count = 0;
	while (fin1.getline(line, sizeof(line)))//��ȡ����������
	{
		count++;
		stringstream word(line);
		word >> x;
		word >> y;
		word >> z;
		word >> flag;//���һ���Ǳ�Ƕ��㰼��,������1��������0
		int flag1 = atoi(flag.c_str());

		coor t;
		t.x = atof(x.c_str());
		t.y = atof(y.c_str());
		t.z = atof(z.c_str());

		if (flag1 == 1)//����
		{
			vallypoints.push_back(t);
			if (count == 1)
				flag_keypoint = 1;//��һ����Ϊ����

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
	//�����������ֿ���Ǵ洢
	int j = 0;
	int index_tip;//��Ӧ�Ķ����±�
	for (int i = 0; i < vallypoints.size(); i++)//������Ϊ�ָ�
	{
		coor tip;
		if (flag_keypoint == 1&&i==0)//��һ������Ϊ����,�����һ������Ϊ��һ�����궥��
		{
			tip = tippoints.back();
			index_tip = 0;//��һ�����궥��С��Ϊ0
		}
		else if (flag_keypoint == 0 && i == 0)
		{
			tip = tippoints.at(0);
			index_tip = 1;//��һ�����궥��С��Ϊ1
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
				&&fabs(contour[j].z - vallypoints[i].z) < 0.001)//������������ʱ���һ����������
			{
				j++;
				break;

			}
				
		}
		petal p = { tip, nowpetal };
		contourTag.push_back(p);//�洢һ��������Ϣ
	}
	//���һ������֮��ĵ���Ϊ��һ������ĵ㣬������ǰ��
	vector<coor> nowpetal;
	for (; j < contour.size(); j++)
		nowpetal.push_back(contour[j]);
	contourTag[0].petal_contour.insert(contourTag[0].petal_contour.begin(),nowpetal.begin(),nowpetal.end());


	//�Ե�һ���������ģ��
	//pre�����û���������ת�������y�������غ�
	double a[2] = {contourTag[0].tip.x,contourTag[0].tip.y};
	double b[2] = { 0, 1 };
	double angle = calangle(a,b);

	coor direction = {0,0,1};
	coor nowtip = rotate(contourTag[0].tip, angle, direction);//��ת����

	//vector<coor> nowpetal;
	nowpetal.clear();
	for (int i = 0; i < contourTag[0].petal_contour.size(); i++)
	{
		coor t = rotate(contourTag[0].petal_contour[i],angle,direction);
		nowpetal.push_back(t);
	}
	//1���ֳ�����������x,y��
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
			
		if (meettip)//�Ѿ����˶���
		{
			contour_r.push_back(nowpetal[i]);
		}
		else
			contour_l.push_back(nowpetal[i]); 

	}
	//�·�û��������������
	//���
	coor nearest;
	if (fabs(contour_l.back().x - nowtip.x) < 0.001&&fabs(contour_l.back().y - nowtip.y) < 0.001
		&&fabs(contour_l.back().z - nowtip.z) < 0.001)//���һ�����Ƿ�Ϊ����
		nearest = contour_l[0];
	else
		nearest = contour_l.back();
	float k = nearest.x / nearest.y;//б�ʣ�����y���x
	for (int i = nearest.y - 1; i >= 0; i--)
	{
		coor t = {k*i,i,0};
		contour_l.push_back(t);
	}
	//�ұ�
	if (fabs(contour_r.back().x - nowtip.x) < 0.001&&fabs(contour_r.back().y - nowtip.y) < 0.001
		&&fabs(contour_r.back().z - nowtip.z) < 0.001)//���һ�����Ƿ�Ϊ����
		nearest = contour_r[0];
	else
		nearest = contour_r.back();
	k = nearest.x / nearest.y;//б�ʣ�����y���x
	for (int i = nearest.y - 1; i >= 0; i--)
	{
		coor t = { k*i, i, 0 };
		contour_r.push_back(t);
	}

	coor o = {0,0,0};
	contour_l.push_back(o);
	contour_r.push_back(o);
	//2�����������ֱ�������ߣ�������������

	//�������������
	FittingCurve fcl = FittingCurve(contour_l);
	fcl.CurveFit();
	double* indexcl = new double[3];
	indexcl = fcl.getP();//������߷��̵�ϵ��
	//�������������
	FittingCurve fcr = FittingCurve(contour_r);
	fcr.CurveFit();
	double* indexcr = new double[3];
	indexcr = fcr.getP();//�ұ����߷��̵�ϵ��
	
	petal_template.clear();//ģ������
	petal_template.push_back(nowtip);
	for (int h = nowtip.z - 1; h > 0; h--)//��y�ߵ�����������
	{
		int y = h*radius / height;//�߶Ƚ���1��Ӧ�İ뾶 
		coor t;
		//���
		t.x = indexcl[0] * y*y + indexcl[1] * y+indexcl[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
		//�ұ�
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
		rangle = acos(mult);//��ʱ����ת�нǻ���
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
	//��ģ�廨��������ת��Tippoints�и���λ��
	coor direction = { 0, 0, 1 };

	for (int i = 0; i < contourTag.size(); i++)
	{
		vector<coor> nowpetal;
		//1�����ÿ�����궥���ģ�廨�궥����ת��
		double a[2] = { 0, 1 };
		double b[2] = { contourTag[i].tip.x, contourTag[i].tip.y };
		double angle = calangle(a, b);
		//2����ģ�廨����ת���ö����Ӧ���괦
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
	
	//ֻ�仯��Ͱ������ϵĲ���
	lastrset = rset;
	int minh = height;
	for (int i = 0; i < vallypoints.size(); i++)
	if (vallypoints[i].z < minh)
		minh = vallypoints[i].z;
	//�ҵ�ÿ���߶Ȼ��ϵĵ㣬��Բ�������ҵ��Ͷ�Ӧԭʼ����Ľ���
	vector<double> changedr;
	vector<int> tag;


	minvallyh = minh;
	for (int i = minh; i < rset.size()-10; i++)
	{
		vector<double> thisr;
		for (int j = 0; j < all_template.size(); j++)//��j������
		{
			vector<coor> insect;//�洢�û����ڶ�Ӧ�߶ȵĽ���
			for (int k = 0; k < all_template[j].size(); k++)
			{
				if (fabs(all_template[j][k].z - i) < 0.001)//�߶�һ��
					insect.push_back(all_template[j][k]);
			}
			//��ԭ������
			for (int k = 0; k < insect.size(); k++)
			{
				//insect[k].y = -insect[k].y;
				//coor newinsect = rotate(insect[k],-(PI/2-angle),derection);
				coor newinsect = insect[k];
				//insect[k].y = -insect[k].y;
				double crossl = newinsect.y / newinsect.x;//б��
				//�����û����ԭʼ�������ҵ�б����ͬ�ĵ��Ӧ�İ뾶
				vector<coor> thispetal = contourTag[j].petal_contour;
				double mindist = radius;//��¼�����ԭ��������ľ���
				double selectr = 0;
				
				for (int index = 0; index < thispetal.size(); index++)
				{
					//thispetal[index].y = -thispetal[index].y;
					//coor newthispetal = rotate(thispetal[index],-(PI/2-angle),derection);
					coor newthispetal = thispetal[index];
					//thispetal[index].y = -thispetal[index].y;
					double t = newthispetal.y / newthispetal.x;
					if (fabs(crossl - t) < 3)//б��һ��,�ҵ���ԭ��������ĵ�
					{
						int b[2] = { fabs(thispetal[index].x - insect[k].x), fabs(thispetal[index].y - insect[k].y) };//�ͽ��������
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

		//����µ�r
		
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

	//���뾶ƽ���������
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
	//ֻ�仯��Ͱ������ϵĲ���
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
		
		for (int j = 0; j < all_template.size(); j++)//��j������
		{
			vector<coor> insect;//�洢��ģ�廨���ڶ�Ӧ�߶ȵĽ���,���Ҽ��㷨����
			vector<coor> normalv;//������
			for (int k = 0; k < all_template[j].size(); k++)
			{
				if ((k>=2&&k<all_template[j].size()-2)&&(fabs(all_template[j][k].z - i) < 0.001))//�߶�һ��	
				{
					insect.push_back(all_template[j][k]);
					//�����Ӧ�ķ�����
					coor last, next;
					last = all_template[j][k - 2];
					next = all_template[j][k + 2];
					//��λ��
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

			//���insect��������ԭʼ������������
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

		//����µ�r

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

	//���뾶ƽ���������
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
	//����ģ��
	
	for (int i = 0; i < petal_template.size(); i++)
	{
			int h = petal_template[i].z;
			double scale = rset[h] / (lastrset[h]);
			petal_template[i].x *= scale;
			petal_template[i].y *= scale;
	}
	
	//ģ�������������
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

	//�������������
	FittingCurve fcl = FittingCurve(contour_l);
	fcl.CurveFit();
	double* indexcl = new double[3];
	indexcl = fcl.getP();//������߷��̵�ϵ��
	//�������������
	FittingCurve fcr = FittingCurve(contour_r);
	fcr.CurveFit();
	double* indexcr = new double[3];
	indexcr = fcr.getP();//�ұ����߷��̵�ϵ��

	petal_template.clear();//ģ������
	contour_l[0].y = rset[contour_l[0].z];
	petal_template.push_back(contour_l[0]);
	for (int h = contour_l[0].z - 1; h > 0; h--)//��y�ߵ�����������
	{
		int y = rset[h];//�߶Ƚ���1��Ӧ�İ뾶 
		coor t;
		//���
		t.x = indexcl[0] * y*y + indexcl[1] * y + indexcl[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
		//�ұ�
		t.x = indexcr[0] * y*y + indexcr[1] * y + indexcr[2];
		t.y = y;
		t.z = h;
		petal_template.push_back(t);
	}
	coor t = { 0, 0, 0 };
	petal_template.push_back(t);

	replaceTemplate();//ģ���滻

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

