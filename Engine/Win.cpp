#include "Win.h"
#include "glm.hpp"
#include "WinS.h"
#include <glm.hpp>
#include "Mouse.h"
#include <vector>


Win::Win(void)
{
	text = new TextGeometry("azazazaadasdasd");
}

Win::Win(glm::vec2 p, glm::vec2 s) :
	size(s),
	pos(p)
{
	text = new TextGeometry("azazazaadasdasd");
}


Win::~Win(void)
{
	if(Items.size() > 0){
		for(int i=0; i< Items.size(); i++){
			delete Items[i];
		}
	}
	Items.clear();
	delete text;
}

void Win::Draw() const
{
	Batched& sb = *WinS::sb;
	
	sb.DrawRectangle(pos, size, glm::vec4(0.F,0.F,0.F,0.75F));
	sb.DrawLine(pos, glm::vec2(pos.x, pos.y + size.y), 2, glm::vec4(1.F,1.F,1.F,0.5F));

	sb.DrawLine(pos, glm::vec2(pos.x + size.x, pos.y), 2, Colors::White);
	sb.DrawLine(pos + glm::vec2(0,20), glm::vec2(pos.x + size.x, pos.y + 20), 2, Colors::White);
	sb.DrawLine(glm::vec2(pos.x, pos.y + size.y), pos + size, 2, Colors::White);
	sb.DrawLine(glm::vec2(pos.x + size.x, pos.y), pos + size, 2, Colors::White);

	text->DrawAt(atCenter(text->Size, pos, glm::vec2(size.x, 20)));

	if(Items.size() > 0){
		for(int i=0; i< Items.size(); i++){
			Items[i]->Draw();
		}
	}
}

void Win::Update()
{
	glm::vec2 wpos = GlobalPos();
	if(!WinS::MouseHooked && inLimsV(Mouse::GetCursorLastPos(), wpos, wpos + glm::vec2(size.x,20))){
		if(Mouse::IsLeftDown()){
			pos += Mouse::GetCursorDelta();
		}
	}

	if(Items.size() > 0){
		for(int i=0; i< Items.size(); i++){
			Items[i]->Update();
		}
	}

	if(!WinS::MouseHooked && inLimsV(Mouse::GetCursorLastPos(), wpos, wpos + size)){
		if(Mouse::IsLeftDown()){
			WinS::ToTop(this);
		}
		WinS::MouseHooked = true;
	}
}

glm::vec2 Win::GlobalPos() const
{
	return pos;
}
