#pragma once
#include <QVector>
#include <QPoint>
#include <vector>
#include "global.h"

using namespace std;

class FittingCurve
{
public:
	FittingCurve(vector<coor> points);
	~FittingCurve();
	double* getP();
	bool CurveFit();//曲线拟合

private:
	void Matrix_T(double *K, int m, int n, double *KT);//返回矩阵K的转置KT.k[m][n]
	void Matrix_Mul(double *Mul1, int Mul1_m, double *Mul2, int Mul2_n, int nm, double *Mul);////Mul1[Mul1_m][nm]*Mul2[nm][Mul2_n]=Mul即矩阵的乘法
	bool Matrix_LU(double *K, int n, double *L, double *U);//对方阵K进行LU分解.分解失败返回False.成功返回True以及分解得到的L与U
	bool Matrix_Inv(double *K, int n, double *InvK);//采用LU分解方法求方阵K的逆InvK,K[n][n]
	bool Matrix_Solve(double *K, double *B, int m, int n, double *x);//Kx=B求解x。K[m][n]。其结果返回最小二乘解,B[m][1]
	bool Matrix_PolyFit(double *x, double *y, int X_Y_Number, int Fit_N, double *ks);

	double* p=new double[3];//二次曲线系数
	vector<coor> vec;
};

