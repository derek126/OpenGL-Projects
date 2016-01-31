#include "CRSpline.h"

#include <iostream>

CRSpline::CRSpline() :
	Incr(0),
	Elapsed(0), 
	SpeedFactor(0.05f)
{
}

CRSpline::~CRSpline()
{
}

void CRSpline::AddPoint(const glm::vec3& Point)
{
	Points.push_back(Point);
	Incr = 1.f / static_cast<GLfloat>(Points.size());
}

glm::vec3* CRSpline::GetPoint(const GLuint& Index)
{
	if (Index >= 0 && Index < Points.size())
	{
		return &Points[Index];
	}

	return nullptr;
}

glm::vec3 CRSpline::GetInterpolatedPoint(const GLfloat& dt)
{
	Elapsed += dt * SpeedFactor;
	GLuint p = static_cast<GLuint>(Elapsed / Incr);

	if (p > Points.size() - 1)
	{
		Elapsed = 0;
	}

	std::cout << p << std::endl;

	auto Clamp = [this](GLuint& p) 
	{
		if (p < 0) p = 0;
		else if (p > Points.size() - 1) p = Points.size() - 1;
	};

	GLuint p0 = p - 1;
	Clamp(p0);
	GLuint p1 = p;
	Clamp(p1);
	GLuint p2 = p + 1;
	Clamp(p2);
	GLuint p3 = p + 2;
	Clamp(p3);

	GLfloat lt = (Elapsed - Incr * static_cast<GLfloat>(p)) / Incr;

	return ComputeEq(lt, Points[p0], Points[p1], Points[p2], Points[p3]);
}

glm::vec3 CRSpline::ComputeEq(const GLfloat& t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
{
	GLfloat tSquared = glm::pow(t, 2);
	GLfloat tCubed = glm::pow(t, 3);

	GLfloat b1 = .5f * (-tCubed + 2.f * tSquared - t);
	GLfloat b2 = .5f * (3.f * tCubed - 5.f * tSquared + 2.f);
	GLfloat b3 = .5f * (-3.f * tCubed + 4.f * tSquared + t);
	GLfloat b4 = .5f * (tCubed - tSquared);

	return (p1 * b1 + p2 * b2 + p3 * b3 + p4 * b4);
}
