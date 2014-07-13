#include "JButton.h"
#include "WComponent.h"
#include "SpriteBatch.h"
#include "WinS.h"
#include "GameMath.h"
#include "Mouse.h"
#include "JHelpers_inl.h"

JButton::JButton(void) :
    pos(0),
    size(50,20)
{
    text = new TextGeometry("Button");
}

JButton::JButton(glm::vec2 pos_, glm::vec2 size_) : 
    pos(pos_),
    size(size_)
{
    text = new TextGeometry("Button");
}


JButton::~JButton(void)
{
    delete text;
}

void JButton::Draw() const
{
    Batched& sb = *WinS::sb;
    glm::vec2 Pos = GlobalPos();
    glm::vec4 col;
    if(aimed){
        col = Colors::White;
    } else {
        col = Colors::Red;
    }
    sb.DrawLine(Pos, glm::vec2(Pos.x, Pos.y + size.y), 2, col);
    sb.DrawLine(Pos, glm::vec2(Pos.x + size.x, Pos.y), 2, col);
    sb.DrawLine(glm::vec2(Pos.x, Pos.y + size.y), Pos + size, 2, col);
    sb.DrawLine(glm::vec2(Pos.x + size.x, Pos.y), Pos + size, 2, col);

    text->DrawAt(atCenter(text->Size, Pos, size));
}

void JButton::Update()
{
    if(WinS::MouseHooked){
        aimed = false;
        return;
    }

    glm::vec2 wpos = GlobalPos();
    if(inLimsV(Mouse::GetCursorPos(), wpos, wpos + size)){
        aimed = true;
        if(Mouse::IsLeftPressed()){
            if(onPress){
                onPress();
            }
        }
    } else {
        aimed = false;
    }
}

glm::vec2 JButton::GlobalPos() const
{
    return parent->GlobalPos() + pos;
}

void JButton::SetText(std::string s)
{
    text->setText(s);
}
