#pragma once
#include "Engine.h"
#define GLM_FORCE_RADIANS
#include <glew.h>
#include <glfw3.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <memory>
#include "..\Engine\SpriteBatch.h"
#include "..\Engine\Font.h"
#include "..\Engine\Camera.h"
#include "../Engine/SphereTesselator.h"
#include "../Engine/Material.h"
#include <glm.hpp>
#ifdef LINUX
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include "../Engine/TreeSphere.h"
#include "ROAMgrid.h"
#include <ClassicNoise.h>
#include <ROAMSurface.h>
#include <chrono>
#include <thread>
#include "../Engine/WinS.h"
#include "../Engine/JButton.h"
#include "../Engine/JLabel.h"

void KeyCallbackGLFW3(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	Keyboard::SetKey(key, scancode, action, mods);
}

void CursorPosCallbackGLFW3(GLFWwindow *window, double xpos, double ypos)
{
	Mouse::SetCursorPos(xpos, ypos);
}

void CursorClientAreaCallbackGLFW3(GLFWwindow *window, int entered)
{
	Mouse::CursorClientArea(entered);
}

void SetWindowSizeCallbackGLFW3(GLFWwindow* window, int a, int b){
	Game::Resize(a, b);
	Mouse::SetWindowSize(a, b);
}

void SetMouseButtonCallbackGLFW3(GLFWwindow *window, int a, int b, int c)
{
	Mouse::SetButton(a, b, c);
}

void WindowFocusCallbackGLFW3(GLFWwindow *window, int focused)
{
	Mouse::WindowFocus(focused);
}

void errorCallbackGLFW3(int error, const char* description)
{
	LOG(ERROR) << description;
}


Game::Game(void)
{
	srand(1);
	Running = true;

	title = "Game";
	width = 1024;
	height = 600;
	fullscreen = false;
}

Game::~Game(void)
{

}

int Game::Initialize()
{
	google::InitGoogleLogging("Jarg.exe");
	google::SetLogFilenameExtension(".log.");
	google::SetLogDestination(google::INFO, "logs/space");
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Game::MINOR_Gl);
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
	LOG(INFO) << "OpenGL: " << std::to_string(glVersion[0]) << "." << std::to_string(glVersion[1]);
	LOG(INFO) << "glfw: " << glfwGetVersionString();
	

	Keyboard::Initialize();
	glfwSetKeyCallback(window, KeyCallbackGLFW3);

	Mouse::Initialize(window);
	Mouse::SetWindowSize(width, height);
//	Mouse::SetFixedPosState(true);
	glfwSetCursorPosCallback(window, CursorPosCallbackGLFW3);
	glfwSetCursorEnterCallback(window, CursorClientAreaCallbackGLFW3);	
	glfwSetWindowFocusCallback(window, WindowFocusCallbackGLFW3);
	glfwSetMouseButtonCallback(window, SetMouseButtonCallbackGLFW3);
	glfwSetWindowSizeCallback(window, SetWindowSizeCallbackGLFW3);
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

void MaterialSetup(GLuint program, const Material &material)
{
	// установка текстуры
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.texture);
	glUniform1i(glGetUniformLocation(program, "material.texture"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.normal);
	glUniform1i(glGetUniformLocation(program, "material.normal"), 1);

	// установка параметров
	glUniform4fv(glGetUniformLocation(program, "material.ambient"),  1, &material.ambient[0]);
	glUniform4fv(glGetUniformLocation(program, "material.diffuse"),  1, &material.diffuse[0]);
	glUniform4fv(glGetUniformLocation(program, "material.specular"), 1, &material.specular[0]);
	glUniform4fv(glGetUniformLocation(program, "material.emission"), 1, &material.emission[0]);

	glUniform1fv(glGetUniformLocation(program, "material.shininess"), 1, &material.shininess);
}

void CameraSetup(GLuint program, Camera &camera, const mat4 &model, const mat4 &mvp)
{
	// расчитаем необходимые матрицы
	//mat4 view           = glm::rotate(camera.rotation) * glm::translate(view, -camera.position);
	mat3 normal         = transpose(mat3(inverse(mat4(1.0f))));

	// передаем матрицы в шейдерную программу
	glUniformMatrix4fv(glGetUniformLocation(program, "transform.model"),          1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "transform.viewProjection"), 1, GL_FALSE, &mvp[0][0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "transform.normal"),         1, GL_TRUE, &normal[0][0]);

	// передаем позицию наблюдателя (камеры) в шейдерную программу
	glUniform3fv(glGetUniformLocation(program, "transform.viewPosition"), 1, &camera.position[0]);
}

void Game::Run()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 1);
	//glEnable(GL_SMOOTH);

	auto BasicShader = std::shared_ptr<JargShader>(new JargShader());
	BasicShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/basic.glsl");
	BasicShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/basic.glsl");
	BasicShader->Link();
	auto mvpBasic = BasicShader->LocateVars("transform.viewProjection");
	auto worldID = BasicShader->LocateVars("transform.model");
	auto innerT = BasicShader->LocateVars("InnerLevel"); int camTest = 2;
	glUniform1i(innerT, camTest);
	auto outerT = BasicShader->LocateVars("OuterLevel"); int outerLevel = 2;
	glUniform1i(outerT, outerLevel);
	auto colorTextureLocation = BasicShader->LocateVars("material.texture");

	Texture test;
	test.Load("img.png");

	Texture hm;
	hm.Load("face.png");

	Material mat;
	mat.texture = test.textureId;
	mat.normal = hm.textureId;
	mat.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	mat.diffuse = vec4(0.3f, 0.5f, 1.0f, 1.0f);
	mat.specular = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	mat.emission = vec4(0.0f, 0.0f, 0.0f, 1.f);
	mat.shininess = 20.0f;

	MaterialSetup(BasicShader->program, mat);

	PointLight pl;
	pl.position = vec4(30.0f, 30.0f, 30.0f, 1.0f);
	pl.ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	pl.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pl.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pl.attenuation = vec3(0.0001f, 0.0001f, 0.0001f);

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

	auto m = Tesselator::SphereTesselate(2, Cube::getMesh());
	//m->loadOBJ("Data\\untitled.obj");
	m->Bind();
	m->Shader = BasicShader.get();
	m->Texture = &test;
	m->World = glm::scale(Identity, vec3(8.5,8.5,8.5));

	auto plane =Tesselator::Tesselate(5, Quad::GetMesh());
	plane->Bind();
	plane->Shader = BasicShader.get();
	plane->Texture = &test;
	plane->World = glm::rotate(Identity, (float)(3.1415/2.0), vec3(1.0,0.0,0.0));
	plane->World = glm::translate(plane->World, vec3(0.0,0.0,10.0));
	plane->World = glm::scale(plane->World, vec3(80.5,80.5,80.5));
	
	auto cube = new Mesh(Cube::getMesh());
	cube->Bind();
	cube->Shader = BasicShader.get();
	cube->Texture = &test;

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
	Win* w;
	for(int i = 0; i< 30; i++) {
		w = new Win(glm::vec2(100 +i*5, 100 +i*5), glm::vec2(200,200));
		ws->windows.push_back(w);

		JButton* jb = new JButton(glm::vec2(10,100), glm::vec2(50,20));
		jb->parent = w;
		w->Items.push_back(jb);

		JLabel* jl = new JLabel(glm::vec2(20,20), "qwertyqwertyuiopasdfghjkl;zxcvbnm,.qwertyuiopasdfghjkl;zxcvbnm,.qwertyuiopasdfghjkl;zxcvbnm,.qwertyuiopasdfghjkl;zxcvbnm,.uiopasdfghjkl;zxcvbnm,.");
		jl->parent = w;
		w->Items.push_back(jl);
	}

    int iters = 0;
	float sec = 0;

// 	auto ts = std::unique_ptr<QuadLod>(new QuadLod());
// 	ts->GenerateFrom(glm::vec3(0.0,0.0,0.0));
// 	ts->m->World = Identity;
// 	ts->m->Shader = BasicShader.get();
// 	ts->m->Texture = &test;
// 	ts->Bind();

	//auto surf = std::unique_ptr<ROAMSurface>(new ROAMSurface());


	Mouse::SetFixedPosState(false);
	while(Running && !glfwWindowShouldClose(window)) 
	{
		glEnable(GL_DEPTH_TEST);
		glShadeModel (GL_SMOOTH);
		gt.Update(glfwGetTime());
		fps.Update(gt);

		//glfwSetWindowTitle(window, a.c_str());

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(204/255.0F, 1.0f, 1.0f, 1.0f);

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

		if(Keyboard::isKeyPress(GLFW_KEY_F3)){
			iters--;
			if(iters < 0){
				iters = 0;
			}
			m = Tesselator::SphereTesselate(iters, Cube::getMesh());
			m->Bind();
			m->Shader = BasicShader.get();
			m->Texture = &test;
			m->World = glm::scale(Identity, vec3(18,18,18));
		}

		if(Keyboard::isKeyPress(GLFW_KEY_F4)){
			iters++;
			if(iters > 9){
				iters = 9;
			}
			m = Tesselator::SphereTesselate(iters, Cube::getMesh());
			m->Bind();
			m->Shader = BasicShader.get();
			m->Texture = &test;
			m->World = glm::scale(Identity, vec3(18,18,18));
		}

		if(Keyboard::isKeyPress(GLFW_KEY_F2)){
			switch(wire){
			case 0:
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				wire = 1;
				break;	
			//case 1:
			//	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			//	wire = 2;
			//	break;
			default:
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				wire = 0;
				break;
			}
		}

		if(Keyboard::isKeyDown(GLFW_KEY_F6)){
			//surf->Test();
		}
		if(Keyboard::isKeyDown(GLFW_KEY_F5)){
			camera.SetLookAt(vec3(0,0,0.0F));
		}

		camera.move_camera = true;
		camera.camera_scale = gt.elapsed/10.0F;
		if(Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT)){
			camera.camera_scale = gt.elapsed*10.0F;
		}

		//camera.Move2D(Mouse::GetCursorDelta().x, Mouse::GetCursorDelta().y);

		pl.position = vec4(50.0f,50.0f,50.0f,1.0f)*m->World;

		auto mpos = Mouse::GetCursorPos();
		PointLightSetup(BasicShader->program, pl);
		MaterialSetup(BasicShader->program, mat);

		BasicShader->BindProgram();

		camera.Update();
		MVP = camera.VP() * model;
		CameraSetup(BasicShader->program, camera, m->World, MVP);

		sec += gt.elapsed;
		if(sec > 0.1) {
			sec = 0;
			//surf->UpdateCells(camera.position/100.0F);
			//surf->Bind();
		}

// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_2D, normalTexture);
// 		glUniform1i(glGetUniformLocation(BasicShader->program, "normalMap"), 0);
		//surf->Render(BasicShader.get());

		m->World = glm::rotate(m->World, (float)gt.elapsed, normalize(vec3(2,1,3)));
		m->Render();

		plane->Render();
// 		sec += gt.elapsed;
// 		if(sec > 0.1) {
// 			sec = 0;
// 			delete ts->root;
// 			ts->m->Indeces.clear();
// 			ts->m->Verteces.clear();
// 			ts->root = nullptr;
// 			ts->GenerateFrom(vec3(pl.position));
// 			ts->Bind();
// 		}
//		ts->Render();
		//cube->World = glm::translate(Identity, vec3(pl.position.x, pl.position.y, pl.position.z));
		//cube->World = glm::scale(cube->World, vec3(500,500,500));
		//cube->Render();

		
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		MVP = camera.GetOrthoProjection();
		TextureShader->BindProgram();
		glUniformMatrix4fv(mvpTex, 1, GL_FALSE, &MVP[0][0]);
		LinesShader->BindProgram();
		glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &MVP[0][0]);

		

		ws->Update(gt);
		ws->Draw();

		sb->DrawQuad(vec2(100,100),vec2(100,100), *font->tex);
		sb->DrawString(vec2(10,10), std::to_string(fps.GetCount()), *font);		
		sb->DrawRectangle(vec2(110,110), vec2(200,200), Colors::Green/2.0f);

		int dc = sb->RenderFinallyWorld();
 		dc += sb->RenderFinally();

		glfwSetWindowTitle(window, std::to_string(dc).c_str());

		Mouse::Update();

		glFlush();
		glfwSwapBuffers(window);
		
		glfwPollEvents();
		
		//std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}

	delete m;
	delete plane;
	delete cube;
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