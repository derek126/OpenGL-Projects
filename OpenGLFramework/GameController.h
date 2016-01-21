#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	virtual void Initialize() = 0;
	virtual void ProcessInput(const GLdouble& dt) = 0;
	virtual void Update(const GLdouble& dt) = 0;
	virtual void Render() = 0;

protected:

	// Changes the camera position and orientation
	void SetCamera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f), const glm::vec3& target = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	// Changes the screen dimensions of the scene
	void SetScreenDimensions(const GLuint& Width, const GLuint& Height, const GLboolean& UpdateWindow = true);

	// State
	GLFWwindow* Window;
	GLboolean bKeys[1024];
	GLchar* Title;
	EProjectionType ProjectionType;
};