#include <iostream>
#include <string>

#include "GameManager.h"
#include "ResourceManager.h"

GameManager::GameManager() :
	Window(nullptr),
	Controller(nullptr)
{
	if (!glfwInit())
	{
		std::cerr << "ERROR::Could not initialize GLFW\n";
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GameManager::~GameManager()
{
	if (Window)
	{
		glfwDestroyWindow(Window);
	}
	glfwTerminate();
}

GameManager& GameManager::GetInstance()
{
	static GameManager Instance;
	return Instance;
}

GLboolean GameManager::Initialize(GameController* NewController)
{
	if (!NewController) return GL_FALSE;

	Controller = std::unique_ptr<GameController>(NewController);

	// Just incase we are switching to another controller
	if (Window)
	{
		glfwDestroyWindow(Window);
	}

	// Create the window
	Window = glfwCreateWindow(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT, Controller->Title, nullptr, nullptr);
	glfwMakeContextCurrent(Window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	Controller->Window = Window;

	glViewport(0, 0, DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

	// Set the callbacks
	glfwSetKeyCallback(Window, GameManager::Key_Callback);
	glfwSetWindowSizeCallback(Window, GameManager::WindowResize_Callback);
	//glfwSetMouseButtonCallback(Window, );
	//glfwSetCursorPosCallback(Window, );

	return GL_TRUE;
}

void GameManager::Run()
{
	Controller->Initialize();

	// Used to display frame time
	GLdouble deltaTime = 0, lastFrame = 0, accumulator = 0, frameTime = glfwGetTime();
	GLuint fpsCounter = 0;

	while (!glfwWindowShouldClose(Window))
	{

		GLdouble currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		fpsCounter++;
		if (currentFrame - frameTime >= 1.0)
		{
			// Shows frame time in the window header
			glfwSetWindowTitle(Window, (Controller->Title + std::string(" ") + std::to_string(1000.0 / (double)fpsCounter) + " ms/frame\n").c_str());
			fpsCounter = 0;
			frameTime += 1.0;
		}

		accumulator += deltaTime;
		while (accumulator >= TARGET_FRAMERATE)
		{
			// Check for events, process any input, and then update the controller
			glfwPollEvents();
			//Controller->ProcessInput(TARGET_FRAMERATE);
			Controller->Update(TARGET_FRAMERATE);
			accumulator -= TARGET_FRAMERATE;
		}

		// Render the controller
		glClearColor(0.16f, 0.18f, 0.22f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Controller->Render();

		glfwSwapBuffers(Window);
	}
}

void GameManager::Key_Callback(GLFWwindow* Window, GLint Key, GLint Scancode, GLint Action, GLint Mode)
{
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, GL_TRUE);
	if (Key >= 0 && Key < 1024)
	{
		if (GAMEMANAGER.Controller)
			GAMEMANAGER.Controller->ProcessInput(Key, Action, Mode);
	}
}

void GameManager::WindowResize_Callback(GLFWwindow* Window, GLint Width, GLint Height)
{
	if (!GAMEMANAGER.Controller) return;
	glViewport(0, 0, Width, Height);
	GAMEMANAGER.Controller->SetScreenDimensions(Width, Height, false);
}
