#pragma once
#include "WComponent.h"
#include <string>
class JLabel : public WComponent
{
public:
	JLabel(void);
	JLabel(glm::vec2 a, std::string s);
	~JLabel(void);
	virtual void Draw() const;
	virtual void Update();
	virtual glm::vec2 GlobalPos() const;
	WContainer* parent;
	std::string text;
private:
	glm::vec2 pos;
	glm::vec2 size;
	bool aimed;
};

