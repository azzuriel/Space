#include "PerfomanceWindow.h"
#include "..\Engine\FPSCounter.h"
#include "..\Engine\JHelpers_inl.h"
#include "..\Engine\WinS.h"
#include "..\Engine\ColorExtender.h"
#define SIZE 100

PerfomanceWindow::PerfomanceWindow(void) :
    avg(0)
{
    size = vec2(200, 200);
    gb = new GraphBox(vec2(20, 20), vec2(SIZE, 100));
    gb->points.resize(SIZE);
    gb->parent = this;
    for (auto point: gb->points)
    {
        point = 0;
    }

    Items.push_back(gb);
}


PerfomanceWindow::~PerfomanceWindow(void)
{
}

void PerfomanceWindow::UpdateValue(int value)
{
    cur = value;
    gb->scale.y = 1.0 / (cur / 50.0);
    gb->points.erase(gb->points.begin());
    gb->points.push_back(cur);
    avg = (avg * (SIZE-1) + cur)/ (float) SIZE;
}

void PerfomanceWindow::Draw() const
{
    Win::Draw();
    auto pos = Win::GlobalPos();
    WinS::sb->DrawString(vec2(22,22)+pos, string_format("cur: %i", cur), Colors::Red, *WinS::font);
    WinS::sb->DrawString(vec2(22,22+9)+pos, string_format("avg: %f", avg), Colors::Red, *WinS::font);
}
