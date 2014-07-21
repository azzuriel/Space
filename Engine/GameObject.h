#pragma once
#include <glm.hpp>
#include <gtx\quaternion.hpp>
#include <vector>
#include <string>

enum GoType {
    STAR = 4,
    GALAXY = 3,
    PLANET = 5,
    SATELLITE = 6
};

enum SpectralType {
    SPT_O = 0,
    SPT_B = 1,
    SPT_A = 2,
    SPT_F = 3,
    SPT_G = 4,
    SPT_K = 5,
    SPT_M = 6
};

enum LuminosityClass {
    ST_CLASS_Zero = 0,
    ST_CLASS_IaPlus = 1,
    ST_CLASS_I = 2, ST_CLASS_Ia = 3, ST_CLASS_Iab = 4, ST_CLASS_Ib = 5,
    ST_CLASS_II = 6, ST_CLASS_IIa = 7, ST_CLASS_IIb = 8,
    ST_CLASS_III = 9, ST_CLASS_IIIa = 10, ST_CLASS_IIIab = 11, ST_CLASS_IIIb = 12,
    ST_CLASS_IV = 13,
    ST_CLASS_V = 14, ST_CLASS_Va = 15, ST_CLASS_Vb = 16,
    ST_CLASS_VI = 17,
    ST_CLASS_VII = 18,
};


class GameObject
{
public:
    GameObject(void);
    ~GameObject(void);
    std::string getFullDebugDescription();
    double M;
    double T;
    double lum;

    GameObject* parent;
    std::vector<GameObject*> childrens;

    double speed;
    double majorAxis, minorAxis;
    double surfTemp;
    double epoch;
    double inclination;

    int type;
    int seed;
    int lumclass, spec, spec2;

    glm::vec3 pos;
    std::vector<glm::vec3> orbit;
};

