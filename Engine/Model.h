#pragma once
#ifndef Model_h__
#define Model_h__
#define COLLADA_MODEL 1

#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <string>
class Model
{
public:
	Model(void);
	Model(std::string name, int model_type = COLLADA_MODEL);
	~Model(void);
	std::vector<Mesh*> meshes;
	std::vector<Material*> materials;
	mat4 World;
	void Bind();
	void Render();
	Mesh* findMeshById(char* str);
	Material* findMaterialById(char* str);
};
#endif // Model_h__

