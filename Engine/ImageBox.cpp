#include "ImageBox.h"
#include "ColorExtender.h"
#include "SpriteBatch.h"
#include "WinS.h"


ImageBox::ImageBox(void) :
    border(false)
{
}

ImageBox::ImageBox(glm::vec2 pos_, glm::vec2 size_) :
    size(size_),
    pos(pos_),
    border(false)
{

}


ImageBox::~ImageBox(void)
{
}

void ImageBox::Draw() const
{
    Batched& sb = *WinS::sb;
    glm::vec2 Pos = GlobalPos();
    glm::vec4 col = Colors::White;

    sb.DrawQuad(Pos, size, *texture);

    if(border) {
        sb.DrawLine(Pos, glm::vec2(Pos.x, Pos.y + size.y), 2, col);
        sb.DrawLine(Pos, glm::vec2(Pos.x + size.x, Pos.y), 2, col);
        sb.DrawLine(glm::vec2(Pos.x, Pos.y + size.y), Pos + size, 2, col);
        sb.DrawLine(glm::vec2(Pos.x + size.x, Pos.y), Pos + size, 2, col);
    }
}

void ImageBox::Update()
{

}

glm::vec2 ImageBox::GlobalPos() const
{
    return parent->GlobalPos() + pos;
}
