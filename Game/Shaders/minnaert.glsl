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

uniform int NoTangent;

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
        vec2  texcoord;
        vec3  normal;
		vec3  tangent;
		vec3  binormal;
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
  Vert.distance  = length(transform.viewPosition - vec3(vertex));
  
  Vert.normal = normalize(transform.normal * normal);
  if(NoTangent != 0) {
	vec3 tangent;
	vec3 v1 = cross(gl_Normal, vec3(1.0, 0.0, 0.0));
	vec3 v2 = cross(gl_Normal, vec3(0.0, 1.0, 0.0));
	if(length(v1)>length(v2))
		tangent=v1;
	else
		tangent=v2;
	Vert.tangent = normalize(transform.normal * tangent);
	Vert.binormal = cross(Vert.normal, Vert.tangent);
  }
  
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
		vec3  tangent;
		vec3  binormal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
		vec4  smcoord;
		vec4  position;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

float PCF(in vec4 smcoord)
{
        float res = 0.0;

        res += textureProjOffset(depthTexture, smcoord, ivec2(-1,-1));
        res += textureProjOffset(depthTexture, smcoord, ivec2( 0,-1));
        res += textureProjOffset(depthTexture, smcoord, ivec2( 1,-1));
        res += textureProjOffset(depthTexture, smcoord, ivec2(-1, 0));
        res += textureProjOffset(depthTexture, smcoord, ivec2( 0, 0));
        res += textureProjOffset(depthTexture, smcoord, ivec2( 1, 0));
        res += textureProjOffset(depthTexture, smcoord, ivec2(-1, 1));
        res += textureProjOffset(depthTexture, smcoord, ivec2( 0, 1));
        res += textureProjOffset(depthTexture, smcoord, ivec2( 1, 1));
        
		return (res / 9.0);
}


const float cutoff = 0.9f;

void main(void)
{
  vec4 Texcol = texture2D(material.texture, Vert.texcoord);
  if (Texcol.a < cutoff)
  {
    discard;
  }
  vec3 normal;
  if(NoTangent != 0) {
	vec3 t = Vert.tangent;
	vec3 b = Vert.binormal;
	vec3 n = Vert.normal;
	mat3 mat = mat3(t.x,b.x,n.x,t.y,b.y,n.y,t.z,b.z,n.z);
	vec3 norm = normalize(texture2D(material.normal, Vert.texcoord).xyz*2.0-1.0);
	vec3 normal = normalize(norm * mat);
  } else {
	normal = normalize(Vert.normal);
  }
  vec4 smcoord = Vert.smcoord;
  vec3 lightDir = normalize(Vert.lightDir);
  vec3 viewDir = normalize(Vert.viewDir);
  
  color = vec4(0,0,0,1);
  

  //color += material.ambient;
  float NdotL = max(dot(normal, lightDir), 0.0);
  color += material.diffuse * NdotL;
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess/2.f), 0.0);
  color += material.specular * RdotVpow;
  
  color += (material.ambient + light.ambient)*material.diffuse;  
  
  color *= Texcol;
  //luma
  color.a = 1 - 0.2126*color.r + 0.7152*color.g + 0.0722*color.b;
}
#endif