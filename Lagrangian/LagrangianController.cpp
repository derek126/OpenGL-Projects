#include "LagrangianController.h"
#include "ResourceManager.h"

#define ScreenX 1920
#define ScreenY 1080

LagrangianController::LagrangianController()
{

}

LagrangianController::~LagrangianController()
{
	if (M1) delete M1;
	if (M2) delete M2;
}

void LagrangianController::Initialize()
{
	GameController::Initialize();
	GAMEMANAGER.SetTargetFrametime(60);

	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

	Camera->SetPosition(glm::vec3(45.f, 10.f, 64.0f));
	Camera->SetFocus(glm::vec3(0.5f, 0.5f, 0.f));
	Camera->SetWorldUp(glm::vec3(0.f, 1.f, 0.f));
	Camera->UpdateView();

	// Hide the mouse cursor
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Set the directional light direction and color
	RESOURCEMANAGER.SetLightDirection(-glm::vec3(45.f, 10.f, 45.0f));
	RESOURCEMANAGER.SetLightColor(glm::vec3(1.f, 1.f, 1.f));

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	InitMasses();
}

void LagrangianController::Update(const GLfloat& dt)
{
	static GLfloat Rot = 0.f;
	Rot += 2.f * dt;

	Model = glm::rotate(glm::mat4(), Rot, glm::vec3(0.f, -1.f, -1.f));
	Model = glm::translate(Model, glm::vec3(32.f, 0.f, 0.f));
	M2->SetModel(Model);
}

void LagrangianController::ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode)
{

}

void LagrangianController::ProcessMouseMove(const GLdouble& dX, const GLdouble& dY)
{
	static GLint normalX = ScreenX / 2;
	static GLint normalY = ScreenY / 2;

	GLboolean bDidUpdate = false;
	if (dX != normalX || dY != normalY)
	{
		Camera->RotateByMouse(static_cast<GLfloat>(dX - normalX) * 0.0005f, static_cast<GLfloat>(dY - normalY) * 0.0005f);
		Camera->UpdateView();

		bDidUpdate = true;
	}

	if (bDidUpdate)
	{
		glfwSetCursorPos(Window, normalX, normalY);
	}
}

void LagrangianController::Render()
{
	RESOURCEMANAGER.GetShader("Masses").SetVector3f("Color", glm::vec3(1.0f, 0.5f, 0.2f), true);
	M1->Draw();

	RESOURCEMANAGER.GetShader("Masses").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);
	M2->Draw();
}

void LagrangianController::InitMasses()
{
	M1 = new Sphere(16.f, 128.f, 128.f);
	M2 = new Sphere(4.f, 128.f, 128.f);

	RESOURCEMANAGER.LoadShader("phong.vs", "phong.fs", nullptr, "Masses");
	M1->SetShader("Masses");
	M2->SetShader("Masses");
}