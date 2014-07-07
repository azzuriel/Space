#include "FPSCounter.h"

#include <glfw3.h>



FPSCounter::FPSCounter(void)
{
    sm_fpsTime = 0;
    sm_fpsC = 0;
}


FPSCounter::~FPSCounter(void)
{
}

void FPSCounter::Update(GameTimer gt)
{
    sm_fpsTime += gt.elapsed;
    sm_fpsC++;
    if(sm_fpsTime > 1.0){
        sm_fpsTime -= 1.0;
        sm_fps = sm_fpsC;
        sm_fpsC = 0;
    }
}

unsigned int FPSCounter::GetCount()
{
    return sm_fps;
}
