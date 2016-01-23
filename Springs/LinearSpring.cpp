#include "LinearSpring.h"

LinearSpring::LinearSpring(PointMass* M1, PointMass* M2, const GLfloat &SpringConstant, const GLfloat& RestLength, const GLfloat &Dampening) :
	Spring(M1, M2, SpringConstant, Dampening),
	RestLength(RestLength)
{
}


LinearSpring::~LinearSpring()
{
}

void LinearSpring::Simulate(const GLfloat& dt)
{
	GLfloat CurrentLength = glm::length(Masses[0]->GetPosition() - Masses[1]->GetPosition());
	glm::vec3 Direction = glm::normalize(Masses[0]->GetPosition() - Masses[1]->GetPosition());

	// F = -kX, where X is the amount the string is displaced from its resting position
	// NOTE: F is NET force. This is multiplied by the direction of the force (the unit vector between the two masses)
	// We subtract V*D from the force to dampen the movement
	glm::vec3 Force0 = -SpringConstant * (RestLength - CurrentLength) * -Direction - Masses[0]->GetVelocity() * DampeningFactor;
	glm::vec3 Force1 = -SpringConstant * (RestLength - CurrentLength) * Direction - Masses[1]->GetVelocity() * DampeningFactor;

	Masses[0]->ApplyForce(Force0);
	Masses[1]->ApplyForce(Force1);
}
