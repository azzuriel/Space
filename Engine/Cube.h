#pragma once
#include "Mesh.h"
class Cube
{
public:
	static Mesh getMesh();
private:
	static Mesh* sm_mesh;
};

