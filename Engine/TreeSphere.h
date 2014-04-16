#pragma once
#include "VertexPositionTexture.h"
#include <vector>
#include <glew.h>
class TreeSphere
{
public:
	TreeSphere(void);
	~TreeSphere(void);
	void GenerateFrom();
};

struct TreeSpherePart{
	std::vector<VertexPositionNormalTexture> vert;
	std::vector<GLuint> ind;
	Vector3 pos;
};
