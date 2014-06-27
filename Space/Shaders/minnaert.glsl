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
  gl_Position   = transform.viewProjection * vertex;
}
#endif

#ifdef _FRAGMENT_

in Vertex {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

void main(void)
{
  vec3 normal   = Vert.normal;
  vec3 lightDir = Vert.lightDir;
  vec3 viewDir  = Vert.viewDir;
  
  color = texture(material.texture, Vert.texcoord) * max( dot ( normal, lightDir ), 0.0 );
  color.w = 1;
  
  const vec4  diffColor = material.diffuse * light.diffuse;
  const float k         = 0.8;

  float d1 = pow ( max ( dot ( normal, lightDir ), 0.0 ), 1.0 + k );
  float d2 = pow ( 1.0 - dot ( normal, viewDir ), 1.0 - k );

  color   = diffColor * d1 * d2;
  color  *= texture(material.texture, Vert.texcoord);
  color.w = 1;
}
#endif