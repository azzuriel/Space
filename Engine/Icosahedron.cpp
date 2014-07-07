#include "Icosahedron.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Mesh.h"

#define VERTEXCOUNT 12
#define INDEXCOUNT 60

static const GLuint __indeces[INDEXCOUNT] =
{
    0,  1,  2,
    0,  2,  3,
    0,  3,  4,
    0,  4,  5,
    0,  5,  1,

    11, 7,  6,
    11, 8,  7,
    11, 9,  8,
    11, 10, 9,
    11, 6,  10,

    2,  1,  6,
    3,  2,  7,
    4,  3,  8,
    5,  4,  9,
    1,  5,  10,

    6,  7,  2,
    7,	8,	3,
    8,	9,	4,
    9,	10,	5,
    10,	6,	1
};

Mesh Icosahedron::getMesh()
{
    if(sm_mesh != nullptr){
        return *sm_mesh;
    }
    Mesh* m = new Mesh();
    float magicAngle = M_PI * 26.565 / 180;
    float segmentAngle = M_PI * 72 / 180;
    float currentAngle = 0;

    glm::vec3 v[VERTEXCOUNT];
    v[0] = glm::vec3(0, 0.5, 0);
    v[11] = glm::vec3(0, -0.5, 0);

    for (int i=1; i<6; i++)
    {
        v[i] = glm::vec3(0.5 * sin(currentAngle) * cos(magicAngle),
            0.5 * sin(magicAngle),
            0.5 * cos(currentAngle) * cos(magicAngle));
        currentAngle += segmentAngle;
    }
    currentAngle = M_PI * 36 / 180;
    for (int i=6; i<11; i++)
    {
        v[i] = glm::vec3(0.5 * sin(currentAngle) * cos(-magicAngle),
            0.5 * sin(-magicAngle),
            0.5 * cos(currentAngle) * cos(-magicAngle));
        currentAngle += segmentAngle;
    }

    m->Verteces.resize(VERTEXCOUNT);
    m->Indeces.resize(INDEXCOUNT);

    for (int i=0; i < VERTEXCOUNT; i++)
    {
        m->Verteces[i].Position = v[i];
        m->Verteces[i].Uv =  glm::vec2(rand()%100/100.0,rand()%100/100.0);
    }

    for (int i=0; i < INDEXCOUNT; i++)
    {
        m->Indeces[i] = __indeces[i];
    }

    Icosahedron::sm_mesh = m;
    return *m;
}

Mesh* Icosahedron::sm_mesh = nullptr;
