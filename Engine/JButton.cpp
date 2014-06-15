#include "JButton.h"
#include "WComponent.h"
#include "SpriteBatch.h"
#include "WinS.h"
#include "GameMath.h"
#include "Mouse.h"

JButton::JButton(void)
{
}

JButton::JButton(glm::vec2 pos_, glm::vec2 size_)
{
	pos = pos_; size = size_;
}


JButton::~JButton(void)
{
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

	sb.DrawString(Pos + glm::vec2(3,-8), "Button", *WinS::font);
}

void JButton::Update()
{
	glm::vec2 wpos = GlobalPos();
	if(inLims(Mouse::GetCursorPos(), wpos, wpos + size)){
		aimed = true;
	} else {
		aimed = false;
	}
}

glm::vec2 JButton::GlobalPos() const
{
	return parent->GlobalPos() + pos;
}
