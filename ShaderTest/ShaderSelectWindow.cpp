#include "ShaderSelectWindow.h"
#include <glm.hpp>
#define w 500
#define h 200

ShaderSelectWindow::ShaderSelectWindow(void) :
    Win(vec2(100,100), vec2(w,h))
{
    float off = 10;
    apply1 = new JButton(vec2(off, h/4*2), vec2(50, 20));
    apply1->parent = this;
    off += apply1->size.x + 20;

    apply2 = new JButton(vec2(off, h/4*2), vec2(50, 20));
    apply2->parent = this;
    off += apply2->size.x + 20;

    apply3 = new JButton(vec2(off, h/4*2), vec2(50, 20));
    apply3->parent = this;
    off += apply3->size.x + 20;

    apply4 = new JButton(vec2(off, h/4*2), vec2(50, 20));
    apply4->parent = this;
    off += apply4->size.x + 20;

    apply5 = new JButton(vec2(off, h/4*2), vec2(50, 20));
    apply5->parent = this;
    off += apply5->size.x + 20;

    Items.push_back(apply1);
    Items.push_back(apply2);
    Items.push_back(apply3);
    Items.push_back(apply4);
    Items.push_back(apply5);
}


ShaderSelectWindow::~ShaderSelectWindow(void)
{
}
