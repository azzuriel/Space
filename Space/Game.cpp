#pragma once
#include "Game.h"
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

    glfwWindowHint(GLFW_SAMPLES, 4);
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

    //*******************************

    Bitmap *b = new Bitmap();

    b->Load("img.png");

    unsigned int texture = 0;
    texture = GenerateOpenglBitmap(*b, true, true);

    b->Free();
    delete b;

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


void CameraSetup(GLuint program, Camera &camera)
{
    auto vp = camera.projection * camera.view;
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.viewProjection"), 1, GL_FALSE, &vp[0][0]);
    glUniform3fv(glGetUniformLocation(program, "transform.viewPosition"), 1, &camera.position[0]);
}

void RenderScene(std::shared_ptr<BasicJargShader> BasicShader, Camera camera, ROAMSurface* planet, Mesh * cube, SkySphere &ss)
{
    CameraSetup(BasicShader->program, camera);
    planet->Render(BasicShader);
    cube->Render();
    ss.m->Render();
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
    //glEnable(GL_SMOOTH);
    Mouse::IsLeftPressed();

    auto BasicShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    BasicShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/minnaert.glsl");
    BasicShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/minnaert.glsl");
    BasicShader->Link();
    BasicShader->UpdateUniforms();
    auto mvpBasic = BasicShader->LocateVars("transform.viewProjection"); //var0
    auto worldID = BasicShader->LocateVars("transform.model"); //var1
    BasicShader->LocateVars("transform.normal"); //var2
    auto colorTextureLocation = BasicShader->LocateVars("material.texture");

    auto StarSphereShader = std::shared_ptr<JargShader>(new JargShader());
    StarSphereShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/starsphere.glsl");
    StarSphereShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/starsphere.glsl");
    StarSphereShader->Link();
    StarSphereShader->LocateVars("transform.viewProjection"); //var0
    StarSphereShader->LocateVars("transform.model"); //var1
    StarSphereShader->LocateVars("transform.normal"); //var2

    auto MinimalShader = std::shared_ptr<BasicJargShader>(new BasicJargShader());
    MinimalShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/minimal.glsl");
    MinimalShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/minimal.glsl");
    MinimalShader->Link();
    MinimalShader->UpdateUniforms();
    MinimalShader->LocateVars("transform.viewProjection"); //var0
    MinimalShader->LocateVars("transform.model"); //var1
    MinimalShader->LocateVars("transform.normal"); //var2

    auto test = std::shared_ptr<Texture>(new Texture());
    test->Load("img.png");

    auto hm = std::shared_ptr<Texture>(new Texture());
    hm->Load("face.png");

    auto st = std::shared_ptr<Texture>(new Texture());
    st->Load("st.png");

    BasicShader->BindProgram();
    auto mat = std::shared_ptr<Material>(new Material());
    mat->texture = test;
    mat->normal = hm;
    mat->ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    mat->diffuse = vec4(0.3f, 0.5f, 1.0f, 1.0f);
    mat->specular = vec4(0.8f, 0.8f, 0.8f, 1.0f);
    mat->emission = vec4(0.0f, 0.0f, 0.0f, 1.f);
    mat->shininess = 20.0f;

    PointLight pl;
    pl.position = vec4(5.0f, 12.0f, 3.0f, 1.0f);
    pl.ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    pl.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pl.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    pl.attenuation = vec3(0.000f, 0.0f, 0.00000f);

    PointLightSetup(BasicShader->program, pl);

    auto LinesShader = std::shared_ptr<JargShader>(new JargShader());
    LinesShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/colored.glsl");
    LinesShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/colored.glsl");
    LinesShader->Link();
    auto mvpLine = LinesShader->LocateVars("MVP");

    auto TextureShader = std::shared_ptr<JargShader>(new JargShader());
    TextureShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/textured.glsl");
    TextureShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/textured.glsl");
    TextureShader->Link();
    auto mvpTex = TextureShader->LocateVars("MVP");

    auto sb = std::unique_ptr<Batched>(new Batched());
    sb->Initialize(TextureShader.get(), LinesShader.get());

    const glm::mat4 Identity = glm::mat4(1.0f);

    auto m = new Mesh(Icosahedron::getMesh());
    //m->loadOBJ("Data\\untitled.obj");
    m->Bind();
    m->shader = BasicShader;
    m->material = mat;

    auto plane =Tesselator::Tesselate(5, Quad::GetMesh());
    plane->Bind();
    plane->shader = BasicShader;
    plane->material = mat;
    plane->World = glm::rotate(Identity, (float)(3.1415/2.0), vec3(1.0,0.0,0.0));
    plane->World = glm::translate(plane->World, vec3(0.0,0.0,10.0));
    plane->World = glm::scale(plane->World, vec3(80.5,80.5,80.5));

    auto cube = new Mesh(Icosahedron::getMesh());
    cube->Bind();
    cube->shader = BasicShader;
    cube->material = mat;


    //auto m = new Model();
    //m->LoadBinary("untitled1.m");
    //m->Bind();
    //for (int i = 0; i<m->meshes.size(); i++)
    //{
    //	m->meshes[i]->shader = BasicShader.get();
    //}

    Camera camera;
    camera.SetWindowSize(width, height);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 MVP = camera.VP() * model;
    camera.SetPosition(vec3(50,0,-200));
    camera.SetLookAt(vec3(50,0,50));

    auto font = std::unique_ptr<Font>(new Font());
    font->Initialize();
    if(!font->Create("font.json")){
        LOG(ERROR) << "failed to load\process font.json";
    }

    WinS* ws = new WinS(sb.get(), *font);

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

    //auto c = std::unique_ptr<Model>(new Model("untitled2.dae"));


    auto sphere = std::unique_ptr<DynamicObject>(new DynamicObject());
    sphere->bpRegister(dynamicsWorld.get());

    int iters = 0;
    float sec = 0;

    float rotated = 0;
    auto rott = std::unique_ptr<float[]>(new float[25]);
    for (int i =0; i<25;i++)
    {
        rott[i] = 0;
    }

    // 	auto ts = std::unique_ptr<QuadLod>(new QuadLod());
    // 	ts->GenerateFrom(glm::vec3(0.0,0.0,0.0));
    // 	ts->m->World = Identity;
    // 	ts->m->Shader = BasicShader.get();
    // 	ts->m->Texture = &test;
    // 	ts->Bind();

    //auto surf = std::unique_ptr<ROAMSurface>(new ROAMSurface());

//     sqlite3 *db;
//     char *zErrMsg = 0;
//     auto rc = sqlite3_open("stars.db", &db);
//     int starsize;
//     if( rc ){
//     LOG(ERROR) << "Can't open database: " << sqlite3_errmsg(db);
//     }else{
//     LOG(INFO) << "Opened database successfully";
//     auto sql = "SELECT COUNT(*) FROM test";
//     rc = sqlite3_exec(db, sql, [](void *arg, int argc, char **argv, char **azColName)->int{ LOG(INFO) << "Total " << (argv[0] ? argv[0] : "NULL")  << " lines"; sscanf(argv[0], "%f", arg); return 0; }, &starsize, &zErrMsg);
//     if(rc){
//     LOG(ERROR) << zErrMsg;
//     }
//     }

    //std::make_shared(new std::vector<>);
    SkySphere ss;
    ss.m->Verteces.resize(88000*3);
    ss.m->Indeces.resize(88000*3);
    /*auto sql = "SELECT * FROM test";
    int starpos =0;
    rc = sqlite3_exec(db, sql, [](void *last, int argc, char **argv, char **azColName)->int{Game::ss.Generate(last, argc, argv, azColName);(*reinterpret_cast<int*>(last))+=3; return 0;}, &starpos, &zErrMsg);
    if(rc){
    LOG(ERROR) << zErrMsg;
    } else {
    LOG(INFO) << "skysphere end";
    }*/
    ss.m->Bind();
    ss.m->shader = BasicShader;
    auto stm = std::shared_ptr<Material>(new Material());
    stm->texture = st;
    ss.m->material = stm;


    ROAMSurface* planet = new ROAMSurface();

    Texture emptytex = Texture();
    emptytex.Empty(vec2(width,height));

    Texture depthtexture = Texture();
    depthtexture.CreateDepth(vec2(width*2, height*2));
    auto test_fbo = FBO_Test(depthtexture);

    SpaceSolver spso = SpaceSolver();
    GameObject star1;
    star1.majorAxis = 0.00001;
    star1.minorAxis = 0.00001;
    spso.objects.push_back(&star1);
    auto so = new GameObject[13];

    so[0].majorAxis = so[0].minorAxis = 0.38;
    spso.objects.push_back(&so[0]);

    so[1].majorAxis = so[1].minorAxis = 0.72;
    spso.objects.push_back(&so[1]);

    so[2].majorAxis = so[2].minorAxis = 1;
    spso.objects.push_back(&so[2]);

    so[3].majorAxis = so[3].minorAxis = 1.52;
    spso.objects.push_back(&so[3]);

    so[4].majorAxis = so[4].minorAxis = 2.76;
    spso.objects.push_back(&so[4]);

    so[5].majorAxis = so[5].minorAxis = 5.20;
    spso.objects.push_back(&so[5]);

    so[6].majorAxis = so[6].minorAxis = 9.54;
    spso.objects.push_back(&so[6]);

    so[7].majorAxis = so[7].minorAxis = 19.22;
    spso.objects.push_back(&so[7]);

    so[8].majorAxis = so[8].minorAxis = 30,06;
    spso.objects.push_back(&so[8]);

    so[9].majorAxis = so[9].minorAxis = 39,2;
    spso.objects.push_back(&so[9]);

    so[10].majorAxis = so[10].minorAxis = 42.1;
    spso.objects.push_back(&so[10]);

    so[11].majorAxis = so[11].minorAxis = 45,2;
    spso.objects.push_back(&so[11]);

    so[12].majorAxis = so[12].minorAxis = 68,03;
    spso.objects.push_back(&so[12]);

    GameObject pl1;
    pl1.pos = vec3(11,0,11);
    pl1.majorAxis = 11*4*2;
    pl1.minorAxis = 11*4;
    pl1.epoch = 11;
    pl1.inclination = 0.2;
    spso.objects.push_back(&pl1);

    GameObject pl2;
    pl2.pos = vec3(11,0,11);
    pl2.majorAxis = 11*4*1.5;
    pl2.minorAxis = 11*4;
    pl2.epoch = 11;
    pl2.inclination = 0.2;
    spso.objects.push_back(&pl2);

    GameObject pl3;
    pl3.pos = vec3(11,0,11);
    pl3.majorAxis = 11*4*1.2;
    pl3.minorAxis = 11*4;
    pl3.epoch = 11;
    pl3.inclination = 0.2;
    spso.objects.push_back(&pl3);

    GameObject pl4;
    pl4.pos = vec3(11,0,11);
    pl4.majorAxis = 11*4*4;
    pl4.minorAxis = 11*4;
    pl4.epoch = 11;
    pl4.inclination = 0.2;
    spso.objects.push_back(&pl4);


    spso.MakeOrbits();

    Camera* cur_cam = &camera;
    Camera lightcam;
    lightcam.SetWindowSize(width, height);
    lightcam.SetPosition(vec3(50,0,-200));
    lightcam.SetLookAt(vec3(50,0,50));

    Mouse::SetFixedPosState(true);
    glCullFace(GL_BACK);
    vec3 camlast;
    while(Running && !glfwWindowShouldClose(window)) 
    {
        glEnable(GL_DEPTH_TEST);
        gt.Update(glfwGetTime());
        fps.Update(gt);
        dynamicsWorld->stepSimulation(gt.elapsed,10);

        //glfwSetWindowTitle(window, a.c_str());

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
            switch(wire){
            case 0:
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glDisable(GL_CULL_FACE);
                wire = 1;
                break;	
            case 1:
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                wire = 2;
                break;
            case 2:
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                wire = 0;
                break;
            }
        }

        if(Keyboard::isKeyDown(GLFW_KEY_DOWN)){
            sphere->fallRigidBody->applyCentralForce(btVector3(0,1,0).rotate(sphere->fallRigidBody->getOrientation().getAxis(), sphere->fallRigidBody->getOrientation().getAngle()));
            sphere->fallRigidBody->activate();
        }

        if(Keyboard::isKeyDown(GLFW_KEY_UP)){
            sphere->fallRigidBody->setLinearVelocity(btVector3(0,0,0));
        }

        //if(Keyboard::isKeyDown(GLFW_KEY_S)){
        //	sphere->fallRigidBody->applyTorque(btVector3(1,0,0));
        //	sphere->fallRigidBody->activate();
        //}

        //if(Keyboard::isKeyDown(GLFW_KEY_W)){
        //	sphere->fallRigidBody->applyTorque(btVector3(-1,0,0));
        //	sphere->fallRigidBody->activate();
        //}

        //if(Keyboard::isKeyDown(GLFW_KEY_A)){
        //	sphere->fallRigidBody->applyTorque(btVector3(0,1,0));
        //	sphere->fallRigidBody->activate();
        //}

        //if(Keyboard::isKeyDown(GLFW_KEY_D)){
        //	sphere->fallRigidBody->applyTorque(btVector3(0,-1,0));
        //	sphere->fallRigidBody->activate();
        //}

        //if(Keyboard::isKeyDown(GLFW_KEY_Q)){
        //	sphere->fallRigidBody->applyTorque(btVector3(0,0,1));
        //	sphere->fallRigidBody->activate();
        //}

        //if(Keyboard::isKeyDown(GLFW_KEY_E)){
        //	sphere->fallRigidBody->applyTorque(btVector3(0,0,-1));
        //	sphere->fallRigidBody->activate();
        //}

        if(Keyboard::isKeyDown(GLFW_KEY_F5)){
            camera.SetLookAt(vec3(0,0,0.0F));
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
            camera.camera_scale = gt.elapsed*10.0F;
        }

        spso.Solve(gt);

        if(Mouse::GetFixedPosState())
            camera.Move2D(Mouse::GetCursorDelta().x, Mouse::GetCursorDelta().y);

        auto mpos = Mouse::GetCursorPos();

        BasicShader->BindProgram();

        btTransform trans;
        sphere->fallRigidBody->getMotionState()->getWorldTransform(trans);
        auto q = trans.getRotation();
        auto t = trans.getOrigin();
        auto vect = vec3(t.getX(),t.getY(),t.getZ());

        auto orient = btVector3(0,1,0).rotate(sphere->fallRigidBody->getOrientation().getAxis(), sphere->fallRigidBody->getOrientation().getAngle());
        auto vecor = vec3(orient.getX(), orient.getY(), orient.getZ());
        //camera.position = vect - vec3(5);
        //camera.SetLookAt(vect);

        //m->World = glm::translate(Identity, vect);
        auto vecc = vec3(q.getAxis().x(), q.getAxis().y(), q.getAxis().z());
        //m->World = glm::rotate(m->World, q.getAngle(), vecc);
        //m->World = glm::scale(m->World, vec3(1,1,1));
        pl.position = glm::vec4(sin(rotated/20.f)*800, 2, cos(rotated/20.f)*800, 1);

        lightcam.position = vec3(pl.position)/3.f;
        lightcam.SetLookAt(vec3(0,0,0));
        

        rotated+=gt.elapsed;
        for (int i=0;i<25;i++)
        {
            rott[i] += gt.elapsed * i / 12.0f;
        }

        camera.Update();
        lightcam.Update();
        lightcam.projection = glm::ortho<float>(-100,100,-100,100,1,1000);

        sec += gt.elapsed;
        if(sec > 0.2 && distance(camlast, camera.position) > 0) {
            sec = 0;
            planet->UpdateCells(camera.position);
            planet->Bind();
        }
        camlast = camera.position;

        PointLightSetup(BasicShader->program, pl);

        ss.m->World = glm::translate(Identity, camera.position);

        
        cube->World = glm::translate(Identity, vec3(sin(rotated)*150, 2, cos(rotated)*150)) * glm::scale(Identity, vec3(20,20,20));

        
        ////////////////////////////////////////////////////////////////////////// WORLD PLACE
        for (unsigned int i = 0; i < spso.objects.size(); i++)
        {
            //cube->World = glm::translate(Identity, spso.objects[i]->pos);
            //cube->Render();
            //sb->DrawLines3d(spso.objects[i]->orbit, Colors::Red);
        }

        //plane->Render();
       

        glBindFramebuffer(GL_FRAMEBUFFER, test_fbo);
        glViewport(0, 0, depthtexture.width, depthtexture.height);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);
        RenderScene(BasicShader, lightcam, planet, cube, ss);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        BasicShader->BindProgram();
        glUniform1i(glGetUniformLocation(BasicShader->program, "depthTexture"), 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, depthtexture.textureId);

        static const mat4 bias(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0);
        auto mult = bias * lightcam.projection * lightcam.view;
        glUniformMatrix4fv(glGetUniformLocation(BasicShader->program, "transform.light"), 1, GL_FALSE, &mult[0][0]);

        RenderScene(BasicShader, *cur_cam, planet, cube, ss);
         
        if(wire == 2) {
            LinesShader->BindProgram();
            glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera.VP()[0][0]);
            dynamicsWorld->debugDrawWorld();
        }
        

        int dc = sb->RenderFinallyWorld();

        ////////////////////////////////////////////////////////////////////////// WORLD PLACE

        glDisable(GL_DEPTH_TEST);
        MVP = camera.GetOrthoProjection();
        TextureShader->BindProgram();
        glUniformMatrix4fv(mvpTex, 1, GL_FALSE, &MVP[0][0]);
        LinesShader->BindProgram();
        glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &MVP[0][0]);

        sb->DrawString(vec2(10,10), std::to_string(fps.GetCount()), vec3(0,0,0), *font);		
        sb->DrawString(vec2(20,20), camera.getFullDebugDescription(), Colors::Red, *font);
        sb->DrawQuad(vec2(100,100), vec2(100,100), emptytex);

        ws->Update(gt);
        ws->Draw();

        LinesShader->BindProgram();
        glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera.VP()[0][0]);

        dc += sb->RenderFinally();        

        Mouse::Update();

        //glFlush();
        glfwSwapBuffers(window);

        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    delete m;
    delete plane;
    delete cube;
    delete[] so;

    delete ws;
    delete planet;

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

int Game::width = 0;
int Game::height = 0;