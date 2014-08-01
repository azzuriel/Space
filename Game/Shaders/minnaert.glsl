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

#ifdef _TESSEVAL_
layout(quads, fractional_odd_spacing) in;

in TCS_OUT
{
    vec2 tc;
} tes_in[];

out TES_OUT
{
    vec2 tc;
} tes_out;

void main(void)
{
	mat4 mvp = transform.viewProjection * transform.model;
	
    //vec2 tc1 = mix(tes_in[0].tc, tes_in[1].tc, gl_TessCoord.x);
    //vec2 tc2 = mix(tes_in[2].tc, tes_in[3].tc, gl_TessCoord.x);
    vec2 tc;//mix(tc2, tc1, gl_TessCoord.y);
	
	float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
	tc = vec2(u, v);

    vec4 p1 = mix(gl_in[0].gl_Position,
                  gl_in[1].gl_Position,
                  gl_TessCoord.x);
    vec4 p2 = mix(gl_in[2].gl_Position,
                  gl_in[3].gl_Position,
                  gl_TessCoord.x);
    vec4 p = mix(p2, p1, gl_TessCoord.y);

    p.z += texture(material.texture, tc).r * 0.1;
	//p.z += (sin(u*3.1415*2)+cos(v*3.1415*2))/10.0;

	tes_out.tc = tc;
    gl_Position = mvp * p;
}
#endif

#ifdef _TESSCONTROL_
layout ( vertices = 4 ) out;    

in VS_OUT
{
    vec2 tc;
} tcs_in[];

out TCS_OUT
{
    vec2 tc;
} tcs_out[]; 

void main ()
{             
    
	if (gl_InvocationID == 0)
    {
	    mat4 mvp = transform.viewProjection * transform.model;
        vec4 p0 = mvp * gl_in[0].gl_Position;
        vec4 p1 = mvp * gl_in[1].gl_Position;
        vec4 p2 = mvp * gl_in[2].gl_Position;
        vec4 p3 = mvp * gl_in[3].gl_Position;
        p0 /= p0.w;
        p1 /= p1.w;
        p2 /= p2.w;
        p3 /= p3.w;
        if (p0.z <= 0.0 ||
            p1.z <= 0.0 ||
            p2.z <= 0.0 ||
            p3.z <= 0.0)
        {
             gl_TessLevelOuter[0] = 0.0;
             gl_TessLevelOuter[1] = 0.0;
             gl_TessLevelOuter[2] = 0.0;
             gl_TessLevelOuter[3] = 0.0;
        }
        else
        {
            float l0 = length(p2.xy - p0.xy) * 32.0 + 16.0;
            float l1 = length(p3.xy - p2.xy) * 32.0 + 16.0;
            float l2 = length(p3.xy - p1.xy) * 32.0 + 16.0;
            float l3 = length(p1.xy - p0.xy) * 32.0 + 16.0;
            gl_TessLevelOuter[0] = l0;
            gl_TessLevelOuter[1] = l1;
            gl_TessLevelOuter[2] = l2;
            gl_TessLevelOuter[3] = l3;
            gl_TessLevelInner[0] = min(l1, l3);
            gl_TessLevelInner[1] = min(l0, l2);
        }
    }
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tcs_out[gl_InvocationID].tc = tcs_in[gl_InvocationID].tc;
}
#endif

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXCOORD) in vec2 texcoord;
layout(location = VERT_NORMAL) in vec3 normal;

out Vertex {
        vec2 tc;
} Vert;

void main(void)
{
  const vec2 vetreces[] = vec2[] (
  vec2(0,0),
  vec2(1,0),
  vec2(0,1),
  vec2(1,1)
  );
  gl_Position = vec4(position, 1);
  Vert.tc = vetreces[gl_VertexID];
}
#endif


#ifdef _FRAGMENT_

layout(location = FRAG_OUTPUT0) out vec4 color;

in TES_OUT
{
    vec2 tc;
} tes_out;

void main(void)
{
  color = vec4(texture(material.texture, tes_out.tc).xyz, 1);
}
#endif