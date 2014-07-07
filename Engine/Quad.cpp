#include "Quad.h"
#include "Mesh.h"


Mesh Quad::GetMesh()
{
    if(sm_mesh != nullptr){
        return *sm_mesh;
    }
    Mesh* m = new Mesh();

    m->Verteces.resize(4);
    m->Indeces.resize(6);

    float s2 = 0.5;
    m->Verteces[0].Position = glm::vec3(-s2,-s2,0);
    m->Verteces[1].Position = glm::vec3(s2,-s2,0);
    m->Verteces[2].Position = glm::vec3(-s2,s2,0);
    m->Verteces[3].Position = glm::vec3(s2,s2,0);
    m->Verteces[0].Uv = glm::vec2(0,0);
    m->Verteces[1].Uv = glm::vec2(1,0);
    m->Verteces[2].Uv = glm::vec2(0,1);
    m->Verteces[3].Uv = glm::vec2(1,1);
    m->Verteces[0].Normal = glm::vec3(0,0,1);
    m->Verteces[1].Normal = glm::vec3(0,0,1);
    m->Verteces[2].Normal = glm::vec3(0,0,1);
    m->Verteces[3].Normal = glm::vec3(0,0,1);

    m->Indeces[0] = 0;
    m->Indeces[1] = 1;
    m->Indeces[2] = 2;
    m->Indeces[3] = 1;
    m->Indeces[4] = 3;
    m->Indeces[5] = 2;

    sm_mesh = m;
    return *m;
}

Mesh* Quad::sm_mesh;
