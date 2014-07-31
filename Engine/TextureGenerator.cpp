#include "TextureGenerator.h"
#include <stdarg.h>
#include "JHelpers_inl.h"
#include "Quad.h"
#include "Mesh.h"
#include "BasicJargShader.h"
#include <functional>

// TODO: наследуемые классы текстурного генератора, с перегрузкой функции OtherUniforms, которая вызывается перед рендером
TextureGenerator::TextureGenerator(void)
{
}


TextureGenerator::~TextureGenerator(void)
{
}

void TextureGenerator::SetTextures(std::shared_ptr<Texture> tex, ...)
{
    texes.push_back(tex);
    va_list argptr;
    int num = 0;
    va_start (argptr, num);
    for(; num; num--) {
        auto t = va_arg(argptr, std::shared_ptr<Texture>);
        texes.push_back(t);
    }
}

void TextureGenerator::SetResultTexture(std::shared_ptr<Texture> _tex)
{
    target = _tex;
}

void TextureGenerator::SetShader(std::shared_ptr<BasicJargShader> _shader)
{
    shader = _shader;
}

void TextureGenerator::RenderOnTempFbo(std::function<void()> func) const
{
    FrameBuffer fbo;
    fbo.bindTexture(*target);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo.FBO);
    glViewport(0, 0, target->width, target->height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    shader->Use();
    for (int i=0; i<texes.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glBindTexture(GL_TEXTURE_2D, texes[i]->textureId);
        std::string str = "inputTex";
        str.append(std::to_string(i));
        glUniform1i(glGetUniformLocation(shader->program, str.c_str()), 0);
    }
    
    func();

    Mesh quad_mesh = Mesh(Quad::GetMesh(2));
    quad_mesh.shader = shader;
    quad_mesh.Bind();
    quad_mesh.Render();
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
