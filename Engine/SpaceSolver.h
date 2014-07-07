#pragma once
#include "GameTimer.h"
#include "GameObject.h"
#include <vector>
class SpaceSolver
{
public:
	SpaceSolver(void);
	~SpaceSolver(void);
	void Solve(GameTimer gt);
	void MakeOrbits(void);
	std::vector<GameObject*> objects;
};

