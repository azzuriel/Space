#include "SpaceSolver.h"
#include "JHelpers_inl.h"
#include <glm.hpp>
#include <detail\type_mat.hpp>
#include <gtc\matrix_transform.inl>

#define epsilon 0.000000001

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
        if (abs(x1 - x0) < epsilon) {
            return x1;
        }
        x0 = x1;
        x1 = x0 - ((x0 - eps*sin(x0) - mean)/(1 - eps*cos(x0)));
    }
}

inline glm::vec3 get_eccentric_xyz_for_epoch(double a, double b, double epoch, double incl, bool uniform = false) {
    const auto focal_lenght = getFocalLength(a, b);
    const auto eps = getExcentricity(a, b);
    epoch = uniform ? epoch : get_eccentric_anomaly(epoch, eps) - M_PI_2;
    auto t = glm::vec3(a*sin(epoch) + focal_lenght, 0, b*cos(epoch));
    if(incl == 0) {
        return t;
    }
    glm::mat4 mat = glm::rotate(glm::mat4(1),(float) incl, glm::vec3(0,0,1));
    return glm::vec3(glm::vec4(t, 1) * mat);
}

void SpaceSolver::MakeOrbits() {
    for (unsigned int i = 0; i < objects.size(); i++) {
        GameObject& go = *objects[i];
        go.orbit.clear();

        const auto pi_size = (int)(M_PI*2*10);
        for (unsigned int j = 0; j < pi_size; j++) {
            go.orbit.push_back(get_eccentric_xyz_for_epoch(go.majorAxis, go.minorAxis, j/10.0, go.inclination, true));
            go.orbit.push_back(get_eccentric_xyz_for_epoch(go.majorAxis, go.minorAxis, (j+1)/10.0, go.inclination, true));
        }
        go.orbit.push_back(get_eccentric_xyz_for_epoch(go.majorAxis, go.minorAxis, pi_size/10.0, go.inclination, true));
        go.orbit.push_back(get_eccentric_xyz_for_epoch(go.majorAxis, go.minorAxis, 0, go.inclination, true));
    }
}

void SpaceSolver::SolveTree(GameTimer gt, const GameObject &root) {
    if(root.childrens.size() == 0){

    } else {
        for (auto i: root.childrens)
        {

        }
    }
}

void SpaceSolver::Solve(GameTimer gt) {
    for (unsigned int i = 0; i < objects.size(); i++) {

        GameObject& go = *objects[i];
        go.epoch += gt.elapsed/(go.majorAxis/10.0)/10000.0;
        if(go.epoch > M_PI*2) {
            go.epoch = go.epoch - M_PI*2;
        }
        auto last = go.pos;
        go.pos = get_eccentric_xyz_for_epoch(go.majorAxis, go.minorAxis, go.epoch, go.inclination);
        go.speed = glm::distance(last, go.pos) / gt.elapsed;
    }
}