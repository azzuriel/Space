#include "Keyboard.h"
#include <deque>

int Keyboard::sm_keys[GLFW_KEY_LAST];

void Keyboard::Initialize()
{
	for(unsigned int i = 0; i < GLFW_KEY_LAST; i++)
	{
		sm_keys[i] = GLFW_RELEASE;
	}
}

int Keyboard::EnquePressed(){
  if(pressedQueue.size() > 0){
    int t = pressedQueue.front();
    pressedQueue.pop_front();
    return t;
  }
  return -1;
}


void Keyboard::SetKey( int key, int scancode, int action, int mods )
{
	sm_keys[key] = action;
	switch(key){
	case GLFW_KEY_LEFT_SHIFT:
	case GLFW_KEY_RIGHT_SHIFT:
		Shift = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_LEFT_CONTROL:
	case GLFW_KEY_RIGHT_CONTROL:
		Ctrl = action != GLFW_RELEASE;
		break;
	case GLFW_KEY_LEFT_ALT:
	case GLFW_KEY_RIGHT_ALT:
		Alt = action != GLFW_RELEASE;
		break;
	default:
		if(action == GLFW_PRESS || action == GLFW_REPEAT) {
			pressedQueue.push_back(key);
		}
		break;
		
	}
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

bool Keyboard::Shift;
bool Keyboard::Alt;
bool Keyboard::Ctrl;


std::deque<int> Keyboard::pressedQueue;
