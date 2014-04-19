#include "ROAMSurface.h"
#include <gtc\matrix_transform.hpp>
#include <JargShader.h>


ROAMSurface::ROAMSurface(void)
{
	for (int i=0;i<1;i++)
	{
		for (int j=0;j<1;j++)
		{
			auto a = new ROAMSurfaceCell();
			a->offset = glm::vec3(i,j,0);
			cells.push_back(a);
		}
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
			patch->tp->tessellate(cam - patch->offset, 0.004F);
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

void ROAMSurface::Render(JargShader* active)
{
	if(cells.size() > 0){
		for (int i=0;i<cells.size();i++)
		{
			cells[i]->Render(active);
		}
	}
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

	tp->computeVariance();
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
	tp->m->Texture = tempt;
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

void ROAMSurfaceCell::Render(JargShader* active)
{	
	tp->m->Shader = active;
	tp->m->World = glm::scale(glm::mat4(1.0f), vec3(100.0F,100.0F,30.0F));
	tp->Render();
}
