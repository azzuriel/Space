#include "JLabel.h"
#include "WinS.h"


JLabel::JLabel(void)
{
}

JLabel::JLabel(glm::vec2 a, std::string s) :
	pos(a),
	text(s)
{
}


JLabel::~JLabel(void)
{
}

void JLabel::Draw() const
{
	WinS::sb->DrawString(GlobalPos(), text, *WinS::font);
}

void JLabel::Update()
{
	
}

glm::vec2 JLabel::GlobalPos() const
{
	return parent->GlobalPos() + pos;
}
