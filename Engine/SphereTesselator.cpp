#include "SphereTesselator.h"
#include "Mesh.h"

#define sqr(x)(x)*(x);

Mesh* Tesselator::Tesselate(int iters, const Mesh& mesh)
{
	Mesh* m = new Mesh(mesh);
	for (int i = 0; i< iters; i++)
	{
		auto temp = SubTesselate(*m);
		delete m;
		m = temp;
	}
	return m;
}

Mesh* Tesselator::SphereTesselate(int iters, const Mesh& mesh)
{
	Mesh* m = new Mesh(mesh);
	for (int i = 0; i< iters; i++)
	{
		auto temp = SphereSubTesselate(*m);
		delete m;
		m = temp;
		temp = nullptr;
	}
	return m;
}

Mesh* Tesselator::SphereSubTesselate(const Mesh& mesh)
{
	Mesh* m = new Mesh();

	int off = 0;
	for (int i =0; i< mesh.Indeces.size() -2; i+= 3)
	{
		VertexPositionTexture t;

		VertexPositionTexture v0 = mesh.Verteces[mesh.Indeces[i]];
		VertexPositionTexture v1 = mesh.Verteces[mesh.Indeces[i+1]];
		VertexPositionTexture v2 = mesh.Verteces[mesh.Indeces[i+2]];

		t = v0;
		t.pos.Normalize();
		m->Verteces.push_back(t);
		t = v1;
		t.pos.Normalize();
		m->Verteces.push_back(t);
		t = v2;
		t.pos.Normalize();
		m->Verteces.push_back(t);

		t = (v0 + v1)/2;
		t.pos.Normalize();
		m->Verteces.push_back(t);

		t = (v0 + v2)/2;
		t.pos.Normalize();
		m->Verteces.push_back(t);

		t = (v1 + v2)/2;
		t.pos.Normalize();
		m->Verteces.push_back(t);

		m->Indeces.push_back(off+0);
		m->Indeces.push_back(off+3);
		m->Indeces.push_back(off+4);

		m->Indeces.push_back(off+3);
		m->Indeces.push_back(off+1);
		m->Indeces.push_back(off+5);

		m->Indeces.push_back(off+5);
		m->Indeces.push_back(off+2);
		m->Indeces.push_back(off+4);

		m->Indeces.push_back(off+3);
		m->Indeces.push_back(off+5);
		m->Indeces.push_back(off+4);

		off+=6;
	}
	return m;
}

Mesh* Tesselator::SubTesselate(const Mesh& mesh)
{
	Mesh* m = new Mesh();

	int off = 0;
	for (int i =0; i< mesh.Indeces.size() -2; i+= 3)
	{
		VertexPositionTexture t;

		m->Verteces.push_back(mesh.Verteces[mesh.Indeces[i]]);
		m->Verteces.push_back(mesh.Verteces[mesh.Indeces[i+1]]);
		m->Verteces.push_back(mesh.Verteces[mesh.Indeces[i+2]]);

		t.pos.x = (mesh.Verteces[mesh.Indeces[i]].pos.x + mesh.Verteces[mesh.Indeces[i+1]].pos.x)/2;
		t.pos.y = (mesh.Verteces[mesh.Indeces[i]].pos.y + mesh.Verteces[mesh.Indeces[i+1]].pos.y)/2;
		t.pos.z = (mesh.Verteces[mesh.Indeces[i]].pos.z + mesh.Verteces[mesh.Indeces[i+1]].pos.z)/2;
		t.uv.x = (mesh.Verteces[mesh.Indeces[i]].uv.x + mesh.Verteces[mesh.Indeces[i+1]].uv.x)/2;
		t.uv.y = (mesh.Verteces[mesh.Indeces[i]].uv.y + mesh.Verteces[mesh.Indeces[i+1]].uv.y)/2;
		m->Verteces.push_back(t);

		t.pos.x = (mesh.Verteces[mesh.Indeces[i]].pos.x + mesh.Verteces[mesh.Indeces[i+2]].pos.x)/2;
		t.pos.y = (mesh.Verteces[mesh.Indeces[i]].pos.y + mesh.Verteces[mesh.Indeces[i+2]].pos.y)/2;
		t.pos.z = (mesh.Verteces[mesh.Indeces[i]].pos.z + mesh.Verteces[mesh.Indeces[i+2]].pos.z)/2;
		t.uv.x = (mesh.Verteces[mesh.Indeces[i]].uv.x + mesh.Verteces[mesh.Indeces[i+2]].uv.x)/2;
		t.uv.y = (mesh.Verteces[mesh.Indeces[i]].uv.y + mesh.Verteces[mesh.Indeces[i+2]].uv.y)/2;
		m->Verteces.push_back(t);

		t.pos.x = (mesh.Verteces[mesh.Indeces[i+1]].pos.x + mesh.Verteces[mesh.Indeces[i+2]].pos.x)/2;
		t.pos.y = (mesh.Verteces[mesh.Indeces[i+1]].pos.y + mesh.Verteces[mesh.Indeces[i+2]].pos.y)/2;
		t.pos.z = (mesh.Verteces[mesh.Indeces[i+1]].pos.z + mesh.Verteces[mesh.Indeces[i+2]].pos.z)/2;
		t.uv.x = (mesh.Verteces[mesh.Indeces[i+1]].uv.x + mesh.Verteces[mesh.Indeces[i+2]].uv.x)/2;
		t.uv.y = (mesh.Verteces[mesh.Indeces[i+1]].uv.y + mesh.Verteces[mesh.Indeces[i+2]].uv.y)/2;
		m->Verteces.push_back(t);

		m->Indeces.push_back(off+0);
		m->Indeces.push_back(off+3);
		m->Indeces.push_back(off+4);

		m->Indeces.push_back(off+3);
		m->Indeces.push_back(off+1);
		m->Indeces.push_back(off+5);

		m->Indeces.push_back(off+5);
		m->Indeces.push_back(off+2);
		m->Indeces.push_back(off+4);

		m->Indeces.push_back(off+3);
		m->Indeces.push_back(off+5);
		m->Indeces.push_back(off+4);

		off+=6;
	}
	return m;
}
