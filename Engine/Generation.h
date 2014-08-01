#pragma once
#include <memory>
#include "BasicJargShader.h"
#include "TextureManager.h"
#include "TextureGenerator.h"
class Generation
{
public:
    Generation(void);
    ~Generation(void);

    static std::shared_ptr<BasicJargShader> cells;
    static std::shared_ptr<Texture> noise;
    static TextureGenerator tg;
    static std::shared_ptr<Texture> temp;
};

