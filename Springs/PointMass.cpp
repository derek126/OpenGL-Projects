#include "PointMass.h"

PointMass::PointMass(const GLfloat Mass, const GLboolean& isFixed) :
	Mass(Mass),
	bIsFixed(isFixed)
{}


PointMass::~PointMass()
{}

void PointMass::ApplyForce(const glm::vec3& Force)
{
	if (bIsFixed) return;
	this->Force += Force;
}

void PointMass::Update(const GLfloat& dt)
{
	if (bIsFixed) return;

	// Vf = V0 + A*t
	Velocity += (Force / Mass) * dt;
	SetTranslation((Velocity * dt) + GetPosition());

	Force = glm::vec3(0);
}

GLboolean PointMass::IsFixedPoint() const
{
	return bIsFixed;
}

glm::vec3 PointMass::GetVelocity() const
{
	return Velocity;
}

GLfloat PointMass::GetMass() const
{
	return Mass;
}

