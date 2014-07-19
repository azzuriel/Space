#pragma once
#include <glm.hpp>

    enum Planes {
        PLANE_TOP = 0,
        PLANE_BOTTOM = 1,
        PLANE_LEFT = 2,
        PLANE_RIGHT = 3,
        PLANE_FRONT = 4,
        PLANE_BACK = 5
    };

    enum Result {
        INERSECT_OUT = 0,
        INERSECT_INTERSECT = 1,
        INERSECT_IN = 2
    };

    class Frustum
    {
    public:
        Frustum(void);
        ~Frustum(void);
        glm::vec4 m_Frustum[6];

        void Build(glm::mat4 view, float aspect, float fov, float far, float near);
        int Contains(glm::vec3 max, glm::vec3 min) const;
};

