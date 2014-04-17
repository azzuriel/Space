#pragma once
#include "VertexPositionTexture.h"
#include <vector>
#include <glew.h>
#include "Mesh.h"
#include <detail\type_vec.hpp>

struct TreeSpherePart{
	Mesh* m;
	Vector3 pos;
	TreeSpherePart *NE, *NW, *SE, *SW;
	TreeSpherePart();
	~TreeSpherePart();
	vec4 rect;
};

class TreeSphere
{
public:
	TreeSphere(void);
	~TreeSphere(void);
	void GenerateFrom(glm::vec4 cam);
	void Render();
	void Bind();
	Mesh* m;
	TreeSpherePart* root;
	
};
