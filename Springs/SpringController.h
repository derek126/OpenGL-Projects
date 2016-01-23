#pragma once

#include "GameController.h"
#include "PointMass.h"
#include "Spring.h"
#include "AngularSpring.h"

#include <vector>

/*
* Sets up and showcases several different springs
*/
class SpringController : public GameController
{

public:

	SpringController();
	~SpringController();

	// GameController Interface
	void Initialize();
	void Update(const GLdouble& dt);
	void ProcessInput(const GLdouble& dt);
	void Render();

private:

	// Sets up the various springs on the screen
	void InitSpring();
	void InitSpringWithDampening();
	void InitSpringMesh();
	void InitAngularSpring();

	// State
	std::vector<PointMass*> Masses;
	std::vector<Spring*> Springs;
	std::vector<AngularSpring*> AngSprings;

	// Used to store the spring mesh
	const GLint SizeX = 64;
	const GLint SizeY = 96;
	std::vector<std::vector<PointMass*>> MeshGrid;
};

