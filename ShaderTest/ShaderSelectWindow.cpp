#include "ShaderSelectWindow.h"
#include <glm.hpp>
#define w 300
#define h 200

ShaderSelectWindow::ShaderSelectWindow(void) :
    Win(vec2(100,100), vec2(w,h))
{
    float off = 10;
    apply1 = JButton(vec2(off, h/4*2), vec2(20, 50));
    off += apply1.size.x + 20;
    apply1 = JButton(vec2(off, h/4*2), vec2(20, 50));
}


ShaderSelectWindow::~ShaderSelectWindow(void)
{
}
