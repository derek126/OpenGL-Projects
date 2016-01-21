#include "GameController.h"
#include "GameManager.h"
#include "ResourceManager.h"

GameController::GameController() :
	bKeys(), 
	Title(""),
	ProjectionType(EProjectionType::PERSP)
{
}

GameController::~GameController()
{
}

void GameController::SetCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	RESOURCEMANAGER.SetViewMatrix(glm::lookAt(position, target, up));
}

void GameController::SetScreenDimensions(const GLuint& Width, const GLuint& Height, const GLboolean& UpdateWindow)
{
	glViewport(0, 0, Width, Height);

	if (ProjectionType == GameController::EProjectionType::PERSP)
	{
		RESOURCEMANAGER.SetProjectionMatrix(glm::perspective(glm::radians(45.0), static_cast<GLdouble>(Width) / Height, 0.1, 1000.0));
	}
	else
	{
		RESOURCEMANAGER.SetProjectionMatrix(glm::ortho(0.0, static_cast<GLdouble>(Width), static_cast<GLdouble>(Height), 0.0, -1.0, 1.0));
	}

	// If the screen was resized, we do not need to adjust it again
	if (UpdateWindow)
	{
		glfwSetWindowSize(Window, Width, Height);
	}
}
