#include "PerfomanceWindow.h"
#include "..\Engine\FPSCounter.h"


PerfomanceWindow::PerfomanceWindow(void) :
    cur_hist(0),
    max(0)
{
    size = vec2(200,200);
    gb = new GraphBox(vec2(20,20), vec2(100,100));
    gb->points.resize(100);
    gb->parent = this;
    for (int i=0;i<gb->points.size();i++)
    {
        gb->points[i] = vec2(i,0);
    }

    Items.push_back(gb);
}


PerfomanceWindow::~PerfomanceWindow(void)
{
}

void PerfomanceWindow::UpdateFps(const FPSCounter &fps)
{
    int cur = fps.GetCount();
    if(max < cur){
        max = cur;
        gb->scale.y = 1.0 / (max / 90.0);
    }
    gb->points[cur_hist].y = cur;
    cur_hist++;
    if(cur_hist > gb->points.size()-1){
        cur_hist = 0;
        max = gb->points[0].y;
        gb->scale.y = 1.0 / (gb->points[0].y / 90.0);
    } 
}
