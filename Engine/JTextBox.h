#pragma once
#include "WComponent.h"
#include <glm.hpp>
#include "TextGeometry.h"

class JTextBox :
	public WComponent
{
public:
	JTextBox(glm::vec2 p, glm::vec2 s);
	JTextBox(void);
	~JTextBox(void);
	virtual void Draw() const;
	virtual void Update();
	virtual glm::vec2 GlobalPos() const;

	WContainer* parent;
	glm::vec2 Size;
	TextGeometry* text;

private:
	glm::vec2 pos;
};

