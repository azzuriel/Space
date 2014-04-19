//version 330 inserted here by programm
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL   2

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXCOORD) in vec2 texcoord;
layout(location = VERT_NORMAL)   in vec3 normal;

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
        Vert.normal   = transform.normal * normal;
        Vert.lightDir = vec3(lightDir);
        Vert.viewDir  = transform.viewPosition - vec3(vertex);
        Vert.distance = length(lightDir);
        gl_Position = transform.viewProjection * vertex;
}
#endif

#ifdef _FRAGMENT_

#define FRAG_OUTPUT0 0

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
        //vec3 normal   = normalize(Vert.normal);
        vec3 lightDir = normalize(Vert.lightDir);
        //vec3 viewDir  = normalize(Vert.viewDir);

        //float attenuation = 1.0 / (light.attenuation[0] +
        //        light.attenuation[1] * Vert.distance +
        //        light.attenuation[2] * Vert.distance * Vert.distance);
		//attenuation = min(attenuation, 2.0);
        //color = material.emission*100;
        //color += material.ambient * light.ambient * attenuation;
		//vec4 hc = texture(material.normal, Vert.texcoord);
        //float NdotL = max(dot(normal, lightDir) * 2.0 - 1.0, 0.0);
        //color += material.diffuse * light.diffuse * NdotL * attenuation;
        //float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
        //color += material.specular * light.specular * RdotVpow * attenuation;
        //color = texture(material.texture, Vert.texcoord);
		vec3 normal = texture(material.texture, Vert.texcoord).xyz;
		vec3 mininormal = texture(material.texture, Vert.texcoord*10.0).xyz;
		vec3 mininormal2 = texture(material.texture, Vert.texcoord*5.0).xyz;
		
		float diffuse = max(0.0, dot(normal + mininormal/20.0 + mininormal/10.0, lightDir));
		//float diffuse = max(0.0, dot(normal, lightDir)); //+ mininormal/20.0 + mininormal/10.0;
		color = vec4(1,0.5,0.5,1) * vec4(diffuse, diffuse, diffuse, 1.0);
}
#endif