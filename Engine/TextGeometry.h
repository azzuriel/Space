#pragma once
#include <string>
#include "Font.h"
#include "glm.hpp"
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
    void append(std::string s);
    void setFixer(int width);
    glm::vec2 Size;
private:
    int fixer;
    const Font* f;
    glm::vec2* uv;
    glm::vec3* vertex;
    glm::vec4* color;
    GLuint* index;
    int cou;
    std::string text;
};

