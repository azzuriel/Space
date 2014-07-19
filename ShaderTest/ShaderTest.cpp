#pragma once
#include "ShaderTest.h"
#include "Engine.h"
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
#include <glew.h>
#include <glfw3.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <memory>
#include "../Engine/SpriteBatch.h"
#include "../Engine/Font.h"
#include "../Engine/Camera.h"
#include "../Engine/SphereTesselator.h"
#include "../Engine/Material.h"
#include <glm.hpp>
#ifdef __linux__
#include <unistd.h>
#endif /* __linux__ */
#ifdef _WIN32
#include <windows.h> 
#endif /* _WIN32 */
#include <math.h>
#include "../Engine/TreeSphere.h"
#include <ClassicNoise.h>
#include <chrono>
#include <thread>
#include "../Engine/WinS.h"
#include "../Engine/JButton.h"
#include "../Engine/JLabel.h"
#include "../Engine/JTextBox.h"
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include "debugDraw.h"
#include "../Engine/DynamicObject.h"
#include "../Engine/JHelpers_inl.h"
#include "../Engine/ColladaRaw.h"
#include <gtc/matrix_transform.hpp>
#include <Model.h>
#include <easylogging++.h>
#include "LodSphere.h"
#include "../Engine/ROAMgrid.h"
#include "../Engine/ROAMSurface.h"
#include <sqlite3.h>
#include "../Engine/SkySphere.h"
#include "AutoVersion.h"
#include "SpaceSolver.h"
#include "JHelpers_inl.h"
#include "../Engine/BasicJargShader.h"
#include "../Engine/GraphBox.h"
#include "WComponent.h"
#include "../Engine/sparse_vector.h"
#include "ShaderSelectWindow.h"
#include "PerfomanceWindow.h"
#include "../Engine/FrameBuffer.h"
#include "../Engine/TextureGenerator.h"
#include "common_gen_const.h"
#include "../Engine/Frustum.h"

void errorCallbackGLFW3(int error, const char* description)
{
    LOG(ERROR) << description;
}


Game::Game(void)
{
    srand(1);
    Running = true;

    title = AutoVersion::GetTitle();
    width = 1024;
    height = 600;
    fullscreen = false;
}


Game::~Game(void)
{

}

int Game::Initialize()
{
    LOG(INFO) << "Jarg initialization start";
    glfwSetErrorCallback(errorCallbackGLFW3);

    int glfwErrorCode = glfwInit();
    if (!glfwErrorCode)
    {
        LOG(ERROR) << "glfwInit error " << glfwErrorCode;
        return glfwErrorCode;
    }

    glfwWindowHint(GLFW_SAMPLES, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Game::MAJOR_GL);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Game::MINOR_GL);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor *monitor = nullptr;
    if(fullscreen)
    {
        monitor = glfwGetPrimaryMonitor();
    }

    window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if (!window)
    {
        glfwTerminate();
        LOG(FATAL) << "Ошибка создания окна GLFW.";
        return false;
    }
    glfwMakeContextCurrent(window);
    //glfwSetWindowTitle(window, AutoVersion::GetTitle().c_str());

    glfwSwapInterval(0);

    glewExperimental=true;

    if (glewInit() != GLEW_OK) 
    {
        LOG(ERROR) << "GLEW не инициализирован.";
        return false;
    }

    int glVersion[2] = {-1, -1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
    
    LOG(INFO) << "Renderer: " << glGetString(GL_RENDERER);
    LOG(INFO) << "Vendor: " <<  glGetString(GL_VENDOR);
    LOG(INFO) << "Version: " <<  glGetString(GL_VERSION);
    LOG(INFO) << "GLSL version: " <<  glGetString(GL_SHADING_LANGUAGE_VERSION);
    LOG(INFO) << "using OpenGL: " << std::to_string(glVersion[0]) << "." << std::to_string(glVersion[1]);
    LOG(INFO) << "glfw: " << glfwGetVersionString();

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
    wire = 0;

    return true;
}

void PointLightSetup(GLuint program, const PointLight &light)
{
    // установка параметров
    glUniform4fv(glGetUniformLocation(program, "light.position"),    1, &light.position[0]);
    glUniform4fv(glGetUniformLocation(program, "light.ambient"),     1, &light.ambient[0]);
    glUniform4fv(glGetUniformLocation(program, "light.diffuse"),     1, &light.diffuse[0]);
    glUniform4fv(glGetUniformLocation(program, "light.specular"),    1, &light.specular[0]);
    glUniform3fv(glGetUniformLocation(program, "light.attenuation"), 1, &light.attenuation[0]);
}


void CameraSetup(GLuint program, const Camera &camera)
{
    auto vp = camera.projection * camera.view;
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.viewProjection"), 1, GL_FALSE, &vp[0][0]);
    glUniform3fv(glGetUniformLocation(program, "transform.viewPosition"), 1, &camera.position[0]);
}

void RenderScene(std::shared_ptr<BasicJargShader> current_shader, const Camera &camera, const Model &model, const PointLight &light, const Frustum & frust)
{
    current_shader->Use();
    CameraSetup(current_shader->program, camera);
    PointLightSetup(current_shader->program, light);
    for (int i = 0; i<model.meshes.size(); i++)
    {
        model.meshes[i]->shader = current_shader;
    }
    model.Render(frust);
}

GLuint FBO_Test(const Texture& depth) {
    GLuint depthFBO = 0;
    GLenum fboStatus;

    glGenFramebuffers(1, &depthFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

    // color out off
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth.textureId, 0);

    if ((fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG(ERROR) << string_format("glCheckFramebufferStatus error 0x%X\n", fboStatus);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return depthFBO;
}

void Game::Run()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 1);

    auto BasicShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    BasicShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/minnaert.glsl");
    BasicShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/minnaert.glsl");
    BasicShader->Link();
    BasicShader->UpdateUniforms();
    auto mvpBasic = BasicShader->LocateVars("transform.viewProjection"); //var0
    auto worldID = BasicShader->LocateVars("transform.model"); //var1
    BasicShader->LocateVars("transform.normal"); //var2
    auto colorTextureLocation = BasicShader->LocateVars("material.texture");

    auto StarSphereShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    StarSphereShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/starsphere.glsl");
    StarSphereShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/starsphere.glsl");
    StarSphereShader->Link();
    StarSphereShader->LocateVars("transform.viewProjection"); //var0
    StarSphereShader->LocateVars("transform.model"); //var1
    StarSphereShader->LocateVars("transform.normal"); //var2

    auto TestShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    TestShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/testshader.glsl");
    TestShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/testshader.glsl");
    TestShader->Link();
    TestShader->UpdateUniforms();
    TestShader->LocateVars("transform.viewProjection"); //var0
    TestShader->LocateVars("transform.model"); //var1
    TestShader->LocateVars("transform.normal"); //var2

    auto DepthTestShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    DepthTestShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/depthtest.glsl");
    DepthTestShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/depthtest.glsl");
    DepthTestShader->Link();
    DepthTestShader->UpdateUniforms();
    DepthTestShader->LocateVars("transform.viewProjection"); //var0
    DepthTestShader->LocateVars("transform.model"); //var1
    DepthTestShader->LocateVars("transform.normal"); //var2

    auto FXAAShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    FXAAShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/fxaa.glsl");
    FXAAShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/fxaa.glsl");
    FXAAShader->Link();

    auto TangentspaceTestShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    TangentspaceTestShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/tangentspacetest.glsl");
    TangentspaceTestShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/tangentspacetest.glsl");
    TangentspaceTestShader->Link();
    TangentspaceTestShader->UpdateUniforms();
    TangentspaceTestShader->LocateVars("transform.viewProjection"); //var0
    TangentspaceTestShader->LocateVars("transform.model"); //var1
    TangentspaceTestShader->LocateVars("transform.normal"); //var2

    auto CellTextureTestShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    CellTextureTestShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/celltexture.glsl");
    CellTextureTestShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/celltexture.glsl");
    CellTextureTestShader->Link();

    auto TransmittanceShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    TransmittanceShader->PushGlobalHeader("common_gen_const.h");
    TransmittanceShader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    TransmittanceShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/transmittance.glsl");
    TransmittanceShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/transmittance.glsl");
    TransmittanceShader->Link();

    auto Irradiance1Shader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    Irradiance1Shader->PushGlobalHeader("common_gen_const.h");
    Irradiance1Shader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    Irradiance1Shader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/irradiance1.glsl");
    Irradiance1Shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/irradiance1.glsl");
    Irradiance1Shader->Link();

    auto Inscatter1Shader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    Inscatter1Shader->PushGlobalHeader("common_gen_const.h");
    Inscatter1Shader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    Inscatter1Shader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/inscatter1.glsl");
    Inscatter1Shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/inscatter1.glsl");
    Inscatter1Shader->loadShaderFromSource(GL_GEOMETRY_SHADER, "Shaders/pas/inscatter1.glsl");
    Inscatter1Shader->Link();

    auto InscatterNShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    InscatterNShader->PushGlobalHeader("common_gen_const.h");
    InscatterNShader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    InscatterNShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/inscatterN.glsl");
    InscatterNShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/inscatterN.glsl");
    InscatterNShader->loadShaderFromSource(GL_GEOMETRY_SHADER, "Shaders/pas/inscatterN.glsl");
    InscatterNShader->Link();

    auto InscatterSShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    InscatterSShader->PushGlobalHeader("common_gen_const.h");
    InscatterSShader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    InscatterSShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/inscatterS.glsl");
    InscatterSShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/inscatterS.glsl");
    InscatterSShader->loadShaderFromSource(GL_GEOMETRY_SHADER, "Shaders/pas/inscatterS.glsl");
    InscatterSShader->Link();

    auto IrradianceNShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    IrradianceNShader->PushGlobalHeader("common_gen_const.h");
    IrradianceNShader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    IrradianceNShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/irradianceN.glsl");
    IrradianceNShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/irradianceN.glsl");
    IrradianceNShader->Link();

    auto CopyIrradianceShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    CopyIrradianceShader->PushGlobalHeader("common_gen_const.h");
    CopyIrradianceShader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    CopyIrradianceShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/copyIrradiance.glsl");
    CopyIrradianceShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/copyIrradiance.glsl");
    CopyIrradianceShader->Link();

    auto CopyInscatterNShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    CopyInscatterNShader->PushGlobalHeader("common_gen_const.h");
    CopyInscatterNShader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    CopyInscatterNShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/copyInscatterN.glsl");
    CopyInscatterNShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/copyInscatterN.glsl");
    CopyInscatterNShader->loadShaderFromSource(GL_GEOMETRY_SHADER, "Shaders/pas/copyInscatterN.glsl");
    CopyInscatterNShader->Link();

    auto CopyInscatter1Shader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    CopyInscatter1Shader->PushGlobalHeader("common_gen_const.h");
    CopyInscatter1Shader->PushGlobalHeader("Shaders/pas/common_gen.glsl");
    CopyInscatter1Shader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/pas/copyInscatter1.glsl");
    CopyInscatter1Shader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/pas/copyInscatter1.glsl");
    CopyInscatter1Shader->loadShaderFromSource(GL_GEOMETRY_SHADER, "Shaders/pas/copyInscatter1.glsl");
    CopyInscatter1Shader->Link();

    auto NormalTestShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    NormalTestShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/normaltest.glsl");
    NormalTestShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/normaltest.glsl");
    NormalTestShader->Link();
    NormalTestShader->UpdateUniforms();
    NormalTestShader->LocateVars("transform.viewProjection"); //var0
    NormalTestShader->LocateVars("transform.model"); //var1
    NormalTestShader->LocateVars("transform.normal"); //var2

    auto MinimalShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    MinimalShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/minimal.glsl");
    MinimalShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/minimal.glsl");
    MinimalShader->Link();
    MinimalShader->UpdateUniforms();
    MinimalShader->LocateVars("transform.viewProjection"); //var0
    MinimalShader->LocateVars("transform.model"); //var1
    MinimalShader->LocateVars("transform.normal"); //var2

    PointLight pl;
    pl.position = vec4(5.0f, 12.0f, 3.0f, 1.0f);
    pl.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    pl.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pl.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pl.attenuation = vec3(0.000f, 0.0f, 0.00000f);

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

    auto FullscreenShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    FullscreenShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/textured_no_matrix.glsl");
    FullscreenShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/textured_no_matrix.glsl");
    FullscreenShader->Link();

    auto fullscreen_Tex = std::shared_ptr<Texture>(new Texture());
    fullscreen_Tex->Empty(vec2(width, height));

    FrameBuffer fullscreen;
    fullscreen.bindTexture(*fullscreen_Tex);

    auto sb = std::unique_ptr<Batched>(new Batched());
    sb->Initialize(TextureShader.get(), LinesShader.get());

    const glm::mat4 Identity = glm::mat4(1.0f);

    auto floor = std::shared_ptr<Texture>(new Texture());
    floor->Load("sponza_ceiling_a_diff.png", true, true);

    auto fabric = std::shared_ptr<Texture>(new Texture());
    fabric->Load("sponza_fabric_diff.png", true, true);

    auto thorn = std::shared_ptr<Texture>(new Texture());
    thorn->Load("sponza_thorn_diff.png");

    auto bricks = std::shared_ptr<Texture>(new Texture());
    bricks->Load("spnza_bricks_a_diff.png", true, true);
    
    auto column_a = std::shared_ptr<Texture>(new Texture());
    column_a->Load("sponza_column_a_diff.png", true, true);

    auto column_b = std::shared_ptr<Texture>(new Texture());
    column_b->Load("sponza_column_b_diff.png", true, true);

    auto column_c = std::shared_ptr<Texture>(new Texture());
    column_c->Load("sponza_column_с_diff.png", true, true);

    auto vase = std::shared_ptr<Texture>(new Texture());
    vase->Load("vase_diff.png", true, true);

    auto hanging = std::shared_ptr<Texture>(new Texture());
    hanging->Load("vase_hanging.png", true, true);

    auto arc = std::shared_ptr<Texture>(new Texture());
    arc->Load("sponza_arch_diff.png", true, true);

    auto chain = std::shared_ptr<Texture>(new Texture());
    chain->Load("chain_texture.png");

    auto lion = std::shared_ptr<Texture>(new Texture());
    lion->Load("lion.png", true, true);

    auto normal = std::shared_ptr<Texture>(new Texture());
    normal->Load("normal.png", true, true);
    auto m = new Model();
    m->LoadBinary("untitled.m");
    m->Bind();
    for (int i = 0; i<m->meshes.size(); i++)
    {
        m->meshes[i]->shader = BasicShader;
    }

    for (int i =0; i< m->materials.size(); i++)
    {
        m->materials[i]->texture = bricks;
    }
    m->materials[0]->texture = thorn;
    m->findMaterialById("fabric_a-material")->texture = fabric;
    m->findMaterialById("fabric_c-material")->texture = fabric;
    m->findMaterialById("fabric_f-material")->texture = fabric;
    m->findMaterialById("fabric_d-material")->texture = fabric;
    m->findMaterialById("fabric_e-material")->texture = fabric;
    m->findMaterialById("fabric_g-material")->texture = fabric;
    m->findMaterialById("floor-material")->texture = floor;
    //m->findMaterialById("floor-material")->normal = normal;
    m->findMaterialById("bricks-material")->texture = bricks;
    m->findMaterialById("column_a-material")->texture = column_a;
    m->findMaterialById("column_b-material")->texture = column_b;
    m->findMaterialById("column_c-material")->texture = column_c;
    m->findMaterialById("arch-material")->texture = arc;
    m->findMaterialById("vase_round-material")->texture = thorn;
    m->findMaterialById("vase_hanging-material")->texture = hanging;
    m->findMaterialById("chain-material")->texture = chain;
    m->findMaterialById("ceiling-material")->texture = bricks;
    m->findMaterialById("Material__25-material")->texture = lion;
    m->findMaterialById("Material__57-material")->texture = thorn;
    
    Camera camera; 
    camera.SetViewport(0, 0, width, height);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = camera.VP() * model;
    camera.SetPosition(vec3(50,0,-200));
    camera.SetLookAt(vec3(50,0,50));

    auto font = std::unique_ptr<Font>(new Font());
    font->Initialize();
    if(!font->Create("font.json")){
        LOG(ERROR) << "failed to load\process font.json";
    }

    std::shared_ptr<Texture> emptytex = std::shared_ptr<Texture>(new Texture());
    emptytex->Empty(vec2(256,64));
    std::shared_ptr<Texture> wp = std::shared_ptr<Texture>(new Texture());
    wp->Load("wp.png");


    auto noise = std::shared_ptr<Texture>(new Texture());
    noise->Load("noise.png", true, true);

    auto depthtexture = std::shared_ptr<Texture>(new Texture());
    depthtexture->CreateDepth(vec2(width, height));

    auto diffusetexture = std::shared_ptr<Texture>(new Texture());
    diffusetexture->Empty(vec2(width, height));

    auto normaltexture = std::shared_ptr<Texture>(new Texture());
    normaltexture->Empty(vec2(width, height));

    auto speculartexture = std::shared_ptr<Texture>(new Texture());
    speculartexture->Empty(vec2(width, height));

    auto test_fbo = FrameBuffer(true);
    test_fbo.bindTexture(*depthtexture, GL_DEPTH_ATTACHMENT);
    test_fbo.bindTexture(*diffusetexture, GL_COLOR_ATTACHMENT0);
    test_fbo.bindTexture(*speculartexture, GL_COLOR_ATTACHMENT1);
    test_fbo.bindTexture(*normaltexture, GL_COLOR_ATTACHMENT2);
    
    //PAS(TransmittanceShader.get(),  \
        Irradiance1Shader.get(), \
        CopyIrradianceShader.get(),\
        InscatterSShader.get(),\
        CopyInscatterNShader.get(),\
        InscatterNShader.get(),\
        Inscatter1Shader.get(),\
        CopyInscatter1Shader.get(), \
        IrradianceNShader.get());


    auto cur_shader = BasicShader;
    WinS *ws = new WinS(sb.get(), *font);
    ShaderSelectWindow *ssw = new ShaderSelectWindow();
    ws->windows.push_back(ssw);
    ssw->apply1->onPress = [&](){cur_shader = BasicShader;};
    ssw->apply1->SetText("Basic");
    ssw->apply2->onPress = [&](){cur_shader = TestShader;};
    ssw->apply2->SetText("Test");
    ssw->apply3->onPress = [&](){cur_shader = DepthTestShader;};
    ssw->apply3->SetText("Depth");
    ssw->apply4->onPress = [&](){cur_shader = NormalTestShader;};
    ssw->apply4->SetText("Normal");
    ssw->apply5->onPress = [&](){cur_shader = TangentspaceTestShader;};
    ssw->apply5->SetText("Tangent");
    ssw->ibox->texture = diffusetexture;
    ssw->ibox1->texture = depthtexture;
    ssw->ibox2->texture = speculartexture;
    ssw->ibox3->texture = normaltexture;

    PerfomanceWindow *pw = new PerfomanceWindow();
    ws->windows.push_back(pw);

    auto broadphase = std::unique_ptr<btDbvtBroadphase>(new btDbvtBroadphase());
    auto collisionConfiguration = std::unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
    auto dispatcher = std::unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(collisionConfiguration.get()));
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher.get());
    auto solver = std::unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver);
    auto dynamicsWorld = std::unique_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(dispatcher.get(), broadphase.get(), solver.get(), collisionConfiguration.get()));
    dynamicsWorld->setGravity(btVector3(0,0,0));
    auto drawer = std::unique_ptr<GLDebugDrawer>(new GLDebugDrawer());
    drawer->setDebugMode(true);
    dynamicsWorld->setDebugDrawer(drawer.get());
    drawer->SetBatched(sb.get());


    auto sphere = std::unique_ptr<DynamicObject>(new DynamicObject());
    sphere->bpRegister(dynamicsWorld.get());

    int iters = 0;
    float sec = 0;

    float rotated = 0;

    Camera* cur_cam = &camera;
    Camera lightcam;
    lightcam.SetViewport(0,0,width, height);
    lightcam.SetPosition(vec3(50,0,-200));
    lightcam.SetLookAt(vec3(50,0,50));

    Mouse::SetFixedPosState(true);
    glCullFace(GL_BACK);
    vec3 camlast;

    Mesh fullquad = Mesh(Quad::GetMesh(2));
    fullquad.shader = TextureShader;
    fullquad.Bind();

    Frustum frustum;

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    wire = 1;
    while(Running && !glfwWindowShouldClose(window)) 
    {
        glEnable(GL_DEPTH_TEST);
        gt.Update(glfwGetTime());
        fps.Update(gt);
        pw->UpdateValue(fps.GetCount());

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2,0.1, 0.3, 1.0f);


        if(Keyboard::isKeyPress(GLFW_KEY_LEFT_CONTROL)){
            Mouse::SetFixedPosState(!Mouse::GetFixedPosState());
        }
        if(Keyboard::isKeyDown(GLFW_KEY_W)){
            camera.Move(FORWARD);
        }
        if(Keyboard::isKeyDown(GLFW_KEY_S)){
            camera.Move(BACK);
        }
        if(Keyboard::isKeyDown(GLFW_KEY_A)){
            camera.Move(LEFT);
        }
        if(Keyboard::isKeyDown(GLFW_KEY_D)){
            camera.Move(RIGHT);
        }

        std::vector<int> a;

        if(Keyboard::isKeyPress(GLFW_KEY_F2)){
            wire++; if(wire >2) wire = 0;
        }

        switch(wire){
        case 0:
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            glDisable(GL_CULL_FACE);
            
            break;	
        case 1:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        case 2:
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
        }


        if(Keyboard::isKeyDown(GLFW_KEY_F5)){
            camera.SetLookAt(vec3(0,0,0.0F));
        }

        if(Keyboard::isKeyDown(GLFW_KEY_F10)){
            pl.position = vec4(camera.position, 1);
            lightcam.position = vec3(pl.position);
            lightcam.rotation_quaternion = camera.rotation_quaternion;
        }

        if(Keyboard::isKeyPress(GLFW_KEY_F11)){
            if(cur_cam == &camera){
                cur_cam = &lightcam;
            } else {
                cur_cam = &camera;
            }
        }

        camera.move_camera = true;
        camera.camera_scale = gt.elapsed/10.0F;
        if(Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT)){
            camera.camera_scale = gt.elapsed*50.0F;
        } else {
            camera.camera_scale = gt.elapsed*10.0F;
        }


        if(Mouse::GetFixedPosState())
            camera.Move2D(Mouse::GetCursorDelta().x, Mouse::GetCursorDelta().y);

        auto mpos = Mouse::GetCursorPos();

        BasicShader->Use();

        


        m->World = glm::rotate(Identity, (float)-M_PI_2, vec3(1.0,0.0,0.0));
        m->World = glm::scale(m->World, vec3(10.0,10.0,10.0));

        rotated+=gt.elapsed;


        camera.Update();
        lightcam.Update();
        //lightcam.projection = glm::ortho<float>(-100,100,-100,100,0.01,1000);
        frustum.Build(camera.view, camera.aspect, camera.field_of_view, camera.far_clip, camera.near_clip);

        PointLightSetup(BasicShader->program, pl);
//      
//         glBindFramebuffer(GL_FRAMEBUFFER, test_fbo.FBO);
//         glViewport(0, 0, depthtexture->width, depthtexture->height);
//         glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//         glDepthMask(GL_TRUE);
//         glClear(GL_DEPTH_BUFFER_BIT);
//         glCullFace(GL_FRONT);
//         RenderScene(MinimalShader, lightcam, *m, pl);

        glBindFramebuffer(GL_FRAMEBUFFER, test_fbo.FBO);
        glViewport(0, 0, width, height);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClearColor(0.0, 0.0, 0.0, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        RenderScene(cur_shader, *cur_cam, *m, pl, frustum);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        sb->DrawQuad(vec2(0,0), vec2(width, height), *diffusetexture);
        
         
        if(wire == 2) {
            LinesShader->Use();
            glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera.VP()[0][0]);
            dynamicsWorld->debugDrawWorld();

            m->RenderBounding(*sb);
        }
        
        m->meshes[123]->World = rotate(m->meshes[123]->World, 0.01f, vec3(0,1,0));
        

        ////////////////////////////////////////////////////////////////////////// WORLD PLACE
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wp->textureId);
        glDisable(GL_DEPTH_TEST);
        MVP = camera.GetOrthoProjection();
        TextureShader->Use();
        glUniformMatrix4fv(mvpTex, 1, GL_FALSE, &MVP[0][0]);
        LinesShader->Use();
        glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &MVP[0][0]);

        sb->DrawString(vec2(10,10), std::to_string(fps.GetCount()+310).append(" fps"), Colors::Red, *font);		
        //sb->DrawString(vec2(20,20), camera.getFullDebugDescription(), Colors::Red, *font);
        //sb->DrawQuad(vec2(100,100), vec2(500,500), *emptytex);

        ws->Update(gt);
        ws->Draw();
        
        LinesShader->Use();
        glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera.VP()[0][0]);
        int dc = sb->RenderFinallyWorld();

        dc += sb->RenderFinally();   
       
        
        Mouse::Update();

        //glFlush();
        glfwSwapBuffers(window);

        glfwPollEvents();

        //std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    delete m;

    delete ws;

    sphere->bpUnregister(dynamicsWorld.get());
}

void Game::Destroy(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::Resize(int a, int b)
{
    width = a;
    height = b;
    glViewport(0,0,a,b);
}

void Game::PAS(JargShader* TransmittanceShader, 
               JargShader* Irradiance1Shader, 
               JargShader* CopyIrradianceShader,
               JargShader* InscatterSShader,
               JargShader* CopyInscatterNShader,
               JargShader* InscatterNShader,
               JargShader* Inscatter1Shader,
               JargShader* CopyInscatter1Shader, 
               JargShader* IrradianceNShader)
{
    const GLuint reflectanceUnit = 0;
    const GLuint transmittanceUnit = 1;
    const GLuint irradianceUnit = 2;
    const GLuint inscatterUnit = 3;
    const GLuint deltaEUnit = 4;
    const GLuint deltaSRUnit = 5;
    const GLuint deltaSMUnit = 6;
    const GLuint deltaJUnit = 7;

    auto reflectanceTexture = std::shared_ptr<Texture>(new Texture());
    reflectanceTexture->Load("normal.png");
    auto transmittanceTexture = std::shared_ptr<Texture>(new Texture());
    transmittanceTexture->EmptyFloatSpace(vec3(TRANSMITTANCE_W, TRANSMITTANCE_H, 0), GL_TEXTURE_2D);
    auto irradianceTexture = std::shared_ptr<Texture>(new Texture());
    irradianceTexture->EmptyFloatSpace(vec3(SKY_W, SKY_H, 0), GL_TEXTURE_2D);
    auto inscatterTexture = std::shared_ptr<Texture>(new Texture());
    inscatterTexture->EmptyFloatSpace(vec3(RES_MU_S * RES_NU, RES_MU, RES_R), GL_TEXTURE_3D, GL_RGBA16F_ARB);
    auto deltaETexture = std::shared_ptr<Texture>(new Texture());
    deltaETexture->EmptyFloatSpace(vec3(SKY_W, SKY_H, 0), GL_TEXTURE_2D);
    auto deltaSRTexture = std::shared_ptr<Texture>(new Texture());
    deltaSRTexture->EmptyFloatSpace(vec3(RES_MU_S * RES_NU, RES_MU, RES_R), GL_TEXTURE_3D);
    auto deltaSMTexture = std::shared_ptr<Texture>(new Texture());
    deltaSMTexture->EmptyFloatSpace(vec3(RES_MU_S * RES_NU, RES_MU, RES_R), GL_TEXTURE_3D);
    auto deltaJTexture = std::shared_ptr<Texture>(new Texture());
    deltaJTexture->EmptyFloatSpace(vec3(RES_MU_S * RES_NU, RES_MU, RES_R), GL_TEXTURE_3D);

    //////////////////////////////////////////////////////////////////////////
    glActiveTexture(GL_TEXTURE0 + transmittanceUnit);
    glBindTexture(GL_TEXTURE_2D, transmittanceTexture->textureId);

    glActiveTexture(GL_TEXTURE0 + irradianceUnit);
    glBindTexture(GL_TEXTURE_2D, irradianceTexture->textureId);

    glActiveTexture(GL_TEXTURE0 + inscatterUnit);
    glBindTexture(GL_TEXTURE_3D, inscatterTexture->textureId);

    glActiveTexture(GL_TEXTURE0 + deltaEUnit);
    glBindTexture(GL_TEXTURE_2D, deltaETexture->textureId);

    glActiveTexture(GL_TEXTURE0 + deltaSRUnit);
    glBindTexture(GL_TEXTURE_3D, deltaSRTexture->textureId);

    glActiveTexture(GL_TEXTURE0 + deltaSMUnit);
    glBindTexture(GL_TEXTURE_3D, deltaSMTexture->textureId);

    glActiveTexture(GL_TEXTURE0 + deltaJUnit);
    glBindTexture(GL_TEXTURE_3D, deltaJTexture->textureId);

    auto drawQuad = []()
    {
        glBegin(GL_TRIANGLE_STRIP);
        glVertex2f(-1.0, -1.0);
        glVertex2f(+1.0, -1.0);
        glVertex2f(-1.0, +1.0);
        glVertex2f(+1.0, +1.0);
        glEnd();
    };

    auto setLayer = [](unsigned int prog, int layer)
    {
        double r = layer / (RES_R - 1.0);
        r = r * r;
        r = sqrt(Rg * Rg + r * (Rt * Rt - Rg * Rg)) + (layer == 0 ? 0.01 : (layer == RES_R - 1 ? -0.001 : 0.0));
        double dmin = Rt - r;
        double dmax = sqrt(r * r - Rg * Rg) + sqrt(Rt * Rt - Rg * Rg);
        double dminp = r - Rg;
        double dmaxp = sqrt(r * r - Rg * Rg);
        glUniform1f(glGetUniformLocation(prog, "r"), float(r));
        glUniform4f(glGetUniformLocation(prog, "dhdH"), float(dmin), float(dmax), float(dminp), float(dmaxp));
        glUniform1i(glGetUniformLocation(prog, "layer"), layer);
    };

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // computes transmittance texture T (line 1 in algorithm 4.1)
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, transmittanceTexture->textureId, 0);
    glViewport(0, 0, TRANSMITTANCE_W, TRANSMITTANCE_H);
    glUseProgram(TransmittanceShader->program);
    drawQuad();

    // computes irradiance texture deltaE (line 2 in algorithm 4.1)
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, deltaETexture->textureId, 0);
    glViewport(0, 0, SKY_W, SKY_H);
    glUseProgram(Irradiance1Shader->program);
    glUniform1i(glGetUniformLocation(Irradiance1Shader->program, "transmittanceSampler"), transmittanceUnit);
    drawQuad();

    // computes single scattering texture deltaS (line 3 in algorithm 4.1)
    // Rayleigh and Mie separated in deltaSR + deltaSM
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, deltaSRTexture->textureId, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, deltaSMTexture->textureId, 0);
    unsigned int bufs[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, bufs);
    glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
    glUseProgram(Inscatter1Shader->program);
    glUniform1i(glGetUniformLocation(Inscatter1Shader->program, "transmittanceSampler"), transmittanceUnit);
    for (int layer = 0; layer < RES_R; ++layer) {
        setLayer(Inscatter1Shader->program, layer);
        drawQuad();
    }
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    // copies deltaE into irradiance texture E (line 4 in algorithm 4.1)
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, irradianceTexture->textureId, 0);
    glViewport(0, 0, SKY_W, SKY_H);
    glUseProgram(CopyIrradianceShader->program);
    glUniform1f(glGetUniformLocation(CopyIrradianceShader->program, "k"), 0.0);
    glUniform1i(glGetUniformLocation(CopyIrradianceShader->program, "deltaESampler"), deltaEUnit);
    drawQuad();

    // copies deltaS into inscatter texture S (line 5 in algorithm 4.1)
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, inscatterTexture->textureId, 0);
    glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
    glUseProgram(CopyInscatter1Shader->program);
    glUniform1i(glGetUniformLocation(CopyInscatter1Shader->program, "deltaSRSampler"), deltaSRUnit);
    glUniform1i(glGetUniformLocation(CopyInscatter1Shader->program, "deltaSMSampler"), deltaSMUnit);
    for (int layer = 0; layer < RES_R; ++layer) {
        setLayer(CopyInscatter1Shader->program, layer);
        drawQuad();
    }

    // loop for each scattering order (line 6 in algorithm 4.1)
    for (int order = 2; order <= 4; ++order) {

        // computes deltaJ (line 7 in algorithm 4.1)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, deltaJTexture->textureId, 0);
        glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
        glUseProgram(InscatterSShader->program);
        glUniform1f(glGetUniformLocation(InscatterSShader->program, "first"), order == 2 ? 1.0 : 0.0);
        glUniform1i(glGetUniformLocation(InscatterSShader->program, "transmittanceSampler"), transmittanceUnit);
        glUniform1i(glGetUniformLocation(InscatterSShader->program, "deltaESampler"), deltaEUnit);
        glUniform1i(glGetUniformLocation(InscatterSShader->program, "deltaSRSampler"), deltaSRUnit);
        glUniform1i(glGetUniformLocation(InscatterSShader->program, "deltaSMSampler"), deltaSMUnit);
        for (int layer = 0; layer < RES_R; ++layer) {
            setLayer(InscatterSShader->program, layer);
            drawQuad();
        }

        // computes deltaE (line 8 in algorithm 4.1)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, deltaETexture->textureId, 0);
        glViewport(0, 0, SKY_W, SKY_H);
        glUseProgram(IrradianceNShader->program);
        glUniform1f(glGetUniformLocation(IrradianceNShader->program, "first"), order == 2 ? 1.0 : 0.0);
        glUniform1i(glGetUniformLocation(IrradianceNShader->program, "transmittanceSampler"), transmittanceUnit);
        glUniform1i(glGetUniformLocation(IrradianceNShader->program, "deltaSRSampler"), deltaSRUnit);
        glUniform1i(glGetUniformLocation(IrradianceNShader->program, "deltaSMSampler"), deltaSMUnit);
        drawQuad();

        // computes deltaS (line 9 in algorithm 4.1)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, deltaSRTexture->textureId, 0);
        glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
        glUseProgram(InscatterNShader->program);
        glUniform1f(glGetUniformLocation(InscatterNShader->program, "first"), order == 2 ? 1.0 : 0.0);
        glUniform1i(glGetUniformLocation(InscatterNShader->program, "transmittanceSampler"), transmittanceUnit);
        glUniform1i(glGetUniformLocation(InscatterNShader->program, "deltaJSampler"), deltaJUnit);
        for (int layer = 0; layer < RES_R; ++layer) {
            setLayer(InscatterNShader->program, layer);
            drawQuad();
        }

        glEnable(GL_BLEND);
        glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
        glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE);

        // adds deltaE into irradiance texture E (line 10 in algorithm 4.1)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, irradianceTexture->textureId, 0);
        glViewport(0, 0, SKY_W, SKY_H);
        glUseProgram(CopyIrradianceShader->program);
        glUniform1f(glGetUniformLocation(CopyIrradianceShader->program, "k"), 1.0);
        glUniform1i(glGetUniformLocation(CopyIrradianceShader->program, "deltaESampler"), deltaEUnit);
        drawQuad();

        // adds deltaS into inscatter texture S (line 11 in algorithm 4.1)
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, inscatterTexture->textureId, 0);
        glViewport(0, 0, RES_MU_S * RES_NU, RES_MU);
        glUseProgram(CopyInscatterNShader->program);
        glUniform1i(glGetUniformLocation(CopyInscatterNShader->program, "deltaSSampler"), deltaSRUnit);
        for (int layer = 0; layer < RES_R; ++layer) {
            setLayer(CopyInscatterNShader->program, layer);
            drawQuad();
        }

        glDisable(GL_BLEND);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glFinish();
    //////////////////////////////////////////////////////////////////////////
}

int Game::width = 0;
int Game::height = 0;