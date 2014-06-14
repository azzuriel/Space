#pragma once
#ifndef Model_h__
#define Model_h__

#include "Mesh.h"
#include "Material.h"
class Model
{
public:
	Model(void);
	~Model(void);
	Mesh mesh;
	Material material;
};
#endif // Model_h__

