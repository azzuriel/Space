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

inline bool inLimsV(glm::vec2 param1, glm::vec2 wpos, glm::vec2 size)
{
  return param1.x >= wpos.x && param1.y >= wpos.y && param1.x <= size.x && param1.y <= size.y;
}

template<class T>
inline bool inLims(T param1, T wpos, T size)
{
  return param1 >= wpos && param1 <= size;
}

inline char keyToChar(int key){
  if(inLims(key, 48, 57)){
    return '0'+key-48;
  }
  if(inLims(key, 65, 90)){
    return 'a'+key-65;
  }
}

template<class T>
inline T atCenter(T size, T zoneBegin, T zoneSize){
  return zoneBegin + zoneSize/2.0f - (size)/2.0f;
}

#endif // WinS_h__
