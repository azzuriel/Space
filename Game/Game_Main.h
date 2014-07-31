#pragma once
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#define _USE_MATH_DEFINES

#include <memory.h>
#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <glm.hpp>
#include <string>

#include <Engine.h>
#include "PerfomanceWindow.h"
#include <thread>
#include "..\Engine\QuadTreePlane.h"

class Game
{
public:
    Game(void);
    ~Game(void);
    int Initialize();
    int Run();
    int Destroy();

    QuadTreePlane *rs;
    std::thread Thread;

    static bool fullscreen;
    static int width; static int height;
    static bool Running;
    std::string title;

    GLFWmonitor *monitor;
    GLFWwindow *window;
    std::unique_ptr<Batched> batched;
    std::unique_ptr<Font> font;
    std::unique_ptr<WinS> ws;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<GameTimer> gt;
    PerfomanceWindow *pw;
    FPSCounter fps;

    static void Resize(int a, int b)
    {
        width = a;
        height = b;
    }
    void Update();
    void Draw();

    void MyMethod();

    void WindowsDesigner();
    static const int MAJOR_GL = 3, MINOR_GL = 3;

    GLint mvpTex, mvpLine;

    std::unique_ptr<Mesh> icos;
    double spd;
    PointLight light;
    Model test;

    int wire;
    std::shared_ptr<BasicJargShader> TextureShader, LinesShader, BasicShader, FXAAShader;
};
