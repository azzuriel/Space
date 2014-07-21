#include "SpaceGenerator.h"
#include "JHelpers_inl.h"
#include <math.h>
#include <boost/random.hpp>

//root is galaxy
GameObject* GenerateStar(GameObject *root)
{
    if(root == nullptr){
        return nullptr;
    }

    int seed = root->seed;

    GameObject *go = new GameObject();
    go->seed = seed + root->childrens.size()*57;
    root->childrens.push_back(go);

    boost::random::mt19937 rng;
    rng.seed(go->seed);
    boost::random::triangle_distribution<> spec_d(0, 5, 7);
    boost::random::lognormal_distribution<> temp_d(0.5, 1);

    srand(go->seed);
    go->spec = (int) spec_d(rng);

    go->M = triangular(0.1, 200, 10);

    go->lum = temp_d(rng) * 10000;


    double tmax = 0;
    double tmin = 0;
    switch (go->spec)
    {
    case SPT_O:  tmax = 3500;  tmin = 2000;  break;
    case SPT_B:  tmax = 5000;  tmin = 3500;  break;
    case SPT_A:  tmax = 6000;  tmin = 5000;  break;
    case SPT_F:  tmax = 7500;  tmin = 6000;  break;
    case SPT_G:  tmax = 10000; tmin = 7500;  break;
    case SPT_K:  tmax = 30000; tmin = 10000; break;
    case SPT_M:  tmax = 60000; tmin = 30000; break;
    default:                                 break;
    }
    boost::random::uniform_real_distribution<> temp_t(tmin, tmax);
    go->T = temp_t(rng);
    go->spec2 = 9 - (int)((go->T - tmin) / (tmax - tmin) * 10.0);

    go->type = STAR;

    return go;
}

