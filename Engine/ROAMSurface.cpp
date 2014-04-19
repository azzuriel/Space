#include "ROAMSurface.h"


ROAMSurface::ROAMSurface(void)
{
	for (int i=0;i<3;i++)
	{
		cells.push_back(new ROAMSurfaceCell());
	}
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
			auto patch = cells[i]->tp;
			//patch->reset();
			//patch->tessellate(cam.position, 0.004F);
			//patch->getTessellation(triPool, colorPool, normalTexelPool);
			//patch->Bind(triPool, colorPool, normalTexelPool);
		}
	}
}

ROAMSurfaceCell::ROAMSurfaceCell()
{
	tp = new TerrainPatch();
}

ROAMSurfaceCell::~ROAMSurfaceCell()
{
	delete tp;
}
