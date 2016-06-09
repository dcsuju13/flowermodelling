#include "MeshFitting.h"


MeshFitting::MeshFitting()
{
}

MeshFitting::MeshFitting(vector<vector<coor>> all, vector<petal> contour)
{
	all_template = all;
	contourTag = contour;

	//生成采样点
	for (int i = 0; i < all_template.size(); i++)
	{
		vector<coor> nowpetal;

		nowpetal.push_back(all_template[i][0]);//顶点写入
		//确定合适梯度
		int delta = all_template[i][0].z/ 20;
		if (delta < 5)
			delta = 5;
		vector<coor> thispetal = all_template[i];
		for (int j = 1; j < thispetal.size() - 1; j = j + 2*delta)//高度为5进行采样
		{
			coor t;
			//左
			t.x = thispetal.at(j).x;
			t.y = thispetal.at(j).y;
			t.z = thispetal.at(j).z;
			nowpetal.push_back(t);
			//中
			t.x = (thispetal.at(j).x + thispetal.at(j + 1).x) / 2;
			t.y = (thispetal.at(j).y + thispetal.at(j + 1).y) / 2;
			t.z = thispetal.at(j).z;
			nowpetal.push_back(t);
			//右
			t.x = thispetal.at(j+1).x;
			t.y = thispetal.at(j+1).y;
			t.z = thispetal.at(j+1).z;
			nowpetal.push_back(t);
		}
		coor t = { 0, 0, 0 };
		nowpetal.push_back(t);

		//记下该花瓣采样点
		all_sample.push_back(nowpetal);
		allsamplepoints.insert(allsamplepoints.end(),nowpetal.begin(),nowpetal.end());
	}
}


MeshFitting::~MeshFitting()
{
}

void MeshFitting::setMinh(double a)
{
	minvallayh = a;
	return;
}

void MeshFitting::triMeshAll()
{
	triall.clear();
	int count = 0;
	for (int i = 0; i < all_sample.size(); i++)
	{
		vector<coor> thissample = all_sample.at(i);

		coor t;
		//第一个三角形索引
		t.x = 0+count;
		t.y = 1+count;
		t.z = 2+count;
		triall.push_back(t);
		//第二个三角形索引
		t.x = 0+count;
		t.y = 2+count;
		t.z = 3+count;
		triall.push_back(t);

		for (int j = 1; j < thissample.size()-6; j+=3)
		{
			//花瓣中间部分，每行有四个三角形
			t.x = j+count;
			t.y = j + 3+count;
			t.z = j + 4+count;
			triall.push_back(t);

			t.x = j+count;
			t.y = j + 4+count;
			t.z = j + 1+count;
			triall.push_back(t);

			t.x =j + 1+count;
			t.y = j + 4+count;
			t.z = j + 2+count;
			triall.push_back(t);

			t.x = j + 2+count;
			t.y = j + 4+count;
			t.z = j + 5+count;
			triall.push_back(t);

		}
		//最后收尾的两个三角
		int last = thissample.size() - 1;
		t.x = last+count;
		t.y = last - 2+count;
		t.z = last - 3+count;
		triall.push_back(t);

		t.x = last+count;
		t.y = last - 1+count;
		t.z = last - 2+count;
		triall.push_back(t);

		count += thissample.size();

	}
	return;

}

void MeshFitting::buildMesh(vector<coor> sample, vector<coor> tri)
{
	
	all_mesh.clear();

	vector<MyMesh::VertexHandle> vhandle;//记录采样点句柄
	for (int i = 0; i < sample.size(); i++)
	{
		
			MyMesh::VertexHandle t;
			t = all_mesh.add_vertex(MyMesh::Point(sample.at(i).x, sample.at(i).y, sample.at(i).z));
			vhandle.push_back(t);
			
	}
	// generate faces
	vector<MyMesh::VertexHandle>  face_vhandles;//记录三角网格面句柄
	for (int i = 0; i < tri.size(); i++)
	{
		face_vhandles.clear();
		int a, b, c;
		//得到对应vhandles的索引
		a = tri.at(i).x;
		b = tri.at(i).y;
		c = tri.at(i).z;

		face_vhandles.push_back(vhandle[a]);
		face_vhandles.push_back(vhandle[b]);
		face_vhandles.push_back(vhandle[c]);
		all_mesh.add_face(face_vhandles);

	}
	// write mesh to output.ply
	//?meshviewer不能打开
	try
	{
		if (!OpenMesh::IO::write_mesh(all_mesh, "all.ply"))
		{
			std::cerr << "Cannot write mesh to file 'mesh.ply'" << std::endl;
			//return;
		}
	}
	catch (std::exception& x)
	{
		std::cerr << x.what() << std::endl;
		//return;
	}
	return;
}


int MeshFitting::meshFit()
{
	vector<float> L;//L矩阵
	vector<coor> lsample;//原始网格的样本点laplace坐标
	vector<int> H;//H矩阵
	vector<coor> u;//需要移动的点的新位置
	vector<vector<coor>> newsample = calnewsample(H, u);//得到目标样本点

	//test

	/*
	allsamplepoints.clear();
	for (int i = 0; i < newsample.size(); i++)
	{
		allsamplepoints.insert(allsamplepoints.end(),newsample[i].begin(),newsample[i].end());

	}
	buildMesh(allsamplepoints, triall);//重新生成网格

	*/
	

	
	if (u.size() <= 0)
		return 1;

	//进行网格变形

	//2.................
	MymeshFit m = MymeshFit();
	m.init(allsamplepoints, triall);//初始化网格参数
	m.buildmesh();//构造网格
	m.allMvcoding();//mean value编码
	L = m.getL();//L矩阵
	lsample = m.getlsample();//原始样本样本点对应laplacian坐标

	//系数矩阵为上L下H
	//系数矩阵A用向量存储
	vector<float> A;
	for (int i = 0; i < L.size(); i++)
	{
		A.push_back(L[i]);
	}
	for (int i = 0; i < H.size(); i++)
	{
		A.push_back(H[i]);
	}
	int row = lsample.size() + u.size();//A的行数
	int col = lsample.size();//A的列数

	//右边矩阵为上lsample下u，n+label行，3列分别计算
	//bx
	//by
	//bz
	vector<float> bx;
	vector<float> by;
	vector<float> bz;
	for (int i = 0; i < lsample.size(); i++)
	{
		bx.push_back(lsample.at(i).x);
		by.push_back(lsample.at(i).y);
		bz.push_back(lsample.at(i).z);
	}
	for (int i = 0; i < u.size(); i++)
	{
		bx.push_back(u.at(i).x);
		by.push_back(u.at(i).y);
		bz.push_back(u.at(i).z);
	}
	matrix cax = matrix();//计算新的x坐标
	cax.init(A, bx, row, col);
	vector<float> px;
	px = cax.getX();

	matrix cay = matrix();//计算新的y坐标
	cay.init(A, by, row, col);
	vector<float> py;
	py = cay.getX();

	matrix caz = matrix();//计算新的z坐标
	caz.init(A, bz, row, col);
	vector<float> pz;
	pz = caz.getX();

	vector<coor> mv;
	if (px.size() == py.size() && px.size() == pz.size() && px.size() == allsamplepoints.size())
	{
		for (int i = 0; i < px.size(); i++)
		{
			coor t;
			t.x = px.at(i);
			t.y = py.at(i);
			t.z = pz.at(i);
			mv.push_back(t);
		}
		allsamplepoints.clear();
		allsamplepoints = mv;//meshp更新为变形后的样本点
		buildMesh(mv,triall);//重新生成网格

		
		
		
		MymeshFit ml = MymeshFit();
		ml.init(allsamplepoints, triall);//初始化网格参数
		ml.buildmesh();//构造网格
		ml.Laplacesmooth();
		vector<coor> laplace = ml.getLaplacemv();
		allsamplepoints.clear();
		for (int i = 0; i < laplace.size(); i++)
			allsamplepoints.push_back(laplace.at(i));
		buildMesh(allsamplepoints,triall);//重新生成网格

		

		
		

		return 1;
	}
	else
		return 0;

	
	

	
	return 1;
}

vector<vector<coor>> MeshFitting::calnewsample(vector<int> &H, vector<coor> &u)
{
	vector<vector<coor>> newsample;
	H.clear();
	u.clear();

	for (int i = 0; i < all_sample.size(); i++)//对每个花瓣找新样本
	{
		vector<coor> thissample = all_sample.at(i);

		thissample.at(0) = contourTag.at(i).tip;//顶点变形
		//求出H和u（u为newsample中和meshp中不同的点）
		u.push_back(thissample[0]);//u中加入需要修改的点
		for (int k = 0; k < allsamplepoints.size(); k++)
		{
			if (k == i*thissample.size())
				H.push_back(1);
			else
				H.push_back(0);

		}

		//将原始轮廓左边缘和右边缘分开存储
		vector<coor> contour_l,contour_r;
		bool totip = false;
		vector<coor> thiscontour = contourTag.at(i).petal_contour;
		for (int j = 0; j < thiscontour.size(); j++)
		{
			if (fabs(thiscontour[j].x - contourTag.at(i).tip.x) < 0.001&&fabs(thiscontour[j].y - contourTag.at(i).tip.y) < 0.001
				&&fabs(thiscontour[j].z - contourTag.at(i).tip.z) < 0.001)
			{
				totip = true;
				continue;
			}
			if (totip)
				contour_r.push_back(thiscontour[j]);
			else
				contour_l.push_back(thiscontour[j]);

		}

		for (int j = 1; j < thissample.size() - 6; j = j + 3)//对轮廓点变形
		{
			/*
			if (fabs(thissample[j].z-minvallayh/2)<10)
			{
				//左

				//求出H和u（u为newsample中和meshp中不同的点）
				u.push_back(thissample[j]);//u中加入需要修改的点
				for (int k = 0; k < allsamplepoints.size(); k++)
				{
					if (k == i*thissample.size()+j)
					H.push_back(1);
					else
					H.push_back(0);

				}
				//右
				//求出H和u（u为newsample中和meshp中不同的点）
				u.push_back(thissample[j+2]);//u中加入需要修改的点
				for (int k = 0; k < allsamplepoints.size(); k++)
				{
					if (k == i*thissample.size() + j+2)
					H.push_back(1);
					else
					H.push_back(0);

				}

			}
			else
			*/
			
			
				//左边
				int count = 0;
				double sumx = 0;
				double sumy = 0;
				double sumz = 0;

				double mindist = 100;

				coor last, next;
				if (j == 1)
				{
					last = thissample[j - 1];
				}
				else
					last = thissample[j - 3];
				next = thissample[j + 3];
				//单位化
				double norm;
				norm = sqrtf(last.x*last.x + last.y*last.y + last.z*last.z);
				last.x /= norm;
				last.y /= norm;
				last.z /= norm;
				norm = sqrtf(next.x*next.x + next.y*next.y + next.z*next.z);
				next.x /= norm;
				next.y /= norm;
				next.z /= norm;
				coor normalv = { last.x + next.x, last.y + next.y, last.z + next.z };//该点垂直向量


				for (int k = 0; k < contour_l.size(); k++)
				{
					if ((fabs(normalv.y / normalv.x - contour_l[k].y / contour_l[k].x) < 0.5) && (fabs(contour_l[k].z - thissample[j].z)<5))
						//if (fabs(contour_l[k].z - thissample[j].z) < 3)
						//if (fabs(sqrtf(contour_l[k].x*contour_l[k].x + contour_l[k].y*contour_l[k].y) - sqrtf(thissample[j].x*thissample[j].x + thissample[j].y*thissample[j].y))<3)
					{
						double dist = fabs(sqrtf(contour_l[k].x*contour_l[k].x + contour_l[k].y*contour_l[k].y) - sqrtf(thissample[j].x*thissample[j].x + thissample[j].y*thissample[j].y));
						if (dist < mindist)
						{
							mindist = dist;
							count++;
							sumx += contour_l[k].x;
							sumy += contour_l[k].y;
							sumz += contour_l[k].z;
							//sumx = contour_l[k].x;
							//sumy = contour_l[k].y;
							//sumz = contour_l[k].z;
						}

					}
				}
				if (count > 0)
				{
					thissample[j].x = sumx / count;
					thissample[j].y = sumy / count;
					thissample[j].z = sumz / count;

					//求出H和u（u为newsample中和meshp中不同的点）
					u.push_back(thissample[j]);//u中加入需要修改的点
					for (int k = 0; k < allsamplepoints.size(); k++)
					{
						if (k == i*thissample.size() + j)
							H.push_back(1);
						else
							H.push_back(0);

					}
				}

				//右边
				count = 0;
				sumx = 0;
				sumy = 0;
				sumz = 0;
				mindist = 100;


				last = thissample[j - 1];
				next = thissample[j + 2 + 3];
				//单位化
				norm = sqrtf(last.x*last.x + last.y*last.y + last.z*last.z);
				last.x /= norm;
				last.y /= norm;
				last.z /= norm;
				norm = sqrtf(next.x*next.x + next.y*next.y + next.z*next.z);
				next.x /= norm;
				next.y /= norm;
				next.z /= norm;
				normalv = { last.x + next.x, last.y + next.y, last.z + next.z };//该点垂直向量


				for (int k = 0; k < contour_r.size(); k++)
				{
					if ((fabs(normalv.y / normalv.x - contour_r[k].y / contour_r[k].x) < 0.5) && (fabs(contour_r[k].z - thissample[j + 2].z)<5))
						//if (fabs(contour_r[k].z - thissample[j + 2].z) < 3)
						//if (fabs(sqrtf(contour_r[k].x*contour_r[k].x + contour_r[k].y*contour_r[k].y) - sqrtf(thissample[j+2].x*thissample[j+2].x + thissample[j+2].y*thissample[j+2].y))<3)
					{
						double dist = fabs(sqrtf(contour_r[k].x*contour_r[k].x + contour_r[k].y*contour_r[k].y) - sqrtf(thissample[j + 2].x*thissample[j + 2].x + thissample[j + 2].y*thissample[j + 2].y));
						if (dist < mindist)
						{
							mindist = dist;
							count++;
							sumx += contour_r[k].x;
							sumy += contour_r[k].y;
							sumz += contour_r[k].z;
							//sumx = contour_r[k].x;
							//sumy = contour_r[k].y;
							//sumz = contour_r[k].z;

						}

					}
				}
				if (count > 0)
				{
					thissample[j + 2].x = sumx / count;
					thissample[j + 2].y = sumy / count;
					thissample[j + 2].z = sumz / count;

					//求出H和u（u为newsample中和meshp中不同的点）
					u.push_back(thissample[j + 2]);//u中加入需要修改的点
					for (int k = 0; k < allsamplepoints.size(); k++)
					{
						if (k == i*thissample.size() + j + 2)
							H.push_back(1);
						else
							H.push_back(0);

					}

				}


				thissample.back() = { 0, 0, 0 };//最下方点
				//求出H和u（u为newsample中和meshp中不同的点）
				u.push_back(thissample.back());//u中加入需要修改的点
				for (int k = 0; k < allsamplepoints.size(); k++)
				{
					if (k == (i + 1)*thissample.size() - 1)
						H.push_back(1);
					else
						H.push_back(0);

				}


		}
		newsample.push_back(thissample);

	}
	return newsample;
}

vector<coor> MeshFitting::RotateMeshPoint(vector<coor> AllSample, vector<coorTag> KeyPoint)
{
	vector<coor> RotateSample;
	//读取花瓣遮挡信息，并保存
	vector<int> CoverInfo;
	for (int i = 0; i < KeyPoint.size(); i++)
	{
		if (KeyPoint[i].tag == 0)
			CoverInfo.push_back(0);
		if (KeyPoint[i].tag == 2)
			CoverInfo.push_back(1);
	}

	//计算花瓣1的遮挡信息和中轴
	int covertype = 0;
	int flowernum = 0;

	if (CoverInfo[0] == 1 && CoverInfo[CoverInfo.size() - 1] == 1)
		covertype = 1;
	else if (CoverInfo[0] == 1 && CoverInfo[CoverInfo.size() - 1] == 0)
		covertype = 3;
	else if (CoverInfo[0] == 0 && CoverInfo[CoverInfo.size() - 1] == 1)
		covertype = 2;

	int p = 0;
	int axleh = 0;
	coor axle = { 0, 0, 0, };
	while (powf(AllSample[p].x, 2) + powf(AllSample[p].y, 2) + powf(AllSample[p].y, 2) > 5)
	{
		if (AllSample[p].z > axleh)
		{
			axle = AllSample[p];
			axleh = AllSample[p].z;
		}
		p = p + 1;
	}

	//旋转花瓣
	for (int i = 0; i < AllSample.size(); i++)
	{
		if (powf(AllSample[i].x, 2) + powf(AllSample[i].y, 2) + powf(AllSample[i].z, 2) < 5 && i < AllSample.size() - 1) //如果是边界点，计算新的边界信息
		{
			flowernum = flowernum++;

			covertype = 0;
			if (CoverInfo[flowernum] == 1 && CoverInfo[flowernum - 1] == 1)
				covertype = 1;
			else if (CoverInfo[flowernum] == 1 && CoverInfo[flowernum - 1] == 0)
				covertype = 3;
			else if (CoverInfo[flowernum] == 0 && CoverInfo[flowernum - 1] == 1)
				covertype = 2;

			int p = i + 1;
			int axleh = 0;
			coor axle = { 0, 0, 0, };
			while (p<AllSample.size()&&powf(AllSample[p].x, 2) + powf(AllSample[p].y, 2) + powf(AllSample[p].z, 2) > 5)
			{
				
				if (AllSample[p].z > axleh)
				{
					axle = AllSample[p];
					axleh = AllSample[p].z;
				}
				p = p + 1;
			}

			RotateSample.push_back(AllSample[i]);
		}
		else  //否则根据遮挡类型旋转或移动花瓣，保存新的坐标
		{
			coor np = AllSample[i];
			Vec3f npf = {(float)np.x,(float)np.y,(float)np.z};
			Vec3f axlef = {(float)axle.x,(float)axle.y,(float)axle.z};
			if (covertype == 1)
				np = PointRotate(axlef, npf, -3.14 / 12);
			else if (covertype == 2)
				np = { np.x, np.y, np.z - 10 };
			else if (covertype == 3)
				np = PointRotate(axlef, npf, 3.14 / 12);

			RotateSample.push_back(np);
		}
	}
	return RotateSample;
}

coor MeshFitting::PointRotate(Vec3f RotateZ, Vec3f RotateP, float angle)
{
	float norm = sqrtf(powf(RotateZ(0), 2) + powf(RotateZ(1), 2) + powf(RotateZ(2), 2));
	float x = RotateZ(0) / norm, y = RotateZ(1) / norm, z = RotateZ(2) / norm;

	Mat A(3, 3, CV_32FC1, Scalar(0));
	A.at<float>(0, 0) = x*x;  A.at<float>(1, 0) = x*y;   A.at<float>(2, 0) = x*z;
	A.at<float>(0, 1) = y*x;  A.at<float>(1, 1) = y*y;   A.at<float>(2, 1) = y*z;
	A.at<float>(0, 2) = z*x;  A.at<float>(1, 2) = z*y;   A.at<float>(2, 2) = z*z;
	//cout << A<<endl;

	Mat B(3, 3, CV_32FC1, Scalar(0));
	B.at<float>(0, 0) = 0;  B.at<float>(0, 1) = -z;   B.at<float>(0, 2) = y;
	B.at<float>(1, 0) = z;  B.at<float>(1, 1) = 0;   B.at<float>(1, 2) = -x;
	B.at<float>(2, 0) = -y;  B.at<float>(2, 1) = x;   B.at<float>(2, 2) = 0;
	//cout << B << endl;

	Mat I(3, 3, CV_32FC1, Scalar(0));
	I.at<float>(0, 0) = 1; I.at<float>(1, 1) = 1; I.at<float>(2, 2) = 1;

	Mat P(1, 3, CV_32FC1, Scalar(0));
	P.at<float>(0, 0) = RotateP(0); P.at<float>(0, 1) = RotateP(1);  P.at<float>(0, 2) = RotateP(2);

	Mat M = A + cos(angle)*(I - A) + sin(angle)*B;
	//cout << M << endl;
	Mat RotatedM = P * M.t();
	//cout << RotatedM << endl;

	Vec3f RotatedP = { RotatedM.at<float>(0, 0), RotatedM.at<float>(0, 1), RotatedM.at<float>(0, 2) };
	coor t = { RotatedM.at<float>(0, 0), RotatedM.at<float>(0, 1), RotatedM.at<float>(0, 2) };
	return t;
}

MyMesh MeshFitting::getAllmesh()
{
	triMeshAll();
	buildMesh(allsamplepoints, triall);
	return all_mesh;
}

MyMesh MeshFitting::getMvmesh()
{
	if (meshFit())
		return all_mesh;

}

MyMesh MeshFitting::getOccmesh()
{
	allsamplepoints = RotateMeshPoint(allsamplepoints,keypoints);
	buildMesh(allsamplepoints, triall);//重新生成网格
	return all_mesh;
}

void MeshFitting::setKeypoints(vector<coorTag> k)
{
	keypoints = k;
}
