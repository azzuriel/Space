#pragma once
#include <glew.h>
#include "TextureManager.h"
class FrameBuffer
{
public:
    FrameBuffer(bool depth = false);
    ~FrameBuffer(void);
    void bindTexture(const Texture& tex, GLuint attach = GL_COLOR_ATTACHMENT0);
    GLuint FBO;
    bool depth;
};

