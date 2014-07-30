#include "Game_Main.h"
#include "..\Engine\GameObject.h"
#include "..\Engine\SpaceGenerator.h"
#include "..\Engine\ROAMSurface.h"
#include <thread>

int Game::Initialize(){
    
    LOG(info) << "Jarg initialization start";
    glfwSetErrorCallback([](int a,const char* description){LOG(error) << description;});

    int glfwErrorCode = glfwInit();
    if (!glfwErrorCode)
    {
        LOG(error) << "glfwInit error " << glfwErrorCode;
        return glfwErrorCode;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Game::MAJOR_GL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Game::MINOR_GL);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    monitor = nullptr;
    if(fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
    }

    window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if (!window)
    {
        glfwTerminate();
        LOG(fatal) << "Ошибка создания окна GLFW.";
        return false;
    }
    glfwMakeContextCurrent(window);
    //glfwSetWindowTitle(window, AutoVersion::GetTitle().c_str());

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) 
    {
        LOG(fatal) << "GLEW не инициализирован.";
        return false;
    }

    GameObject go;
    go.seed = 123;
    for (int i =0; i<100; i++)
    {
        GenerateStar(&go);
    }

    int glVersion[2] = {-1, -1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 

    LOG(info) << "Renderer: " << glGetString(GL_RENDERER);
    LOG(info) << "Vendor: " <<  glGetString(GL_VENDOR);
    LOG(info) << "Version: " <<  glGetString(GL_VERSION);
    LOG(info) << "GLSL version: " <<  glGetString(GL_SHADING_LANGUAGE_VERSION);
    LOG(info) << "using OpenGL: " << std::to_string(glVersion[0]) << "." << std::to_string(glVersion[1]);
    LOG(info) << "glfw: " << glfwGetVersionString();

    Keyboard::Initialize();
    glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods){Keyboard::SetKey(key, scancode, action, mods);});

    Mouse::Initialize(window);
    Mouse::SetWindowSize(width, height);
    //	Mouse::SetFixedPosState(true);
    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos){Mouse::SetCursorPos(xpos, ypos);});
    glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered){Mouse::CursorClientArea(entered);});	
    glfwSetWindowFocusCallback(window, [](GLFWwindow *window, int focused){Mouse::WindowFocus(focused);});
    glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int a, int b, int c){Mouse::SetButton(a, b, c);});
    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int a, int b){Game::Resize(a, b); Mouse::SetWindowSize(a, b);});
    glfwSetScrollCallback(window, [](GLFWwindow *window, double a, double b){Mouse::Scroll(b);});
    wire = 1;
    spd = 1;

    //////////////////////////////////////////////////////////////////////////
    LinesShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    LinesShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/colored.glsl");
    LinesShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/colored.glsl");
    LinesShader->Link();
    mvpLine = LinesShader->LocateVars("MVP");

    TextureShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    TextureShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/textured.glsl");
    TextureShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/textured.glsl");
    TextureShader->Link();
    mvpTex = TextureShader->LocateVars("MVP");

    FXAAShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    FXAAShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/fxaa.glsl");
    FXAAShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/fxaa.glsl");
    FXAAShader->Link();

    BasicShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    BasicShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/minnaert.glsl");
    BasicShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/minnaert.glsl");
    BasicShader->Link();
    BasicShader->UpdateUniforms();
    BasicShader->LocateVars("transform.viewProjection"); //var0
    BasicShader->LocateVars("transform.model"); //var1
    BasicShader->LocateVars("transform.normal"); //var2
    BasicShader->LocateVars("material.texture");

    //////////////////////////////////////////////////////////////////////////

    light.position = vec4(5.0f, 12.0f, 3.0f, 1.0f);
    light.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    light.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    light.attenuation = vec3(0.000f, 0.0f, 0.00000f);

    batched = std::unique_ptr<Batched>(new Batched());
    batched->Initialize(TextureShader.get(), LinesShader.get());

    font = std::unique_ptr<Font>(new Font());
    font->Initialize();
    //actually not load json
    if(!font->Create("font.json")){
        LOG(error) << "failed to load\process font.json";
    }

    WindowsDesigner();
    rs = new ROAMSurface();

    camera = std::unique_ptr<Camera>(new Camera());
    camera->SetViewport(0, 0, width, height);
    camera->SetPosition(vec3(2,2,2));
    camera->SetLookAt(vec3(0));
    gt = std::unique_ptr<GameTimer>(new GameTimer);

    auto tex = std::shared_ptr<Texture>(new Texture());
    tex->Load("normal.png");

    icos = std::unique_ptr<Mesh>(new Mesh(Cube::getMesh()));
    icos->Bind();
    icos->shader = BasicShader;
    icos->material = std::shared_ptr<Material>(new Material());
    icos->material->texture = tex;

    Thread = std::thread([&](){ 
        while(true) {
        if(rs->Loaded) {
            rs->UpdateCells(camera->position);
            rs->Loaded = false;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        }
    });

    //test.LoadBinary("untitled.m");
    test.Bind();
    for (int i = 0; i<test.meshes.size(); i++)
    {
        test.meshes[i]->shader = BasicShader;
    }

    return true;
}