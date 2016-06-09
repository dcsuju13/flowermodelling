#include "MymeshFit.h"


MymeshFit::MymeshFit()
{
}


MymeshFit::~MymeshFit()
{
	
}

void MymeshFit::init(vector<coor> sample, vector<coor> tri)
{
	countP = sample.size();
	countT = tri.size();

	for (int i = 0; i < sample.size(); i++)//数组初始化
	{
		point[i] = (struct node*)malloc(sizeof(struct node));
		point[i]->x = 0;
		point[i]->y = 0;
		point[i]->z = 0;
		point[i]->edgef = NULL;
		point[i]->id = i;
		point[i]->laplace[1, 2, 3] = { 0 };
		point[i]->n[1, 2, 3] = { 0 };
		trimesh[i][0] = 0;
		trimesh[i][1] = 0;
		trimesh[i][2] = 0;
	}
	
	
	//初始化样本点
	for (int i = 0; i < sample.size(); i++)
	{
		point[i]->x = sample.at(i).x;
		point[i]->y = sample.at(i).y;
		point[i]->z = sample.at(i).z;

		if (abs(point[i]->x) < 1e-4)
				point[i]->x = 0.01;
		if (abs(point[i]->y) < 1e-4)
				point[i]->y = 0.01;
		if (abs(point[i]->z) < 1e-4)
				point[i]->z = 0.01;
		
	}
	//初始化索引
	for (int i = 0; i < tri.size(); i++)
	{
		trimesh[i][0] = tri.at(i).x;
		trimesh[i][1] = tri.at(i).y;
		trimesh[i][2] = tri.at(i).z;

	}

	return;
}

void MymeshFit::buildmesh()
{
	for (int i = 0; i < countT; i++)
	{
		if (trimesh[i][0] + trimesh[i][1] + trimesh[i][2] < 1e-4)
		{
			break;
		}
		//依次建立边，对应的修改点的链接
		initedge(point[trimesh[i][0]], point[trimesh[i][1]]);
		initedge(point[trimesh[i][1]], point[trimesh[i][0]]);
		initedge(point[trimesh[i][0]], point[trimesh[i][2]]);
		initedge(point[trimesh[i][2]], point[trimesh[i][0]]);
		initedge(point[trimesh[i][1]], point[trimesh[i][2]]);
		initedge(point[trimesh[i][2]], point[trimesh[i][1]]);

	}
	return;
}

void MymeshFit::initedge(struct node *s, struct node *e)   //用来建立网格结构的函数
{
	struct edge* newedge = (struct edge*)malloc(sizeof(struct edge));
	newedge->start = s;
	newedge->end = e;
	newedge->next = NULL;
	if (s->edgef == NULL)
	{
		s->edgef = newedge;
	}
	else
	{
		struct edge *te;
		te = s->edgef;
		int block = 0;
		if (e->x - s->x >= 0 && e->y - s->y > 0)  block = 1;
		if (e->x - s->x < 0 && e->y - s->y >= 0)  block = 2;
		if (e->x - s->x <= 0 && e->y - s->y < 0)  block = 3;
		if (e->x - s->x > 0 && e->y - s->y <= 0)  block = 4;

		bool firstandbig = false;
		while (true) //将新的边插入到队
		{
			//先比较block
			int blocke = 0;
			if (te->end->x - s->x >= 0 && te->end->y - s->y > 0)  blocke = 1;
			if (te->end->x - s->x < 0 && te->end->y - s->y >= 0)  blocke = 2;
			if (te->end->x - s->x <= 0 && te->end->y - s->y < 0)  blocke = 3;
			if (te->end->x - s->x > 0 && te->end->y - s->y <= 0)  blocke = 4;

			if (block > blocke)
			if (te->next != NULL)
				te = te->next;
			else
			{
				firstandbig = true;
				break;
			}
			else if (block == blocke)//再比较角度
			{
				float eorx = abs(e->x - s->x) / sqrt(pow((e->x - s->x), 2) + pow((e->y - s->y), 2));
				float eorxe = abs(te->end->x - s->x) / sqrt(pow((te->end->x - s->x), 2) + pow((te->end->y - s->y), 2));
				if (block == 2 || block == 4)
				if (eorx > eorxe)
				{
					if (te->next != NULL) te = te->next;
					else
					{
						firstandbig = true;
						break;
					}
				}
				else break;
				else
				if (eorx < eorxe)
				{
					if (te->next != NULL) te = te->next;
					else
					{
						firstandbig = true;
						break;
					}
				}
				else break;
			}
			else break;
			//if (te->end != e) te = te->next;
			//else break;
		}
		//插入正确的位置
		if (e->id != te->end->id)
		{
			if (firstandbig == true)
				te->next = newedge;
			else if (s->edgef == te)
			{
				s->edgef = newedge;
				newedge->next = te;
			}
			else
			{
				struct edge *ne;
				ne = s->edgef;
				while (true)
				{
					if (ne->next == te)
					{
						ne->next = newedge;
						newedge->next = te;
						break;
					}
					else if (ne->next != NULL) ne = ne->next;
					else break;
				}
			}
		}
		//if(te->end->id != e->id) te->next = newedge;
	}
}

void MymeshFit::mvcoding(struct node *point, float L[MAX_SIZE][MAX_SIZE])    //用来求解稀疏矩阵L的函数
{
	if (point->edgef == NULL)
	{
		//初始化laplace坐标
		point->laplace[0] = 0;
		point->laplace[1] = 0;
		point->laplace[2] = 0;
		printf("解得点%d的拉普拉斯坐标：%4.3f,%4.3f,%4.3f\n", point->id, point->laplace[0], point->laplace[1], point->laplace[2]);
		L[point->id][point->id] = 1;//++
		return;
	}
	//求每个点法向量
	int count = 0;
	float l[3] = { 0 };
	//初始化laplace坐标
	point->laplace[0] = 0;
	point->laplace[1] = 0;
	point->laplace[2] = 0;
	//第一次遍历1-邻接点：求解平均数l
	struct edge *te = point->edgef;
	while (true)
	{
		count++;
		l[0] += te->end->x;
		l[1] += te->end->y;
		l[2] += te->end->z;
		if (te->next == NULL) break;
		else te = te->next;
	}
	l[0] = l[0] / count;
	l[1] = l[1] / count;
	l[2] = l[2] / count;
	//第二次遍历1-邻接点：求解法向量n
	float n[3] = { 0 };
	te = point->edgef;
	while (true)
	{
		float vj[3], vj1[3] = { 0 };
		vj[0] = te->end->x - l[0];
		vj[1] = te->end->y - l[1];
		vj[2] = te->end->z - l[2];
		if (te->next == NULL)
		{
			vj1[0] = point->edgef->end->x - l[0];
			vj1[1] = point->edgef->end->y - l[1];
			vj1[2] = point->edgef->end->z - l[2];
		}
		else
		{
			vj1[0] = te->next->end->x - l[0];
			vj1[1] = te->next->end->y - l[1];
			vj1[2] = te->next->end->z - l[2];
		}
		n[0] += vj1[1] * vj[2] - vj1[2] * vj[1];
		n[1] += vj1[2] * vj[0] - vj1[0] * vj[2];
		n[2] += vj1[0] * vj[1] - vj1[1] * vj[0];
		if (te->next == NULL) break;
		else te = te->next;
	}
	float sumvjs = sqrt(pow(n[0], 2) + pow(n[1], 2) + pow(n[2], 2));
	//............................
	if (sumvjs > 0)
	{
		n[0] = n[0] / sumvjs;
		n[1] = n[1] / sumvjs;
		n[2] = n[2] / sumvjs;
	}


	//第三次遍历1-邻接点：求对应的1阶边在切平面的投影
	float d = -(point->x*n[0] + point->y*n[1] + point->z*n[2]);
	te = point->edgef;
	float sumw = 0;
	struct arraylist *ts = NULL;   //这个指针头用于存放这个点的所有邻接w值
	while (true)
	{
		//求对应的1阶边在切平面的投影
		float dk = n[0] * te->end->x + n[1] * te->end->y + n[2] * te->end->z + d;
		float vjk[3] = { 0 }, vjk0[3] = { 0 }, vjk1[3] = { 0 };
		vjk[0] = te->end->x - dk*n[0] - te->start->x;
		vjk[1] = te->end->y - dk*n[1] - te->start->y;
		vjk[2] = te->end->z - dk*n[2] - te->start->z;
		//上一条边的投影
		struct edge * ne = point->edgef;
		if (te == point->edgef) //如果中间的边是第一个边的话，上一个边就是指针队尾
		while (true)
		{
			if (ne->next == NULL) break;
			else ne = ne->next;
		}
		else
		while (true)       //否则就是上一个指针
		{
			if (ne->next == te) break;
			else ne = ne->next;
		}
		float dk0 = n[0] * ne->end->x + n[1] * ne->end->y + n[2] * ne->end->z + d;
		vjk0[0] = ne->end->x - dk*n[0] - ne->start->x;
		vjk0[1] = ne->end->y - dk*n[1] - ne->start->y;
		vjk0[2] = ne->end->z - dk*n[2] - ne->start->z;
		//下一条边的投影
		if (te->next == NULL) ne = point->edgef; //如果中间的边是指针队尾的话，下一条边就是指针头
		else ne = te->next;                      //否则顺序指向下一条边
		float dk1 = n[0] * ne->end->x + n[1] * ne->end->y + n[2] * ne->end->z + d;
		vjk1[0] = ne->end->x - dk*n[0] - ne->start->x;
		vjk1[1] = ne->end->y - dk*n[1] - ne->start->y;
		vjk1[2] = ne->end->z - dk*n[2] - ne->start->z;
		//计算每个边的w值，并按照数据结构存入L（结构见struct arraylist）
		//float agl1 = acos((vjk[0] * vjk0[0] + vjk[1] * vjk0[1] + vjk[2] * vjk0[2]) / sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) / sqrt(pow(vjk0[0], 2) + pow(vjk0[1], 2) + pow(vjk0[2], 2)));
		//float agl2 = acos((vjk[0] * vjk1[0] + vjk[1] * vjk1[1] + vjk[2] * vjk1[2]) / sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) / sqrt(pow(vjk1[0], 2) + pow(vjk1[1], 2) + pow(vjk1[2], 2)));
		float agl1 = acos((vjk[0] * vjk0[0] + vjk[1] * vjk0[1] + vjk[2] * vjk0[2]) / (sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) * sqrt(pow(vjk0[0], 2) + pow(vjk0[1], 2) + pow(vjk0[2], 2))));
		float agl2 = acos((vjk[0] * vjk1[0] + vjk[1] * vjk1[1] + vjk[2] * vjk1[2]) / (sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) * sqrt(pow(vjk1[0], 2) + pow(vjk1[1], 2) + pow(vjk1[2], 2))));
		float w;

		float a1 = (vjk[0] * vjk0[0] + vjk[1] * vjk0[1] + vjk[2] * vjk0[2]) / (sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) * sqrt(pow(vjk0[0], 2) + pow(vjk0[1], 2) + pow(vjk0[2], 2)));
		float a2 = (vjk[0] * vjk1[0] + vjk[1] * vjk1[1] + vjk[2] * vjk1[2]) / (sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) * sqrt(pow(vjk1[0], 2) + pow(vjk1[1], 2) + pow(vjk1[2], 2)));
		//........................
		if ((sqrtf(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2)) != 0) && ((a1 >= -1) && (a1 <= 1)) && ((a2 >= -1) && (a2 <= 1)))
			w = (tan(agl1 / 2) + tan(agl2 / 2)) / sqrt(pow(vjk[0], 2) + pow(vjk[1], 2) + pow(vjk[2], 2));
		else
			w = 0.01;

		struct arraylist *ta = (struct arraylist*)malloc(sizeof(struct arraylist));
		ta->end = te->end;
		ta->l = w;
		sumw += w;
		//将结果存入数据结构中
		ta->next = ts;
		ts = ta;
		//循环结构体
		if (te->next == NULL) break;
		else te = te->next;
	}
	//计算每个点的权值和laplace坐标,存入数组中
	float  sumvjl[3] = { 0 };
	while (true)
	{
		float lam;
		if (sumw != 0)
			lam = ts->l / sumw;
		else
			lam = 0;

		L[point->id][ts->end->id] = -lam;//?
		sumvjl[0] += lam*ts->end->x;
		sumvjl[1] += lam*ts->end->y;
		sumvjl[2] += lam*ts->end->z;
		if (ts->next == NULL) break;
		else ts = ts->next;
	}
	L[point->id][point->id] = 1;//++
	point->laplace[0] = point->x - sumvjl[0];
	point->laplace[1] = point->y - sumvjl[1];
	point->laplace[2] = point->z - sumvjl[2];
	printf("解得点%d的拉普拉斯坐标：%4.3f,%4.3f,%4.3f\n", point->id, point->laplace[0], point->laplace[1], point->laplace[2]);
}

void MymeshFit::allMvcoding()//对整个网格进行编码
{
	////遍历网格进行MeanVealue编码变形----------------------------------------------------------------------------------------------------------
	for (int i = 0; i < countP; i++)
	{
		//if (abs(point[i]->x) + abs(point[i]->x) + abs(point[i]->x) < 1e-4) break;//?
		if (abs(point[i]->x) + abs(point[i]->y) + abs(point[i]->z) < 1e-4) break;
		mvcoding(point[i], arrayl);
	}
}

void MymeshFit::Laplacesmooth()//对网格进行laplace平滑
{
	laplacemv.clear();
	//遍历网格进行Laplace平滑-----------------------------------------------------------------------------------------------------------------
	printf("开始平滑\n");
	float p[MAX_SIZE][3];
	//float q[MAX_SIZE][3];
	float b[MAX_SIZE][3];
	for (int i = 0; i < countP; i++) //数组初始化
	{
		coor t;
		t.x = point[i]->x;
		t.y = point[i]->y;
		t.z= point[i]->z;
		laplacemv.push_back(t);
		p[i][0] = 0;
		p[i][1] = 0;
		p[i][2] = 0;
		b[i][0] = 0;
		b[i][1] = 0;
		b[i][2] = 0;
	}
	//printf("平滑前：%f,%f,%f\n", q[532][0], q[532][1], q[532][2]);
	for (int t = 0; t < REPEATTIME; t++)   //平滑的迭代次数 = REPEATTIME
	{
		for (int i = 0; i < laplacemv.size(); i++)
		{
			if (abs(laplacemv[i].x) < 1e-4 && abs(laplacemv[i].y) < 1e-4 && abs(laplacemv[i].z) < 1e-4) break;
			struct edge *te = point[i]->edgef;
			if (te == NULL)	 continue;
			float sum[3] = { 0 };
			int count = 0;
			while (true)
			{
				sum[0] += laplacemv[te->end->id].x;
				sum[1] += laplacemv[te->end->id].y;
				sum[2] += laplacemv[te->end->id].z;
				count++;
				if (te->next != NULL) te = te->next;
				else break;
			}
			p[i][0] = sum[0] / count;
			p[i][1] = sum[1] / count;
			p[i][2] = sum[2] / count;

			b[i][0] = p[i][0] - POB*point[i]->x - (1 - POB)*laplacemv[i].x;
			b[i][1] = p[i][1] - POB*point[i]->y - (1 - POB)*laplacemv[i].y;
			b[i][2] = p[i][2] - POB*point[i]->z - (1 - POB)*laplacemv[i].z;

		}

		for (int i = 0; i <laplacemv.size(); i++)
		{
			if (abs(laplacemv[i].x) < 1e-4 && abs(laplacemv[i].y) < 1e-4 && abs(laplacemv[i].z) < 1e-4) break;
			struct edge *te = point[i]->edgef;
			if (te == NULL)	 continue;
			float sum[3] = { 0 };
			int count = 0;
			while (true)
			{
				sum[0] += b[te->end->id][0];
				sum[1] += b[te->end->id][1];
				sum[2] += b[te->end->id][2];
				count++;
				if (te->next != NULL) te = te->next;
				else break;
			}
			laplacemv[i].x = p[i][0] - POP*b[i][0] - (1 - POP)*sum[0] / count;
			laplacemv[i].y = p[i][1] - POP*b[i][1] - (1 - POP)*sum[1] / count;
			laplacemv[i].z = p[i][2] - POP*b[i][2] - (1 - POP)*sum[2] / count;

			p[i][0] = 0;
			p[i][1] = 0;
			p[i][2] = 0;

			b[i][0] = 0;
			b[i][1] = 0;
			b[i][2] = 0;
		}
		//printf("第%d次变形：%f,%f,%f\n", t, q[532][0], q[532][1], q[532][2]);
	}
	//保存文件-------------------------------------------------------------------------------------------------------------------------
	/*
	fpnode = fopen("nodechanged.txt", "w+");
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (abs(q[i][0]) > 1e-4 || abs(q[i][1]) > 1e-4 || abs(q[i][2]) > 1e-4)     //浮点型与零比较
			fprintf(fpnode, "%4.1f %4.1f %4.1f \n", q[i][0], q[i][1], q[i][2]);
		else
		{
			printf("输出点个数%d", i);
			break;
		}
	}
	fclose(fpnode);
	*/
	return;
}

vector<coor> MymeshFit::getlsample()
{
	for (int i = 0; i < countP; i++)
	{
		//fprintf(fpnode, "%4.4f %4.4f %4.4f\n", point[i]->laplace[0], point[i]->laplace[1], point[i]->laplace[2]);
		coor t;
		t.x = point[i]->laplace[0];
		t.y = point[i]->laplace[1];
		t.z = point[i]->laplace[2];

		lsample.push_back(t);

	}

	for (int i = 0; i < lsample.size(); i++)
	{
		cout << lsample[i].x << " " << lsample[i].y << " " << lsample[i].z << endl;
	}

	//保存laplace坐标
	fpnode = fopen("laplacecoding.txt", "w+");
	for (int i = 0; i < lsample.size(); i++)
	{
		//if (abs(lsample[i].x) > 1e-4 || abs(lsample[i].y) > 1e-4 || abs(lsample[i].z) > 1e-4)     //浮点型与零比较
			fprintf(fpnode, "%4.4f %4.4f %4.4f\n", lsample[i].x, lsample[i].y, lsample[i].z);
		//else break;
	}
	fclose(fpnode);
	printf("读写完成");
	return lsample;

}

vector<float> MymeshFit::getL()
{
	for (int i = 0; i < countP; i++)
	{
		//for (int j = 0; j < MAX_SIZE; j++)
		for (int j = 0; j <countP; j++)
		{
			Lm.push_back(arrayl[i][j]);
		}
	}
	Lrow = countP;
	Lcol = countP;
	//保存L
	fpnode = fopen("arrayl.txt", "w+");
	for (int i = 0; i < countP; i++)
	{
		for (int j = 0; j < countP; j++)
		{
			fprintf(fpnode, "%4.3f ", arrayl[i][j]);
		}
		fprintf(fpnode, "\n");
	}
	fclose(fpnode);
	return Lm;
	
	

}

vector<coor> MymeshFit::getLaplacemv()
{
	return laplacemv;
}



