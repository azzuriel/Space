#pragma once
#include <glm.hpp>
#include "Mesh.h"
#include "BasicJargShader.h"
#include <memory>
#include "Camera.h"
class QuadTreeNode
{
public:
    QuadTreeNode(glm::vec3 _max, glm::vec3 _min);
    ~QuadTreeNode(void);
    void Init(std::shared_ptr<BasicJargShader> shader, float K, float level);
    void Update(glm::vec3 camera);
    void Draw();
    Mesh *mesh;

    float K;
    float level;
    glm::vec3 min, max;
    QuadTreeNode *NW, *SW, *NE, *SE;
};

class QuadTreePlane
{
public:
    float K;
    QuadTreePlane(void);
    ~QuadTreePlane(void);
    void Init(std::shared_ptr<BasicJargShader> shader, const Camera &cam);
    void Update(glm::vec3 camera);
    void Draw();
    std::shared_ptr<BasicJargShader> basic;
    QuadTreeNode *root;
    mat4 World;
};

