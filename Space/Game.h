#pragma once
#include "Engine.h"
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <memory>
#include "..\Engine\GameTimer.h"
#include "..\Engine\Render.h"
#include "..\Engine\FPSCounter.h"

class Game
{
public:
	Game(void);
	~Game(void);
	void Run();
	int Initialize();
	bool Running;
	std::string title;
	bool fullscreen;
	int width, height;
	GLFWwindow* window;
	GameTimer gt;
	FPSCounter fps;

	std::unique_ptr<Render> render;
	bool wire;
};
