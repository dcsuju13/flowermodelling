#pragma once
#include <vector>
#include "global.h"


// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
// ----------------------------------------------------------------------------
typedef OpenMesh::TriMesh_ArrayKernelT<>  MyMesh;

using namespace std;

class MultLayer
{
public:
	MultLayer();
	MultLayer(MyMesh m1,MyMesh m2);
	~MultLayer();

	MyMesh getMesh1();
private:
	void combineMesh();//ºÏ²¢Íø¸ñ


	MyMesh mesh1;
	MyMesh mesh2;
};

