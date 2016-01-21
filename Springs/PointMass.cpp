#include "PointMass.h"

#include <array>

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

	/*auto Eval = [this](const glm::vec3& Accel, const GLfloat& dt)
	{
		std::array<glm::vec3, 2> Out;

		Out[0] = Velocity + Accel * dt;
		Out[1] = Force / Mass;

		return Out;
	};

	std::array<std::array<glm::vec3, 2>, 4> D;

	D[0] = Eval(glm::vec3(0.f), 0.f);
	D[1] = Eval(D[0][1], dt * 0.5f);
	D[2] = Eval(D[1][1], dt * 0.5f);
	D[3] = Eval(D[2][1], dt);

	glm::vec3 Pos = (D[0][0] + 2.f * (D[1][0] + D[2][0]) + D[3][0]) / 6.f;
	glm::vec3 Vel = (D[0][1] + 2.f * (D[1][1] + D[2][1]) + D[3][1]) / 6.f;

	SetTranslation(GetPosition() + Pos * dt);
	Velocity += Vel * dt;*/

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

