#include "GameController.h"
#include "GameManager.h"
#include "ResourceManager.h"

GameController::GameController() :
	Title(""),
	ProjectionType(EProjectionType::PERSP)
{
	Camera = new class Camera();
}

GameController::~GameController()
{
	if (Camera) delete Camera;
}

void GameController::Initialize()
{
	Camera->UpdateView();
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
