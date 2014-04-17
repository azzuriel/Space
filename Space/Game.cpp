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
#include "../Engine/MAterial.h"
#ifdef LINUX
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include <math.h>
#include "../Engine/TreeSphere.h"



void mySleep(int sleepMs)
{
#ifdef LINUX
	usleep(sleepMs * 1000);
#endif
#ifdef _WIN32
	Sleep(sleepMs);
#endif
}

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
	google::SetLogFilenameExtension(".txt");
	google::SetLogDestination(google::INFO, "logs/");
	LOG(INFO) << "Jarg initialization start";
	glfwSetErrorCallback(errorCallbackGLFW3);

	int glfwErrorCode = glfwInit();
	if (!glfwErrorCode)
	{
		LOG(ERROR) << "glfwInit error " << glfwErrorCode;
		return glfwErrorCode;
	}

	//glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
		LOG(FATAL) << "ќшибка создани€ окна GLFW.";
		return false;
	}
	glfwMakeContextCurrent(window);
	//glfwSetWindowTitle(window, AutoVersion::GetTitle().c_str());

	glfwSwapInterval(0);

	int glVersion[2] = {-1, -1};
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 
	LOG(INFO) <<"OpenGL: "<<glVersion[0]<<"."<<glVersion[1]<<" glfw: " << glfwGetVersionString();


	render = std::unique_ptr<Render>(new Render());
	render->Initialize();
	render->SetWindowSize(width, height);

	Keyboard::Initialize();
	glfwSetKeyCallback(window, KeyCallbackGLFW3);

	Mouse::Initialize(window);
	Mouse::SetWindowSize(width, height);
//	Mouse::SetFixedPosState(true);
	glfwSetCursorPosCallback(window, CursorPosCallbackGLFW3);
	glfwSetCursorEnterCallback(window, CursorClientAreaCallbackGLFW3);	
	glfwSetWindowFocusCallback(window, WindowFocusCallbackGLFW3);
	glfwSetMouseButtonCallback(window, SetMouseButtonCallbackGLFW3);
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
	glBindTexture(GL_TEXTURE_2D, material.normal);
	glUniform1i(glGetUniformLocation(program, "material.texture"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, material.normal);
	glUniform1i(glGetUniformLocation(program, "material.normal"), 0);

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

	// передаем позицию наблюдател€ (камеры) в шейдерную программу
	glUniform3fv(glGetUniformLocation(program, "transform.viewPosition"), 1, &camera.position[0]);
}

void Game::Run()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 1);
	//glEnable(GL_SMOOTH);

	auto BasicShader = std::shared_ptr<JargShader>(new JargShader());
	BasicShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/basic.glsl");
	BasicShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/basic.glsl");
	//BasicShader->loadShaderFromSource(GL_TESS_CONTROL_SHADER, "Shaders/basic.glsl");
	//BasicShader->loadShaderFromSource(GL_TESS_EVALUATION_SHADER, "Shaders/basic.glsl");
	BasicShader->link();
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
	LinesShader->LoadFromFile("Shaders/lines.fs", "Shaders/lines.vs");
	auto mvpLine = LinesShader->LocateVars("MVP");

	auto sb = std::unique_ptr<Batched>(new Batched());
	sb->Initialize(BasicShader.get(), LinesShader.get());

	const glm::mat4 Identity = glm::mat4(1.0f);

	auto m = Tesselator::SphereTesselate(5, Cube::getMesh());
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
	glm::mat4 MVP = camera.CalculateMatrix() * model;

	//auto font = std::unique_ptr<Font>(new Font());
	//font->Initialize();
	//font->Create("font.json");

    int iters = 0;
	float sec = 0;

	auto ts = std::unique_ptr<TreeSphere>(new TreeSphere());
	ts->GenerateFrom(glm::vec4(0.0,0.0,0.0,0.0));
	ts->Bind();

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
			camTest++;
	camTest++;camTest++;camTest++;
		}
		if(Keyboard::isKeyDown(GLFW_KEY_F5)){
			camTest--;
		camTest--;camTest--;camTest--;
		}

		glUniform1f(innerT, 1);

		auto mpos = Mouse::GetCursorPos();
		pl.position = vec4(mpos.x*10.0-1000, 10, mpos.y*10.0-1000, 1.0);
		camera.position = vec3(40,40,40);

		PointLightSetup(BasicShader->program, pl);
		MaterialSetup(BasicShader->program, mat);

		BasicShader->BindProgram();
		camera.view = glm::lookAt(vec3(5000+camTest,500,40), vec3(40+camTest,30,30), vec3(0,1,0));
		MVP = camera.CalculateMatrix() * model;
		CameraSetup(BasicShader->program, camera, m->World, MVP);



		// Use our shader
		
		glUniformMatrix4fv(mvpBasic, 1, GL_FALSE, &MVP[0][0]);
		glUniform1i(colorTextureLocation, 1);

		//m->World = glm::rotate(m->World, (float)gt.elapsed, vec3(1,0,1));
		//m->Render();
		//plane->Render();
		sec += gt.elapsed;
		if(sec > 0.1) {
			sec = 0;
			delete ts->root;
			ts->m->Indeces.clear();
			ts->m->Verteces.clear();
			ts->root = nullptr;
			ts->GenerateFrom(vec4(5000+camTest,500,40,1));
			ts->Bind();
		}
		ts->Render();

				//cube->World = glm::scale(Identity, vec3(20,20,20));
		//cube->World = glm::translate(cube->World, vec3(pl.position.x, pl.position.y, pl.position.z));

		cube->Render();

		glfwSetWindowTitle(window, std::to_string((long double)fps.GetCount()).c_str());
		//sb->DrawString(Vector2(10,10), std::to_string((long double)fps.GetCount()), *font);

		Mouse::Update();

		glfwSwapBuffers(window);
		glfwPollEvents();
		mySleep(16);
	}

	//delete m;
	//glfwDestroyWindow(window);
	//glfwTerminate();
}