#pragma once
#include <vector>
#include <glm.hpp>
#include "WComponent.h"
#include <memory>
#include "TextureManager.h"
class ImageBox : public WComponent
{
public:
    ImageBox(void);
    ~ImageBox(void);
    ImageBox(glm::vec2 pos_, glm::vec2 size_);

    virtual void Draw() const;

    virtual void Update();

    virtual glm::vec2 GlobalPos() const;

    std::shared_ptr<Texture> texture;

    WContainer* parent;
    bool border;

private:
    glm::vec2 pos;
    glm::vec2 size;
    bool aimed;
};

