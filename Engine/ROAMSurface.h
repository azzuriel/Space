#pragma once
#include <vector>
#include <ROAMgrid.h>
class ROAMSurfaceCell {
public:
	TerrainPatch* tp;
	glm::vec3 offset;
	ROAMSurfaceCell();
	~ROAMSurfaceCell();
};

class ROAMSurface {
public:
	ROAMSurface(void);
	~ROAMSurface(void);
	void UpdateCells(glm::vec3 cam);
	std::vector<ROAMSurfaceCell*> cells;
};

