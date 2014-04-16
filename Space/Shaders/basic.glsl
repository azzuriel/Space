#version 410 core
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
        vec3 normal   = normalize(Vert.normal);
        vec3 lightDir = normalize(Vert.lightDir);
        vec3 viewDir  = normalize(Vert.viewDir);

        float attenuation = 1.0 / (light.attenuation[0] +
                light.attenuation[1] * Vert.distance +
                light.attenuation[2] * Vert.distance * Vert.distance);
		//attenuation = min(attenuation, 2.0);
        color = material.emission*100;
        color += material.ambient * light.ambient * attenuation;
		//vec4 hc = texture(material.normal, Vert.texcoord);
        float NdotL = max(dot(normal, lightDir) * 2.0 - 1.0, 0.0);
        color += material.diffuse * light.diffuse * NdotL * attenuation;
        float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
        color += material.specular * light.specular * RdotVpow * attenuation;
        color *= texture(material.texture, Vert.texcoord);
		color = vec4(0.0,0.0,0.0,1.0);
}
#endif

#ifdef _TESSCONTROL_
#define ID gl_InvocationID
layout ( vertices = 3 ) out;            // input patch consists of 3 vertices

uniform int InnerLevel = 2;
uniform int OuterLevel = 2;

uniform float level;

out VertVS {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
} vertvs[];

in VertES {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
} vertes[];

void main ()
{                                       // copy current vertex to output
	vertvs[ID].texcoord = vertes[ID].texcoord; 
	vertvs[ID].normal = vertes[ID].normal; 
	vertvs[ID].lightDir = vertes[ID].lightDir; 
	vertvs[ID].viewDir = vertes[ID].viewDir; 
	vertvs[ID].distance = vertes[ID].distance; 
    if ( gl_InvocationID == 0 )         // set tessellation level, can do only for one vertex
    {
        gl_TessLevelInner [0] = InnerLevel;
        gl_TessLevelOuter [0] = OuterLevel;
        gl_TessLevelOuter [1] = OuterLevel;
        gl_TessLevelOuter [2] = OuterLevel;
   }
   gl_out [ID].gl_Position = gl_in[ID].gl_Position;
}
#endif

#ifdef _TESSEVAL_
layout(triangles, equal_spacing) in;

in VertES {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
} vertes[];

out Vertex {
        vec2  texcoord;
        vec3  normal;
        vec3  lightDir;
        vec3  viewDir;
        float distance;
} vertfs;

void main(void)
{
		vertfs.texcoord = vertes[0].texcoord;
		gl_Position = gl_TessCoord.x * gl_in [0].gl_Position + 
					  gl_TessCoord.y * gl_in [1].gl_Position +
					  gl_TessCoord.z * gl_in [2].gl_Position;
}
#endif