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

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_TEXCOORD) in vec2 texcoord;

out vec2 fragTexcoord;
out vec4 vertexColor;

void main()
{	
	gl_Position =  vec4(position, 1.0);
	fragTexcoord = texcoord;
}
#endif

#ifdef _FRAGMENT_

layout(binding=0) uniform sampler2D colorTexture;

in vec2 fragTexcoord;
in vec4 vertexColor;

void main()
{
	gl_FragData[0] = texture(colorTexture, fragTexcoord)*vertexColor;
}
#endif