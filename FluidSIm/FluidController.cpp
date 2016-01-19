#include "FluidController.h"
#include "ResourceManager.h"

FluidController::FluidController()
{
}


FluidController::~FluidController()
{
}

void FluidController::Initialize()
{
	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(1920, 1080);
	SetCamera(glm::vec3(0.f, 0.f, 10.0f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.0f, 0.f));

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Initialize the shader to be used for the masses
	RESOURCEMANAGER.LoadShader("basic.vs", "basic.fs", nullptr, "Basic");
}

void FluidController::Update(const GLdouble& dt)
{
}

void FluidController::ProcessInput(const GLdouble& dt)
{
}

void FluidController::Render()
{
}
