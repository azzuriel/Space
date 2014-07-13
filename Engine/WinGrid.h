#pragma once
#include "WComponent.h"
#include "Win.h"
#include <glm.hpp>
class WinGrid : public WComponent, public WContainer
{
public:
    WinGrid(void);
    ~WinGrid(void);

    virtual void Draw() const;

    virtual void Update();

    virtual glm::vec2 GlobalPos() const;
    WContainer *parent;
    glm::vec2 pos;
    glm::vec2 size;
};

