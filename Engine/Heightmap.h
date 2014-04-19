#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <stddef.h>

	struct Heightmap
	{
		float *map;

		float *normal_map;

		size_t width, height;

		float minZ, maxZ;

	};

	void Heightmap_print(Heightmap *map);

	Heightmap *Heightmap_read(const char *filename);

	void Heightmap_delete(Heightmap *map);

	void Heightmap_normalize(Heightmap *map);

	void Heightmap_calculate_normals(Heightmap *map);

	void Heightmap_get_normal(Heightmap *map, int x, int y, float *nx, float *ny, float *nz);

	float Heightmap_get(Heightmap *map, int x, int y);

#endif // HEIGHTMAP_H