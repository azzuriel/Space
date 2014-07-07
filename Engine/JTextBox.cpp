#include "JTextBox.h"
#include "TextGeometry.h"
#include "SpriteBatch.h"
#include "WinS.h"
#include "Keyboard.h"
#include "JHelpers_inl.h"

JTextBox::JTextBox(void)
{
    text = new TextGeometry();
}

JTextBox::JTextBox(glm::vec2 p, glm::vec2 s) :
    pos(p),
    Size(s)
{
    text = new TextGeometry();
    text->setFixer(s.x);
}


JTextBox::~JTextBox(void)
{
    if(!text) {
        delete text;
    }
}

void JTextBox::Draw() const
{
    Batched& sb = *WinS::sb;
    glm::vec2 Pos = GlobalPos();
    glm::vec4 col = Colors::White;
    sb.DrawLine(Pos, glm::vec2(Pos.x, Pos.y + Size.y), 2, col);
    sb.DrawLine(Pos, glm::vec2(Pos.x + Size.x, Pos.y), 2, col);
    sb.DrawLine(glm::vec2(Pos.x, Pos.y + Size.y), Pos + Size, 2, col);
    sb.DrawLine(glm::vec2(Pos.x + Size.x, Pos.y), Pos + Size, 2, col);

    text->DrawAt(Pos + vec2(2,2));
}

glm::vec2 JTextBox::GlobalPos() const
{
    return pos + parent->GlobalPos();
}

void JTextBox::Update()
{
    if(!WinS::KeyboardHooked){
        auto t = Keyboard::EnquePressed();
        if(t!= -1) {
            text->setText(text->getText()+=keyToChar(t, Keyboard::Shift));
        }
    }
}