#pragma once
#include<iostream>
#include "math.h"
#include<vector>

using namespace std;
class matrix
{
public:
	matrix();
	void init(vector<float> a,vector<float> p,int m,int n);//初始化A和b
	void transpose(vector<float> a, int m, int n, vector<float> &b);//求矩阵a转置
	void trmul(vector<float> a, vector<float> b, int m, int n, int k, vector<float> &c);//矩阵相乘
	int chlk(vector<float> a, int n, int m, vector<float> &d);//平方根法（Cholesky法）求实正定对称方程组的解
	void fun(vector<float> r, vector<float> p, int m, int n);//求超定方程的最小二乘解
	vector<float> &getX();
	~matrix();
private:
	vector<float> A;//系数矩阵A
	int A_row;//A行数
	int A_col;//A列数
	vector<float> b;
	vector<float> x;//结果矩阵
	
};

