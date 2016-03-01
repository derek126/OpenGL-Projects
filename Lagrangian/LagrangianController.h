#pragma once

#include "GameController.h"
#include <Sphere.h>

class LagrangianController : public GameController
{

public:

	LagrangianController();
	~LagrangianController();

	// GameController Interface
	virtual void Initialize() override;
	virtual void Update(const GLfloat& dt) override;
	virtual void ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode) override;
	virtual void ProcessMouseMove(const GLdouble& dX, const GLdouble& dY) override;
	virtual void Render() override;

private:

	void InitMasses();

	Sphere* M1; // The gravitational body (fixed)
	Sphere* M2; // Moveable

	glm::mat4 Model; // The model matrix of the moveable object

	GLfloat rPos, rAcc, rVel;
	GLfloat tPos, tAcc, tVel;
};