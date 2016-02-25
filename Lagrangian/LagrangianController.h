#pragma once

#include "GameController.h"

/*
*/
class LagrangianController : public GameController
{

public:

	LagrangianController();
	~LagrangianController();

	// GameController Interface
	virtual void Initialize() override;
	virtual void Update(const GLdouble& dt) override;
	virtual void ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode) override;
	virtual void ProcessMouseMove(const GLdouble& dX, const GLdouble& dY) override;
	virtual void Render() override;
};