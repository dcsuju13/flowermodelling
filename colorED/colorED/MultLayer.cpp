#include "MultLayer.h"


MultLayer::MultLayer()
{
}


MultLayer::MultLayer(MyMesh m1, MyMesh m2)
{
	mesh1 = m1;
	mesh2 = m2;
}

MultLayer::~MultLayer()
{
}

void MultLayer::combineMesh()
{
	//mesh2中的点连接在Mesh1的点后边
	vector<MyMesh::VertexHandle> vhandle;//记录采样点句柄
	for (auto it = mesh2.vertices_begin(); it != mesh2.vertices_end(); ++it)
	{
		MyMesh::VertexHandle t;
		t=mesh1.add_vertex(MyMesh::Point(mesh2.point(it.handle()).data()[0], mesh2.point(it.handle()).data()[1], mesh2.point(it.handle()).data()[2]));;
		vhandle.push_back(t);
	}

	//mesh2中的面连接在mesh1的面后边
	for (auto it = mesh2.faces_begin(); it != mesh2.faces_end(); ++it)
	{
		vector<MyMesh::VertexHandle>  face_vhandles;//记录三角网格面句柄
		for (auto ith = mesh2.fh_begin(it.handle()); ith != mesh2.fh_end(it.handle()); ++ith)
		{
			auto fromv = mesh2.from_vertex_handle(ith.handle());

			int count = 0;
			for (auto i = mesh2.vertices_begin(); i !=mesh2.vertices_end(); i++)
			{
				if (fabs(mesh2.point(i.handle()).data()[0] - mesh2.point(fromv).data()[0]) < 0.001&&fabs(mesh2.point(i.handle()).data()[1] - mesh2.point(fromv).data()[1]) < 0.001
					&&fabs(mesh2.point(i.handle()).data()[2] - mesh2.point(fromv).data()[2]) < 0.001)
					break;
				count++;
			}
			face_vhandles.push_back(vhandle[count]);
		}
		mesh1.add_face(face_vhandles);
	}

	try
	{
		if (!OpenMesh::IO::write_mesh(mesh1, "mult.ply"))
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


MyMesh MultLayer::getMesh1()
{
	combineMesh();
	return mesh1;
}