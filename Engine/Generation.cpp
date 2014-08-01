#include "Generation.h"


Generation::Generation(void)
{
     cells = std::shared_ptr<BasicJargShader>(new BasicJargShader());
     cells->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/celltexture.glsl");
     cells->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/celltexture.glsl");
     cells->Link();

     noise = std::shared_ptr<Texture>(new Texture());
     noise->Load("noise.png");

     temp = std::shared_ptr<Texture>(new Texture());
     temp->Empty(vec2(128,128));

     tg = TextureGenerator();
}


Generation::~Generation(void)
{
}

std::shared_ptr<Texture> Generation::temp;

TextureGenerator Generation::tg;

std::shared_ptr<Texture> Generation::noise;

std::shared_ptr<BasicJargShader> Generation::cells;
