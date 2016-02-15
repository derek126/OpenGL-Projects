#pragma once
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>

/*
* A catmull rom spline
*/
class CRSpline
{
public:
	CRSpline();
	~CRSpline();

	void AddPoint(const glm::vec3& Point);
	glm::vec3* GetPoint(const GLuint& Index);
	glm::vec3 GetInterpolatedPoint(const GLfloat& t);

private:

	glm::vec3 ComputeEq(const GLfloat& t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);

	std::vector<glm::vec3> Points;
	GLfloat Incr, Elapsed, SpeedFactor;
};

