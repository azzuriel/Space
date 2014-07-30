#include <glew.h>
#include <glfw3.h>
#include "Engine.h"
#include <GameObject.h>
#include <SpaceGenerator.h>
#include "..\Engine\SpaceSolver.h"


int main(){
    LOG(info) << "Jarg initialization start";
    glfwSetErrorCallback([](int a,const char* description){LOG(error) << description;});

    int glfwErrorCode = glfwInit();
    if (!glfwErrorCode)
    {
        LOG(error) << "glfwInit error " << glfwErrorCode;
        return glfwErrorCode;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto monitor = nullptr;

    auto window = glfwCreateWindow(1024, 768, "sgtest", monitor, nullptr);
    if (!window)
    {
        glfwTerminate();
        LOG(fatal) << "Ошибка создания окна GLFW.";
        return false;
    }
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) 
    {
        LOG(fatal) << "GLEW не инициализирован.";
        return false;
    }

    Camera cam;
    cam.SetViewport(0,0,1024,768);

    auto LinesShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    LinesShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/colored.glsl");
    LinesShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/colored.glsl");
    LinesShader->Link();
    auto mvpLine = LinesShader->LocateVars("MVP");

    auto TextureShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    TextureShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/textured.glsl");
    TextureShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/textured.glsl");
    TextureShader->Link();
    auto mvpTex = TextureShader->LocateVars("MVP");

    Batched batched;
    batched.Initialize(TextureShader.get(), LinesShader.get());

    SpaceSolver ss;

    GameObject go;
    while(!glfwWindowShouldClose(window)){
        auto MVP = cam.GetOrthoProjection();
        TextureShader->Use();
        glUniformMatrix4fv(mvpTex, 1, GL_FALSE, &MVP[0][0]);
        LinesShader->Use();
        glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &MVP[0][0]);


        auto last = GenerateStar(&go);
        batched.DrawRectangle(vec2(last->T / 30000 * 500, log10(last->lum) * 50), vec2(2,2), TempToCol(last->T));
        auto t = GetStarClass(*last);

        batched.RenderFinally();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return true;
}