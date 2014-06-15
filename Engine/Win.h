#pragma once
#ifndef Win_h__
#define Win_h__
#include "glm.hpp"
#include <glm.hpp>
#include "WComponent.h"
#include <vector>
class Win : public WContainer
{
public:
	Win(void);
	Win(glm::vec2 p, glm::vec2 s);
	~Win(void);
	void Draw() const;
	void Update();
	glm::vec2 GlobalPos() const;
	glm::vec2 size;
	glm::vec2 pos;
	std::vector<WComponent*> Items;
};

#endif // Win_h__

