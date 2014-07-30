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

uniform struct Transform
{
        mat4 model;
        mat4 viewProjection;
        mat3 normal;
        vec3 viewPosition;
		mat4 light;
} transform;

uniform struct PointLight
{
        vec4 position;
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        vec3 attenuation;
} light;

uniform sampler2DShadow depthTexture;
uniform struct Material
{
        sampler2D texture;
		sampler2D normal;
        vec4  ambient;
        vec4  diffuse;
        vec4  specular;
        vec4  emission;
        float shininess;
} material;

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXCOORD) in vec2 texcoord;
layout(location = VERT_NORMAL) in vec3 normal;

out Vertex {
		vec4  position;
} Vert;

void main(void)
{
  vec4 vertex   = transform.model * vec4(position, 1.0);
  
  Vert.position = transform.viewProjection * vertex;
  gl_Position   = Vert.position;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
		vec4  position;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

void main(void)
{
  float depth = Vert.position.z / 1000.0;
  color.rgb = vec3(depth, depth, depth);
  color.w = 1;
}
#endif