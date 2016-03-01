#include "LagrangianController.h"
#include "ResourceManager.h"

#include <array>
#include <iostream>
#include <cmath>

#define ScreenX 1920
#define ScreenY 1080

// Real numbers... Nope
/*
#define G 6.67408f * glm::pow(10.f, -11)
#define M 1.989f * glm::pow(10.f, 30)
#define R 149.6f * glm::pow(10.f, 6)
#define iVel 7.2921159f * glm::pow(10.f, -5)
*/

// Nice looking numbers... Yeah!
#define G 1.f
#define M 131072.f
#define R 128.f
#define iVel 0.16f

LagrangianController::LagrangianController() :
	rPos(R), rAcc(0.f), rVel(0.f),
	tPos(0.f), tAcc(0.f), tVel(iVel)
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
	rAcc = rPos * glm::pow(tVel, 2) - (M * G / glm::pow(rPos, 2));
	rVel += rAcc * dt;
	rPos += rVel * dt;

	tAcc = -(2.f * rVel * tVel) / rPos;
	tVel += tAcc * dt;
	tPos += tVel * dt;

	//std::cout << "Rad Accel: " << rAcc << std::endl;
	//std::cout << "Rad Vel: " << rVel << std::endl;
	//std::cout << "Rad Pos: " << rPos << std::endl;

	//std::cout << "Theta Accel: " << tAcc << std::endl;
	//std::cout << "Theta Vel: " << tVel << std::endl;
	//std::cout << "Theta Pos: " << tPos << std::endl;

	Model = glm::rotate(glm::mat4(), tPos, glm::vec3(0.f, -1.f, -1.f));
	Model = glm::translate(Model, glm::vec3(rPos, 0.f, 0.f));
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