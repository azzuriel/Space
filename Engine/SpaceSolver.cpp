#include "SpaceSolver.h"
#include "JHelpers_inl.h"
#include <glm.hpp>
#include <detail\type_mat.hpp>
#include <gtc\matrix_transform.inl>

SpaceSolver::SpaceSolver(void)
{
}


SpaceSolver::~SpaceSolver(void)
{
}

double get_eccentric_anomaly(double mean, double eps) {
    auto x0 = eps > 0.8 ? M_PI : mean;
    auto x1 = x0 - ((x0 - eps*sin(x0) - mean)/(1 - eps*cos(x0)));
    while (true) {
        if (abs(x1 - x0) < 0.000001) {
            return x1;
        }
        x0 = x1;
        x1 = x0 - ((x0 - eps*sin(x0) - mean)/(1 - eps*cos(x0)));
    }
}

inline glm::vec3 solveSome(double a, double b, double epoch, double incl) {
    auto focal_lenght = getFocalLength(a, b);
    auto eps = getExcentricity(a, b);
    epoch = get_eccentric_anomaly(epoch, eps);
    auto t = glm::vec3(a*sin(epoch) + focal_lenght, 0, b*cos(epoch));
    if(incl == 0) {
        return t;
    }
    glm::mat4 mat = glm::rotate(glm::mat4(1),(float) incl, glm::vec3(0,0,1));
    return glm::vec3(glm::vec4(t, 1) * mat);
}

void SpaceSolver::MakeOrbits() {
    for (unsigned int i = 0; i < objects.size(); i++) {
        GameObject* go = objects[i];
        go->orbit.clear();
        for (unsigned int j = 0; j < 62; j++) {
            go->orbit.push_back(solveSome(go->majorAxis, go->minorAxis, j/10.0, go->inclination));
            go->orbit.push_back(solveSome(go->majorAxis, go->minorAxis, (j+1)/10.0, go->inclination));
        }
    }
}

void SpaceSolver::Solve(GameTimer gt) {
    for (unsigned int i = 0; i < objects.size(); i++) {

        GameObject* go = objects[i];
        go->epoch += gt.elapsed/(go->majorAxis/10.0);
        if(go->epoch > M_PI*2) {
            go->epoch = go->epoch - M_PI*2;
        }
        go->pos = solveSome(go->majorAxis, go->minorAxis, go->epoch, go->inclination);
    }
}