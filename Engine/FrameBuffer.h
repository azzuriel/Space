#pragma once
#include <glew.h>
#include "TextureManager.h"
class FrameBuffer
{
public:
    FrameBuffer(bool depth = false);
    ~FrameBuffer(void);
    void BindTexture(const Texture& tex);
    GLuint FBO;
    bool depth;
};

