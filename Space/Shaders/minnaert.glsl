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
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
		vec4  smcoord;
		vec3  tangentSurf2light;
		vec3  tangentSurf2view;
} Vert;

void main(void)
{
  vec4 vertex   = transform.model * vec4(position, 1.0);

  
  vec4 lightDir = light.position - vertex; // без вертекс?!
  Vert.texcoord = texcoord;
  Vert.viewDir  = transform.viewPosition - vec3(vertex);
  Vert.normal   = transform.normal * normal;
  Vert.distance = length(lightDir);
  Vert.lightDir = lightDir.xyz;
  
  vec3 tangent;
  vec3 v1 = cross(normal, vec3(0.0,0.0,-1.0));
  vec3 v2 = cross(normal, vec3(0.0,-1.0,0.0));
  if(length(v1)>length(v2)){
	tangent = v1;
  } else {
	tangent = v2;
  }
  vec3 n = transform.normal * normal;
  vec3 t = transform.normal * tangent;
  vec3 b = cross(n,t);
  mat3 mat = mat3(t.x, b.x, n.x, t.y, b.y, n.y, t.z, b.z, n.z);
  
  vec3 vector   = normalize(vec3(light.position - vertex));
  Vert.tangentSurf2light = mat * vector;
  
  vector = normalize(-vec3(light.position));
  Vert.tangentSurf2view = mat * vector;
  
  Vert.smcoord  = transform.light * vertex;
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
		vec4  smcoord;
		vec3  tangentSurf2light;
		vec3  tangentSurf2view;
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


void main(void)
{
  vec3 texcol = vec3(1,1,1);
  vec3 normal   = normalize(Vert.normal);
  vec4 smcoord = Vert.smcoord;
  vec3 lightDir = normalize(Vert.lightDir);
  
  vec3 surf2light = normalize(Vert.tangentSurf2light);
  vec3 norm = normalize(vec3(texture(material.normal, Vert.texcoord))*2.0 - 1.0);
  norm += normal;
  norm = normalize(norm);
  float dcont = max(0.0, dot(norm, surf2light));
  vec3 diffuse = dcont*(vec3(material.diffuse));
  
  vec3 surf2view=normalize(Vert.tangentSurf2view);
  vec3 reflection=reflect(-surf2light, norm);
  
  float darker = max(0.0, dot(normal, lightDir));
  float darker2 = max(0.0, dot(norm, lightDir));
  
  float scont = pow(max(0.0, dot(surf2view, reflection)), material.shininess);
  vec3 specular = scont * vec3(material.specular);
  
  float shadow = PCF(smcoord);
  
  color   =  vec4((vec3(material.ambient)+diffuse+specular)*shadow*darker*darker2, 1.0);
  //color.rgb = specular;
  color.w = 1.0;
}
#endif