#pragma once
#pragma once
#ifndef JButton_h__
#define JButton_h__
#include "WComponent.h"
#include <string>
#include "glm.hpp"

class JButton : public WComponent
{
public:
	JButton(void);
	JButton(glm::vec2 pos_, glm::vec2 size_);
	~JButton(void);

	void Draw() const;
	void Update();
	glm::vec2 GlobalPos() const;
	std::string Text;
	WContainer* parent;
private:
	glm::vec2 pos;
	glm::vec2 size;
	bool aimed;
};
#endif // JButton_h__

