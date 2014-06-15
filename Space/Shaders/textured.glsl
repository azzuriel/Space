//version inserted here by programm
#define VERT_POSITION 0
#define VERT_TEXCOORD 1
#define VERT_NORMAL 2
#define VERT_COLOR 3
#define FRAG_OUTPUT0 0

#ifdef _VERTEX_

layout(location = VERT_POSITION) in vec3 position;
layout(location = VERT_COLOR) in vec4 color;
layout(location = VERT_TEXCOORD) in vec2 texcoord;

out vec2 fragTexcoord;
out vec4 vertexColor;

uniform mat4 MVP;

void main()
{	
	gl_Position =  MVP * vec4(position, 1.0);
	fragTexcoord = texcoord;
	vertexColor = color;
}
#endif

#ifdef _FRAGMENT_

uniform sampler2D colorTexture;

in vec2 fragTexcoord;
in vec4 vertexColor;

layout(location = FRAG_OUTPUT0) out vec4 color;

void main()
{
	color = texture(colorTexture, fragTexcoord)*vertexColor;
}
#endif