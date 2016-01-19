#pragma once

#include "GameController.h"

/*
* Sets up and showcases several different springs
*/
class FluidController : public GameController
{

public:

	FluidController();
	~FluidController();

	// GameController Interface
	void Initialize();
	void Update(const GLdouble& dt);
	void ProcessInput(const GLdouble& dt);
	void Render();
};

