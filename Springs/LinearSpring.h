#pragma once

#include "Spring.h"

/*
* A spring creates forces between the two supplied point mass objects
* Supports dampening and variable spring constants
*/
class LinearSpring : public Spring
{
public:

	LinearSpring(PointMass* M1, PointMass* M2, const GLfloat &SpringConstant, const GLfloat& RestLength, const GLfloat &Dampening = 0);
	~LinearSpring();

	// Update the forces on the masses at this instance
	virtual void Simulate(const GLfloat& dt) override;

private:

	GLfloat RestLength;
};

