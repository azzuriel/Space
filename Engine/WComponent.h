#pragma once
#ifndef WComponent_h__
#define WComponent_h__
#include "glm.hpp"

class WComponent {
public:
    virtual void Draw() const = 0;
    virtual void Update() = 0;
    virtual glm::vec2 GlobalPos() const = 0;
};

class WContainer {
public:
    virtual glm::vec2 GlobalPos() const = 0;
};
#endif // WComponent_h__
