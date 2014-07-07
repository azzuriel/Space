#pragma once
#ifndef WinS_h__
#define WinS_h__
#include "SpriteBatch.h"
#include "TextureManager.h"
#include <vector>
#include "Win.h"
#include "GameTimer.h"
#include <list>
#include "glm.hpp"

class WinS{
public:
    WinS();
    WinS(Batched* sb_, const Font& fnt);
    ~WinS();
    static std::vector<Win*> windows;
    static bool MouseHooked;
    static bool KeyboardHooked;

    void Draw();
    void Update(GameTimer gt);
    static void ToTop(Win* w);
    static Batched* sb;
    static Texture* bp;
    static const Font* font;
};

#endif // WinS_h__
