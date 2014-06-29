#ifndef TERRAIN_PATCH_HPP
#define TERRAIN_PATCH_HPP

#include "heightmap.h"
#include "binary_triangle_tree.h"

#include "glm.hpp"
#include "Mesh.h"

class TerrainPatch
{
private:
	Heightmap *m_map;

	size_t m_worldX, m_worldY;

	float *m_leftVariance;
	float *m_rightVariance;
	size_t m_varianceSize;

	float m_varianceLimit;

	BTTNode *m_leftRoot;
	BTTNode *m_rightRoot;

	size_t m_leftLeaves;
	size_t m_rightLeaves;

	BTTNode *m_triPool;
	size_t m_poolSize;
	size_t m_poolNext;
	void Init();

public:
	TerrainPatch(int offset_x = 0, int offset_y = 0);
	~TerrainPatch();

	void print() const;

	void computeVariance(int maxTessellationLevels = 14);

	void reset();

	void tessellate(const glm::vec3 &view, float errorMargin = 0.001);

	/*(left_num_leaves + right_num_leaves)*(number of elements per triangle)*/
	void getTessellation(float *vertices, float *colors, float *normalTexels);

	size_t amountOfLeaves() const;

	size_t poolSize() const;

	Heightmap *getHeightmap();

	void Bind(float *vertices, float *colors, float *normalTexels);
	void Render();
	Mesh* m;

private:
	BTTNode *allocateNode();

	void split(BTTNode *node);

	void tessellateRecursive(
		BTTNode *node, const glm::vec3 &view, float errorMargin,
		int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y,
		float *variance, int variance_idx);

	void computeVarianceRecursive(
		int maxTessellationLevels, int level, float *varianceTree, int idx, Heightmap *map,
		int left_x, int left_y, float left_z,
		int right_x, int right_y, float right_z,
		int apex_x, int apex_y, float apex_z);

	void getTessellationRecursive(
		BTTNode *node, Heightmap *map,
		float *vertices, float *colors, float *normalTexels, int *idx,
		int left_x, int left_y, int right_x, int right_y, int apex_x, int apex_y);

};

inline size_t TerrainPatch::amountOfLeaves() const
{
	return m_leftLeaves + m_rightLeaves;
}

inline size_t TerrainPatch::poolSize() const
{
	return m_poolSize;
}

inline Heightmap *TerrainPatch::getHeightmap()
{
	return m_map;
}

#endif // TERRAIN_PATCH_H