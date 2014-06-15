#pragma once
#ifndef WinS_h__
#define WinS_h__
#include "SpriteBatch.h"
#include "TextureManager.h"
#include <vector>
#include "Win.h"
#include "GameTimer.h"
#include <list>

class WinS{
public:
	WinS();
	WinS(Batched* sb_, const Font& fnt);
	~WinS();
	static std::vector<Win*> windows;
	static bool MouseHooked;

	void Draw();
	void Update(GameTimer gt);
	static void ToTop(Win* w);
	static Batched* sb;
	static Texture* bp;
	static const Font* font;
};

inline bool inLims(glm::vec2 param1, glm::vec2 wpos, glm::vec2 size)
{
	return param1.x >= wpos.x && param1.y >= wpos.y && param1.x <= size.x && param1.y <= size.y;
}

#endif // WinS_h__
