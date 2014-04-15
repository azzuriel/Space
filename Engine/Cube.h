#pragma once
#include "Mesh.h"
#include <memory>
class Cube
{
public:
	static Mesh getMesh();
private:
	static std::shared_ptr<Mesh> sm_mesh;
};

