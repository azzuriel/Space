#pragma once
#ifndef Mesh_h__
#define Mesh_h__

#include "VertexPositionTexture.h"
#include <glew.h>
#include <vector>
#include "JargShader.h"
#include "TextureManager.h"
#include <string>
#include <Material.h>
#include "BasicJargShader.h"
#include <memory>
class Mesh
{
public:
    Mesh(void);
    ~Mesh(void);
    void Create(std::vector<VertexPositionNormalTexture> verteces, std::vector<GLuint> indeces);
    void Bind(int type = 0);
    void Render();
    void Render(mat4 Model);
    void Combine(Mesh* com);
    bool loadOBJ(std::string path);
    void computeNormal();
    void Unindex();
    void MergeVerteces();
    std::vector<VertexPositionNormalTexture> Verteces;
    std::vector<GLuint> Indeces;
    std::shared_ptr<BasicJargShader> shader;
    std::shared_ptr<Material> material;
    mat4 World;
    std::string id;
private:
    GLuint m_vao;
    GLuint* m_vbo;
};
#endif // Mesh_h__

