#include "Keyboard.h"

int Keyboard::sm_keys[GLFW_KEY_LAST];

void Keyboard::Initialize()
{
	for(unsigned int i = 0; i < GLFW_KEY_LAST; i++)
	{
		sm_keys[i] = GLFW_RELEASE;
	}
}


void Keyboard::SetKey( int key, int scancode, int action, int mods )
{
	sm_keys[key] = action;
}

bool Keyboard::isKeyPress( int key )
{
	if(sm_keys[key] == GLFW_PRESS)
	{
		sm_keys[key] = GLFW_REPEAT;
		return true;
	}
	return false;
}

bool Keyboard::isKeyUp( int key )
{
	if(sm_keys[key] == GLFW_RELEASE)
		return true;
	return false;
}

bool Keyboard::isKeyDown( int key )
{
	if(sm_keys[key] == GLFW_REPEAT || sm_keys[key] == GLFW_PRESS)
		return true;
	return false;
}
