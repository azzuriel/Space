#pragma once
#include <glew.h>
#include <gtc\matrix_transform.inl>
struct Material	
{
	GLuint texture, normal;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 emission;
	float shininess;
};	

struct PointLight
{
	glm::vec4  position;
	glm::vec4  ambient;
	glm::vec4  diffuse;
	glm::vec4  specular;
	glm::vec3 attenuation;
};

