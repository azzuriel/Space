#include "GameObject.h"
#include <string>
#include "JHelpers_inl.h"


GameObject::GameObject(void) :
    majorAxis(2),
    minorAxis(1),
    epoch(0),
    inclination(0)
{
}


GameObject::~GameObject(void)
{
}

std::string GameObject::getFullDebugDescription()
{
    return string_format("a = %f\nb = %f\npos = %s\nspeed = %f", 
                            majorAxis, 
                            minorAxis, 
                            std::to_string(pos).c_str(),
                            speed);
}
