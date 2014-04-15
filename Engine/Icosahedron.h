#pragma once
#ifndef Icosahedron_h__
#define Icosahedron_h__

#include "Mesh.h"
class Icosahedron
{
public:
	static Mesh getMesh();
private:
	static Mesh* sm_mesh;
};
#endif // Icosahedron_h__

