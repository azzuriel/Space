#pragma once
#include "TextureManager.h"
#include <vector>
#include "FrameBuffer.h"
#include "JargShader.h"
#include "BasicJargShader.h"
#include <memory>
#include <functional>

class TextureGenerator
{
public:
    TextureGenerator(void);
    ~TextureGenerator(void);
    void SetTextures(std::shared_ptr<Texture> tex);
    void SetResultTexture(std::shared_ptr<Texture> _tex);
    void SetShader(std::shared_ptr<BasicJargShader> _shader);
    void RenderOnTempFbo(std::function<void()> func = [](){}) const;
    void SetParams(float tex);
    void Reset();
private:
    std::vector<std::shared_ptr<Texture>> texes;
    std::vector<float> params;
    std::shared_ptr<BasicJargShader> shader;
    std::shared_ptr<Texture> target;
};

