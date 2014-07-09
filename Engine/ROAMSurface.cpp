#include "ROAMSurface.h"
#include <JargShader.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "BasicJargShader.h"

ROAMSurface::ROAMSurface(void)
{
    for (int i=0;i<6;i++)
    {
        auto a = new ROAMSurfaceCell();
        a->offset = glm::vec3(0,0,0);
        cells.push_back(a);
    }
    i = 0;
}

ROAMSurface::~ROAMSurface(void)
{
    if(cells.size() > 0){
        for (int i=0;i<cells.size();i++)
        {
            delete cells[i];
        }
    }
}

void ROAMSurface::UpdateCells(glm::vec3 cam)
{
    if(cells.size() > 0){
        for (int i=0;i<cells.size();i++)
        {
            auto patch = cells[i];
            patch->tp->reset();
            auto transp = transpose(mat3(inverse(cells[i]->tp->m->World)));
            patch->tp->tessellate((cam - patch->offset)*transp);
            patch->tp->getTessellation(patch->triPool, patch->colorPool, patch->normalTexelPool);
        }
    }
}

void ROAMSurface::Bind()
{
    if(cells.size() > 0){
        for (int i=0;i<cells.size();i++)
        {
            cells[i]->Bind();
        }
    }
}

void ROAMSurface::Render(BasicJargShader* active)
{
    glm::mat4 Identity = glm::mat4(1);
    Identity = scale(Identity, vec3(100,100,100));
    cells[0]->tp->m->World = Identity;
    cells[0]->Render(active);
    cells[1]->tp->m->World = rotate(Identity, (float)M_PI, vec3(1,0,0));
    cells[1]->Render(active);
    cells[2]->tp->m->World = rotate(Identity, (float)M_PI_2, vec3(1,0,0));
    cells[2]->Render(active);
    cells[3]->tp->m->World = rotate(Identity, (float)-M_PI_2, vec3(1,0,0));
    cells[3]->Render(active);
    cells[4]->tp->m->World = rotate(Identity, (float)M_PI_2, vec3(0,1,0));
    cells[4]->Render(active);
    cells[5]->tp->m->World = rotate(Identity, (float)-M_PI_2, vec3(0,1,0));
    cells[5]->Render(active);
}

void ROAMSurface::Test()
{
    delete cells[0];
    i++;
    auto a = new ROAMSurfaceCell();
    a->offset = glm::vec3(i,i,0);
    cells.push_back(a);
}

ROAMSurfaceCell::ROAMSurfaceCell(float x, float y)
{
    tp = new TerrainPatch(x, y);

    tp->computeVariance(12);
    tp->m->World = glm::mat4(1.0f);
    //patch->m->Shader = BasicShader.get();
    auto m_poolSize = tp->poolSize();

    triPool = new float[m_poolSize*9];
    colorPool = new  float[m_poolSize*9];
    normalTexelPool = new float[m_poolSize*6]; 

    Heightmap *map = tp->getHeightmap();
    GLuint normalTexture = -1;
    glGenTextures(1, &normalTexture);
    glBindTexture(GL_TEXTURE_2D, normalTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, map->width, map->height, 0, GL_RGB, GL_FLOAT, map->normal_map);
    Texture* tempt = new Texture();
    tempt->textureId = normalTexture;
    tp->m->material = new Material();
    tp->m->material->normal = tempt;
    //tp->m->material->texture = tempt;
}

ROAMSurfaceCell::~ROAMSurfaceCell()
{
    delete tp;
    delete[] triPool;
    delete[] colorPool;
    delete[] normalTexelPool;
}

void ROAMSurfaceCell::Bind()
{
    tp->Bind(triPool, colorPool, normalTexelPool);
}

void ROAMSurfaceCell::Render(BasicJargShader* active)
{	
    tp->m->shader = active;
    tp->Render();
}
