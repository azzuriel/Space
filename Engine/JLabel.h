#pragma once
#include "WComponent.h"
#include <string>
#include <glew.h>
#include "TextGeometry.h"
class JLabel : public WComponent
{
public:
	JLabel(void);
	JLabel(glm::vec2 a);
	~JLabel(void);
	virtual void Draw() const;
	virtual void Update();
	virtual glm::vec2 GlobalPos() const;
	WContainer* parent;
	TextGeometry* text;
	
private:
	glm::vec2 pos;
	glm::vec2 size;
	bool aimed;
};

