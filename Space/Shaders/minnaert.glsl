/*******************************************************************************
	Copyright (C) 2014 Samsonov Andrey

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

uniform int NoTangent;

uniform struct PointLight
{
        vec4 position;
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        vec3 attenuation;
} light;

uniform sampler2D depthTexture;
uniform sampler2D noise;
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
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
		vec4  smcoord;
		vec4  position;
} Vert;

void main(void)
{
  vec4 vertex    = transform.model * vec4(position, 1.0);
  vec4 lightDir  = light.position - vertex; // без вертекс?!
  Vert.texcoord  = texcoord;
  Vert.distance  = length(lightDir);
  
  Vert.viewDir  = transform.viewPosition - vec3(vertex);
  Vert.lightDir = lightDir.xyz;
  
  Vert.smcoord  = transform.light * vertex;
  Vert.position = transform.viewProjection * vertex;
  gl_Position   = Vert.position;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
		vec4  smcoord;
		vec4  position;
} Vert;

float PCF(in vec4 smcoord)
{
        //float res = 0.0;

        //res += textureProjOffset(depthTexture, smcoord, ivec2(-1,-1));
        //res += textureProjOffset(depthTexture, smcoord, ivec2( 0,-1));
        //res += textureProjOffset(depthTexture, smcoord, ivec2( 1,-1));
        //res += textureProjOffset(depthTexture, smcoord, ivec2(-1, 0));
        //res += textureProjOffset(depthTexture, smcoord, ivec2( 0, 0));
        //res += textureProjOffset(depthTexture, smcoord, ivec2( 1, 0));
        //res += textureProjOffset(depthTexture, smcoord, ivec2(-1, 1));
        //res += textureProjOffset(depthTexture, smcoord, ivec2( 0, 1));
        //res += textureProjOffset(depthTexture, smcoord, ivec2( 1, 1));
        
		//return (res / 9.0);
		return 1.0f;
}

const float cutoff = 0.9f;

void main(void)
{
  vec4 Texcol = texture2D(material.texture, Vert.texcoord);
  if (Texcol.a < cutoff)
  {
    discard;
  }
  vec3 lightDir = normalize(Vert.lightDir);
  vec3 viewDir = normalize(Vert.viewDir);
  
  vec3 normal = normalize(Vert.normal);
  vec3 origin = viewDir * Vert.position.z/1000.0;
  vec3 rvec = texture(noise, Vert.texcoord * 1.0).xyz * 2.0 - 1.0;
  vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 tbn = mat3(tangent, bitangent, normal);
  
  gl_FragData[1] = vec4(0,0.5,1,1);
  gl_FragData[2] = vec4(normal, 1.0);
  gl_FragData[0] = Texcol;
}
#endif