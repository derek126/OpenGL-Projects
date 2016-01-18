#pragma once

#include "Cube.h"

/*
* The pointmass class is represented by a cube in 3D space, it allows for forces
* to be applied to simulate motion.
*/
class PointMass : public Cube
{
public:

	PointMass(const GLfloat Mass, const GLboolean& isFixed = false);
	~PointMass();

	// Apply a 3d force to the point mass
	void ApplyForce(const glm::vec3& Force);

	// Update the mass velocity, and reset force
	void Update(const GLfloat& dt);

	// Getters
	GLboolean IsFixedPoint() const;
	glm::vec3 GetVelocity() const;
	GLfloat GetMass() const;

private:

	// State
	glm::vec3 Velocity, Force;
	GLboolean bIsFixed;
	GLfloat Mass;
};

