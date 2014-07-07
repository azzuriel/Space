#ifndef Keyboard_h__
#define Keyboard_h__

#include <glfw3.h>
#include <deque>
#include <ctype.h>

class Keyboard
{
private:
    static int sm_keys[GLFW_KEY_LAST];

public:

    static void Initialize();

    static void SetKey(int key, int scancode, int action, int mods);

    static bool isKeyPress(int key);
    static bool isKeyUp(int key);
    static bool isKeyDown(int key);
    static int EnquePressed();
    static std::deque<int> pressedQueue;
    static bool Shift, Ctrl, Alt;
};

#endif // Keyboard_h__