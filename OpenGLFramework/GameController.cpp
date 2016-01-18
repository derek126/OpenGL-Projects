#include "GameController.h"
#include "GameManager.h"

GameController::GameController() :
	bKeys(), 
	Title(""),
	ProjectionType(EProjectionType::PERSP)
{
	View = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	SetScreenDimensions(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
}

GameController::~GameController()
{

}

void GameController::SetCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	View = glm::lookAt(position, target, up);
}

void GameController::SetScreenDimensions(const GLuint& Width, const GLuint& Height, const GLboolean& UpdateWindow)
{
	glViewport(0, 0, Width, Height);

	if (ProjectionType == GameController::EProjectionType::PERSP)
	{
		Projection = glm::perspective(glm::radians(45.0), static_cast<GLdouble>(Width) / Height, 0.1, 1000.0);
	}
	else
	{
		Projection = glm::ortho(0.0, static_cast<GLdouble>(Width), static_cast<GLdouble>(Height), 0.0, -1.0, 1.0);
	}

	// If the screen was resized, we do not need to adjust it again
	if (UpdateWindow)
	{
		glfwSetWindowSize(Window, Width, Height);
	}
}
