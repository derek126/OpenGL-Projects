#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <map>
#include <memory>
#include <vector>

#define GAMEMANAGER GameManager::GetInstance()
#define DEFAULT_SCREENWIDTH 800
#define DEFAULT_SCREENHEIGHT 600

/*
* The all knowing singleton 'god' object for our game
*/
class GameManager
{
	GameManager();
	~GameManager();

	GameManager(GameManager const&) = delete;
	void operator =(GameManager const&) = delete;

public:

	static GameManager& GetInstance();

	// Create the window and does other various setup
	GLboolean Initialize(class GameController* NewController);

	// Sets the target framerate
	void SetTargetFrametime(const GLuint& Rate);

	// Sets clear color of the window
	void SetClearColor(const glm::vec4& Color);

	// Initiate the gameloop for the controller
	void Run();

private:

	// State
	GLFWwindow* Window;
	std::unique_ptr<class GameController> Controller;
	GLfloat TargetFrametime;
	glm::vec4 ClearColor;

	// Callbacks
	static void Key_Callback(GLFWwindow* Window, GLint Key, GLint Scancode, GLint Action, GLint Mode);
	static void WindowResize_Callback(GLFWwindow* Window, GLint Width, GLint Height);
	static void MouseMove_Callback(GLFWwindow* Window, GLdouble dX, GLdouble dY);
};

