#include "WinGrid.h"

WinGrid::WinGrid(void)
{

}


WinGrid::~WinGrid(void)
{
}

void WinGrid::Draw() const
{
    throw std::logic_error("The method or operation is not implemented.");
}

void WinGrid::Update()
{
    throw std::logic_error("The method or operation is not implemented.");
}

glm::vec2 WinGrid::GlobalPos() const
{
     return pos + parent->GlobalPos();
}
