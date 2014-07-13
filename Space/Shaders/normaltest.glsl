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
		vec3  normal;
} Vert;

void main(void)
{
  vec4 vertex   = transform.model * vec4(position, 1.0);
  
  vec4 pos      = transform.viewProjection * vertex;
  Vert.normal   = transform.normal * normal;
  
  gl_Position   = pos;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
		vec3  normal;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

void main(void)
{
  color.rgb = normalize(Vert.normal);
  color.w = 1;
}
#endif