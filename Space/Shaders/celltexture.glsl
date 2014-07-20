/*******************************************************************************
	Copyright (C) 2014 Andrey Samsonov

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

//version inserted here by programm
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0
uniform sampler2D inputTex1;

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXCOORD) in vec2 texcoord;

out Vertex {
		vec3  position;
} Vert;

void main(void)
{
  vec4 pos      = vec4(position, 1.0);
  Vert.position = position;
  
  gl_Position   = pos;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
		vec3  position;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

vec4 gpuGetCell3D(const in int x, const in int y, const in int z)
{
	float u = (x + y * 31) / 256.0;
	float v = (z - x * 3) / 256.0;
	return(texture2D(inputTex1, vec2(u, v)));
}

vec2 gpuCellNoise3D(const in vec3 xyz)
{
	int xi = int(floor(xyz.x));
	int yi = int(floor(xyz.y));
	int zi = int(floor(xyz.z));

	float xf = xyz.x - float(xi);
	float yf = xyz.y - float(yi);
	float zf = xyz.z - float(zi);

	float dist1 = 9999999.0;
	float dist2 = 9999999.0;
	vec3 cell;

	for (int z = -1; z <= 1; z++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				cell = gpuGetCell3D(xi + x, yi + y, zi + z).xyz;
				cell.x += (float(x) - xf);
				cell.y += (float(y) - yf);
				cell.z += (float(z) - zf);
				float dist = dot(cell, cell);
				if (dist < dist1)
				{
					dist2 = dist1;
					dist1 = dist;
				}
				else if (dist < dist2)
				{
					dist2 = dist;
				}
			}
		}
	}
	return vec2(sqrt(dist1), sqrt(dist2));
}

void main(void)
{
  vec2 b = gpuCellNoise3D(Vert.position*20.0);
  float a = b.x / b.y;
  color = vec4(a,a,a,a);
  //color.w = 1;
}
#endif