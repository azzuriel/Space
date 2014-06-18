#pragma once
#include "Engine.h"
#include <string>
#include <glew.h>
#include <glfw3.h>
#include <memory>
#include "..\Engine\GameTimer.h"
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
	static int width, height;
	GLFWwindow* window;
	GameTimer gt;
	FPSCounter fps;

	static void Resize(int a, int b);

	int wire;

	static const int MAJOR_GL = 3, MINOR_Gl = 3;
};
