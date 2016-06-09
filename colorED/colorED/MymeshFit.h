#pragma once
#include <vector>
#include <iostream>
#include "global.h"

using namespace std;


#include "stdlib.h"
#include "stdio.h"
#include "math.h"

#define MAX_SIZE 2500
#define REPEATTIME 8
#define POB 0.3
#define POP 1.0

static float arrayl[MAX_SIZE][MAX_SIZE];



class MymeshFit
{
	
public:
	int Lrow;//矩阵L的行数
	int Lcol;//矩阵L的列数


	MymeshFit();
	void init(vector<coor> sample, vector<coor> tri);
	void buildmesh();//根据点和索引建立网格
	void initedge(struct node *s, struct node *e);  //用来建立网格结构的函数
	void mvcoding(struct node *point, float L[MAX_SIZE][MAX_SIZE]);   //用来求解稀疏矩阵L和laplace坐标的函数
	void allMvcoding();//对整个网格进行编码
	void Laplacesmooth();//对网格进行laplace平滑
	vector<coor> getlsample();
	vector<float> getL();
	vector<coor> getLaplacemv();
	~MymeshFit();
private:
	vector<coor> lsample;//样本点的laplace坐标集合
	vector<float> Lm;//矩阵L的向量形式

	vector<coor> laplacemv;//laplace平滑后的坐标
	
	int countP;//记录输入点的个数
	int countT;//记录面的个数
	FILE *fpnode, *fptri;//样本点文件和索引文件
	struct node *point[MAX_SIZE];//输入点
	int trimesh[MAX_SIZE][3];//索引
};

