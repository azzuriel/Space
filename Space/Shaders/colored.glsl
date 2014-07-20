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
 
layout (location = VERT_POSITION) in vec3 inPosition;
layout (location = VERT_COLOR) in vec4 inColor;
 
out vec4 vertexColor;
 
uniform mat4 MVP; 
 
void main()
{
	gl_Position = MVP * vec4(inPosition, 1.0);
	vertexColor = inColor;
}
#endif 
 
#ifdef _FRAGMENT_
 
in vec4 vertexColor;
layout(location = FRAG_OUTPUT0) out vec4 color;
 
void main()
{
	color = vertexColor;
}
#endif