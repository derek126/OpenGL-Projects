#include "LagrangianController.h"
#include "ResourceManager.h"

#include <array>
#include <cmath>
#include <glm\gtc\random.hpp>
#include <glm\gtx\string_cast.hpp>

#define ScreenX 1920
#define ScreenY 1080

// Real numbers for earth and sun
/*
#define G 6.67408f * glm::pow(10.f, -11)
#define M 1.989f * glm::pow(10.f, 30)
#define R 149.6f * glm::pow(10.f, 6)
#define iVel 7.2921159f * glm::pow(10.f, -5)
*/

// Nice looking numbers...
#define G 1.f
#define M 131072.f // of Planet

#define NUM_BODIES 3

// Distance of moons
#define R1 128.f
#define R2 64.f
#define R3 156.f
// Initial angular velocities
#define iVel1 0.16f
#define iVel2 0.64f
#define iVel3 0.12f

LagrangianController::LagrangianController()
{
	Moons.resize(NUM_BODIES);
	Radial.resize(NUM_BODIES);
	Angular.resize(NUM_BODIES);
	Axis.resize(NUM_BODIES);

	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		Axis[i] = glm::vec3(glm::linearRand(0.f, 1.f), glm::linearRand(0.f, 1.f), glm::linearRand(0.f, 1.f));
	}

	// Set initial values
	Radial[0]["Position"] = R1;
	Radial[0]["Velocity"] = 0.f;
	Radial[0]["Acceleration"] = 0.f;

	Angular[0]["Position"] = 0.f;
	Angular[0]["Velocity"] = iVel1;
	Angular[0]["Acceleration"] = 0.f;

	Radial[1]["Position"] = R2;
	Radial[1]["Velocity"] = 0.f;
	Radial[1]["Acceleration"] = 0.f;

	Angular[1]["Position"] = 0.f;
	Angular[1]["Velocity"] = iVel2;
	Angular[1]["Acceleration"] = 0.f;

	Radial[2]["Position"] = R3;
	Radial[2]["Velocity"] = 0.f;
	Radial[2]["Acceleration"] = 0.f;

	Angular[2]["Position"] = 0.f;
	Angular[2]["Velocity"] = iVel3;
	Angular[2]["Acceleration"] = 0.f;
}

LagrangianController::~LagrangianController()
{
	if (Planet) delete Planet;
	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		if (Moons[i]) delete Moons[i];
	}
}

void LagrangianController::Initialize()
{
	GameController::Initialize();
	GAMEMANAGER.SetTargetFrametime(120);

	// Increase screen dimensions and then set the camera location
	SetScreenDimensions(ScreenX, ScreenY);

	Camera->SetPosition(glm::vec3(45.f, 10.f, 256.0f));
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
	// Euler
	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		Radial[i]["Acceleration"] = Radial[i]["Position"] * glm::pow(Angular[i]["Velocity"], 2) - (M * G / glm::pow(Radial[i]["Position"], 2));
		Radial[i]["Velocity"] += Radial[i]["Acceleration"] * dt;
		Radial[i]["Position"] += Radial[i]["Velocity"] * dt;
	}

	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		Angular[i]["Acceleration"] = -(2.f * Radial[i]["Velocity"] * Angular[i]["Velocity"]) / Radial[i]["Position"];
		Angular[i]["Velocity"] += Angular[i]["Acceleration"] * dt;
		Angular[i]["Position"] += Angular[i]["Velocity"] * dt;
	}

	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		glm::mat4 Model;
		Model = glm::rotate(Model, Angular[i]["Position"], Axis[i]);
		Model = glm::translate(Model, glm::vec3(Radial[i]["Position"], 0.f, 0.f));
		Moons[i]->SetModel(Model);
	}
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
	Planet->Draw();

	RESOURCEMANAGER.GetShader("Masses").SetVector3f("Color", glm::vec3(0.f, 0.75f, 1.f), true);
	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		Moons[i]->Draw();
	}
}

void LagrangianController::InitMasses()
{
	RESOURCEMANAGER.LoadShader("phong.vs", "phong.fs", nullptr, "Masses");

	Planet = new Sphere(16.f, 128, 128);
	Planet->SetShader("Masses");

	for (GLuint i = 0; i < NUM_BODIES; i++)
	{
		Moons[i] = new Sphere(4.f, 128, 128);
		Moons[i]->SetShader("Masses");
	}
}