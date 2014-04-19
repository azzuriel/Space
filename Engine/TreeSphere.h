#pragma once
#include "VertexPositionTexture.h"
#include <vector>
#include <glew.h>
#include "Mesh.h"
#include <detail\type_vec.hpp>

struct QuadLodPart{
public:
	Mesh* m;
	glm::vec3 pos;
	QuadLodPart *NE, *NW, *SE, *SW;
	QuadLodPart();
	~QuadLodPart();
	vec3 start, end;
	float initialSize;
};

class QuadLod
{
public:
	QuadLod(void);
	~QuadLod(void);
	void GenerateFrom(glm::vec3 cam);
	void Render();
	void Bind();
	Mesh* m;
	QuadLodPart* root;
	
};
