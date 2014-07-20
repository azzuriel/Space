#include "Game_Main.h"
#include "PerfomanceWindow.h"
#include <gtc\matrix_transform.inl>

Game::Game(void)
{
}


Game::~Game(void)
{
}

int Game::Run(){
    while(Running && !glfwWindowShouldClose(window)){
        Update();
        Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return true;
}

int Game::Destroy(){
    glfwDestroyWindow(window);
    glfwTerminate();

    return true;
}

void Game::Update()
{
    gt->Update(glfwGetTime());
    fps.Update(*gt);

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

    if(Keyboard::isKeyPress(GLFW_KEY_LEFT_CONTROL)){
        Mouse::SetFixedPosState(!Mouse::GetFixedPosState());
    }

    if(Mouse::GetFixedPosState())
        camera->Move2D(Mouse::GetCursorDelta().x, Mouse::GetCursorDelta().y);

    if(Keyboard::isKeyDown(GLFW_KEY_W)){
        camera->Move(FORWARD);
    }
    if(Keyboard::isKeyDown(GLFW_KEY_S)){
        camera->Move(BACK);
    }
    if(Keyboard::isKeyDown(GLFW_KEY_A)){
        camera->Move(LEFT);
    }
    if(Keyboard::isKeyDown(GLFW_KEY_D)){
        camera->Move(RIGHT);
    }

    camera->move_camera = true;
    if(Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT)){
        camera->camera_scale = gt->elapsed*50.0F;
    } else {
        camera->camera_scale = gt->elapsed*10.0F;
    }

    if(Keyboard::isKeyDown(GLFW_KEY_F5)){
        camera->SetLookAt(vec3(0,0,0.0F));
    }

    camera->Update();

    pw->UpdateValue(fps.GetCount());
    ws->Update(*gt);

    //
    Mouse::Update();
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
    auto vp = camera.VP();
    glUniformMatrix4fv(glGetUniformLocation(program, "transform.viewProjection"), 1, GL_FALSE, &vp[0][0]);
    glUniform3fv(glGetUniformLocation(program, "transform.viewPosition"), 1, &camera.position[0]);
}

void Game::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2,0.1, 0.3, 1.0f);


    BasicShader->Use();
    CameraSetup(BasicShader->program, *camera);
    PointLightSetup(BasicShader->program, light);

    icos->Render();
    test.World = glm::rotate(mat4(1), (float)-M_PI_2, vec3(1.0,0.0,0.0));
    test.World = glm::scale(test.World, vec3(10.0,10.0,10.0));
    test.Render();

    glDisable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

    auto MVP = camera->GetOrthoProjection();
    TextureShader->Use();
    glUniformMatrix4fv(mvpTex, 1, GL_FALSE, &MVP[0][0]);
    LinesShader->Use();
    glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &MVP[0][0]);

    batched->DrawLine2d(glm::vec2(1), glm::vec2(10), Colors::White);
    batched->DrawCube3d(vec3(0), vec3(1), Colors::Red);
    batched->DrawString(vec2(100,100), MetersSpeedString(spd), Colors::Red, *font);
    spd *= 1.001;

    LinesShader->Use();
    glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera->VP()[0][0]);
    test.RenderBounding(*batched);
    batched->RenderFinallyWorld();

    ws->Draw();

    batched->RenderFinally();
}

//WinS store and finalize all components
void Game::WindowsDesigner()
{
    ws = std::unique_ptr<WinS>(new WinS(batched.get(), *font));

    pw = new PerfomanceWindow();
    ws->windows.push_back(pw);
}

bool Game::Running = true;

bool Game::fullscreen = false;

int Game::height = 720;

int Game::width = 1280;
