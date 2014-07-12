#define VERT_POSITION 0

#ifdef _VERTEX_
layout(location = VERT_POSITION) in vec3 position;

uniform struct Transform
{
        mat4 model;
        mat4 viewProjection;
        mat3 normal;
        vec3 viewPosition;
} transform;

void main(void)
{
		vec4 vertex   = transform.model * vec4(position, 1.0);
        gl_Position   = transform.viewProjection * vertex;
}
#endif

#ifdef _FRAGMENT_
void main(void)
{
}
#endif