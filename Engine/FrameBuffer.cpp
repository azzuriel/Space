#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(bool _depth)
{
    depth = _depth;
    FBO = 0;
    GLenum fboStatus;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    if(depth){
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer(void)
{
    glDeleteFramebuffers(1, &FBO);
}

void FrameBuffer::BindTexture(const Texture& tex)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, depth ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0, tex.textureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
