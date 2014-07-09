#pragma once
#include <glm.hpp>
#include <gtx\quaternion.hpp>
#include <vector>
#include <string>

class GameObject
{
public:
    GameObject(void);
    ~GameObject(void);
    std::string getFullDebugDescription();
    double M;
    GameObject* center;
    double speed;
    double majorAxis, minorAxis;
    double surfTemp;
    double epoch;
    double inclination;

    glm::vec3 pos;
    std::vector<glm::vec3> orbit;
};

