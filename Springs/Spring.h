#pragma once

#include "PointMass.h"

#include <array>

/*
* A spring creates forces between the two supplied point mass objects
* Supports dampening and variable spring constants
*/
class Spring
{
public:

	Spring(PointMass* M1, PointMass* M2, const GLfloat &SpringConstant, const GLfloat& RestLength, const GLfloat &Dampening = 0);
	~Spring();

	// Update the forces on the masses at this instance
	void Simulate(const GLfloat& dt);

private:

	// State
	std::array<PointMass*, 2> Masses;
	GLfloat SpringConstant, DampeningFactor;
	GLfloat RestLength;
};

