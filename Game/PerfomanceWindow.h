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
    void UpdateValue(int value);
    float avg;
    int cur;
    void Draw() const;
};

