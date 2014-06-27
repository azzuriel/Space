#pragma once
#include "Engine.h"
#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE
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
#ifdef __linux__
#include <unistd.h>
#endif /* __linux__ */
#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */
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
	LOG(INFO) << "Jarg initialization start";
	glfwSetErrorCallback(errorCallbackGLFW3);

	int glfwErrorCode = glfwInit();
	if (!glfwErrorCode)
	{
		LOG(ERROR) << "glfwInit error " << glfwErrorCode;
		return glfwErrorCode;
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);
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
	glfwSetKeyCallback(window, [](GLFWwindow *win, int key, int scancode, int action, int mods){Keyboard::SetKey(key, scancode, action, mods);});

	Mouse::Initialize(window);
	Mouse::SetWindowSize(width, height);
	//	Mouse::SetFixedPosState(true);
	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos){Mouse::SetCursorPos(xpos, ypos);});
	glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered){Mouse::CursorClientArea(entered);});	
	glfwSetWindowFocusCallback(window, [](GLFWwindow *window, int focused){Mouse::WindowFocus(focused);});
	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int a, int b, int c){Mouse::SetButton(a, b, c);});
	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int a, int b){Game::Resize(a, b); Mouse::SetWindowSize(a, b);});
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


void CameraSetup(GLuint program, Camera &camera, const mat4 &mvp)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "transform.viewProjection"), 1, GL_FALSE, &mvp[0][0]);
	glUniform3fv(glGetUniformLocation(program, "transform.viewPosition"), 1, &camera.position[0]);
}

void Game::Run()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 1);
	//glEnable(GL_SMOOTH);
	Mouse::IsLeftPressed();

	auto BasicShader = std::shared_ptr<JargShader>(new JargShader());
	BasicShader->loadShaderFromSource(GL_VERTEX_SHADER, "Shaders/basic.glsl");
	BasicShader->loadShaderFromSource(GL_FRAGMENT_SHADER, "Shaders/basic.glsl");
	BasicShader->Link();
	auto mvpBasic = BasicShader->LocateVars("transform.viewProjection"); //var0
	auto worldID = BasicShader->LocateVars("transform.model"); //var1
	BasicShader->LocateVars("transform.normal"); //var2
	auto colorTextureLocation = BasicShader->LocateVars("material.texture");

	Texture test;
	test.Load("img.png");

	Texture hm;
	hm.Load("face.png");

	BasicShader->BindProgram();
	Material mat;
	mat.texture = &test;
	mat.normal = &hm;
	mat.ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	mat.diffuse = vec4(0.3f, 0.5f, 1.0f, 1.0f);
	mat.specular = vec4(0.8f, 0.8f, 0.8f, 1.0f);
	mat.emission = vec4(0.0f, 0.0f, 0.0f, 1.f);
	mat.shininess = 20.0f;

	PointLight pl;
	pl.position = vec4(5.0f, 12.0f, 3.0f, 1.0f);
	pl.ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
	pl.diffuse = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pl.specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	pl.attenuation = vec3(0.005f, 0.0f, 0.00002f);

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

// 	auto m = new Mesh(Icosahedron::getMesh());
// 	m->Unindex();
// 	m->computeNormal();
// 	m->MergeVerteces();
// 	//m->loadOBJ("Data\\untitled.obj");
// 	m->Bind();
// 	m->shader = BasicShader.get();
// 	m->material = &mat;
// 	m->World = glm::scale(Identity, vec3(3,3,3));

	auto plane =Tesselator::Tesselate(5, Quad::GetMesh());
	plane->Bind();
	plane->shader = BasicShader.get();
	plane->material = &mat;
	plane->World = glm::rotate(Identity, (float)(3.1415/2.0), vec3(1.0,0.0,0.0));
	plane->World = glm::translate(plane->World, vec3(0.0,0.0,10.0));
	plane->World = glm::scale(plane->World, vec3(80.5,80.5,80.5));

	auto cube = new Mesh(Icosahedron::getMesh());
	cube->Bind();
	cube->shader = BasicShader.get();
	cube->material = &mat;


	auto m = new Model("untitled3.dae");
	m->Bind();
	m->meshes[0]->shader = BasicShader.get();
	m->meshes[1]->shader = BasicShader.get();

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
	float* rott = new float[25];
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


	Mouse::SetFixedPosState(false);
	while(Running && !glfwWindowShouldClose(window)) 
	{
		glEnable(GL_DEPTH_TEST);
		gt.Update(glfwGetTime());
		fps.Update(gt);
		dynamicsWorld->stepSimulation(gt.elapsed,10);

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

		if(Keyboard::isKeyPress(GLFW_KEY_F2)){
		switch(wire){
			case 0:
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				wire = 1;
				break;	
			case 1:
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
				wire = 2;
				break;
			case 2:
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
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

		if(Keyboard::isKeyDown(GLFW_KEY_D)){
			sphere->fallRigidBody->applyTorque(btVector3(1,0,0));
			sphere->fallRigidBody->activate();
		}

		if(Keyboard::isKeyDown(GLFW_KEY_A)){
			sphere->fallRigidBody->applyTorque(btVector3(-1,0,0));
			sphere->fallRigidBody->activate();
		}

		if(Keyboard::isKeyDown(GLFW_KEY_W)){
			sphere->fallRigidBody->applyTorque(btVector3(0,1,0));
			sphere->fallRigidBody->activate();
		}

		if(Keyboard::isKeyDown(GLFW_KEY_S)){
			sphere->fallRigidBody->applyTorque(btVector3(0,-1,0));
			sphere->fallRigidBody->activate();
		}

		if(Keyboard::isKeyDown(GLFW_KEY_Q)){
			sphere->fallRigidBody->applyTorque(btVector3(0,0,1));
			sphere->fallRigidBody->activate();
		}

		if(Keyboard::isKeyDown(GLFW_KEY_E)){
			sphere->fallRigidBody->applyTorque(btVector3(0,0,-1));
			sphere->fallRigidBody->activate();
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



		auto mpos = Mouse::GetCursorPos();

		BasicShader->BindProgram();

		btTransform trans;
		sphere->fallRigidBody->getMotionState()->getWorldTransform(trans);
		auto q = trans.getRotation();
		auto t = trans.getOrigin();
		auto vect = vec3(t.getX(),t.getY(),t.getZ());

		camera.position = vect+vec3(5);
		camera.SetLookAt(vect);

		m->World = glm::translate(Identity, vect);
		auto vecc = vec3(q.getAxis().x(), q.getAxis().y(), q.getAxis().z());
		m->World = glm::rotate(m->World, q.getAngle(), vecc);
		m->World = glm::scale(m->World, vec3(1,1,1));
		pl.position = glm::vec4(sin(rotated)*10, 2, cos(rotated)*10, 1);
		rotated+=gt.elapsed;
		for (int i=0;i<25;i++)
		{
			rott[i] += gt.elapsed * i / 12.0f;
		}

		cube->World = glm::translate(Identity, vec3(pl.position));

		camera.Update();
		MVP = camera.VP() * model;

		PointLightSetup(BasicShader->program, pl);
		CameraSetup(BasicShader->program, camera, MVP);
		////////////////////////////////////////////////////////////////////////// WORLD PLACE
		m->Render();
		plane->Render();
		cube->Render();
		for (int i=0;i<25;i++)
		{
			m->World = glm::translate(Identity, glm::vec3(sin(rott[i])*15, 2, cos(rott[i])*15));
			m->Render();
		}
		////////////////////////////////////////////////////////////////////////// WORLD PLACE

		if(wire == 2) {
			LinesShader->BindProgram();
			glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera.VP()[0][0]);
			dynamicsWorld->debugDrawWorld();
		}

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		MVP = camera.GetOrthoProjection();
		TextureShader->BindProgram();
		glUniformMatrix4fv(mvpTex, 1, GL_FALSE, &MVP[0][0]);
		LinesShader->BindProgram();
		glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &MVP[0][0]);



		ws->Update(gt);
		ws->Draw();

		sb->DrawString(vec2(10,10), std::to_string(fps.GetCount()), vec3(0,0,0), *font);		
		sb->DrawString(vec2(20,20), sphere->getFullDebugDescription(), Colors::Red, *font);

		LinesShader->BindProgram();
		glUniformMatrix4fv(mvpLine, 1, GL_FALSE, &camera.VP()[0][0]);
		int dc = sb->RenderFinallyWorld();

		dc += sb->RenderFinally();
		

		glfwSetWindowTitle(window, std::to_string(dc).c_str());

		Mouse::Update();

		//glFlush();
		glfwSwapBuffers(window);

		glfwPollEvents();

		//std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}

	delete m;
	delete plane;
	delete cube;

	sphere->bpUnregister(dynamicsWorld.get());

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