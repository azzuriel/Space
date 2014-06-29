#pragma once
#include <glew.h>
#include <glm.hpp>
#include <string>
#include "TextureManager.h"
struct Material	
{
	Material(void);
	Texture *texture, *normal;
	glm::vec4 emission;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	float index_of_refraction;
	std::string id;
};	



struct PointLight
{
	PointLight(void);
	glm::vec4 position;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;

	//////////////////////////////////////////////////////////////////////////
	// x - constant attenuation
	// y - linear attenuation
	// z - quadratic attenuation
	//////////////////////////////////////////////////////////////////////////
	glm::vec3 attenuation;
};

