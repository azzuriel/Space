#pragma once
#pragma once
#ifndef JButton_h__
#define JButton_h__
#include "WComponent.h"
#include <string>
#include "glm.hpp"
#include "TextGeometry.h"
#include "WinS.h"
#include <functional>

class JButton : public WComponent
{
public:
    JButton(void);
    JButton(glm::vec2 pos_, glm::vec2 size_);
    ~JButton(void);

    void Draw() const;
    void Update();
    glm::vec2 GlobalPos() const;
    void SetText(std::string s);
    WContainer* parent;
    TextGeometry* text;
    std::function<void()> onPress;
    glm::vec2 pos;
    glm::vec2 size;
    bool aimed;
};
#endif // JButton_h__

