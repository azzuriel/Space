#pragma once
#ifndef Mouse_h__
#define Mouse_h__

#include <glfw3.h>
#include "Vector.h"

class Mouse
{
private:
	static GLFWwindow *sm_window;

	static double sm_xpos;
	static double sm_ypos;

	static double sm_dxpos;
	static double sm_dypos;

	static double sm_lastxpos;
	static double sm_lastypos;

	static double sm_deltaxpos;
	static double sm_deltaypos;

	static unsigned int sm_windowWidth;
	static unsigned int sm_windowHeight;

	static bool sm_stateFixedMousePos;
	static bool sm_isCursorClientArea;
	static bool sm_isWindowFocused;
	static int sm_buttons[10];

public:

	static void CursorClientArea(int entered);
	static void SetButton(int a, int b, int c);
	static void SetCursorPos(double xpos, double ypos);
	static void WindowFocus(int focused);

	static void Initialize( GLFWwindow *win );

	static void SetFixedPosState( bool state);

	static bool GetFixedPosState()
	{
		return sm_stateFixedMousePos;
	}

	static void SetWindowSize(unsigned int width, unsigned int height);

	static void GetCursorPos(double &xpos, double &ypos);
	static Vector2 GetCursorPos();
	static Vector2 GetCursorLastPos();
	static Vector2 GetCursorDelta();

	static double IsMoveCursorX();
	static double IsMoveCursorY();

	static bool IsRightPressed();
	static bool IsLeftPressed();
	static void Update();
};


#endif // Mouse_h__