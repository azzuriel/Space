#include "JLabel.h"
#include "WinS.h"
#include "WComponent.h"


JLabel::JLabel(void)
{
	text = new TextGeometry();
}

JLabel::JLabel(glm::vec2 a) :
	pos(a)
{
	text = new TextGeometry();
}


JLabel::~JLabel(void)
{
	if(text){
		delete text;
	}
}

void JLabel::Draw() const
{
	text->DrawAt(GlobalPos());
}

void JLabel::Update()
{
	
}

glm::vec2 JLabel::GlobalPos() const
{
	return parent->GlobalPos() + pos;
}

