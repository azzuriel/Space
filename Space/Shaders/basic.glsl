//version inserted here by programm
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3

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

  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;

uniform struct Transform
{
  mat4 model;
  mat4 viewProjection;
  mat3 normal;
  vec3 viewPosition;
} transform;

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXCOORD) in vec2 texcoord;
layout(location = VERT_NORMAL)   in vec3 normal;

// параметры для фрагментного шейдера
out Vertex {
  vec2  texcoord;
  vec3  normal;
  vec3  lightDir;
  vec3  viewDir;
  float distance;
} Vert;

void main(void)
{
  // переведем координаты вершины в мировую систему координат
  vec4 vertex   = transform.model * vec4(position, 1.0);

  // направление от вершины на источник освещения в мировой системе координат
  vec4 lightDir = light.position - vertex;

  // передадим во фрагментный шейдер некоторые параметры
  // передаем текстурные координаты
  Vert.texcoord = texcoord;
  // передаем нормаль в мировой системе координат
  Vert.normal   = transform.normal * normal;
  // передаем направление на источник освещения
  Vert.lightDir = vec3(lightDir);
  // передаем направление от вершины к наблюдателю в мировой системе координат
  Vert.viewDir  = transform.viewPosition - vec3(vertex);
  // передаем рассятоние от вершины до источника освещения
  Vert.distance = length(lightDir);

  // переводим координаты вершины в однородные
  gl_Position = transform.viewProjection * vertex;
}
#endif

#ifdef _FRAGMENT_

#define FRAG_OUTPUT0 0

// параметры полученные из вершинного шейдера
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
  // нормализуем полученные данные для коррекции интерполяции
  vec3 normal   = normalize(Vert.normal);
  vec3 lightDir = normalize(Vert.lightDir);
  vec3 viewDir  = normalize(Vert.viewDir);

  // коэффициент затухания
  float attenuation = 1.0 / (light.attenuation[0] +
    light.attenuation[1] * Vert.distance +
    light.attenuation[2] * Vert.distance * Vert.distance);

  // добавим собственное свечение материала
  color = material.emission;

  // добавим фоновое освещение
  color += material.ambient * light.ambient * attenuation;

  // добавим рассеянный свет
  float NdotL = max(dot(normal, lightDir), 0.0);
  color += material.diffuse * light.diffuse * NdotL * attenuation;

  // добавим отраженный свет
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
  color += material.specular * light.specular * RdotVpow * attenuation;

  // вычислим итоговый цвет пикселя на экране с учетом текстуры
  color *= texture(material.texture, Vert.texcoord);
}
#endif