#include "TextGeometry.h"
#include "WinS.h"
#include "glm.hpp"
TextGeometry::TextGeometry(std::string text, const Font &font) :
    color(nullptr),
    vertex(nullptr),
    uv(nullptr),
    index(nullptr),
    fixer(1000)
{
    f = &font;
    setText(text);
}

TextGeometry::TextGeometry(std::string text) :
    color(nullptr),
    vertex(nullptr),
    uv(nullptr),
    index(nullptr),
    fixer(1000)
{
    f = WinS::font;
    setText(text);
}

TextGeometry::TextGeometry(const Font &font) :
    color(nullptr),
    vertex(nullptr),
    uv(nullptr),
    index(nullptr),
    fixer(1000)
{
    f = &font;
}

TextGeometry::TextGeometry() :
    color(nullptr),
    vertex(nullptr),
    uv(nullptr),
    index(nullptr),
    fixer(1000)
{
    f = WinS::font;
}


TextGeometry::~TextGeometry(void)
{
    if(!color) {
        delete[] color;
        delete[] vertex;
        delete[] uv;
        delete[] index;
    }
}

std::string TextGeometry::getText() const
{
    return text;
}

void TextGeometry::setText(std::string s)
{
    if(!color) {
        delete[] color;
        delete[] vertex;
        delete[] uv;
        delete[] index;
    }

    auto n = s.length();
    color = new glm::vec4[n*4];
    vertex = new glm::vec3[n*4];
    uv = new glm::vec2[n*4];
    index = new GLuint[n*6];
    cou = 0;

    text = s;
    Size = WinS::sb->GetStringData(vec2(0), s, Colors::White, fixer, *f, vertex, uv, color, index, cou);
}

void TextGeometry::DrawAt(glm::vec2 pos)
{
    WinS::sb->DrawStored(pos, *WinS::font->tex, vertex, uv, color, index, cou);
}

void TextGeometry::append(std::string s)
{
    setText(text.append(s));
}

/************************************************************************/
/* maximum width in pixels                                                                     */
/************************************************************************/
void TextGeometry::setFixer(int width)
{
    fixer = width;
    setText(text);
}
