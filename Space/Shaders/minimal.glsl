/*******************************************************************************
	Copyright (C) 2014 Samsonov Andrey

	This software is distributed freely under the terms of the MIT License.
	See "license.txt" or "http://copyfree.org/licenses/mit/license.txt".
*******************************************************************************/

#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

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
out vec2 texcoordout;

uniform struct Transform
{
        mat4 model;
        mat4 viewProjection;
        mat3 normal;
        vec3 viewPosition;
} transform;

void main(void)
{
		vec4 vertex   = transform.model * vec4(position, 1.0);
        gl_Position   = transform.viewProjection * vertex;
		texcoordout = texcoord;
}
#endif

#ifdef _FRAGMENT_
const float cutoff = 0.9f;
in vec2 texcoordout;
void main(void)
{
  vec4 Texcol = texture2D(material.texture, texcoordout);
  if (Texcol.a < cutoff)
  {
    discard;
  }
}
#endif