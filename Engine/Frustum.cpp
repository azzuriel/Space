#include "Frustum.h"
#include <glm.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

Frustum::Frustum(void)
{
}


Frustum::~Frustum(void)
{
}

glm::vec4 planeWithPoints(glm::vec3 a, glm::vec3 b, glm::vec3 c){
    double A = a.y * (b.z - c.z) + b.y * (c.z - a.z) + c.y * (a.z - b.z);
    double B = a.z * (b.x - c.x) + b.z * (c.x - a.x) + c.z * (a.x - b.x);
    double C = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
    double D = -(a.x * (b.y * c.z - c.y * b.z) + b.x * (c.y * a.z - a.y * c.z) + c.x * (a.y * b.z - b.y * a.z));
    return glm::vec4(A,B,C,D);
}

void NormalizePlane(glm::vec4 &plane) {
    float nf = 1.0f / sqrtf(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);

    plane.x = nf * plane.x;
    plane.y = nf * plane.y;
    plane.z = nf * plane.z;
    plane.w = nf * plane.w;
}

void Frustum::Build(glm::mat4 viewMatrix, float aspect, float fov, float far, float near)
{
    float tang = tanf(fov * M_PI / 360.0f);
    float nh = near * tang; // near height
    float nw = nh * aspect; // near width
    float fh = far * tang; // far height
    float fw = fh * aspect; // far width

    //camera position
    auto p = glm::vec3(viewMatrix[3][0],viewMatrix[3][1],viewMatrix[3][2]);

    // the left vector
    glm::vec3 X = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    // the up vector
    glm::vec3 Y = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
    // the look vector
    glm::vec3 Z = glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]);

    auto nc = p - Z * near; // center of the near plane
    auto fc = p - Z * far; // center of the far plane

    // the distance to get to the left or right edge of the near plane from nc
    auto Xnw = X * nw;
    // the distance to get to top or bottom of the near plane from nc
    auto Ynh = Y * nh;
    // the distance to get to the left or right edge of the far plane from fc
    auto Xfw = X * fw;
    // the distance to get to top or bottom of the far plane from fc
    auto Yfh = Y * fh;

    auto ntl = nc + Ynh - Xnw; // "near top left"
    auto ntr = nc + Ynh + Xnw; // "near top right" and so on
    auto nbl = nc - Ynh - Xnw;
    auto nbr = nc - Ynh + Xnw;

    auto ftl = fc + Yfh - Xfw;
    auto ftr = fc + Yfh + Xfw;
    auto fbl = fc - Yfh - Xfw;
    auto fbr = fc - Yfh + Xfw;

    m_Frustum[PLANE_TOP] = planeWithPoints(ntr,ntl,ftl);
    m_Frustum[PLANE_BOTTOM] = planeWithPoints(nbl,nbr,fbr);
    m_Frustum[PLANE_LEFT] = planeWithPoints(ntl,nbl,fbl);
    m_Frustum[PLANE_RIGHT] = planeWithPoints(nbr,ntr,fbr);
    m_Frustum[PLANE_FRONT] = planeWithPoints(ntl,ntr,nbr);
    m_Frustum[PLANE_BACK] = planeWithPoints(ftr,ftl,fbl);

    // Normalize all the sides
    NormalizePlane(m_Frustum[PLANE_LEFT]);
    NormalizePlane(m_Frustum[PLANE_RIGHT]);
    NormalizePlane(m_Frustum[PLANE_TOP]);
    NormalizePlane(m_Frustum[PLANE_BOTTOM]);
    NormalizePlane(m_Frustum[PLANE_FRONT]);
    NormalizePlane(m_Frustum[PLANE_BACK]);
}

int Frustum::Contains(glm::vec3 max, glm::vec3 min) const
{
    glm::vec4 sphCenter = glm::vec4((max + min)/2.f, 1.f);
    float sphRadius = glm::distance(max, min) / 2.f;

    float fDistance;

    for(int i = 0; i < 6; ++i) {

        // find the distance to this plane
        fDistance = glm::dot(m_Frustum[i], sphCenter)+glm::length(m_Frustum[i]);

        // if this distance is < -sphere.radius, we are outside
        if(fDistance < -sphRadius)
            return INERSECT_OUT;

        // else if the distance is between +- radius, then we intersect
        if(fabs(fDistance) < sphRadius)
            return INERSECT_INTERSECT;
    }

    // otherwise we are fully in view
    return INERSECT_IN;
}
