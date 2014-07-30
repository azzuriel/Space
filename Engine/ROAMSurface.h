#pragma once
#include <vector>
#include <ROAMgrid.h>
#include <JargShader.h>
#include "BasicJargShader.h"
class ROAMSurfaceCell {
public:
    TerrainPatch* tp;
    glm::vec3 offset;
    ROAMSurfaceCell(float x = 0, float y = 0);
    ~ROAMSurfaceCell();
    void Bind();
    void Render(std::shared_ptr<BasicJargShader> active);

    float *triPool;
    float *colorPool;
    float *normalTexelPool;
};

class ROAMSurface {
public:
    ROAMSurface(void);
    ~ROAMSurface(void);
    void UpdateCells(glm::vec3 cam);
    std::vector<ROAMSurfaceCell*> cells;

    void Bind();
    void Render(std::shared_ptr<BasicJargShader> active);
    void Test();
    int i;
    bool Loaded;
};