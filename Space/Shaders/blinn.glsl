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
} transform;

uniform struct PointLight
{
        vec4 position;
        vec4 ambient;
        vec4 diffuse;
        vec4 specular;
        vec3 attenuation;
} light;

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
		vec3 bissector;
} Vert;

void main(void)
{
  vec4 vertex   = transform.model * vec4(position, 1.0);

  vec4 lightDir = light.position - vertex;

  Vert.texcoord = texcoord;
  Vert.viewDir  = normalize(transform.viewPosition - vertex.xyz);
  Vert.normal   = normalize(transform.normal *normal);
  Vert.distance = length(lightDir);
  Vert.lightDir = normalize(lightDir).xyz;
  gl_Position = transform.viewProjection * vertex;
  Vert.bissector = (lightDir.xyz+Vert.viewDir)/abs(lightDir.xyz+Vert.viewDir);
}
#endif

#ifdef _FRAGMENT_

in Vertex {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
		vec3  bissector;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

void main(void)
{
  vec3 normal   = Vert.normal;
  vec3 lightDir = Vert.lightDir;
  vec3 viewDir  = Vert.viewDir;

  const vec4  diffColor = vec4 ( 0.5, 0.0, 0.0, 1.0 );
  const vec4  specColor = vec4 ( 0.7, 0.7, 0.0, 1.0 );
  const float specPower = 30.0;

  vec4 diff = diffColor * max ( dot ( normal, lightDir ), 0.0 );
  vec4 spec = specColor * pow ( max ( dot ( normal, Vert.bissector ), 0.0 ), specPower );

  color   = diff + spec;
  color.w = 1;
}
#endif