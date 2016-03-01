#pragma once

#include "Drawable.h"
#include <vector>

/*
* A basic sphere primitive that can be drawn
*/
class Sphere : public Drawable
{

public:

	Sphere(const GLfloat& R = 1.f, const GLuint& DivX = 16, const GLuint& DivY = 16);
	virtual ~Sphere() override;
	virtual void Draw() override;

	GLfloat GetRadius() const;

	// These are replaced by radius
	virtual void Scale(const glm::vec3& ScaleAmnt) override {};
	virtual void SetScale(const glm::vec3& ScaleAmnt) override {};
	virtual void SetScale(const glm::mat4& Scale) override {}

private:

	void CreateSphere();

	GLfloat Radius;

	GLuint SDivX, SDivY;

	std::vector<GLuint> Indices;
	std::vector<glm::vec3> Vertices, Normals;
	std::vector<glm::vec2> UVs;
};

