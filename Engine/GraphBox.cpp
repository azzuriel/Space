#include "GraphBox.h"
#include "ColorExtender.h"
#include "SpriteBatch.h"
#include "WinS.h"


GraphBox::GraphBox(void)
{
}

GraphBox::GraphBox(glm::vec2 pos_, glm::vec2 size_) :
    size(size_),
    pos(pos_)
{

}


GraphBox::~GraphBox(void)
{
}

void GraphBox::Draw() const
{
    Batched& sb = *WinS::sb;
    glm::vec2 Pos = GlobalPos();
    glm::vec4 col = Colors::White;

    sb.DrawLine(Pos, glm::vec2(Pos.x, Pos.y + size.y), 2, col);
    sb.DrawLine(Pos, glm::vec2(Pos.x + size.x, Pos.y), 2, col);
    sb.DrawLine(glm::vec2(Pos.x, Pos.y + size.y), Pos + size, 2, col);
    sb.DrawLine(glm::vec2(Pos.x + size.x, Pos.y), Pos + size, 2, col);
    
    if(points.size() > 1)
    for (int i=0; i< points.size() - 1; i++)
    {
        sb.DrawLine2d(points[i] * vec2(1,-1) + Pos + vec2(0, size.y), points[i+1] * vec2(1,-1) + Pos + vec2(0, size.y), Colors::Red);
    }
}

void GraphBox::Update()
{
    
}

glm::vec2 GraphBox::GlobalPos() const
{
    return parent->GlobalPos() + pos;
}
