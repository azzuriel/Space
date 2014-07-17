#pragma once
#include <vector>
#include <glm.hpp>
#include "WComponent.h"
#include <list>
class GraphBox : public WComponent
{
public:
    GraphBox(void);
    ~GraphBox(void);
    GraphBox(glm::vec2 pos_, glm::vec2 size_);

    virtual void Draw() const;

    virtual void Update();

    virtual glm::vec2 GlobalPos() const;

    std::list<float> points;
    glm::vec2 scale;

    WContainer* parent;

private:
    glm::vec2 pos;
    glm::vec2 size;
    bool aimed;
};

