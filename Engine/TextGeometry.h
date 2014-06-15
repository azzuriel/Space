#pragma once
#include <string>
#include "Font.h"
class TextGeometry
{
public:
	TextGeometry(std::string text, const Font &font);
	TextGeometry(const Font &font);
	TextGeometry();
	TextGeometry(std::string text);
	~TextGeometry(void);
	std::string getText() const;
	void setText(std::string s);
	void DrawAt(glm::vec2 pos);
private:
	const Font* f;
	glm::vec2* uv;
	glm::vec3* vertex;
	glm::vec4* color;
	GLuint* index;
	int cou;
	std::string text;
};

