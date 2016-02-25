#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "GameManager.h"

/*
* An abstract base class meant to house the logic for a game
*/
class GameController
{
	friend class GameManager;

public:

	// The projection type
	enum EProjectionType
	{
		ORTHO,
		PERSP
	};

	GameController();
	virtual ~GameController();

	// Functions meant to be overidden to construct a game/scene
	virtual void Initialize();
	virtual void ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode) = 0;
	virtual void ProcessMouseMove(const GLdouble& dX, const GLdouble& dY) = 0;
	virtual void Update(const GLfloat& dt) = 0;
	virtual void Render() = 0;

protected:

	// Changes the screen dimensions of the scene
	void SetScreenDimensions(const GLuint& Width, const GLuint& Height, const GLboolean& UpdateWindow = true);

	// State
	GLFWwindow* Window;
	GLchar* Title;
	EProjectionType ProjectionType;
	class Camera* Camera;
};