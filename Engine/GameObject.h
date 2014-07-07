#pragma once
#include <glm.hpp>
#include <gtx\quaternion.hpp>
#include <vector>

class GameObject
{
public:
	GameObject(void);
	~GameObject(void);
	double M;
	GameObject* center;
	double speed;
	double majorAxis, minorAxis;
	double surfTemp;
	double epoch;
	double inclination;

	glm::vec3 pos;
	std::vector<glm::vec3> orbit;
};

