#pragma once

#include "GameController.h"
#include "PointMass.h"
#include "Spring.h"
#include "AngularSpring.h"
#include "CRSpline.h"

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
	virtual void Initialize() override;
	virtual void Update(const GLfloat& dt) override;
	virtual void ProcessInput(const GLint& Key, const GLint& Action, const GLint& Mode) override;
	virtual void ProcessMouseMove(const GLdouble& dX, const GLdouble& dY) override;
	virtual void Render() override;

private:

	// Sets up the various springs on the screen
	void InitSpring();
	void InitSpringWithDampening();
	void InitSpringMesh();
	void InitAngularSpring();

	// Updates the camera position (uses the spline)
	void UpdateCamera(const GLfloat& dt);
	CRSpline Spline;

	// State
	std::vector<PointMass*> Masses;
	std::vector<Spring*> Springs;
	std::vector<AngularSpring*> AngSprings;
	GLboolean bIsCameraMoving;
	GLfloat MouseSensitivity;

	// Used to store the spring mesh
	const GLint SizeX = 64;
	const GLint SizeY = 96;
	std::vector<std::vector<PointMass*>> MeshGrid;
};

