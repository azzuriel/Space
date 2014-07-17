#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(bool _depth)
{
    depth = _depth;
    FBO = 0;
    GLenum fboStatus;

    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


FrameBuffer::~FrameBuffer(void)
{
    glDeleteFramebuffers(1, &FBO);
}

void FrameBuffer::bindTexture(const Texture& tex, GLuint attach/*GL_COLOR_ATTACHMENT0*/)
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, attach, tex.textureId, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
