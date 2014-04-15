#include "Mouse.h"


GLFWwindow *Mouse::sm_window;

double Mouse::sm_xpos;
double Mouse::sm_ypos;

double Mouse::sm_dxpos;
double Mouse::sm_dypos;

unsigned int Mouse::sm_windowWidth;
unsigned int Mouse::sm_windowHeight;

bool Mouse::sm_stateFixedMousePos;
bool Mouse::sm_isCursorClientArea;
bool Mouse::sm_isWindowFocused;

void Mouse::Initialize( GLFWwindow *win )
{
	sm_window = win;
	sm_xpos = 1.0;
	sm_ypos = 1.0;
	sm_dxpos = 0.0;
	sm_dypos = 0.0;
	sm_windowWidth = 0;
	sm_windowHeight = 0;
	sm_stateFixedMousePos = false;
	sm_isCursorClientArea = true;
	sm_isWindowFocused = true;

	glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
}


void Mouse::SetButton( int button, int state ,int c )
{
	Mouse::sm_buttons[button] = state;
}

void Mouse::SetCursorPos( double x, double y )
{
	if(!sm_isWindowFocused)
		return;

	sm_dxpos = x - sm_xpos;
	sm_dypos = y - sm_ypos;
	sm_deltaxpos = sm_dxpos;
	sm_deltaypos = sm_dypos;
	sm_lastxpos = sm_xpos;
	sm_lastypos = sm_ypos;

	if(!sm_isCursorClientArea)
	{
		sm_dxpos = 0.0;
		sm_dypos = 0.0;
	}

	if(sm_stateFixedMousePos)
	{
		// устанавливаем курсор в центр экрана
		sm_xpos = double(sm_windowWidth) / 2.0;
		sm_ypos = double(sm_windowHeight) / 2.0;
		glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
	}
	else
	{
		sm_xpos = x;
		sm_ypos = y;
	}
}

double Mouse::IsMoveCursorX()
{
	double dx = sm_dxpos;
	sm_dxpos = 0.0;
	return dx;
}

double Mouse::IsMoveCursorY()
{
	double dy = sm_dypos;
	sm_dypos = 0.0;
	return dy;
}

void Mouse::GetCursorPos( double &x, double &y )
{
	x = sm_xpos;
	y = sm_ypos;
}

Vector2 Mouse::GetCursorPos()
{
	return Vector2(sm_xpos, sm_ypos);
}

void Mouse::SetFixedPosState( bool state )
{
	sm_stateFixedMousePos = state;
	if(sm_stateFixedMousePos)
	{
		sm_xpos = double(sm_windowWidth) / 2.0;
		sm_ypos = double(sm_windowHeight) / 2.0;
		sm_dxpos = 0.0;
		sm_dypos = 0.0;
		glfwSetCursorPos(sm_window, sm_xpos, sm_ypos);
	}
}

void Mouse::SetWindowSize( unsigned int width, unsigned int height )
{
	sm_windowWidth = width;
	sm_windowHeight = height;
}

void Mouse::CursorClientArea( int entered )
{
	if(entered == GL_TRUE)
	{
		sm_isCursorClientArea = true;
		glfwGetCursorPos(sm_window, &sm_xpos, &sm_ypos);
	}
	else
	{
		sm_isCursorClientArea = false;
	}
}

void Mouse::Update(){
	sm_deltaypos = 0;
	sm_deltaxpos = 0;
}

void Mouse::WindowFocus( int focused )
{
	if(focused == GL_TRUE)
	{
		sm_isWindowFocused = true;
		glfwGetCursorPos(sm_window, &sm_xpos, &sm_ypos);
	}
	else
	{
		sm_isWindowFocused = false;
	}
}

bool Mouse::IsRightPressed()
{
	return sm_buttons[GLFW_MOUSE_BUTTON_RIGHT] == GLFW_PRESS;
}

bool Mouse::IsLeftPressed()
{
	return sm_buttons[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS;
}

Vector2 Mouse::GetCursorDelta()
{
	return Vector2(sm_deltaxpos, sm_deltaypos);
}

Vector2 Mouse::GetCursorLastPos()
{
	return Vector2(sm_lastxpos, sm_lastypos);
}

double Mouse::sm_lastypos;
double Mouse::sm_lastxpos;
double Mouse::sm_deltaypos;
double Mouse::sm_deltaxpos;
int Mouse::sm_buttons[10];

