#pragma once
#ifndef SphereTesselator_h__
#define SphereTesselator_h__

#include "Mesh.h"
class Tesselator
{
public:
	static Mesh* Tesselate(int iters, const Mesh& mesh);
	static Mesh* SphereTesselate(int iters, const Mesh& mesh);
private:
	static Mesh* SubTesselate(const Mesh& mesh);
	static Mesh* SphereSubTesselate(const Mesh& mesh);
};
#endif // SphereTesselator_h__

