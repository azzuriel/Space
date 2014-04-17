#pragma once
#include "VertexPositionTexture.h"
#include <vector>
#include <glew.h>
#include "Mesh.h"
#include <detail\type_vec.hpp>

struct TreeSpherePart{
public:
	Mesh* m;
	Vector3 pos;
	TreeSpherePart *NE, *NW, *SE, *SW;
	TreeSpherePart();
	~TreeSpherePart();
	vec3 start, end;
	float initialSize;
};

class TreeSphere
{
public:
	TreeSphere(void);
	~TreeSphere(void);
	void GenerateFrom(glm::vec3 cam);
	void Render();
	void Bind();
	Mesh* m;
	TreeSpherePart* root;
	
};
