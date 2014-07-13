#pragma once
#include "..\Engine\Win.h"
#include "..\Engine\GraphBox.h"
#include <glm.hpp>
#include "..\Engine\FPSCounter.h"
class PerfomanceWindow :
    public Win
{
public:
    PerfomanceWindow(void);
    ~PerfomanceWindow(void);
    GraphBox *gb;
    void UpdateFps(const FPSCounter &fps);
    int cur_hist;
    int max;
};

