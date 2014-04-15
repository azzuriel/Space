#pragma once
#ifndef Mesh_h__
#define Mesh_h__

#include "VertexPositionTexture.h"
#include <glew.h>
#include <vector>
#include "JargShader.h"
#include "TextureManager.h"
class Mesh
{
public:
	Mesh(void);
	~Mesh(void);
	void Create(std::vector<VertexPositionTexture> verteces, std::vector<GLuint> indeces);
	void Bind();
	void Render();
	void Combine();
	std::vector<VertexPositionTexture> Verteces;
	std::vector<GLuint> Indeces;
	JargShader* Shader;
	Texture* Texture;
	mat4 World;
private:
	GLuint m_vao;
	GLuint* m_vbo;
};
#endif // Mesh_h__

