#ifndef _filedst
#define _filedst
#include <QtWidgets/QMainWindow>
#include <vector>

using namespace std;
extern QString g_filedst;
extern int brushsize ;

typedef struct coor{
	double x;
	double y;
	double z;
	//double n;
}coor;

typedef struct coorTag{
	coor co;//����
	int tag;//���
}coorTag;

typedef struct petal{
	coor tip;//�û��궥��
	vector<coor> petal_contour;//�û�������
}petal;


struct node;
struct edge;

struct node{
	float x, y, z;
	struct edge *edgef;
	int id;
	float n[3];
	float laplace[3];
};

struct edge{
	struct node *start;
	struct node *end;
	struct edge *next;
};

struct arraylist
{
	struct node *end;
	float l;
	struct arraylist *next;
};

extern coor t;
#endif