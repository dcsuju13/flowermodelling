#include "EdgeDetect.h"


EdgeDetect::EdgeDetect()
{
}

EdgeDetect::EdgeDetect(QPointF fcenter,int count)
{
	f_o = fcenter;
	areanumber = count;
}

EdgeDetect::~EdgeDetect()
{
}

Mat EdgeDetect::GradientCaculate(Mat GaussL, Mat ResultL, Mat Mask, int a)
{
	int height = GaussL.rows;
	int weight = GaussL.cols;
	Mat GaussLmasked;
	GaussL.copyTo(GaussLmasked, Mask);
	Mat_<Vec3f> gaussmetrixL = GaussLmasked;
	Mat VL(height, weight, CV_32FC3, Scalar(0));
	Mat HL(height, weight, CV_32FC3, Scalar(0));
	Mat ALLEdge(height, weight, CV_32FC1, Scalar(0));
	float delta[3][2] = { 0 };
	//Mat metrixH[3][3] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	//Mat metrixV[3][3] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	Mat metrixH(3, 3, CV_32FC1, Scalar(0));
	metrixH.at<float>(0, 0) = -0.125;
	metrixH.at<float>(0, 1) = 0;
	metrixH.at<float>(0, 2) = 0.125;
	metrixH.at<float>(1, 0) = -0.25;
	metrixH.at<float>(1, 1) = 0;
	metrixH.at<float>(1, 2) = 0.25;
	metrixH.at<float>(2, 0) = -0.125;
	metrixH.at<float>(2, 1) = 0;
	metrixH.at<float>(2, 2) = 0.125;
	Mat metrixV(3, 3, CV_32FC1, Scalar(0));
	metrixV.at<float>(0, 0) = 0.125;
	metrixV.at<float>(0, 1) = 0.25;
	metrixV.at<float>(0, 2) = 0.125;
	metrixV.at<float>(1, 0) = 0;
	metrixV.at<float>(1, 1) = 0;
	metrixV.at<float>(1, 2) = 0;
	metrixV.at<float>(2, 0) = -0.125;
	metrixV.at<float>(2, 1) = -0.25;
	metrixV.at<float>(2, 2) = -0.125;

	filter2D(GaussL, HL, 3, metrixH);
	filter2D(GaussL, VL, 3, metrixV);

	Mat HLmasked, VLmasked;
	HL.copyTo(HLmasked, Mask);
	VL.copyTo(VLmasked, Mask);

	Mat_<Vec3f> gaussmetrixHL = HLmasked;
	Mat_<Vec3f> gaussmetrixVL = VLmasked;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < weight; j++)
		if (gaussmetrixL(i, j)[0] != 0)
		{
			//for (int k = 0; k < 3; k++)
			//{
			//	float dx = 0;
			//	float dy = 0;
			//	float countx = 0;
			//	float county = 0;
			//	for (int m = -1; m <= 1; m++)
			//	for (int n = -1; n <= 1; n++)
			//	{
			//		if (i + m >= 0 && i + m < height && j + n >= 0 && j + n < weight)
			//		{
			//			/*dx += metrixH[m + 1][n + 1] * (gaussmetrix(i, j)[k] - gaussmetrix(i + m, j + n)[k]);
			//			countx += abs(metrixH[m + 1][n + 1]);
			//			dy += metrixV[m + 1][n + 1] * (gaussmetrix(i, j)[k] - gaussmetrix(i + m, j + n)[k]);
			//			county += abs(metrixV[m + 1][n + 1]);*/
			//		}
			//	}
			//	delta[k][0] = dx / countx;
			//	delta[k][1] = dy / county;
			//}
			float gxx = pow(gaussmetrixHL(i, j)[0], 2) + pow(gaussmetrixHL(i, j)[1], 2) + pow(gaussmetrixHL(i, j)[2], 2);
			float gyy = pow(gaussmetrixVL(i, j)[0], 2) + pow(gaussmetrixVL(i, j)[1], 2) + pow(gaussmetrixVL(i, j)[2], 2);
			float gxy = abs(gaussmetrixHL(i, j)[0] * gaussmetrixVL(i, j)[0]) + abs(gaussmetrixHL(i, j)[1] * gaussmetrixVL(i, j)[1]) + abs(gaussmetrixHL(i, j)[2] * gaussmetrixVL(i, j)[2]);
			float dir = 0.5 * atan(2 * gxy / abs(gxx - gyy));
			float gbp = 0.5*((gxx + gyy) + abs(gxx - gyy)*cos(2 * dir) + 2 * gxy*sin(2 * dir));
			float g = pow(gbp, 0.5);
			if (g > a)
				ResultL.at<float>(i, j) = ResultL.at<float>(i, j) + 1;
			ALLEdge.at<float>(i, j) = g;
		}
	}
	return ALLEdge;
}

vector<Vec2i> EdgeDetect::Walk(Vec3i StartP, int direct, Mat ResultL, Mat EdgeL, vector<Vec2i> theedge)
{
	int maxp = 0;
	int NewDir = 0;
	EdgeL.at<int>(StartP(0), StartP(1)) = 1;
	Vec2i thepoint = { StartP(0), StartP(1) };
	theedge.push_back(thepoint);
	for (int m = -1; m < 2; m++)
	for (int n = -1; n < 2; n++)
	{
		if (StartP(0) + m >= 0 && StartP(0) + m < EdgeL.rows && StartP(1) + n >= 0 && StartP(1) + n < EdgeL.cols)
		if (EdgeL.at<int>(StartP(0) + m, StartP(1) + n) == 0)
			EdgeL.at<int>(StartP(0) + m, StartP(1) + n) = -1;
	}
	switch (direct)
	{
	case 1:
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) - 1);
			NewDir = 1;
		}
		if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0), StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0), StartP(1) - 1);
			NewDir = 2;
		}
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1)) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1));
			NewDir = 4;
		}
		if (maxp == 0)
		{
			if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) + 1, StartP(1) - 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) - 1);
				NewDir = 3;
			}
			if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) - 1, StartP(1) + 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) + 1);
				NewDir = 7;
			}
		}
		break;
	case 2:
		if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0), StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0), StartP(1) - 1);
			NewDir = 2;
		}
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) - 1);
			NewDir = 3;
		}
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) - 1);
			NewDir = 1;
		}
		if (maxp == 0)
		{
			if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) + 1, StartP(1)) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) + 1, StartP(1));
				NewDir = 6;
			}
			if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) - 1, StartP(1)) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) - 1, StartP(1));
				NewDir = 4;
			}
		}
		break;
	case 3:
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) - 1);
			NewDir = 3;
		}
		if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0), StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0), StartP(1) - 1);
			NewDir = 2;
		}
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1)) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1));
			NewDir = 6;
		}
		if (maxp == 0)
		{
			if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) - 1, StartP(1) - 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) - 1);
				NewDir = 1;
			}
			if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) + 1, StartP(1) + 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) + 1);
				NewDir = 9;
			}
		}
		break;
	case 4:
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1)) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1));
			NewDir = 4;
		}
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) - 1);
			NewDir = 1;
		}
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) + 1);
			NewDir = 7;
		}
		if (maxp == 0)
		{
			if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0), StartP(1) + 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0), StartP(1) + 1);
				NewDir = 8;
			}
			if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0), StartP(1) - 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0), StartP(1) - 1);
				NewDir = 2;
			}
		}
		break;
	case 6:
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1)) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1));
			NewDir = 6;
		}
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1) - 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) - 1);
			NewDir = 3;
		}
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) + 1);
			NewDir = 9;
		}
		if (maxp == 0)
		{
			if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0), StartP(1) + 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0), StartP(1) + 1);
				NewDir = 8;
			}
			if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0), StartP(1) - 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0), StartP(1) - 1);
				NewDir = 2;
			}
		}
		break;
	case 7:
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) + 1);
			NewDir = 7;
		}
		if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0), StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0), StartP(1) + 1);
			NewDir = 8;
		}
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1)) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1));
			NewDir = 4;
		}
		if (maxp == 0)
		{
			if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) - 1, StartP(1) - 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) - 1);
				NewDir = 1;
			}
			if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) + 1, StartP(1) + 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) + 1);
				NewDir = 9;
			}
		}
		break;
	case 8:
		if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0), StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0), StartP(1) + 1);
			NewDir = 8;
		}
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) + 1);
			NewDir = 9;
		}
		if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) - 1, StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) + 1);
			NewDir = 7;
		}
		if (maxp == 0)
		{
			if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) + 1, StartP(1)) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) + 1, StartP(1));
				NewDir = 6;
			}
			if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) - 1, StartP(1)) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) - 1, StartP(1));
				NewDir = 4;
			}
		}
		break;
	case 9:
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) + 1);
			NewDir = 9;
		}
		if (StartP(0) >= 0 && StartP(0) < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
		if (ResultL.at<int>(StartP(0), StartP(1) + 1) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0), StartP(1) + 1);
			NewDir = 8;
		}
		if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) >= 0 && StartP(1) < EdgeL.cols)
		if (ResultL.at<int>(StartP(0) + 1, StartP(1)) > maxp)
		{
			maxp = ResultL.at<int>(StartP(0) + 1, StartP(1));
			NewDir = 6;
		}
		if (maxp == 0)
		{
			if (StartP(0) + 1 >= 0 && StartP(0) + 1 < EdgeL.rows && StartP(1) - 1 >= 0 && StartP(1) - 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) + 1, StartP(1) - 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) + 1, StartP(1) - 1);
				NewDir = 3;
			}
			if (StartP(0) - 1 >= 0 && StartP(0) - 1 < EdgeL.rows && StartP(1) + 1 >= 0 && StartP(1) + 1 < EdgeL.cols)
			if (ResultL.at<int>(StartP(0) - 1, StartP(1) + 1) > maxp)
			{
				maxp = ResultL.at<int>(StartP(0) - 1, StartP(1) + 1);
				NewDir = 7;
			}
		}
		break;
	default:
		break;
	}
	//������Χ����û�п����ߵĵ�
	vector<Vec2i> NewVecEdge;
	if (maxp != 0)  //Ҫ���еĻ��������ǲ����Ѿ��߹���
	{
		bool Walked = false;
		Vec3i NextWalk;
		NextWalk(2) = StartP(2) + 1;
		StartP(2)++;
		switch (NewDir)
		{
		case 1:
			if (EdgeL.at<int>(StartP(0) - 1, StartP(1) - 1) == 1) Walked = true;
			NextWalk(0) = StartP(0) - 1;
			NextWalk(1) = StartP(1) - 1;
			break;
		case 2:
			if (EdgeL.at<int>(StartP(0), StartP(1) - 1) == 1) Walked = true;
			NextWalk(0) = StartP(0);
			NextWalk(1) = StartP(1) - 1;
			break;
		case 3:
			if (EdgeL.at<int>(StartP(0) + 1, StartP(1) - 1) == 1) Walked = true;
			NextWalk(0) = StartP(0) + 1;
			NextWalk(1) = StartP(1) - 1;
			break;
		case 4:
			if (EdgeL.at<int>(StartP(0) - 1, StartP(1)) == 1) Walked = true;
			NextWalk(0) = StartP(0) - 1;
			NextWalk(1) = StartP(1);
			break;
		case 6:
			if (EdgeL.at<int>(StartP(0) + 1, StartP(1)) == 1) Walked = true;
			NextWalk(0) = StartP(0) + 1;
			NextWalk(1) = StartP(1);
			break;
		case 7:
			if (EdgeL.at<int>(StartP(0) - 1, StartP(1) + 1) == 1) Walked = true;
			NextWalk(0) = StartP(0) - 1;
			NextWalk(1) = StartP(1) + 1;
			break;
		case 8:
			if (EdgeL.at<int>(StartP(0), StartP(1) + 1) == 1) Walked = true;
			NextWalk(0) = StartP(0);
			NextWalk(1) = StartP(1) + 1;
			break;
		case 9:
			if (EdgeL.at<int>(StartP(0) + 1, StartP(1) + 1) == 1) Walked = true;
			NextWalk(0) = StartP(0) + 1;
			NextWalk(1) = StartP(1) + 1;
			break;
		default:
			break;
		}

		if (Walked == false)  NewVecEdge = Walk(NextWalk, NewDir, ResultL, EdgeL, theedge);      // �����û�߹����͵ݹ�һ��
		else NewVecEdge = theedge;
	}
	else
	{
		NewVecEdge = theedge;
	}
	return NewVecEdge;
}

Mat EdgeDetect::WalkEnd(Mat EdgeL, Mat ResultL)
{
	for (int i = 0; i < EdgeL.rows; i++)
	for (int j = 0; j < EdgeL.cols; j++)
	if (EdgeL.at<int>(i, j) == -1)
	{
		ResultL.at<int>(i, j) = 0;
		EdgeL.at<int>(i, j) = 0;
	}
	return ResultL;
}

vector<Vec2i> EdgeDetect::ConnectNode(int type, vector<Vec2i> TargrtChain, vector<Vec2i> EdgeChain, Mat EdgeL)
{
	if (type == 0) // ���type = 0,targetchain����һ��;
	{
		vector<Vec2i> TargetCopy = TargrtChain;
		for (int i = 0; i < TargrtChain.size(); i++)
			TargrtChain[i] = TargetCopy[TargrtChain.size() - 1 - i];
	}
	Vec2i T = TargrtChain[0], S = EdgeChain[EdgeChain.size() - 1];

	if (T != EdgeChain[0] && EdgeChain.size() != 1 && TargrtChain.size() != 1)
	{
		float mindistpre = powf(powf(T(0) - S(0), 2) + powf(T(1) - S(1), 2), 0.5);
		int mindistpreid = EdgeChain.size() - 1;
		for (int i = EdgeChain.size() - 1; i >= 0; i--)   //Ѱ�Ҿ���Ŀ��chain����ĵ㿪ʼ���ӣ���ɾ������ĵ�
		{
			float ndistpre = powf(powf(T(0) - EdgeChain[i](0), 2) + powf(T(1) - EdgeChain[i](1), 2), 0.5);
			if (ndistpre < mindistpre)
			{
				mindistpre = ndistpre;
				mindistpreid = i;
			}
		}
		for (int i = EdgeChain.size() - 1; i > mindistpreid; i--)
			EdgeChain.erase(EdgeChain.begin() + i);
		S = EdgeChain[mindistpreid];
	}

	int startnumber = 0;  //targetchain��ʼ��λ��
	//������ʼ�����ֹ��
	Vec2i np = S;
	float argKy = T(1) - S(1);
	float argKx = T(0) - S(0);
	float argKe = argKy / argKx;
	float argK = atan(argKe);
	while (np != T)
	{
		float mindist = powf(powf(T(0) - np(0), 2) + powf(T(1) - np(1), 2), 0.5);
		float minargK = 100;
		int npx = np(0), npy = np(1);
		for (int i = -1; i <= 1; i++) // �����߹�ȥ,�Ƕ���ӽ��Ҿ���������
			for (int j = -1; j <= 1; j++)
			{
				float ndist = powf(powf(T(0) - (npx + i), 2) + powf(T(1) - (npy + j), 2), 0.5);
				if (npx + i != T(0) || npy + j != T(1))
				{
					float argP;
					float argPy = T(1) - npy - j;
					float argPx = T(0) - npx - i;
					float argPe = argPy / argPx;
					if (T(0) == npx + i)
					{
						if (T(1) < npy + j)
							argP = 3.1416926 / 2;
						else
							argP = 3.1415926 / 2 * 3;
					}
					else
						argP = atan(argPe);
					float argdelta = abs(argP - argK);
					if (ndist < mindist && argdelta < minargK)
					{
						minargK = argdelta;
						np = { npx + i, npy + j };
					}
				}
				else
				{
					np = T;
					break;
				}
			}

		if (EdgeL.at<int>(np(0), np(1)) == 1)
			for (int i = 0; i < TargrtChain.size(); i++)
				if (TargrtChain[i] == np)
					startnumber = i;

		if (startnumber != 0)
			break;
		else
			EdgeChain.push_back(np);
	}
	//��Ŀ������ӵ���Ե����
	for (int i = startnumber; i < TargrtChain.size(); i++)
	{
		EdgeChain.push_back(TargrtChain[i]);
	}

	return EdgeChain;
}

vector<Vec2i> EdgeDetect::CEDContours(Mat ResultL, int areaunmber, Vec2i CircleCentre)
{
	int height = ResultL.rows;
	int weight = ResultL.cols;

	Mat DirectL(height, weight, CV_32SC1, Scalar(0));
	Mat EdgeL(height, weight, CV_32SC1, Scalar(0));
	Mat OutPut(height, weight, CV_32SC1, Scalar(0));
	vector<vector<Vec2i>> edgepoint;
	vector<Vec3i> keypoint;
	//��ȷ��ê�㣬��ê�����һ�������У����Ӵ�С����-------------------------------------------------------------------------
	for (int i = 0; i < height; i++)
	for (int j = 0; j < weight; j++)
	{
		if (ResultL.at<int>(i, j) > 16)
		{
			//ȷ�������ķ��򣬴洢�ھ����У�����㣺0�����£�1�����ң�2�����ϣ�3�����ϣ�4��
			int np = 0;
			int nm = 0, nn = 0;
			for (int m = -1; m <= 1; m++)
			for (int n = -1; n <= 1; n++)
			{
				if (m != 0 || n != 0)
				if (i + m >= 0 && i + m < height && j + n >= 0 && j + n < weight)
				if (ResultL.at<int>(i + m, j + n) > np)
				{
					np = ResultL.at<int>(i + m, j + n);
					nm = m;
					nn = n;
				}
			}
			if (np == 0) DirectL.at<int>(i, j) = 0;
			else if (nm == 0) DirectL.at<int>(i, j) = 1;
			else if (nn == 0) DirectL.at<int>(i, j) = 2;
			else if (nm*nn == -1) DirectL.at<int>(i, j) = 3;
			else DirectL.at<int>(i, j) = 4;

			//����������Ӧ����������㣬�ֱ����ǵķ����Ƿ���ͬ
			int np1 = 0, np2 = 0;
			int nm1 = 0, nn1 = 0, nm2 = 0, nn2 = 0;
			for (int m = -1; m <= 1; m++)
			for (int n = -1; n <= 1; n++)
			{
				if (m != 0 || n != 0)
				{
					if (i + m + nm >= 0 && i + m + nm < height && j + n + nn >= 0 && j + n + nn < weight)
					if (ResultL.at<int>(i + nm + m, j + nn + n) > np1)
					{
						np1 = ResultL.at<int>(i + nm + m, j + nn + n);
						nm1 = m;
						nn1 = n;
					}
					if (i + m - nm >= 0 && i + m - nm < height && j + n - nn >= 0 && j + n - nn < weight)
					if (ResultL.at<int>(i - nm + m, j - nn + n) > np2)
					{
						np2 = ResultL.at<int>(i - nm + m, j - nn + n);
						nm2 = m;
						nn2 = n;
					}
				}
			}


			//���������ͬ�Ļ����ͽ���������ê��������
			if ((nm == nm1 && nn == nn1) || (nm == -nm1 && nn == -nn1))
			if ((nm == nm2 && nn == nn2) || (nm == -nm2 && nn == -nn2))
			{
				if (keypoint.size() != 0 && keypoint[keypoint.size() - 1][2] < ResultL.at<int>(i, j))
				{
					for (int n = 0; n < keypoint.size(); n++)
					if (keypoint[n][2] < ResultL.at<int>(i, j))
					{
						Vec3i a;
						a[0] = i;
						a[1] = j;
						a[2] = ResultL.at<int>(i, j);
						keypoint.insert(keypoint.begin() + n, a);
						break;
					}
				}
				else
				{
					Vec3i a;
					a[0] = i;
					a[1] = j;
					a[2] = ResultL.at<int>(i, j);
					keypoint.push_back(a);
				}
			}
		}
		else DirectL.at<int>(i, j) = 0;
	}
	//���ζ�ȡê�㣬��沢�����Ե��Ϣ������Ϣ�Ͷ�Ӧ�Ľڵ㡢���ȣ�������ɺ�Ӷ�����ɾ�����ê�㣨ѭ��ֱ�����е�ê�����Ϊ�գ�
	while (!keypoint.empty())
	{
		//��ȡê��,����ê����û�б��߹����߹�ֱ��ɾ�ˣ���һ��
		int positionx = keypoint[0][0];
		int positiony = keypoint[0][1];
		if (EdgeL.at<int>(positionx, positiony) == 1 || ResultL.at<int>(positionx, positiony) == 0)
		{
			keypoint.erase(keypoint.begin());
			//printf("ê�������%d\n", keypoint.size());
			continue;
		}
		//��ȡ����������������
		int dir = DirectL.at<int>(positionx, positiony);
		int rdir = 0, ldir = 0;
		Vec3i rightc, leftc;
		switch (dir)
		{
		case 1:
			rightc(0) = positionx;
			rightc(1) = positiony + 1;
			rightc(2) = 0;
			leftc(0) = positionx;
			leftc(1) = positiony - 1;
			leftc(2) = 0;
			rdir = 8;
			ldir = 2;
			break;
		case 2:
			rightc(0) = positionx + 1;
			rightc(1) = positiony;
			rightc(2) = 0;
			leftc(0) = positionx - 1;
			leftc(1) = positiony;
			leftc(2) = 0;
			rdir = 6;
			ldir = 4;
			break;
		case 3:
			rightc(0) = positionx + 1;
			rightc(1) = positiony - 1;
			rightc(2) = 0;
			leftc(0) = positionx - 1;
			leftc(1) = positiony + 1;
			leftc(2) = 0;
			rdir = 3;
			ldir = 7;
			break;
		case 4:
			rightc(0) = positionx + 1;
			rightc(1) = positiony + 1;
			rightc(2) = 0;
			leftc(0) = positionx - 1;
			leftc(1) = positiony - 1;
			leftc(2) = 0;
			rdir = 9;
			ldir = 1;
			break;
		default:
			break;
		}
		EdgeL.at<int>(positionx, positiony) = 1;
		Vec3i rightnode = { positionx, positiony, 0 };
		Vec3i leftnode = { positionx, positiony, 0 };
		//����walk����
		vector<Vec2i> rightedge, leftedge, theedge;
		if (rightc(0) >= 0 && rightc(0) < EdgeL.rows && rightc(1) >= 0 && rightc(1) < EdgeL.cols)
			rightedge = Walk(rightc, rdir, ResultL, EdgeL, rightedge);
		if (leftc(0) >= 0 && leftc(0) < EdgeL.rows && leftc(1) >= 0 && leftc(1) < EdgeL.cols)
			leftedge = Walk(leftc, ldir, ResultL, EdgeL, leftedge);
		ResultL = WalkEnd(EdgeL, ResultL);
		//�����ұߺϳ�Ϊһ����
		for (int i = leftedge.size() - 1; i >= 0; i--)
		{
			theedge.push_back(leftedge[i]);
		}
		Vec2i midnode = { positionx, positiony };
		theedge.push_back(midnode);
		for (int i = 0; i < rightedge.size(); i++)
		{
			theedge.push_back(rightedge[i]);
		}
		//printf(" %d + %d = %d", rightedge.size(), leftedge.size(), theedge.size());
		//���ݷ���ֵ���������Ϣ
		edgepoint.push_back(theedge);
		//ɾ��ê��
		keypoint.erase(keypoint.begin());////////////////////////////////////////////////////////
	//	printf("ê�������%d\n", keypoint.size());
	}

	//���ս׶Σ�ɾ��������
	int sigma = 0;
	for (int i = 0; i < edgepoint.size(); i++)
	{
		int edgesize = edgepoint[i].size();
		sigma += edgesize;
	}
	//printf("%d���������:%d", max, edgepoint[max].size());
	sigma = sigma / edgepoint.size();
	//vector<Vec2i> EdgeChain = edgepoint[max];
	//edgepoint.erase(edgepoint.begin() + max);

	//printf("sigema ��%d ", sigma);
	for (int i = 0; i < edgepoint.size(); i++)
	{
		if (edgepoint[i].size() < sigma + 25)
		{
			edgepoint.erase(edgepoint.begin() + i);
			i = i-1;
		}
	}

	int max = 0;
	for (int i = 0; i < edgepoint.size(); i++)
	{
		if (edgepoint[i].size() > edgepoint[max].size())
		{
			max = i;
		}
	}

	EdgeL = 0;
	for (int i = 0; i < edgepoint.size(); i++)
		for (int j = 0; j < edgepoint[i].size(); j++)
			EdgeL.at<int>(edgepoint[i][j](0), edgepoint[i][j](1)) = 1;
	imwrite("EdgeLTEST.jpg", EdgeL * 255);

	//��ʼ����
	////test-------------------------------------------------
	//for (int i = 0; i < EdgeChain.size(); i++)
	//{
	//	OutPut.at<int>(EdgeChain[i](0), EdgeChain[i](1)) = 1;
	//}
	//imwrite("Test.jpg", OutPut * 255);

	vector<Vec2i> EdgeChain;
	if (areaunmber == 1)  //���������߼�-��·
	{
		EdgeChain = edgepoint[max];
		OutPut = 0;
		for (int i = 0; i < EdgeChain.size(); i++)
		{
			OutPut.at<int>(EdgeChain[i](0), EdgeChain[i](1)) = 1;
		}
		imwrite("OutPut.jpg", OutPut * 255);
		edgepoint.erase(edgepoint.begin() + max);

		//printf("sigema ��%d ", sigma);
		for (int i = 0; i < edgepoint.size(); i++)
		{
			if (edgepoint[i].size() < sigma)
			{
				edgepoint.erase(edgepoint.begin() + i);
				i = i - 1;
				//printf("ʣ��߸���: %d \n", edgepoint.size());
			}
		}

		while (!edgepoint.empty())
		{
			int Minid = -1;
			int type = 0;
			//printf("S:%d,%d  E:%d,%d", EdgeChain[0](0), EdgeChain[0](1), EdgeChain[EdgeChain.size() - 1](0), EdgeChain[EdgeChain.size() - 1](1));
			float edgechaindist = powf(powf(EdgeChain[0](0) - EdgeChain[EdgeChain.size() - 1](0), 2) + powf(EdgeChain[0](1) - EdgeChain[EdgeChain.size() - 1](1), 2), 0.5);
			//���������ڵ��У��������Ľڵ㣬ͬʱ��¼������ıߵĳ���
			int Maxsize = 0;
			for (int s = 1; s * 25 < min(weight, height); s++) // ������������Χ
			{
				for (int i = 0; i < edgepoint.size(); i++)
				{
					Vec2i SP = EdgeChain[EdgeChain.size() - 1];
					Vec2i TP1 = edgepoint[i][0];
					Vec2i TP2 = edgepoint[i][edgepoint[i].size() - 1];
					float dist1 = powf(powf(SP(0) - TP1(0), 2) + powf(SP(1) - TP1(1), 2), 0.5);
					float dist2 = powf(powf(SP(0) - TP2(0), 2) + powf(SP(1) - TP2(1), 2), 0.5);
					float dist = min(dist1, dist2);
					float chaindist = powf(powf(EdgeChain[0](0) - EdgeChain[EdgeChain.size() - 1](0), 2) + powf(EdgeChain[0](1) - EdgeChain[EdgeChain.size() - 1](1), 2), 0.5);
					int size = edgepoint[i].size();
					if (chaindist > 25)
					{
						if (dist < s * 25 && dist <  edgechaindist)
						{
							if (size > Maxsize)
							{
								if (dist1 == min(dist1, dist2))
								{
									type = 1;
								}
								else type = 0;
								Maxsize = size;
								Minid = i;
							}
						}
					}
				}
				if (Minid != -1)
					break;
			}
			//������Ȩֵ�еľ���С�ڿ�ʼ�ڵ�ͽ����ڵ�ľ��룬����
			if (Minid == -1)
			{
				vector<Vec2i> EndChain;
				EndChain.push_back(EdgeChain[0]);
				EdgeChain = ConnectNode(1, EndChain, EdgeChain, EdgeL);
				break;
			}
			else
			{
				EdgeChain = ConnectNode(type, edgepoint[Minid], EdgeChain, EdgeL);
				edgepoint.erase(edgepoint.begin() + Minid);
			}
			////���ӽڵ㣬��·�������е���ӣ�������м������ߣ������ǲ���Ŀ��ߣ��ǵĻ��ʹ����￪ʼ��
			OutPut = 0;
			for (int i = 0; i < EdgeChain.size(); i++)
			{
				OutPut.at<int>(EdgeChain[i](0), EdgeChain[i](1)) = 1;
			}
			imwrite("OutPut.jpg", OutPut * 255);
		}
	}
	else //��������߼�-�����
	{
		Vec2i PreEdgeP1, PreEdgeP2;
		//ѭ��-��Ե������
		Vec2i SP1, SP2;
		for (int i = 0; i < areaunmber; i++)
		{
			//��ȡ����λ��
			string prename = "edgearea", typen = ".jpg", id;
			stringstream ids;
			ids << i;
			ids >> id;
			string allfilename = prename + id + typen;
			Mat maskM = imread(allfilename);
			Mat maskMG;
			maskMG = 0;
			cvtColor(maskM, maskMG, CV_RGB2GRAY);
			maskMG.convertTo(maskMG, CV_32FC1);
			if (id == "1")
			{
				imwrite("maskMG.jpg", maskMG);
			}

			//��������߶�
			OutPut = 0;
			for (int j = 0; j < edgepoint.size(); j++)
				for (int k = 0; k < edgepoint[j].size(); k++)
					OutPut.at<float>(edgepoint[j][k][0], edgepoint[j][k][1]) = 1;
			imwrite("OutPut.jpg", OutPut);
			//ɸѡ�߶�
			vector<vector<Vec2i>> MKP;
			for (int j = 0; j < edgepoint.size(); j++)
			{
				//��ÿ���߶α������е㣬�����������е����߶β�����MKP��
				bool line = false;
				vector<Vec2i> mkpline;
				for (int k = 0; k < edgepoint[j].size(); k++)
				{
					if (maskMG.at<float>(edgepoint[j][k][0], edgepoint[j][k][1]) != 0)
					{
						line = true;
						mkpline.push_back(edgepoint[j][k]);
					}
					else if (line == true && mkpline.size() > 20)
						break;
					else if (line == true)
					{
						mkpline.clear();
					}
				}
				if (mkpline.size() != 0)
					MKP.push_back(mkpline);
				/*if (maskMG.at<float>(edgepoint[j][0][0], edgepoint[j][0][1]) != 0 && maskMG.at<float>(edgepoint[j][edgepoint[j].size() - 1][0], edgepoint[j][edgepoint[j].size() - 1][1]) != 0)
					MKP.push_back(edgepoint[j]);*/
			}
			//���ɸѡ����߶μ���
			OutPut = 0;
			for (int j = 0; j < MKP.size();j++)
			for (int k = 0; k < MKP[j].size(); k++)
				OutPut.at<int>(MKP[j][k][0], MKP[j][k][1]) = 1;
			imwrite("OutPut0.jpg", OutPut*255);
			//��λ�����
			vector<Vec2i> MinEdge1 = MKP[0];
			float PCdist = sqrt(pow(MinEdge1[0][0] - CircleCentre[0], 2) + pow(MinEdge1[0][1] - CircleCentre[1], 2));
			int minid = 0;
			for (int j = 0; j < MKP.size(); j++)
			{
				float Tdist1 = sqrt(pow(MKP[j][0][0] - CircleCentre[0], 2) + pow(MKP[j][0][1] - CircleCentre[1], 2));
				float Tdist2 = sqrt(pow(MKP[j][MKP[j].size() - 1][0] - CircleCentre[0], 2) + pow(MKP[j][MKP[j].size() - 1][1] - CircleCentre[1], 2));
				if (Tdist1 < PCdist || Tdist2 < PCdist)
				{
					PCdist = min(Tdist1, Tdist2);
					MinEdge1 = MKP[j];
					minid = j;
				}
			}
			MKP.erase(MKP.begin() + minid);
			//MinEdge1 ��������
			float MEdist1 = sqrt(pow(MinEdge1[0][0] - CircleCentre[0], 2) + pow(MinEdge1[0][1] - CircleCentre[1], 2));
			float MEdist2 = sqrt(pow(MinEdge1[MinEdge1.size() - 1][0] - CircleCentre[0], 2) + pow(MinEdge1[MinEdge1.size() - 1][1] - CircleCentre[1], 2));
			if (MEdist1 > MEdist2)
			{
				vector<Vec2i> TEMPv;
				for (int i = 1; i <= MinEdge1.size(); i++)
				{
					TEMPv.push_back(MinEdge1[MinEdge1.size() - i]);
				}
				MinEdge1 = TEMPv;
			}
			//��λ��һ�ߵ������
			if (MKP.size() != 0)
			{
				minid = 0;
				vector<Vec2i> MinEdge2 = MKP[0];
				PCdist = sqrt(pow(MinEdge2[0][0] - CircleCentre[0], 2) + pow(MinEdge2[0][1] - CircleCentre[1], 2));
				int bm = -1;
				Vec2i TargetP;
				for (int s = 50; s > 20; s = s - 20)
				{
					for (int j = 0; j < MKP.size(); j++)
					{
						float PPdist1 = sqrt(pow(MKP[j][0][0] - MinEdge1[0][0], 2) + pow(MKP[j][0][1] - MinEdge1[0][1], 2));
						float PPdist2 = sqrt(pow(MKP[j][MKP[j].size() - 1][0] - MinEdge1[0][0], 2) + pow(MKP[j][MKP[j].size() - 1][1] - MinEdge1[0][1], 2));
						if (PPdist1 > s || PPdist2 > s)
						{
							float Tdist;
							float Tdist1 = sqrt(pow(MKP[j][0][0] - CircleCentre[0], 2) + pow(MKP[j][0][1] - CircleCentre[1], 2));
							float Tdist2 = sqrt(pow(MKP[j][MKP[j].size() - 1][0] - CircleCentre[0], 2) + pow(MKP[j][MKP[j].size() - 1][1] - CircleCentre[1], 2));
							if (PPdist1 > s && PPdist2 > s)
								Tdist = min(Tdist1, Tdist2);
							else if (PPdist1 > s)
								Tdist = Tdist1;
							else if (PPdist2 > s)
								Tdist = Tdist2;
							if (Tdist < PCdist)
							{
								PCdist = Tdist;
								MinEdge2 = MKP[j];
								minid = j;
								if (Tdist1 > Tdist2)
									bm = 1;
								else
									bm = 0;
							}
						}
					}
					if (bm == 1)
					{
						TargetP = MinEdge2[0];
						break;
					}
					else if (bm == 0)
					{
						TargetP = MinEdge2[MinEdge2.size() - 1];
						break;
					}
				}
				if (bm == -1)
				{
					TargetP = MinEdge2[MinEdge2.size() - 1];
				}
				//���ԣ��۲�õ��������
				OutPut = 0;
				for (int j = 0; j < MinEdge1.size(); j++)
					OutPut.at<int>(MinEdge1[j](0), MinEdge1[j](1)) = 1;
				for (int j = 0; j < MinEdge2.size(); j++)
					OutPut.at<int>(MinEdge2[j](0), MinEdge2[j](1)) = 1;
				imwrite("OutPut.jpg", OutPut*255);
				//�жϣ����MinEdge1�����˾��붼С��MinEdge2��Minedge1���ȳ���150����ôֻʹ��Minedge1������������
				float MD1 = sqrt(pow(MinEdge1[MinEdge1.size() - 1][0] - CircleCentre[0], 2) + pow(MinEdge1[MinEdge1.size() - 1][1] - CircleCentre[1], 2));
				float MD21 = sqrt(pow(MinEdge2[0][0] - CircleCentre[0], 2) + pow(MinEdge2[0][1] - CircleCentre[1], 2));
				float MD22 = sqrt(pow(MinEdge2[MinEdge2.size() - 1][0] - CircleCentre[0], 2) + pow(MinEdge2[MinEdge2.size() - 1][1] - CircleCentre[1], 2));
				//���������ڵ��У��������Ľڵ㣬ͬʱ��¼������ıߵĳ���
				if (!((min(MD21, MD22) > MD1 && MinEdge1.size() > 150) || (MinEdge1.size() > 150 && MinEdge2.size() < 20)))
					while (MKP.size() > 0)
					{
						int Maxsize = 0;
						int type = 0, Minid = -1;
						for (int s = 1; s * 25 < min(weight, height); s++) // ������������Χ
						{
							for (int i = 0; i < MKP.size(); i++)
							{
								Vec2i SP = MinEdge1[MinEdge1.size() - 1];
								Vec2i TP1 = MKP[i][0];
								Vec2i TP2 = MKP[i][MKP[i].size() - 1];
								float dist1 = powf(powf(SP(0) - TP1(0), 2) + powf(SP(1) - TP1(1), 2), 0.5);
								float dist2 = powf(powf(SP(0) - TP2(0), 2) + powf(SP(1) - TP2(1), 2), 0.5);
								float dist = min(dist1, dist2);
								float targetdist = powf(powf(TargetP(0) - MinEdge1[MinEdge1.size() - 1](0), 2) + powf(TargetP(1) - MinEdge1[MinEdge1.size() - 1](1), 2), 0.5);
								int size = MKP[i].size();
								if (targetdist > 25) //������Χ�ڳ��������߶�
								{
									if (dist < s * 25)
									{
										if (size > Maxsize)
										{
											if (dist1 == min(dist1, dist2))
											{
												type = 1;
											}
											else
											{
												type = 0;
											}
											Maxsize = size;
											Minid = i;
										}
									}
								}
							}
							if (Minid != -1)
								break;
						}
						//������Ȩֵ�еľ���С�ڿ�ʼ�ڵ�ͽ����ڵ�ľ��룬����
						if (Minid == -1 || MKP[minid] == MinEdge2)
						{
							float PPdistt1 = sqrt(pow(MinEdge2[0][0] - MinEdge1[MinEdge1.size() - 1][0], 2) + pow(MinEdge2[0][1] - MinEdge1[MinEdge1.size() - 1][1], 2));
							float PPdistt2 = sqrt(pow(MinEdge2[MinEdge2.size() - 1][0] - MinEdge1[MinEdge1.size() - 1][0], 2) + pow(MinEdge2[MinEdge2.size() - 1][1] - MinEdge1[MinEdge1.size() - 1][1], 2));
							int connecttype = 0;
							if (PPdistt1 > PPdistt2)
								connecttype = 0;
							else
								connecttype = 1;
							MinEdge1 = ConnectNode(connecttype, MinEdge2, MinEdge1, EdgeL);
							break;
						}
						else
						{
							MinEdge1 = ConnectNode(type, MKP[Minid], MinEdge1, EdgeL);
							MKP.erase(MKP.begin() + Minid);
						}
					}
			}
			//for (int i = 0; i < MinEdge1.size(); i++)
			//{
			//	OutPut.at<int>(MinEdge1[i](0), MinEdge1[i](1)) = 1;
			//}
			//imwrite("OutPut.jpg", OutPut * 255);
			//��MinEdge1�������ӵ�����
			SP1 = MinEdge1[0];
			SP2 = MinEdge1[MinEdge1.size() - 1];
			float K = SP1[1] / SP1[0];
			if (EdgeChain.empty())
			{
				PreEdgeP1 = MinEdge1[0];
				PreEdgeP2 = MinEdge1[MinEdge1.size() - 1];
			}
			else
			{
				Vec2i NEdgeP1 = MinEdge1[0];
				Vec2i NEdgeP2 = MinEdge1[MinEdge1.size() - 1];
				float PreDist1 = sqrt(pow(NEdgeP1[0] - PreEdgeP1[0], 2) + pow(NEdgeP1[1] - PreEdgeP1[1], 2));
				float PreDist2 = sqrt(pow(NEdgeP2[0] - PreEdgeP1[0], 2) + pow(NEdgeP2[1] - PreEdgeP1[1], 2));
				float PreDist3 = sqrt(pow(NEdgeP1[0] - PreEdgeP2[0], 2) + pow(NEdgeP1[1] - PreEdgeP2[1], 2));

				if (PreDist1 > PreDist2)
				{
					vector<Vec2i> Temp1 = MinEdge1;
					for (int i = 0; i < MinEdge1.size(); i++)
					{
						MinEdge1[i] = Temp1[MinEdge1.size() - 1 - i];
					}
				}
				if (PreDist1 < PreDist3)
				{
					vector<Vec2i> Temp1 = EdgeChain;
					for (int i = 0; i < EdgeChain.size(); i++)
					{
						EdgeChain[i] = Temp1[EdgeChain.size() - 1 - i];
					}
				}

			}
			MinEdge1.erase(MinEdge1.begin() + MinEdge1.size() - 1);

			vector<Vec2i> CC;
			CC.push_back(CircleCentre);
			for (int j = 0; j < MinEdge1.size(); j++)
				OutPut.at<float>(MinEdge1[j][0], MinEdge1[j][1]) = 1;
			imwrite("OutPut.jpg", OutPut);
			MinEdge1 = ConnectNode(1, CC, MinEdge1, EdgeL);
			//MinEdge1.erase(MinEdge1.begin() + MinEdge1.size() - 1);
			for (int j = 0; j < MinEdge1.size(); j++)
				OutPut.at<float>(MinEdge1[j][0], MinEdge1[j][1]) = 1;
			imwrite("OutPut.jpg", OutPut);
			MinEdge1 = ConnectNode(1, MinEdge1, CC, EdgeL);
			//MinEdge1.erase(MinEdge1.begin());
			for (int j = 0; j < MinEdge1.size(); j++)
				OutPut.at<float>(MinEdge1[j][0], MinEdge1[j][1]) = 1;
			imwrite("OutPut.jpg", OutPut);
			//���������Ե
			if (EdgeChain.empty())
			{
				EdgeChain = MinEdge1;
				//EdgeChain.erase(EdgeChain.begin() + EdgeChain.size() - 1);
			}
			else
			{
				for (int j = 0; j < MinEdge1.size() - 1; j++)
					EdgeChain.push_back(MinEdge1[j]);
			}
			MKP.clear();
		}

	}
	//���ԣ��������edgechain����˳��Բ���
	/*OutPut = 0;*/
	//for (int i = 0; i * 50 < EdgeChain.size(); i++)
	//{
	//	for (int j = 0; j < 50;j++)
	//		OutPut.at<float>(EdgeChain[i*50+j][0], EdgeChain[i*50+j][1]) = 1;
	//	imwrite("Edgechain.jpg", OutPut);
	//}
	return EdgeChain;
}

vector<Vec3i> EdgeDetect::GetKeyPoint(vector<Vec2i> EdgeChain, Vec2i CircleCentre)
{
	vector<Vec3i> keypoint;
	int countup, countdown = 0;
	int size = EdgeChain.size();
	//��ʼ�����У���¼ǰ��50����ͺ���50����,���������ظ�һ��
	vector<Vec2i> pre25p, next25p;
	for (int i = EdgeChain.size() - 50; i < EdgeChain.size(); i++)
		pre25p.push_back(EdgeChain[i]);
	for (int i = 1; i < 51; i++)
		next25p.push_back(EdgeChain[i]);
	for (int i = 0; i < size; i++)
		EdgeChain.push_back(EdgeChain[i]);
	//˳�����,�������ڵĵ��ǲ������еļ�ֵ,��ˢ�����е�Ԫ��
	bool up = false, down = false;
	for (int i = 0; i < size; i++)
	{
		bool maxpre = true, minpre = true, maxnext = true, minnext = true;
		float dist = powf(powf(EdgeChain[i](0) - CircleCentre(0), 2) + powf(EdgeChain[i](1) - CircleCentre(1), 2), 0.5);
		for (int j = 0; j < 50; j++)
		{
			float distpre = powf(powf(pre25p[j](0) - CircleCentre(0), 2) + powf(pre25p[j](1) - CircleCentre(1), 2), 0.5);
			float distnext = powf(powf(next25p[j](0) - CircleCentre(0), 2) + powf(next25p[j](1) - CircleCentre(1), 2), 0.5);
			if (distpre > dist)  maxpre = false;
			if (distpre < dist)  minpre = false;
			if (distnext > dist)  maxnext = false;
			if (distnext < dist)  minnext = false;
		}

		if (maxpre == true && maxnext == true && up == false)
		{
			Vec3i upkeypoint = { EdgeChain[i](0), EdgeChain[i](1), 1 };
			keypoint.push_back(upkeypoint);
			up = true;
			down = false;
		}

		if (EdgeChain[i][0] == CircleCentre[0] && EdgeChain[i][1] == CircleCentre[1] && down == false)
		{
			Vec3i downkeypoint{ EdgeChain[i](0), EdgeChain[i](1), 0 };
			keypoint.push_back(downkeypoint);
			down = true;
			up = false;
		}

		if (minpre == true && minnext == true && down == false)
		{
			Vec3i downkeypoint = { EdgeChain[i](0), EdgeChain[i](1), 0 };
			//...........
			if (keypoint.empty() || (abs(downkeypoint(0) - keypoint[0](0))>0.01&&abs(downkeypoint(1) - keypoint[0](1))>0.1))
			     keypoint.push_back(downkeypoint);
			down = true;
			up = false;
		}


		pre25p.erase(pre25p.begin());
		pre25p.push_back(EdgeChain[i]);
		next25p.erase(next25p.begin());
		next25p.push_back(EdgeChain[i +51]);
	}

	return keypoint;
}

vector<Vec3i> EdgeDetect::govalley(Mat EdgeL, vector<Vec3i> KeyChain, vector<Vec2i> EdgeChain, Vec2i CircleCentre)
{
	Mat InerEdgeL(EdgeL.rows, EdgeL.cols, CV_32SC1, Scalar(0));
	int size = EdgeChain.size();
	for (int j = 0; j < size; j++)
		EdgeChain.push_back(EdgeChain[j]);
	for (int j = 0; j < size; j++)
		EdgeChain.push_back(EdgeChain[j]);
	//�ӹȵף�˳��edgelԲ�ķ����ߣ�ֱ����Χ�Ҳ������ӽ��ĵ�
	for (int i = 0; i < KeyChain.size(); i++)
	{
		if (KeyChain[i](2) == 0)
		{
			vector<Vec2i> valleypoint;
			Vec2i TP = { KeyChain[i](0), KeyChain[i](1) }, NP = { KeyChain[i](0), KeyChain[i](1) };
			while (TP != CircleCentre)
			{
				float mindist = powf(powf(TP(0) - CircleCentre(0), 2) + powf(TP(1) - CircleCentre(1), 2), 0.5);
				float maxpower = 0;
				for (int m = -1; m <= 1; m++)
				for (int n = -1; n <= 1; n++)
				{
					float dist = powf(powf(TP(0) + m - CircleCentre(0), 2) + powf(TP(1) + n - CircleCentre(1), 2), 0.5);
					if (dist < mindist)
					{
						if (EdgeL.at<float>(TP(0) + m, TP(1) + n) > maxpower)
						{
							maxpower = EdgeL.at<float>(TP(0) + m, TP(1) + n);
							NP = { TP(0) + m, TP(1) + n };
						}
					}
				}
				if (NP == TP)
				{
					break;
				}
				else
				{
					valleypoint.push_back(NP);
					TP = NP;
				}
			}
			//ȷ���ڵ���ϵ,��ȡǰ�����ͺ������,�Ա�valley��ǰ�����
			float alpha0 = 0, alpha1 = 0, alpha2 = 0;
			Vec2f p0, p1, p2, p;
			float k0, k1, k2;
			for (int j = size; j < 2 * size; j++)
			{
				if (EdgeChain[j](0) == KeyChain[i](0) && EdgeChain[j](1) == KeyChain[i](1))
				{
					p = EdgeChain[j];
					if (valleypoint.size() < 4)
						continue;
					//p0 = valleypoint[min(4,(int)valleypoint.size())];
					p1 = EdgeChain[j - 10];
					p2 = EdgeChain[j + 10];
					k0 = (p0(1) - p(1)) / (p0(0) - p(0));
					k1 = (p1(1) - p(1)) / (p1(0) - p(0));
					k2 = (p2(1) - p(1)) / (p2(0) - p(0));
					if (p0(0) - p(0) != 0)
						alpha0 = atanf(k0) * 180 / 3.14;
					else
						alpha0 = 90;
					if (p1(0) - p(0) != 0)
						alpha1 = atanf(k1) * 180 / 3.14;
					else
						alpha1 = 90;
					if (p2(0) - p(0) != 0)
						alpha2 = atanf(k2) * 180 / 3.14;
					else
						alpha2 = 90;
				}
			}
			if (abs(alpha1 - alpha0) < abs(alpha2 - alpha0)) //ǰһ����������
				KeyChain[i](2) = 0;
			else                                             //��һ����������
				KeyChain[i](2) = 2;

			for (int j = 0; j < valleypoint.size(); j++)
				InerEdgeL.at<int>(valleypoint[j](0), valleypoint[j](1)) = 1;
		}
	}
	imwrite("InerEdgeL.jpg", InerEdgeL * 255);
	return KeyChain;
}

Vec3f EdgeDetect::PointRotate(Vec3f RotateZ, Vec3f RotateP, float angle)
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
	return RotatedP;
}

void EdgeDetect::caculateEdge()
{
	Vec2i CircleCentre = { int(f_o.y()), int(f_o.x()) };      //����============================================================================
	
	Mat OrigenL = imread(g_filedst.toStdString());
	//imwrite("edgearea1.jpg",edgearea);
	int height = OrigenL.rows;
	int weight = OrigenL.cols;
	Mat ResultL(height, weight, CV_32FC1, Scalar(0));
	Mat GaussL(height, weight, CV_32FC3, Scalar(0));
	Mat AllEdgeL(height, weight, CV_32FC1, Scalar(0));
	Mat CannyResultL(height, weight, CV_32FC3, Scalar(0));

	//�ϲ���������
	Mat AllareaM(height, weight, CV_32FC1, Scalar(0));
	for (int i = 0; i < areanumber; i++)
	{

		string prename = "edgearea", type = ".jpg", id;
		stringstream ids;
		ids << i;
		ids >> id;
		string allfilename = prename + id + type;
		Mat TareaM = imread(allfilename);
		Mat TareaMGray(height, weight, CV_32FC1, Scalar(0));
		cvtColor(TareaM, TareaMGray, CV_RGB2GRAY);
		TareaMGray.convertTo(TareaMGray, CV_32FC1);
		add(AllareaM, TareaMGray, AllareaM);
	}
	imwrite("edgearea.jpg", AllareaM);

	Mat edgearea = imread("edgearea.jpg");

	for (int i = 1; i < 20; i++)
	{
		GaussianBlur(OrigenL, GaussL, Size(7, 7), i*0.25, i*0.25);
		Canny(GaussL, CannyResultL, 180, 60);
		Mat edgearea1(height, weight, CV_32FC1, Scalar(0));
		cvtColor(edgearea, edgearea1, CV_RGB2GRAY);
		Mat CannyMasked;
		CannyResultL.copyTo(CannyMasked, edgearea1);
		imwrite("CannyResultL.jpg", CannyMasked * 255);
		CannyMasked.convertTo(CannyMasked, CV_32FC1);
		add(ResultL, CannyMasked / 255, ResultL);

		if (i == 1)
		{
			AllEdgeL = GradientCaculate(GaussL, AllEdgeL, edgearea, 16);
			imwrite("AllEdgeL.jpg", AllEdgeL * 255);
		}
		printf("%d\n", i * 5);
	}
	EdgeChain = CEDContours(ResultL, areanumber, CircleCentre);
	EdgeChain.erase(EdgeChain.begin() + EdgeChain.size() - 1);
	Mat EdgeChainL(height, weight, CV_32SC1, Scalar(0));
	EdgeChain.erase(EdgeChain.begin() + EdgeChain.size() - 1);
	for (int i = 0; i < EdgeChain.size(); i++)
		EdgeChainL.at<int>(EdgeChain[i](0), EdgeChain[i](1)) = 1;
	imwrite("EdgeChainL.jpg", EdgeChainL * 255);
	KeyChain = GetKeyPoint(EdgeChain, CircleCentre);

	//�ڱ�Ե
	KeyChain = govalley(AllEdgeL, KeyChain, EdgeChain, CircleCentre);

	//��Բ���
	int toppointsize = 0;
	for (int i = 0; i < KeyChain.size(); i++)
	{
		if (KeyChain[i](2) == 1)
			toppointsize = toppointsize + 1;
	}
	CvMat *TopPoint = cvCreateMat(toppointsize, 1, CV_32FC2);
	for (int i = 0; i < KeyChain.size(); i++)
	{
		if (KeyChain[i](2) == 1)
		{
			toppointsize--;
			float x = KeyChain[i](0);
			float y = KeyChain[i](1);
			cvSet1D(TopPoint, toppointsize, cvScalar(y, x, 0, 0));
		}
	}
	CvBox2D EllipseBox = cvFitEllipse2(TopPoint);
	CvMat *Ellipse = cvCreateMat(height, weight, CV_32FC3);
	cvEllipseBox(Ellipse, EllipseBox, Scalar(255, 255, 255));
	Mat EllipseOut = Mat::Mat(Ellipse);
	imwrite("Ellipse.jpg", EllipseOut);

	//����ؼ�����
	float Ea = max(EllipseBox.size.height, EllipseBox.size.width) / 2; //��Բ����=============================================================
	radius = Ea;//�뾶
	float Eb = min(EllipseBox.size.height, EllipseBox.size.width) / 2; //��Բ����
	Vec2f EllipseCentre = { EllipseBox.center.y, EllipseBox.center.x }; //��Բ����==========================================================
	o = QPointF(EllipseBox.center.y, EllipseBox.center.x);//��Բ����
	
	float CCdist = powf(powf(EllipseCentre(0) - CircleCentre(0), 2) + powf(EllipseCentre(1) - CircleCentre(1), 2), 0.5);
	float angleQ = asin(Eb / Ea);      //��Բ�����ƽ��ļн� 
	angle = angleQ;
	float R = CCdist / cos(angleQ);  //��Բ���᳤��=========================================================================================
	height = R;

	float angleO = atan(Ea / R);      //��Բ�Ž�============================================================================================
	//Vec2f V = { (-EllipseCentre(0) + CircleCentre(0)) / CCdist, (-EllipseCentre(1) + CircleCentre(1)) / CCdist }; //��Բ������ƽ���ϵ�ͶӰ
	//........................
	Vec2f V = { (EllipseCentre(0) - CircleCentre(0)) / CCdist, (EllipseCentre(1) - CircleCentre(1)) / CCdist }; //��Բ������ƽ���ϵ�ͶӰ
	float vx = cos(angleQ)*V(0), vy = cos(angleQ)*V(1), vz = sin(angleQ);     //Բ׶��������������ķ�����
	float coso = cos(angleO);  //Բ׶�Žǵ�����ֵ
	Vec3f rotatez = { vx, -vy, 0 }; //��ת��
	

	//��������ͶӰ��Բ׶����
	contour.clear();
	vector<Vec2i> newEdgeChain;
	vector<Vec3f> RotatePChain;        //�����������=======================================================================================
	for (int i = 0; i < EdgeChain.size(); i++)
	{
		float x = EdgeChain[i](0) - CircleCentre(0);
		float y = EdgeChain[i](1) - CircleCentre(1);
		

		float a = powf(coso, 2) - powf(vz, 2);     //������бԲ׶�ķ���
		float b = -2 * vz*(vx*x + vy*y);
		float c = powf(coso, 2) * (powf(x, 2) + powf(y, 2)) - powf((vx*x + vy*y), 2);

		float z1 = (-b + powf((powf(b, 2) - 4 * a*c), 0.5)) / (2 * a);  //���߶�
		float z2 = (-b - powf((powf(b, 2) - 4 * a*c), 0.5)) / (2 * a);

		float agl1 = (vx*x + vy*y + vz*z1) / powf(x*x + y*y + z1*z1, 0.5);   //ɸѡ���ý�
		float agl2 = (vx*x + vy*y + vz*z2) / powf(x*x + y*y + z2*z2, 0.5);
		if (agl1 < 0)
			z1 = -10000;
		if (agl2 < 0)
			z2 = -1000;
		float z = max(z1, z2);

		//Vec3f RotateP = { x, y, z };
		Vec3f RotateP = { y, x, z };
		//.........................
		

		Vec3f RotatedP = PointRotate(rotatez, RotateP, (PI/2-angleQ));//..............................
		RotatePChain.push_back(RotatedP);
		
		coor t = { RotatedP(0), -RotatedP(1), RotatedP(2) };//.....................
		if ((!contour.empty())&&(fabs(contour.back().x) < 1e-4)&&(fabs(contour.back().y) < 1e-4)&&(fabs(contour.back().z) < 1e-4)&&
			(fabs(t.x) < 1e-4)&&(fabs(t.y) < 1e-4)&&(fabs(t.z) < 1e-4))
		{
		}
		else
		{
			contour.push_back(t);
			newEdgeChain.push_back(EdgeChain[i]);

		}
	}
	EdgeChain.clear();
	EdgeChain = newEdgeChain;

	//�Ѷ���Ͱ����ͶӰ��Բ׶����
	keypoints.clear();
	keypicture.clear();
	vector<Vec4f> RotateKChain;         //����Ͱ��������==============================================================================
	for (int i = 0; i < KeyChain.size(); i++)
	{
		QPointF p(KeyChain[i](1), KeyChain[i](0));
		keypicture.push_back(p);

		float x = KeyChain[i](0) - CircleCentre(0);
		float y = KeyChain[i](1) - CircleCentre(1);

		float a = powf(coso, 2) - powf(vz, 2);     //������бԲ׶�ķ���
		float b = -2 * vz*(vx*x + vy*y);
		float c = powf(coso, 2) * (powf(x, 2) + powf(y, 2)) - powf((vx*x + vy*y), 2);

		float z1 = (-b + powf((powf(b, 2) - 4 * a*c), 0.5)) / (2 * a);  //���߶�
		float z2 = (-b - powf((powf(b, 2) - 4 * a*c), 0.5)) / (2 * a);

		float agl1 = (vx*x + vy*y + vz*z1) / powf(x*x + y*y + z1*z1, 0.5);   //ɸѡ���ý�
		float agl2 = (vx*x + vy*y + vz*z2) / powf(x*x + y*y + z2*z2, 0.5);
		if (agl1 < 0)
			z1 = -10000;
		if (agl2 < 0)
			z2 = -1000;
		float z = max(z1, z2);

		//Vec3f RotateP = { x, y, z };
		Vec3f RotateP = { y, x, z };
		//Vec3f RotatedP = PointRotate(rotatez, RotateP, angleO);
		//........................
		Vec3f RotatedP = PointRotate(rotatez, RotateP, (PI / 2 - angleQ));//..............................
		Vec4f RotatedKP = { RotatedP(0), RotatedP(1), RotatedP(2), (float)KeyChain[i](2) };
		RotateKChain.push_back(RotatedKP);


		coor t = { RotatedP(0), -RotatedP(1), RotatedP(2) };//.....................
		coorTag t1= { t, (float)KeyChain[i](2) };
		keypoints.push_back(t1);
	}
	return;
}

QPointF EdgeDetect::getF_o()
{
	return f_o;
}

QPointF EdgeDetect::getO()
{
	return o;
}

vector<coor> EdgeDetect::getContour()
{
	return contour;
}

vector<coorTag> EdgeDetect::getKeypoints()
{
	return keypoints;
}

vector<QPointF> EdgeDetect::getKeypic()
{
	return keypicture;
}

int EdgeDetect::getRadius()
{
	return radius;
}

int EdgeDetect::getHeight()
{
	return height;
}

double EdgeDetect::getAngle()
{
	return angle;
}

vector<Vec2i> EdgeDetect::getContourpic()
{
 	return EdgeChain;
}


vector<Vec3i> EdgeDetect::getKeyChain()
{
	return KeyChain;
}

