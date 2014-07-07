#pragma once
#ifndef Quad_h__
#define Quad_h__

#include "Mesh.h"
class Quad
{
public:
    static Mesh GetMesh();
private:
    static Mesh* sm_mesh;
};
#endif // Quad_h__

