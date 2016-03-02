#pragma once

#include "GameController.h"
#include <Sphere.h>
#include <array>
#include <map>
#include <string>

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

	Sphere* Planet; // The gravitational body (fixed)
	std::vector<Sphere*> Moons; // Moveable

	std::vector<std::map<std::string, GLfloat>> Radial;
	std::vector<std::map<std::string, GLfloat>> Angular;
};